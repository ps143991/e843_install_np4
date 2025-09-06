/*****************************************************************************
 * Copyright (C) 2009-2021   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    : 03/10/22 v4 integration                   (A. Matta)     *
 *---------------------------------------------------------------------------*
 * Decription: This class manage root input                                  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "NPRootInput.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

#include "NPException.h"
#include "NPFunction.h"
#include "NPRootAsciiFile.h"
using namespace std;
using namespace nptool;

////////////////////////////////////////////////////////////////////////////////
void RootInput::Init(std::vector<std::string> arg) {
  // case of a single tree with tree name : --input root,TreeFile.root,TreeName
  if (arg.size() == 3) {
    message("green", "root", "nptool::RootInput", "Initializing input TChain using : " + arg[2]);
    // Open File
    m_RootFile = new TFile(arg[2].c_str());
    if (m_RootFile->IsZombie()) {
      throw(nptool::Error("RootInput", "Input file " + arg[2] + " does not exist"));
    }
    // Get Tree
    m_TreeName = arg[1];
    m_RootChain = static_cast<TChain*>(m_RootFile->FindObjectAny(arg[1].c_str()));
    if (!m_RootChain) {
      throw(nptool::Error("RootInput", "Tree " + arg[1] + " not found in " + arg[2]));
    }
  }
  else if (arg.size() == 2) {
    // case of a RunToTreat.txt file : --input root,RunToTreat.txt
    if (arg[1].find(":") == std::string::npos) {
      message("green", "root", "nptool::RootInput", "Initializing input TChain using : " + arg[1]);
      nptool::InputParser parser(arg[1]);
      ReadInputFile(parser);
    }

    // case of an address + port (remote circular tree)
    else {
      int split_pos = arg[1].find(":");
      std::string host = arg[1].substr(0, split_pos);
      std::string str_port = arg[1].substr(split_pos + 1, arg[1].size());
      unsigned int port = stoi(str_port);
      m_TreeClient = new nptool::root::RootTreeClient(host, port);
    }
  }
  else {
    throw(nptool::Error(
        "RootInput",
        "Take 2 or 3 argument: --output root,<FileName.root>,<TreeName> or --output root,<RunToTreat.yaml>"));
  }

  if (m_RootChain)
    m_Entries = m_RootChain->GetEntries();
  else if (m_TreeClient)
    m_Entries = m_TreeClient->GetCurrentTree()->GetEntries();

  m_CurrentEntry = 0;
}

////////////////////////////////////////////////////////////////////////////////
void RootInput::ReadInputFile(nptool::InputParser& parser) {
  vector<std::shared_ptr<nptool::InputBlock>> blocks = parser.GetAllBlocksWithToken("Tree");
  m_TreeName = blocks[0]->GetString("name");
  std::vector<std::string> lines = blocks[0]->GetVectorString("files");
  unsigned int size = lines.size();
  for (unsigned int i = 0; i < size; i++) {
    if (lines[i].find(".root") != string::npos)
      m_TreePath.push_back(lines[i]);
    else if (lines[i].find(".tree") != string::npos)
      ReadTreeFile(lines[i]);
  }

  vector<std::shared_ptr<nptool::InputBlock>> friends = parser.GetAllBlocksWithToken("Friend");
  unsigned int sizeF = friends.size();
  for (unsigned int i = 0; i < sizeF; i++) {
    m_FriendsPath.insert(pair<string, vector<string>>(friends[i]->GetMainValue(), friends[i]->GetLines()));
  }
  // Initialise the chain
  m_RootChain = new TChain(m_TreeName.c_str());

  // Add all the files
  size = m_TreePath.size();
  std::string firstfile;
  for (unsigned int i = 0; i < size; i++) {
    cout << "  - Adding file : " << m_TreePath[i].c_str() << endl;
    m_RootChain->Add(m_TreePath[i].c_str());
    // Test if the file is a regex or a single file
    double counts;
    std::string command = "ls " + m_TreePath[i] + " > .ls_return";
    counts = system(command.c_str());
    std::ifstream return_ls(".ls_return");
    std::string files;
    while (return_ls >> files) {
      if (counts == 0)
        firstfile = files;
      counts++;
    }
  }

  // Case of user made Friends
  for (auto it = m_FriendsPath.begin(); it != m_FriendsPath.end(); it++) {
    TChain* chain = new TChain(it->first.c_str());
    cout << "  - Adding friend : " << endl;
    for (auto itp = it->second.begin(); itp != it->second.end(); itp++) {
      cout << "    - " << (*itp).c_str() << endl;
      chain->Add((*itp).c_str());
    }
    m_RootChain->AddFriend(chain);
  }

  // Case of tree file
  for (auto it = m_FriendsTreePath.begin(); it != m_FriendsTreePath.end(); it++) {
    TChain* chain = new TChain(m_TreeName.c_str());
    cout << "  - Adding friend : " << endl;
    for (auto itp = it->second.begin(); itp != it->second.end(); itp++) {
      cout << "    - " << (*itp).c_str() << endl;
      chain->Add((*itp).c_str());
    }
    m_RootChain->AddFriend(chain);
  }
}

////////////////////////////////////////////////////////////////////////////////
void RootInput::ReadTreeFile(std::string path) {
  ifstream tree(path.c_str());
  path = path.substr(0, path.rfind("/") + 1);
  std::string buffer;
  bool first = true;
  unsigned int count = 0;
  while (tree >> buffer) {
    if (first) {
      m_TreePath.push_back(path + buffer);
      count++;
      first = false;
    }
    else {
      m_FriendsTreePath[count++].push_back(path + buffer);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
std::string RootInput::DumpAsciiFile(const char* type, const char* folder) {
  std::string name = "fail";

  std::string sfolder = folder;
  // create folder if not existing
  // first test if folder exist
  struct stat dirInfo;
  int res = stat(folder, &dirInfo);
  if (res != 0) { // if folder does not exist, create it
    std::string cmd = "mkdir " + sfolder;
    if (system(cmd.c_str()) != 0)
      std::cout << "RootInput::DumpAsciiFile() problem creating directory" << std::endl;
  }

  std::string stype = type;
  if (stype == "EventGenerator") {
    RootAsciiFile* aFile = (RootAsciiFile*)m_RootFile->Get(stype.c_str());

    if (aFile) {
      // build file name
      std::string title = aFile->GetTitle();
      size_t pos = title.rfind("/");
      if (pos != std::string::npos)
        name = sfolder + title.substr(pos);
      else
        name = sfolder + "/" + title;
      aFile->WriteToFile(name.c_str());
    }
  }

  else if (stype == "DetectorConfiguration") {
    RootAsciiFile* aFile = (RootAsciiFile*)m_RootFile->Get(stype.c_str());
    if (aFile) {
      // build file name
      std::string title = aFile->GetTitle();
      size_t pos = title.rfind("/");
      if (pos != std::string::npos)
        name = sfolder + title.substr(pos);
      else
        name = sfolder + "/" + title;
      aFile->WriteToFile(name.c_str());
    }
  }

  else if (stype == "Calibration") {
    RootAsciiFile* aFile = (RootAsciiFile*)m_RootFile->Get(stype.c_str());
    if (aFile) {
      // build file name
      std::string title = aFile->GetTitle();
      size_t pos = title.rfind("/");
      if (pos != std::string::npos)
        name = sfolder + title.substr(pos);
      else
        name = sfolder + "/" + title;
      aFile->WriteToFile(name.c_str());
    }
  }

  else if (stype == "RunToTreat") {
  }
  else {
    std::cout << "RootInput::DumpAsciiFile() unkwown keyword" << std::endl;
  }

  return name;
}

////////////////////////////////////////////////////////////////////////////////
RootInput::~RootInput() {
  if (m_RootFile) {
    // delete default directory ./.tmp
    struct stat dirInfo;
    int res = stat("./.tmp", &dirInfo);
    if (res == 0) { // if does exist, delete it
      if (system("rm -rf ./.tmp") != 0)
        std::cout << "RootInput::~RootInput() problem deleting ./.tmp directory" << std::endl;
    }
    std::cout << std::endl << "Root Input summary" << std::endl;
    std::cout << "  - Number of bites read: " << m_RootFile->GetBytesRead() << std::endl;
    std::cout << "  - Number of transactions: " << m_RootFile->GetReadCalls() << std::endl;
    // Close the Root file
    m_RootFile->Close();
  }
}
////////////////////////////////////////////////////////////////////////////////
void RootInput::Attach(std::string branch, std::string, void* ptr) {
  if (m_RootChain) {
    m_RootChain->SetBranchStatus(branch.c_str(), true);

    m_RootChain->SetBranchAddress(branch.c_str(), ptr);
    message("green", "root", "nptool::RootInput", branch + " attached to input chain");
  }
  if (m_TreeClient) {
    m_TreeClient->GetCurrentTree()->SetBranchStatus(branch.c_str(), true);

    m_TreeClient->GetCurrentTree()->SetBranchAddress(branch.c_str(), ptr);
    message("green", "root", "nptool::RootInput", branch + " attached to input circular tree");
  }
};
////////////////////////////////////////////////////////////////////////////////
bool RootInput::GetNextEntry() {
  if (m_CurrentEntry < m_Entries) {
    if (m_RootChain)
      m_RootChain->GetEntry(m_CurrentEntry++);
    else if (m_TreeClient) {
      m_TreeClient->GetCurrentTree()->GetEntry(m_CurrentEntry++);
    }
    else
      return false;
    return true;
  }
  else if (m_TreeClient) {
    m_TreeClient->RequestNewTree();
    m_Entries = m_TreeClient->GetEntries();
    m_CurrentEntry = 0;
    return GetNextEntry();
  }
  return false;
};
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VDataInput> ConstructDataInput() { return std::make_shared<RootInput>(); }
}

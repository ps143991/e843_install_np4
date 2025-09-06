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

#include "NPRootOutput.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

#include "NPApplication.h"
#include "NPException.h"
#include "NPFunction.h"
#include "NPInputParser.h"
#include "NPRootAsciiFile.h"
using namespace std;
using namespace nptool;

////////////////////////////////////////////////////////////////////////////////
void RootOutput::Init(std::vector<std::string> arg) {
  // case of a file with treename : --output root,TreeName,FileName.root
  auto app = nptool::Application::GetApplication();
  //  cout << arg.size() << endl;
  if (arg.size() == 3) {
    auto outputpath = app->GetOutputPath() + arg[2];
    message("green", "root", "nptool::RootOutput", "Initializing output TTree : " + arg[1] + " at " + outputpath);
    // Open File
    m_RootFile = new TFile(outputpath.c_str(), "RECREATE");
    if (m_RootFile->IsZombie()) {
      throw(nptool::Error("RootOutput", "Fail to create output file " + outputpath));
    }

    // New Tree
    m_TreeName = arg[1];
    m_RootTree = new TTree(m_TreeName.c_str(), "nptool tree");
  }

  // case of a root tree in memory : --output root,<port number>
  else if (arg.size() == 2) {
    message("green", "root", "nptool::RootOutput", "Initializing output tree server on port : " + arg[1]);
    unsigned int port = atoi(arg[1].data());
    unsigned int size = 1000;
    if (app->HasFlag("--root-circular-tree-size"))
      size = app->GetIntArg("--root-circular-tree-size");

    m_TreeServer = std::make_shared<nptool::root::RootTreeServer>(size, port);
    m_RootTree = m_TreeServer->GetTree();
  }
  else {
    throw(nptool::Error("RootOutput", "Take 3 or 2 arguments: (file) --output "
                                      "root,<TreeName>,<FileName.root> | (server) --output root,<port>"));
  }
}

void RootOutput::AttachAsciiFile(std::string name, std::string title, std::string path) {
  if (m_RootFile) {
    gDirectory->cd(m_RootFile->GetPath());
    auto file = new RootAsciiFile(name.c_str(), title.c_str(), path.c_str());
    file->Write();
  }
}

////////////////////////////////////////////////////////////////////////////////
RootOutput::~RootOutput() {
  if (m_RootFile) {
    m_RootTree->Write();
    m_RootFile->Close();
    message("green", "root", "nptool::RootOutput", "Output root file closed");
  }
}
////////////////////////////////////////////////////////////////////////////////
void RootOutput::Attach(std::string branch, std::string class_name, void* ptr) {
  if (m_RootTree) {
    if (class_name.find("/") == std::string::npos){ 
      // suitable for vector cases
      if (class_name == "std::vector<int>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<int>*>(ptr));
      }
      else if (class_name == "std::vector<unsigned int>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<unsigned int>*>(ptr));
      }
      else if (class_name == "std::vector<short>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<short>*>(ptr));
      }
      else if (class_name == "std::vector<unsigned short>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<unsigned short>*>(ptr));
      }
      else if (class_name == "std::vector<long>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<long>*>(ptr));
      }
      else if (class_name == "std::vector<unsigned long>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<unsigned long>*>(ptr));
      }
      else if (class_name == "std::vector<long long>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<long long>*>(ptr));
      }
      else if (class_name == "std::vector<unsigned long long>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<unsigned long long>*>(ptr));
      }
      else if (class_name == "std::vector<float>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<float>*>(ptr));
      }
      else if (class_name == "std::vector<double>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<double>*>(ptr));
      }
      else if (class_name == "std::vector<bool>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<bool>*>(ptr));
      }
      else if (class_name == "std::vector<char>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<char>*>(ptr));
      }
      else if (class_name == "std::vector<unsigned char>") {
        m_RootTree->Branch(branch.c_str(), static_cast<std::vector<unsigned char>*>(ptr));
      }
      else{
        m_RootTree->Branch(branch.c_str(), class_name.c_str(), ptr, 32000, 1);
      }
      message("green", "root", "nptool::RootOutput", branch + " attached to output tree");
    }
    else {
      auto back = class_name.back();
      if (back == 'C')
        m_RootTree->Branch(branch.c_str(), (Char_t*)ptr);
      else if (back == 'B')
        m_RootTree->Branch(branch.c_str(), (Char_t*)ptr);
      else if (back == 'b')
        m_RootTree->Branch(branch.c_str(), (UChar_t*)ptr);
      else if (back == 'S')
        m_RootTree->Branch(branch.c_str(), (Short_t*)ptr);
      else if (back == 's')
        m_RootTree->Branch(branch.c_str(), (UShort_t*)ptr);
      else if (back == 'I')
        m_RootTree->Branch(branch.c_str(), (Int_t*)ptr);
      else if (back == 'i')
        m_RootTree->Branch(branch.c_str(), (UInt_t*)ptr);
      else if (back == 'F')
        m_RootTree->Branch(branch.c_str(), (Float_t*)ptr);
      else if (back == 'f')
        m_RootTree->Branch(branch.c_str(), (Float16_t*)ptr);
      else if (back == 'D')
        m_RootTree->Branch(branch.c_str(), (Double_t*)ptr);
      else if (back == 'd')
        m_RootTree->Branch(branch.c_str(), (Double32_t*)ptr);
      else if (back == 'L')
        m_RootTree->Branch(branch.c_str(), (Long64_t*)ptr);
      else if (back == 'l')
        m_RootTree->Branch(branch.c_str(), (ULong64_t*)ptr);
      else if (back == 'G')
        m_RootTree->Branch(branch.c_str(), (Long_t*)ptr);
      else if (back == 'g')
        m_RootTree->Branch(branch.c_str(), (ULong_t*)ptr);
      else if (back == 'O')
        m_RootTree->Branch(branch.c_str(), (bool*)ptr);
      else {
        message("yellow", "root", "nptool::RootOutput", " Fail to attach branch " + branch + " , type unrecognised");
        return;
      }
      message("green", "root", "nptool::RootOutput", branch + " attached to output tree");
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
void RootOutput::Fill() {
  if (!m_TreeServer)
    m_RootTree->Fill();
  else
    m_TreeServer->Fill();
}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
  std::shared_ptr<nptool::VDataOutput> ConstructDataOutput() { return std::make_shared<RootOutput>(); }
}

#ifndef NPRootInput_hh
#define NPRootInput_hh 1
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

// C++ header
#include <map>
#include <memory>
#include <string>
#include <vector>

// ROOT headers
#include "TChain.h"
#include "TFile.h"

// nptool
#include "NPInputParser.h"
#include "NPRootTreeClient.h"
#include "NPVDataInput.h"

// std::shared_ptr<nptool::VDataInput> ConstructDataInput();
namespace nptool {
  class RootInput : public nptool::VDataInput {
   public:
    // Constructor
    RootInput() {
      m_NumberOfFriend = 0;
      m_RootChain = nullptr;
      m_RootFile = nullptr;
      m_TreeClient = nullptr;
      m_Entries = 0;
      m_CurrentEntry = 0;
    };

    // Destructor
    ~RootInput();

   public: // inheritted from VDataInput, must be implemented
    // Initialise the input
    void Init(std::vector<std::string> arg);
    // use to attach link an object to the input
    void Attach(std::string, std::string, void*);
    // return false on last entry
    bool GetNextEntry();
    // return the current entry read
    long long GetCurrentEntryNumber() { return m_CurrentEntry; };
    // return the number of entries in the file
    long long GetEntries() { return m_Entries; };
    // return true if the entry contain configuration file
    bool HasConfigFile() { return false; };

   public:
    std::string DumpAsciiFile(const char* type, const char* folder = "./.tmp");

   public:
    // Return the private chain and file
    TChain* GetChain() { return m_RootChain; }
    TFile* GetFile() { return m_RootFile; }
    void SetChain(TChain* c) { m_RootChain = c; }

    // Read Input file
    void ReadInputFile(nptool::InputParser& parser);
    void ReadTreeFile(std::string path);

   private:
    long long m_Entries;
    long long m_CurrentEntry;

    TChain* m_RootChain;
    TFile* m_RootFile;
    std::string m_TreeName;              // the main tree name
    std::vector<std::string> m_TreePath; // the main tree path
    nptool::root::RootTreeClient* m_TreeClient;

    // Used for user made tree friends
    //// list of Friends tree indexed by their tree name
    std::multimap<std::string, std::vector<std::string>> m_FriendsPath;

    // Used for .tree file input
    //// list of Friends tree path indexed by number
    std::map<unsigned int, std::vector<std::string>> m_FriendsTreePath;
    int m_NumberOfFriend;
  };
} // namespace nptool
#endif // ROOTINPUT_hh

#ifndef NPRootOutput_hh
#define NPRootOutput_hh 1
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
#include "TFile.h"
#include "TTree.h"
#include "TMacro.h"

// nptool
#include "NPVDataOutput.h"

// Root plugin
#include "NPRootTreeServer.h"

namespace nptool {
  class RootOutput : public nptool::VDataOutput {
   public:
    // Constructor
    RootOutput() {
      m_RootTree = nullptr;
      m_RootFile = nullptr;
    };

    // Destructor
    ~RootOutput();

   public: // inheritted from VDataOutput, must be implemented
    // Initialise the input
    void Init(std::vector<std::string> arg);
    // use to attach link an object to the input
    void Attach(std::string, std::string, void*);
    // write current event to file
    void Fill();
    long long GetEntries() { return m_RootTree->GetEntries(); };

   public:
    void AttachAsciiFile(std::string name, std::string title, std::string path);

   public:
    // Return the private chain and file
    TTree* GetTree() { return m_RootTree; }
    TFile* GetFile() { return m_RootFile; }

   private:
    TTree* m_RootTree;
    TFile* m_RootFile;
    std::shared_ptr<nptool::root::RootTreeServer> m_TreeServer;
    std::string m_TreeName; // the tree name
    std::string m_TreePath; // the tree path
  };
}
// namespace nptool
#endif // NPRootOutput_hh

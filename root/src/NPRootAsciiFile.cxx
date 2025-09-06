/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : january 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class allows to store any ascii file in a ROOT TTree.            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// Class headers
#include "NPRootAsciiFile.h"

// ROOT headers
#include "TList.h"

// C++ headers
#include <fstream>
#include <iostream>

using namespace std;
using namespace nptool;
ClassImp(nptool::RootAsciiFile)

////////////////////////////////////////////////////////////////////////////////
RootAsciiFile::RootAsciiFile()
: TMacro() {}

////////////////////////////////////////////////////////////////////////////////
RootAsciiFile::RootAsciiFile(const char* name, const char* title, const char* inputAsciiFile) : TMacro(name, title) {
  ReadFile(inputAsciiFile);
}

////////////////////////////////////////////////////////////////////////////////
RootAsciiFile::RootAsciiFile(string inputAsciiFile) {
  TString name(inputAsciiFile);
  name(TRegexp(".*/")) = "";
  name.ReplaceAll(".",":");
  name(TRegexp(".*:")) = "";
  SetName(name);

  SetTitle(inputAsciiFile.c_str());
  ReadFile(inputAsciiFile.c_str());
}

////////////////////////////////////////////////////////////////////////////////
RootAsciiFile::~RootAsciiFile() {}

////////////////////////////////////////////////////////////////////////////////
bool RootAsciiFile::IsEmpty() const {
  if (GetListOfLines()->GetSize() == 0)
    return true;
  else
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void RootAsciiFile::Append(const char* inputAsciiFile) {
  ReadFile(inputAsciiFile); // TODO
}

////////////////////////////////////////////////////////////////////////////////
void RootAsciiFile::WriteToFile(const char* outputAsciiFile) const {
  ofstream outputStream(outputAsciiFile);

  if (!fLines) return ;
  for(TObject *obj: *GetListOfLines())
    outputStream << obj->GetName() << endl;

  outputStream.close();
}

////////////////////////////////////////////////////////////////////////////////
void RootAsciiFile::WriteToFile(const char* outputAsciiFile, UInt_t begin, UInt_t end) const {
  ofstream outputStream(outputAsciiFile);
  if (!fLines) return ;
  for (UInt_t i = 0; i < fLines->GetEntries(); i++) {
    if (i < begin || i > end)
      continue;
     outputStream << fLines->At(i)->GetName() << endl;
  }
  outputStream.close();
}



////////////////////////////////////////////////////////////////////////////////
void RootAsciiFile::Print(UInt_t begin, UInt_t end) const {
  if (!fLines) return ;
  for (UInt_t i = 0; i < fLines->GetEntries(); i++) {
    if (i < begin || i > end)
      continue;
    cout << fLines->At(i)->GetName() << endl;
  }
}


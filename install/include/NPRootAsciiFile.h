#ifndef NPRootAsciiFile_h
#define NPRootAsciiFile_h
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
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

// ROOT headers
#include "TNamed.h"
#include "TMacro.h"
#include "TString.h"
#include "TRegexp.h"


// C++ headers
#include <string>
#include <vector>

namespace nptool {
  class RootAsciiFile : public TMacro {

//  private:
//    std::vector<std::string> fLines;

   public:
    RootAsciiFile();
    RootAsciiFile(const char* name, const char* title, const char* inputAsciiFile);
    RootAsciiFile(std::string name, std::string title, std::string inputAsciiFile){ RootAsciiFile(name.c_str(), title.c_str(), inputAsciiFile.c_str()); };
    RootAsciiFile(std::string inputAsciiFile);
    RootAsciiFile(const char* inputAsciiFile){ RootAsciiFile(std::string(inputAsciiFile));};
    virtual ~RootAsciiFile();

    void Append(const char* inputAsciiFile); // TODO
    void AppendLine(const char* AsciiLine) { AddLine(AsciiLine) ;} ;  
    void WriteToFile(const char* outputAsciiFile) const; 
    void WriteToFile(const char* outputAsciiFile, UInt_t begin, UInt_t end) const; 
    void Print(UInt_t begin, UInt_t end) const; 
    bool IsEmpty() const; 

    ClassDef(RootAsciiFile, 1); // Class RootAsciiFile for storing ascii text
  };
} // namespace nptool
#endif

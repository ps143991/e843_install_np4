#ifndef __ZDDDATA__
#define __ZDDDATA__
/*****************************************************************************
 * Copyright (C) 2009-2022   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Hugo Jacob  contact address: hjacob@ijclab.in2p3.fr                        *
 *                                                                           *
 * Creation Date  : October 2022                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold ZDD Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL
#include <iostream>
#include <vector>
using namespace std;

// ROOT
#include "TObject.h"

class TZDDData : public TObject {
  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order
  // to allow multiplicity treatment
 private:
  // IC
  vector<uint16_t> fZDD_IC_E;
  vector<uint16_t> fZDD_IC_N;
  vector<uint64_t> fZDD_IC_TS;

  // Plastic
  vector<uint16_t> fZDD_PL_E;
  vector<uint16_t> fZDD_PL_N;
  vector<uint64_t> fZDD_PL_TS;

  // DC
  vector<uint16_t> fZDD_DC_E;
  vector<uint16_t> fZDD_DC_N;
  vector<uint64_t> fZDD_DC_TS;

  // EXOZDD
  vector<uint16_t> fZDD_EXO_E;
  vector<uint16_t> fZDD_EXO_N;
  vector<uint64_t> fZDD_EXO_TS;

  //////////////////////////////////////////////////////////////
  // Constructor and destructor
 public:
  TZDDData();
  ~TZDDData();

  //////////////////////////////////////////////////////////////
  // Inherited from TObject and overriden to avoid warnings
 public:
  void Clear();
  void Clear(const Option_t*){};
  void Dump() const;

  //////////////////////////////////////////////////////////////
  // Getters and Setters
  // Prefer inline declaration to avoid unnecessary called of
  // frequently used methods
  // add //! to avoid ROOT creating dictionnary for the methods
 public:
  //////////////////////    SETTERS    ////////////////////////
  // Energy
  inline void SetZDDIC(const uint16_t& DetNumb, const uint16_t& Energy, const uint64_t& TimeStamp) {
    fZDD_IC_E.push_back(Energy);
    fZDD_IC_N.push_back(DetNumb);
    fZDD_IC_TS.push_back(TimeStamp);
  }; //!
  inline void SetZDDPL(const uint16_t& DetNumb, const uint16_t& Energy, const uint64_t& TimeStamp) {
    fZDD_PL_E.push_back(Energy);
    fZDD_PL_N.push_back(DetNumb);
    fZDD_PL_TS.push_back(TimeStamp);
  }; //!
  inline void SetZDDDC(const uint16_t& DetNumb, const uint16_t& Energy, const uint64_t& TimeStamp) {
    fZDD_DC_E.push_back(Energy);
    fZDD_DC_N.push_back(DetNumb);
    fZDD_DC_TS.push_back(TimeStamp);
  }; //!
  inline void SetZDDEXO(const uint16_t& DetNumb, const uint16_t& Energy, const uint64_t& TimeStamp) {
    fZDD_EXO_E.push_back(Energy);
    fZDD_EXO_N.push_back(DetNumb);
    fZDD_EXO_TS.push_back(TimeStamp);
  }; //!

  //////////////////////    GETTERS    ////////////////////////
  inline uint16_t	GetZDD_ICMult()		                const {return fZDD_IC_E.size();}
  inline uint16_t GetZDD_ICE(const uint16_t& i) const { return fZDD_IC_E[i]; }
  inline uint16_t GetZDD_ICN(const uint16_t& i) const { return fZDD_IC_N[i]; }
  inline uint64_t GetZDD_ICTS(const uint16_t& i) const { return fZDD_IC_TS[i]; }
  
  inline uint16_t	GetZDD_PLMult()		                const {return fZDD_PL_E.size();}
  inline uint16_t GetZDD_PLE(const uint16_t& i) const { return fZDD_PL_E[i]; }
  inline uint16_t GetZDD_PLN(const uint16_t& i) const { return fZDD_PL_N[i]; }
  inline uint64_t GetZDD_PLTS(const uint16_t& i) const { return fZDD_PL_TS[i]; }
  
  inline uint16_t	GetZDD_DCMult()		                const {return fZDD_DC_E.size();}
  inline uint16_t GetZDD_DCE(const uint16_t& i) const { return fZDD_DC_E[i]; }
  inline uint16_t GetZDD_DCN(const uint16_t& i) const { return fZDD_DC_N[i]; }
  inline uint64_t GetZDD_DCTS(const uint16_t& i) const { return fZDD_DC_TS[i]; }
  
  inline uint16_t	GetZDD_EXOMult()		                const {return fZDD_EXO_E.size();}
  inline uint16_t GetZDD_EXOE(const uint16_t& i) const { return fZDD_EXO_E[i]; }
  inline uint16_t GetZDD_EXON(const uint16_t& i) const { return fZDD_EXO_N[i]; }
  inline uint64_t GetZDD_EXOTS(const uint16_t& i) const { return fZDD_EXO_TS[i]; }

  //////////////////////////////////////////////////////////////
  // Required for ROOT dictionnary
  ClassDef(TZDDData, 1) // ZDDData structure
};

#endif
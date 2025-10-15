/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girard-alcindor@ijclab.in2p3.fr                          *
 *                                                                           *
 * Creation Date  :    08/03/24                                              *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This class is heavily based on the nptool v3 EXOGAM detector              *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "ExogamData.h"
exogam::ExogamData::ExogamData() { Clear();}

exogam::ExogamData::~ExogamData() {}

void exogam::ExogamData::Clear() {
  fExo_Crystal.clear();
  fExo_E.clear();
  fExo_E_HG.clear(); // High gain x20
  fExo_TS.clear();
  fExo_TDC.clear();
  fExo_BGO.clear();
  fExo_CsI.clear();
  fExo_Outer1.clear();
  fExo_Outer2.clear();
  fExo_Outer3.clear();
  fExo_Outer4.clear();
}

void exogam::ExogamData::Dump() const {
}

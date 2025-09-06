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
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TacData.h"
tac::TacData::TacData() {}

tac::TacData::~TacData() {}

void tac::TacData::Clear() {
  fTAC_Time.clear();
  fTAC_N.clear();
  fTAC_Name.clear();
  fTAC_TS.clear();
}

void tac::TacData::Dump() const {}

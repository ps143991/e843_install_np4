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
 * This class is heavily based on the nptool v3 CATS detector                *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "CatsData.h"
cats::CatsData::CatsData() {}

cats::CatsData::~CatsData() {}

void cats::CatsData::Clear() {
  // X
  fCATS_DetX.clear();
  fCATS_StripX.clear();
  fCATS_ChargeX.clear();
  // Y
  fCATS_DetY.clear();
  fCATS_StripY.clear();
  fCATS_ChargeY.clear();
  // (Qfil)
  fCATS_DetQ.clear();
  fCATS_Charge.clear();

  // TS
  fCATS_TimeStamp.clear();
}

void cats::CatsData::Dump() const {

  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

  // X
  cout << "CATS_MultX = " << fCATS_DetX.size() << endl;
  for (unsigned short i = 0; i < fCATS_DetX.size(); i++)
    cout << "DetX: " << fCATS_DetX[i] << " StripX: " << fCATS_StripX[i] << " ChargeX: " << fCATS_ChargeX[i] << endl;
  // Y
  cout << "CATS_MultY = " << fCATS_DetY.size() << endl;
  for (unsigned short i = 0; i < fCATS_DetY.size(); i++)
    cout << "DetY: " << fCATS_DetY[i] << " StripY: " << fCATS_StripY[i] << " EnergyY: " << fCATS_ChargeY[i] << endl;
  // (Qfil)
  cout << "CATS_MultQ = " << fCATS_DetQ.size() << endl;
  for (unsigned short i = 0; i < fCATS_DetQ.size(); i++)
    cout << "DetQ: " << fCATS_DetQ[i] << " Charge: " << fCATS_Charge[i] << endl;
}

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
#include "CatsPhysics.h"

///////////////////////////////////////////////////////////////
void cats::CatsPhysics::Clear(){  
  DetNumber.clear(); 
  ChargeX.clear(); 
  PositionX.clear();
  ChargeY.clear(); 
  PositionY.clear();
  PositionZ.clear();
  Mult1X.clear();
  Mult1Y.clear();
  Mult2X.clear();
  Mult2Y.clear();
  StripNumberX.clear();
  StripNumberY.clear();

  CalStripNumberX.clear();
  CalStripNumberY.clear();
  CalDetNumberX.clear();
  CalDetNumberY.clear();
  CalChargeX.clear();
  CalChargeY.clear();

   // TS
  CATS_TS.clear();

}

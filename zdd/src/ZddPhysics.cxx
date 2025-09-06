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
 * This class is heavily based on the nptool v3 ZDD detector                 *
 *                                                                           *
 *****************************************************************************/
#include "ZddPhysics.h"

///////////////////////////////////////////////////////////////////////////
void zdd::ZddPhysics::Clear() {
  IC_Nbr.clear();
  IC_E.clear();
  IC_TS.clear();

  PL_Nbr.clear();
  PL_E.clear();
  PL_TS.clear();

  ICSum.clear();
  ICSumMult.clear();
  ICSumTS.clear();
  ICSumHere.clear();
  ICSumGroupMult.clear();

  // SortIC.clear();
  // SortPL.clear();
  DC_Nbr.clear();
  DC_DriftTime.clear();

  Exo_Nbr.clear();
  Exo_E.clear();
  Exo_TS.clear();
}

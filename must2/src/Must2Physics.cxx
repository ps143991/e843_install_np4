#include "Must2Physics.h"

/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girard-alcindor@ijclab.in2p3.fr                          *
 *                                                                           *
 * Creation Date  : 08/03/24                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This class hold MUST2 Physics data                                        *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This class is heavily based on the nptool v3 MUST2 detector               *
 *                                                                           *
 *****************************************************************************/

void must2::Must2Physics::Clear() {

  TelescopeNumber.clear();
  EventType.clear();
  TotalEnergy.clear();

  // Si X
  Si_E.clear();
  Si_T.clear();
  Si_X.clear();
  Si_Y.clear();

  // Si(Li)
  SiLi_E.clear();
  SiLi_T.clear();
  SiLi_N.clear();

  // CsI
  CsI_E.clear();
  CsI_E_Raw.clear();
  CsI_T.clear();
  CsI_N.clear();

  Si_EX.clear();
  Si_TX.clear();
  Si_EY.clear();
  Si_TY.clear();
  TelescopeNumber_X.clear();
  TelescopeNumber_Y.clear();

  M2_TS.clear();
}

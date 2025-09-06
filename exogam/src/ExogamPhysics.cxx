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
#include "ExogamPhysics.h"

///////////////////////////////////////////////////////////////////////////
void exogam::ExogamPhysics::Clear() {
  // Only threshold and cal applied to exogam
  // Single hit information of same sizes
  Exo_E.clear();
  Exo_EHG.clear();
  Exo_Outer1.clear();
  Exo_Outer2.clear();
  Exo_Outer3.clear();
  Exo_Outer4.clear();
  Exo_FlangeN.clear();
  Exo_CrystalN.clear();
  Exo_TS.clear();
  Exo_TDC.clear();

  // Add_Back variable, here size of vectors are different from previous ones
  // after add-back
  Exo_EAddBack.clear();
  Exo_FlangeNAddBack.clear();
  Exo_SizePreAddBack.clear();
  Exo_CrystalNAddBack.clear();
  Exo_OuterNAddBack.clear();
  Exo_Theta.clear();
  Exo_Phi.clear();
}

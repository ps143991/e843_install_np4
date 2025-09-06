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
#ifndef NPExogamPhysics_H
#define NPExogamPhysics_H

#include <vector>
namespace exogam {
class ExogamPhysics {
public:
  ExogamPhysics(){};
  ~ExogamPhysics(){};

public:
  void Clear();
  void Dump() const;

  // Only threshold and cal applied to exogam
  // Single hit information of same sizes
  std::vector<double> Exo_E;
  std::vector<double> Exo_EHG;
  std::vector<double> Exo_Outer1;
  std::vector<double> Exo_Outer2;
  std::vector<double> Exo_Outer3;
  std::vector<double> Exo_Outer4;
  std::vector<unsigned int> Exo_FlangeN;
  std::vector<unsigned int> Exo_CrystalN;
  std::vector<unsigned int> Exo_TDC;
  std::vector<unsigned long long> Exo_TS;
  
  // Add_Back variable, here size of vectors are different from previous ones after add-back
  std::vector<double> Exo_EAddBack;
  std::vector<unsigned int> Exo_FlangeNAddBack;
  std::vector<unsigned int> Exo_SizePreAddBack;
  std::vector<unsigned int> Exo_CrystalNAddBack;
  std::vector<int> Exo_OuterNAddBack;
  std::vector<double> Exo_Theta;
  std::vector<double> Exo_Phi;

};
} // namespace exogam
#endif

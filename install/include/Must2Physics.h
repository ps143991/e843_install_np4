#ifndef NPMust2Physics_H
#define NPMust2Physics_H

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

#include <vector>
namespace must2 {
class Must2Physics {
public:
  Must2Physics(){};
  ~Must2Physics(){};

public:
  // Telescope
  std::vector<int> TelescopeNumber;

  //   Si
  std::vector<double> Si_E;
  std::vector<double> Si_T;
  std::vector<int>    Si_X;
  std::vector<int>    Si_Y;

  // Use for checking purpose
  std::vector<double> Si_EX;
  std::vector<double> Si_TX;
  std::vector<double> Si_EY;
  std::vector<double> Si_TY;
  std::vector<int>    TelescopeNumber_X;
  std::vector<int>    TelescopeNumber_Y;

  //   Si(Li)
  std::vector<double> SiLi_E;
  std::vector<double> SiLi_T;
  std::vector<int>    SiLi_N;

  //   CsI
  std::vector<double> CsI_E;
  std::vector<double> CsI_E_Raw;
  std::vector<double> CsI_T;
  std::vector<int>    CsI_N;

  // TS
  std::vector<unsigned long long> M2_TS;

  // Physical Value
  std::vector<double> TotalEnergy;

  //   Provide a Classification of Event
  std::vector<int> EventType;

  // TVector3 GetPositionOfInteraction(const int i) const;
  // TVector3 GetTelescopeNormal(const int i) const;

  void Clear();
  void Dump() const;
};
} // namespace must2
#endif

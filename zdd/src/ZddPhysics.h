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
#ifndef NPZddPhysics_H
#define NPZddPhysics_H

#include <vector>
namespace zdd {
class ZddPhysics {
public:
  ZddPhysics(){};
  ~ZddPhysics(){};

public:
  void Clear();
  void Dump() const;

  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
  public:
    std::vector<int>      IC_Nbr;
    std::vector<double>   IC_E;
    std::vector<unsigned long long>   IC_TS;

    // Sum of group of IC 
    std::vector<double> ICSum;
    // Multiplicity of IC groups
    std::vector<unsigned short> ICSumGroupMult;
    // Number of IC in group
    std::vector<unsigned short> ICSumMult;
    // First TS of IC group
    std::vector<unsigned long long> ICSumTS;
    // 1111 all are here, 1011, IC 3 missing, 0011 IC 3 and 4 missing...
    std::vector<unsigned int> ICSumHere;

    std::vector<int>      PL_Nbr;
    std::vector<double>   PL_E;
    std::vector<unsigned long long>   PL_TS;

    std::vector<int> DC_Nbr;
    std::vector<unsigned int> DC_E;
    std::vector<unsigned long long> DC_TS;
    
    std::vector<double>   DC_DriftTime;

    std::vector<int>      Exo_Nbr;
    std::vector<double>   Exo_E;
    std::vector<unsigned long long>   Exo_TS;

};
} // namespace zdd
#endif

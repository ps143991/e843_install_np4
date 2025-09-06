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
#ifndef NPCatsPhysics_H
#define NPCatsPhysics_H

#include <string>
#include <vector>
namespace cats {
class CatsPhysics {
public:
  CatsPhysics() {};
  ~CatsPhysics() {};

public:
  void Clear();
  void Dump() const;

public:
  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
  std::vector<unsigned short> DetNumber;
  std::vector<double>         PositionX;
  std::vector<double>         PositionY;
  std::vector<double>         PositionZ;
  std::vector<double>         StripNumberX;
  std::vector<double>         StripNumberY;
  std::vector<double>         ChargeX;
  std::vector<double>         ChargeY;
  std::vector<unsigned int>   Mult1X;
  std::vector<unsigned int>   Mult1Y;
  std::vector<unsigned int>   Mult2X;
  std::vector<unsigned int>   Mult2Y;

  double PositionOnTargetX;
  double PositionOnTargetY;
  double PositionOnMask1X;
  double PositionOnMask1Y;
  double PositionOnMask2X;
  double PositionOnMask2Y;
  double m_Zproj;


  // New variables for TestCalibration (to avoid conflicts with normal analysis)
  std::vector<int>    CalStripNumberX;
  std::vector<int>    CalStripNumberY;
  std::vector<int>    CalDetNumberX;
  std::vector<int>    CalDetNumberY;
  std::vector<double> CalChargeX;
  std::vector<double> CalChargeY;

  double CalPositionOnTargetX;
  double CalPositionOnTargetY;
  double CalPositionOnMask1X;
  double CalPositionOnMask1Y;
  double CalPositionOnMask2X;
  double CalPositionOnMask2Y;

  // TS
  std::vector<unsigned long long> CATS_TS;

public:
  /////////////////////           GETTERS           ////////////////////////
  inline unsigned short GetCATSMult() const { return DetNumber.size(); }
  inline unsigned short GetCATSDet(const int& i) const { return DetNumber[i]; }

  inline unsigned short GetCATSStripX(const int& i) const {
    return StripNumberX[i];
  }
  inline double GetCATSChargeX(const int& i) const { return ChargeX[i]; }
  inline double GetCATSPosX(const int& i) const { return PositionX[i]; }

  inline unsigned short GetCATSStripY(const int& i) const {
    return StripNumberY[i];
  }
  inline double GetCATSChargeY(const int& i) const { return ChargeY[i]; }
  inline double GetCATSPosY(const int& i) const { return PositionY[i]; }
};
} // namespace cats
#endif

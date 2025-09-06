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
#ifndef NPCatsData_H
#define NPCatsData_H

#include <vector>
namespace cats {
class CatsData {
private:
public:
  CatsData();
  virtual ~CatsData();

  void Clear();
  void Dump() const;

public:
  // X strips
  std::vector<unsigned short> fCATS_DetX;
  std::vector<unsigned short> fCATS_StripX;
  std::vector<unsigned short> fCATS_ChargeX;

  // Y strips
  std::vector<unsigned short> fCATS_DetY;
  std::vector<unsigned short> fCATS_StripY;
  std::vector<unsigned short> fCATS_ChargeY;

  // Q Fil
  std::vector<unsigned short> fCATS_DetQ;
  std::vector<unsigned short> fCATS_Charge;

  // TS
  std::vector<unsigned long long> fCATS_TimeStamp;

public:
  /////////////////////           SETTERS           ////////////////////////
  // X
  inline void SetStripX(const unsigned short& DetX, const unsigned short& StripX,
      const unsigned short& ChargeX) {
    fCATS_DetX.push_back(DetX);
    fCATS_StripX.push_back(StripX);
    fCATS_ChargeX.push_back(ChargeX);
  }
  // Y
  inline void SetStripY(const unsigned short& DetY, const unsigned short& StripY,
      const unsigned short& ChargeY) {
    fCATS_DetY.push_back(DetY);
    fCATS_StripY.push_back(StripY);
    fCATS_ChargeY.push_back(ChargeY);
  }

  // Q fil
  inline void SetQ(const unsigned short& DetQ, const unsigned short& Charge) {
    fCATS_DetQ.push_back(DetQ);
    fCATS_Charge.push_back(Charge);
  }

  // TimeStamp
  inline void SetCatsTS(const unsigned long long TS) {
    fCATS_TimeStamp.push_back(TS);
  }

  /////////////////////           GETTERS           ////////////////////////
  // X
  inline unsigned short GetCATSMultX() const {  return fCATS_DetX.size(); }
  inline unsigned short GetCATSDetX(const int& i) const { return fCATS_DetX[i]; }
  inline unsigned short GetCATSStripX(const int& i) const {
    return fCATS_StripX[i];
  }
  inline unsigned short GetCATSChargeX(const int& i) const {
    return fCATS_ChargeX[i];
  }
  // Y
  inline unsigned short GetCATSMultY() const { return fCATS_DetY.size(); }
  inline unsigned short GetCATSDetY(const int& i) const { return fCATS_DetY[i]; }
  inline unsigned short GetCATSStripY(const int& i) const {
    return fCATS_StripY[i];
  }
  inline unsigned short GetCATSChargeY(const int& i) const {
    return fCATS_ChargeY[i];
  }
  // Q fil
  inline unsigned short GetCATSMultQ() const { return fCATS_DetQ.size(); }
  inline unsigned short GetCATSDetQ(const int& i) const { return fCATS_DetQ[i]; }
  inline unsigned short GetCATSCharge(const int& i) const {
    return fCATS_Charge[i];
  }

  // TimeStamp
  unsigned long long GetCatsTS(const int& i){return fCATS_TimeStamp[i];}
};
} // namespace cats
#endif

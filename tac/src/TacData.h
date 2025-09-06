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
 *                                                                           *
 *****************************************************************************/
#ifndef NPTacData_H
#define NPTacData_H

#include <vector>
namespace tac {
class TacData {
private:
public:
  TacData();
  virtual ~TacData();

  void Clear();
  void Dump() const;

  //////////////////////////////////////////////////////////////
  // data members are hold into vectors in order
  // to allow multiplicity treatment
private:
  // Energy
  std::vector<unsigned int>       fTAC_Time;
  std::vector<unsigned int>       fTAC_N;
  std::vector<std::string>        fTAC_Name;
  std::vector<unsigned long long> fTAC_TS;

  //////////////////////////////////////////////////////////////
  // Getters and Setters
  // Prefer inline declaration to avoid unnecessary called of
  // frequently used methods
  // add //! to avoid ROOT creating dictionnary for the methods
public:
  //////////////////////    SETTERS    ////////////////////////
  // Channel -> N
  inline void SetTAC(const unsigned int& Channel, const unsigned int& Time,
                     const unsigned long long& TS, const std::string& Name) {
    fTAC_N.push_back(Channel);
    fTAC_Name.push_back(Name);
    fTAC_Time.push_back(Time);
    fTAC_TS.push_back(TS);
  }; //!

  //////////////////////    GETTERS    ////////////////////////
  // Energy
  inline unsigned int GetTAC_Mult() const { return fTAC_Time.size(); }
  inline unsigned int GetTAC_Time(const unsigned int& i) const {
    return fTAC_Time[i];
  } //!
  inline unsigned int GetTAC_N(const unsigned int& i) const {
    return fTAC_N[i];
  } //!
  inline std::string GetTAC_Name(const unsigned int& i) const {
    return fTAC_Name[i];
  } //!
  inline unsigned long long GetTAC_TS(const unsigned long long& i) const {
    return fTAC_TS[i];
  } //!
};
} // namespace tac
#endif

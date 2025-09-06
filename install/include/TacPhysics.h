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
#ifndef NPTacPhysics_H
#define NPTacPhysics_H

#include <vector>
#include <string>
namespace tac {
class TacPhysics {
public:
  TacPhysics(){};
  ~TacPhysics(){};

public:
  void Clear();
  void Dump() const;

  //////////////////////////////////////////////////////////////
  // data obtained after BuildPhysicalEvent() and stored in
  // output ROOT file
public:
  std::vector<unsigned int>       TAC_Time;
  std::vector<std::string>        TAC_Name;
  std::vector<unsigned long long> TAC_TS;
};
} // namespace tac
#endif

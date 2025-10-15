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
#ifndef NPExogamData_H
#define NPExogamData_H

#include <vector>
namespace exogam {
  class ExogamData {
   private:

   public:
    ExogamData();
    virtual ~ExogamData();

    void Clear();
    void Dump() const;

 public:
  std::vector<unsigned int>       fExo_Crystal;
  std::vector<unsigned int>       fExo_E;
  std::vector<unsigned int>       fExo_E_HG; // High gain x20
  std::vector<unsigned long long> fExo_TS;
  std::vector<unsigned int>       fExo_TDC;
  std::vector<unsigned int>       fExo_BGO;
  std::vector<unsigned int>       fExo_CsI;
  std::vector<unsigned int>       fExo_Outer1;
  std::vector<unsigned int>       fExo_Outer2;
  std::vector<unsigned int>       fExo_Outer3;
  std::vector<unsigned int>       fExo_Outer4;

  std::vector<unsigned int> fExo_LastTDC;

  /////////////////////           SETTERS           ////////////////////////
  inline void SetExo(const unsigned int& Crystal,const unsigned int& Energy,
  const unsigned int& Energy_HG,const unsigned long long& TS,const unsigned int& TDC,
  const unsigned int& BGO,const unsigned int& CsI,const unsigned int& Outer1,
  const unsigned int& Outer2,const unsigned int& Outer3,const unsigned int& Outer4) { 
  fExo_Crystal.push_back(Crystal);
  fExo_E.push_back(Energy);
  fExo_E_HG.push_back(Energy_HG);
  fExo_TS.push_back(TS);
  fExo_TDC.push_back(TDC);
  fExo_BGO.push_back(BGO);
  fExo_CsI.push_back(CsI);
  fExo_Outer1.push_back(Outer1);
  fExo_Outer2.push_back(Outer2);
  fExo_Outer3.push_back(Outer3);
  fExo_Outer4.push_back(Outer4);
  }
  /////////////////////           GETTERS           ////////////////////////
  inline unsigned int       GetExoMult() { return fExo_Crystal.size(); }
  inline unsigned int       GetExoCrystal(const unsigned int& i) const  { return fExo_Crystal[i]; }
  inline unsigned int       GetExoE(const unsigned int& i) const  { return fExo_E[i]; }
  inline unsigned int       GetExoEHG(const unsigned int& i) const  { return fExo_E_HG[i]; }
  inline unsigned long long GetExoTS(const unsigned int& i) const  { return fExo_TS[i]; }
  inline unsigned int       GetExoTDC(const unsigned int& i) const  { return fExo_TDC[i]; }
  inline unsigned int       GetExoBGO(const unsigned int& i) const  { return fExo_BGO[i]; }
  inline unsigned int       GetExoCsI(const unsigned int& i) const  { return fExo_CsI[i]; }
  inline unsigned int       GetExoOuter1(const unsigned int& i) const { return fExo_Outer1[i]; }
  inline unsigned int       GetExoOuter2(const unsigned int& i) const  { return fExo_Outer2[i]; }
  inline unsigned int       GetExoOuter3(const unsigned int& i) const  { return fExo_Outer3[i]; }
  inline unsigned int       GetExoOuter4(const unsigned int& i) const  { return fExo_Outer4[i]; }


  };
} // namespace exogam
#endif

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
#ifndef NPZddData_H
#define NPZddData_H

#include <vector>
#include <iostream>

namespace zdd {
class ZddData {
private:
public:
  ZddData();
  virtual ~ZddData();

  void Clear();
  void Dump() const;

  //////////////////////////////////////////////////////////////
  // data members are hold into std::vectors in order
  // to allow multiplicity treatment
public:
  // IC
  std::vector<unsigned int>       fZDD_IC_E;
  std::vector<unsigned int>       fZDD_IC_N;
  std::vector<unsigned long long> fZDD_IC_TS;

  // Plastic
  std::vector<unsigned int>       fZDD_PL_E;
  std::vector<unsigned int>       fZDD_PL_N;
  std::vector<unsigned long long> fZDD_PL_TS;
  //std::vector<unsigned int>       central_PlR_E;
  std::vector<unsigned long long> central_PlR_TS;

  // DC
  std::vector<unsigned int> fZDD_DC_E;
  std::vector<unsigned int> fZDD_DC_N;
  std::vector<unsigned long long> fZDD_DC_TS;

  std::vector<unsigned int> fZDD_DC_1_E;
  std::vector<unsigned int> fZDD_DC_2_E;
  std::vector<unsigned int> fZDD_DC_3_E;
  std::vector<unsigned int> fZDD_DC_4_E;

  std::vector<unsigned long long> fZDD_DC_1_TS;
  std::vector<unsigned long long> fZDD_DC_2_TS;
  std::vector<unsigned long long> fZDD_DC_3_TS;
  std::vector<unsigned long long> fZDD_DC_4_TS;

  // EXOZDD
  std::vector<unsigned int>       fZDD_EXO_E;
  std::vector<unsigned int>       fZDD_EXO_N;
  std::vector<unsigned long long> fZDD_EXO_TS;
public:
  std::vector<unsigned long long>       fZDD_EXO_Time;

  //////////////////////////////////////////////////////////////
  // Getters and Setters
  // Prefer inline declaration to avoid unnecessary called of
  // frequently used methods
  // add //! to avoid ROOT creating dictionnary for the methods
public:
  //////////////////////    SETTERS    ////////////////////////
  inline void SetZDDIC(const unsigned int& DetNumb, const unsigned int& Energy,
                       const unsigned long long& TimeStamp) {
    fZDD_IC_E.push_back(Energy);
    fZDD_IC_N.push_back(DetNumb);
    fZDD_IC_TS.push_back(TimeStamp);
  }; //!
  inline void SetZDDPL(const unsigned int& DetNumb, const unsigned int& Energy,
                       const unsigned long long& TimeStamp) {
    fZDD_PL_E.push_back(Energy);
    fZDD_PL_N.push_back(DetNumb);
    fZDD_PL_TS.push_back(TimeStamp);
    if (DetNumb==2){central_PlR_TS.push_back(TimeStamp);}

    //std::cout<<TimeStamp<< " &&&&&&&&" << std::endl;
  }; //!

  //void SetZDDDC(const unsigned int& DetNumb, const unsigned int& Energy, const unsigned long long& TimeStamp);

  inline void SetZDDDC(const unsigned int& DetNumb, const unsigned int& Energy, const unsigned long long& TimeStamp) {
    //std::cout<<"PS: debug: setzddc ran with values: "<<DetNumb<<" "<<Energy<<" "<<TimeStamp<<std::endl;
    if(DetNumb==0){fZDD_DC_1_E.push_back(Energy); fZDD_DC_1_TS.push_back(TimeStamp);}
    if(DetNumb==1){fZDD_DC_2_E.push_back(Energy); fZDD_DC_2_TS.push_back(TimeStamp);}
    if(DetNumb==2){fZDD_DC_3_E.push_back(Energy); fZDD_DC_3_TS.push_back(TimeStamp);}
    if(DetNumb==3){fZDD_DC_4_E.push_back(Energy); fZDD_DC_4_TS.push_back(TimeStamp);}
    //if(fZDD_DC_E.size()!=0){std::cout << " DATA HERE, size" << GetZDD_DCMult() << " " << fZDD_DC_E.size() << std::endl;}
    fZDD_DC_E.push_back(Energy);
    fZDD_DC_N.push_back(DetNumb);
    fZDD_DC_TS.push_back(TimeStamp);
    //std::cout<<"Size of fZDD_DC_E is: PS "<<fZDD_DC_E.size()<<std::endl;
    //std::cout<<"Size of fZDD_DC_1_E is: PS "<<fZDD_DC_1_E.size()<<std::endl;
  };


  inline void SetZDDEXO(const unsigned int& DetNumb, const unsigned int& Energy,
                        //const unsigned long long& TimeStamp) {
                        const unsigned long long& TimeStamp, 
                        const unsigned long long& Time) {

//	std::cout << " I AM GETTING -------------" << std::endl;
//	std::cout << DetNumb << std::endl;
//	std::cout << Energy << std::endl;
//	std::cout << TimeStamp << std::endl;
//	std::cout << Time << std::endl;
//	std::cout << "type of time = "<< typeid(Time).name() << std::endl;

    fZDD_EXO_E.push_back(Energy);
    fZDD_EXO_N.push_back(DetNumb);
    fZDD_EXO_TS.push_back(TimeStamp);
    fZDD_EXO_Time.push_back(Time);
    // std::cout<<"setting zdd exo: "<<DetNumb<<" "<<Energy<<" "<<TimeStamp<<" "<<Time<<std::endl;
  }; //!

  //////////////////////    GETTERS    ////////////////////////
  inline unsigned int GetZDD_ICMult() const { return fZDD_IC_E.size();  }
  inline unsigned int GetZDD_ICE(const unsigned int& i) const {

    return fZDD_IC_E[i];
  }
  inline unsigned int GetZDD_ICN(const unsigned int& i) const {
    return fZDD_IC_N[i];
  }
  inline unsigned long long GetZDD_ICTS(const unsigned int& i) const {
    return fZDD_IC_TS[i];
  }

  inline unsigned int GetZDD_PLMult() const { return fZDD_PL_E.size(); }
  inline unsigned int GetZDD_PLE(const unsigned int& i) const {
    return fZDD_PL_E[i];
  }
  inline unsigned int GetZDD_PLN(const unsigned int& i) const {
    return fZDD_PL_N[i];
  }
  inline unsigned long long GetZDD_PLTS(const unsigned int& i) const {
    return fZDD_PL_TS[i];
  }
  inline unsigned long long Get_Central_Right_PL_TS(const unsigned int& i)const{
    return central_PlR_TS[i];
  }

  //inline unsigned int GetZDD_DCMult() const {  if (fZDD_DC_E.size()>0) {std::cout<<"returned size is: "<<fZDD_DC_E.size()<<std::endl;} return fZDD_DC_E.size();}
  inline unsigned int GetZDD_DCMult() const { return fZDD_DC_E.size();}
  inline unsigned int GetZDD_DCE(const unsigned int& i) const {
    return fZDD_DC_E[i];
  }
  inline unsigned int GetZDD_DCN(const unsigned int& i) const {
    return fZDD_DC_N[i];
  }
  inline unsigned long long GetZDD_DCTS(const unsigned int& i) const {
    return fZDD_DC_TS[i];
  }

  inline unsigned int	GetZDD_DC_Mult(int i) const {
    if (i==0){return fZDD_DC_1_E.size();}
    if (i==1){return fZDD_DC_2_E.size();}
    if (i==2){return fZDD_DC_3_E.size();}
    if (i==3){return fZDD_DC_4_E.size();}
    return 0;
  };
  
  inline unsigned int GetZDD_DC_E(int ps, int i) const {
    if (ps==0){return fZDD_DC_1_E[i];}
    if (ps==1){return fZDD_DC_2_E[i];}
    if (ps==2){return fZDD_DC_3_E[i];}
    if (ps==3){return fZDD_DC_4_E[i];}
    return 0;
  };
  
  inline unsigned long long GetZDD_DC_TS(int ps, int i) const {
    if (ps==0){return fZDD_DC_1_TS[i];}
    if (ps==1){return fZDD_DC_2_TS[i];}
    if (ps==2){return fZDD_DC_3_TS[i];}
    if (ps==3){return fZDD_DC_4_TS[i];}
    return 0;
  };

  inline unsigned int GetZDD_EXOMult() const {/*std::cout<<"returned mult is: "<<fZDD_EXO_E.size()<<std::endl;*/ return fZDD_EXO_E.size(); }

  inline unsigned int GetZDD_EXOE(const unsigned int& i) const {
    return fZDD_EXO_E[i];
  }

  inline unsigned int GetZDD_EXON(const unsigned int& i) const {
    return fZDD_EXO_N[i];
  }

  inline unsigned long long GetZDD_EXOTS(const unsigned int& i) const {
    return fZDD_EXO_TS[i];
  }
  inline unsigned long long GetZDD_EXOTime(const unsigned int& i) const {
    return fZDD_EXO_Time[i];
  }
};
} // namespace zdd
#endif

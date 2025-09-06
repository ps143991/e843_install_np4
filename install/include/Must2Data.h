#ifndef NPMust2Data_H
#define NPMust2Data_H

/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girard-alcindor@ijclab.in2p3.fr                          *
 *                                                                           *
 * Creation Date  : 08/03/24                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This class hold MUST2 raw data                                            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This class is heavily based on the nptool v3 MUST2 detector               *
 *                                                                           *
 *****************************************************************************/

#include <vector>
namespace must2 {
  class Must2Data {
   private:
    // DSSD
    // X strips
    // Energy
    std::vector<unsigned short> fMM_StripXE_DetectorNbr;
    std::vector<unsigned short> fMM_StripXE_StripNbr;
    std::vector<double> fMM_StripXE_Energy;
    // Time
    std::vector<unsigned short> fMM_StripXT_DetectorNbr;
    std::vector<unsigned short> fMM_StripXT_StripNbr;
    std::vector<double> fMM_StripXT_Time;
    // Y strips
    // Energy
    std::vector<unsigned short> fMM_StripYE_DetectorNbr;
    std::vector<unsigned short> fMM_StripYE_StripNbr;
    std::vector<double> fMM_StripYE_Energy;
    // Time
    std::vector<unsigned short> fMM_StripYT_DetectorNbr;
    std::vector<unsigned short> fMM_StripYT_StripNbr;
    std::vector<double> fMM_StripYT_Time;


    // SiLi
    // Energy
    std::vector<unsigned short> fMM_SiLiE_DetectorNbr;
    std::vector<unsigned short> fMM_SiLiE_PadNbr;
    std::vector<double> fMM_SiLiE_Energy;
    // Time
    std::vector<unsigned short> fMM_SiLiT_DetectorNbr;
    std::vector<unsigned short> fMM_SiLiT_PadNbr;
    std::vector<double> fMM_SiLiT_Time;

    // CsI
    // Energy
    std::vector<unsigned short> fMM_CsIE_DetectorNbr;
    std::vector<unsigned short> fMM_CsIE_CristalNbr;
    std::vector<double> fMM_CsIE_Energy;
    // Time
    std::vector<unsigned short> fMM_CsIT_DetectorNbr;
    std::vector<unsigned short> fMM_CsIT_CristalNbr;
    std::vector<double> fMM_CsIT_Time;

   public:
    // TS
    std::vector<unsigned long long> fMM_TimeStamp;

   public:
    Must2Data();
    virtual ~Must2Data();

    void Clear();
    void Dump() const;

    /////////////////////           SETTERS           ////////////////////////
    // DSSD
    // (X,E)
    inline void SetStripXE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy) {
      fMM_StripXE_DetectorNbr.push_back(DetNbr);
      fMM_StripXE_StripNbr.push_back(StripNbr);
      fMM_StripXE_Energy.push_back(Energy);
    }

    // (X,T)
    inline void SetStripXT(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time) {
      fMM_StripXT_DetectorNbr.push_back(DetNbr);
      fMM_StripXT_StripNbr.push_back(StripNbr);
      fMM_StripXT_Time.push_back(Time);
    }
    // (Y,E)
    inline void SetStripYE(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Energy) {
      fMM_StripYE_DetectorNbr.push_back(DetNbr);
      fMM_StripYE_StripNbr.push_back(StripNbr);
      fMM_StripYE_Energy.push_back(Energy);
    }

    // (Y,T)
    inline void SetStripYT(const unsigned short& DetNbr, const unsigned short& StripNbr, const double& Time) {
      fMM_StripYT_DetectorNbr.push_back(DetNbr);
      fMM_StripYT_StripNbr.push_back(StripNbr);
      fMM_StripYT_Time.push_back(Time);
    }

    // SiLi
    // E
    inline void SetSiLiE(const unsigned short& DetNbr, const unsigned short& PadNbr, const double& Energy) {
      fMM_SiLiE_DetectorNbr.push_back(DetNbr);
      fMM_SiLiE_PadNbr.push_back(PadNbr);
      fMM_SiLiE_Energy.push_back(Energy);
    }

    // T
    inline void SetSiLiT(const unsigned short& DetNbr, const unsigned short& PadNbr, const double& Time) {
      fMM_SiLiT_DetectorNbr.push_back(DetNbr);
      fMM_SiLiT_PadNbr.push_back(PadNbr);
      fMM_SiLiT_Time.push_back(Time);
    }

    // CsI
    // E
    inline void SetCsIE(const unsigned short& DetNbr, const unsigned short& CristalNbr, const double& Energy) {
      fMM_CsIE_DetectorNbr.push_back(DetNbr);
      fMM_CsIE_CristalNbr.push_back(CristalNbr);
      fMM_CsIE_Energy.push_back(Energy);
    }

    // T
    inline void SetCsIT(const unsigned short& DetNbr, const unsigned short& CristalNbr, const double& Time) {
      fMM_CsIT_DetectorNbr.push_back(DetNbr);
      fMM_CsIT_CristalNbr.push_back(CristalNbr);
      fMM_CsIT_Time.push_back(Time);
    }

    // TimeStamp
    inline void SetMMTS(const unsigned long long TS) {
      fMM_TimeStamp.push_back(TS);
    }

    /////////////////////           GETTERS           ////////////////////////
    // DSSD
    // (X,E)
    unsigned short GetMMStripXEMult() const { return fMM_StripXE_DetectorNbr.size(); }
    unsigned short GetMMStripXEDetectorNbr(const int& i) const { return fMM_StripXE_DetectorNbr[i]; }
    unsigned short GetMMStripXEStripNbr(const int& i) const { return fMM_StripXE_StripNbr[i]; }
    double GetMMStripXEEnergy(const int& i) const { return fMM_StripXE_Energy[i]; }
    // (X,T)
    unsigned short GetMMStripXTMult() const { return fMM_StripXT_DetectorNbr.size(); }
    unsigned short GetMMStripXTDetectorNbr(const int& i) const { return fMM_StripXT_DetectorNbr[i]; }
    unsigned short GetMMStripXTStripNbr(const int& i) const { return fMM_StripXT_StripNbr[i]; }
    double GetMMStripXTTime(const int& i) const { return fMM_StripXT_Time[i]; }
    // (Y,E)
    unsigned short GetMMStripYEMult() const { return fMM_StripYE_DetectorNbr.size(); }
    unsigned short GetMMStripYEDetectorNbr(const int& i) const { return fMM_StripYE_DetectorNbr[i]; }
    unsigned short GetMMStripYEStripNbr(const int& i) const { return fMM_StripYE_StripNbr[i]; }
    double GetMMStripYEEnergy(const int& i) const { return fMM_StripYE_Energy[i]; }
    // (Y,T)
    unsigned short GetMMStripYTMult() const { return fMM_StripYT_DetectorNbr.size(); }
    unsigned short GetMMStripYTDetectorNbr(const int& i) const { return fMM_StripYT_DetectorNbr[i]; }
    unsigned short GetMMStripYTStripNbr(const int& i) const { return fMM_StripYT_StripNbr[i]; }
    double GetMMStripYTTime(const int& i) const { return fMM_StripYT_Time[i]; }

    // SiLi
    //(E)
    unsigned short GetMMSiLiEMult() const { return fMM_SiLiE_DetectorNbr.size(); }
    unsigned short GetMMSiLiEDetectorNbr(const int& i) const { return fMM_SiLiE_DetectorNbr[i]; }
    unsigned short GetMMSiLiEPadNbr(const int& i) const { return fMM_SiLiE_PadNbr[i]; }
    double GetMMSiLiEEnergy(const int& i) const { return fMM_SiLiE_Energy[i]; }
    //(T)
    unsigned short GetMMSiLiTMult() const { return fMM_SiLiT_DetectorNbr.size(); }
    unsigned short GetMMSiLiTDetectorNbr(const int& i) const { return fMM_SiLiT_DetectorNbr[i]; }
    unsigned short GetMMSiLiTPadNbr(const int& i) const { return fMM_SiLiT_PadNbr[i]; }
    double GetMMSiLiTTime(const int& i) const { return fMM_SiLiT_Time[i]; }

    // CsI
    //(E)
    unsigned short GetMMCsIEMult() const { return fMM_CsIE_DetectorNbr.size(); }
    unsigned short GetMMCsIEDetectorNbr(const int& i) const { return fMM_CsIE_DetectorNbr[i]; }
    unsigned short GetMMCsIECristalNbr(const int& i) const { return fMM_CsIE_CristalNbr[i]; }
    double GetMMCsIEEnergy(const int& i) const { return fMM_CsIE_Energy[i]; }
    //(T)
    unsigned short GetMMCsITMult() const { return fMM_CsIT_DetectorNbr.size(); }
    unsigned short GetMMCsITDetectorNbr(const int& i) const { return fMM_CsIT_DetectorNbr[i]; }
    unsigned short GetMMCsITCristalNbr(const int& i) const { return fMM_CsIT_CristalNbr[i]; }
    double GetMMCsITTime(const int& i) const { return fMM_CsIT_Time[i]; }

    unsigned long long GetMMTS(const int& i){return fMM_TimeStamp[i];}
  };
} // namespace must2
#endif

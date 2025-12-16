#ifndef AnalysisZDD_h
#define AnalysisZDD_h

#include "NPVUserAnalysis.h"
#include "ZddDetector.h"
#include "TacDetector.h"
#include "Must2Detector.h"
#include "ExogamDetector.h"
#include "CatsDetector.h"
#include "EbyEDetector.h"
#include "NPReaction.h"
#include "NPEnergyLoss.h"

#include "NPRootOutput.h"
#include "TH2.h"
#include "Math/Vector3D.h"
#include "TCanvas.h"


using namespace nptool;

namespace user_analysis {
  class Analysis_ZDD : public nptool::VUserAnalysis {
   public:
    Analysis_ZDD(){};
    ~Analysis_ZDD(){};

   public:
    void Init();
    void TreatEvent();
    bool FillOutputCondition() { return true; };
    std::shared_ptr<tac::TacDetector> tac;
    std::shared_ptr<zdd::ZddDetector> zdd;
    void SetDataOutput(std::shared_ptr<nptool::VDataOutput>);
    void SetDataInput(std::shared_ptr<nptool::VDataInput>){};
    void End();
    void Clear();

   private:     //for GATCONF selection
    void TreatGATCONF();
    bool decider = false;
    std::shared_ptr<ebye::EbyEDetector> gatconf;
    unsigned int GATCONFMASTER;



   private:                                        //Variables declaration

          ////ZDD ///

      std::vector<unsigned int> PL_E1;
      std::vector<unsigned int> PL_E2;
      std::vector<unsigned int> PL_E3;
      std::vector<unsigned int> PL_E4;
      std::vector<unsigned int> PL_E5;
      std::vector<unsigned int> PL_E6;
      std::vector<unsigned int> PL_E7;
      std::vector<unsigned int> PL_E8;
      std::vector<unsigned int> PL_E9;
      std::vector<unsigned int> PL_E10;
      std::vector<unsigned int> IC_1;
      std::vector<unsigned int> IC_2;
      std::vector<unsigned int> IC_3;
      std::vector<unsigned int> IC_4;
      std::vector<unsigned int> IC_5;
      std::vector<unsigned int> IC_9;
      std::vector<unsigned int> DC_E1;
      std::vector<unsigned int> DC_E2;
      std::vector<unsigned int> DC_E3;
      std::vector<unsigned int> DC_E4;
      std::vector<unsigned long long> DC_TS1;
      std::vector<unsigned long long> DC_TS2;
      std::vector<unsigned long long> DC_TS3;
      std::vector<unsigned long long> DC_TS4;
      std::vector<unsigned int> EXO_E1;
      std::vector<unsigned int> EXO_E2;
      std::vector<unsigned int> EXO_E3;
      std::vector<unsigned int> EXO_E4;

      double  IC_ZDD_sum;

      double PL_center;
      double PL_centerLog;

    private:                                         // functions declaration
    void TreatZDD();

  };
} // namespace user_analysis
#endif

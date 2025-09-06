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
    std::shared_ptr<cats::CatsDetector> cats;

    void SetDataOutput(std::shared_ptr<nptool::VDataOutput>);
    void SetDataInput(std::shared_ptr<nptool::VDataInput>){};
    void End();
    void Clear();

    private:                                        //Variables declaration


    private:                                         // functions declaration

  };
} // namespace user_analysis
#endif

#ifndef AnalysisTAC_h
#define AnalysisTAC_h

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
  class Analysis_TAC : public nptool::VUserAnalysis {
   public:
    Analysis_TAC(){};
    ~Analysis_TAC(){};

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
    bool UnallocateBeforeBuild(); 
    bool theAuthority = false; // PS: couldn't think of a better name.. i regret this choice...
    void BeamSpot_PL(); 

   private:     //for GATCONF selection
    void TreatGATCONF();
    bool decider = false;
    std::shared_ptr<ebye::EbyEDetector> gatconf;
    unsigned int GATCONFMASTER;


    private:                                        //Variables declaration
    
    

        Double_t per_bin_value1 = (26541 - 19617)/20;                  // for 20 cm
        Double_t per_bin_value2 = (26322 - 19836)/20;                  // for 20 cm
        Double_t per_bin_value3 = (26052 - 20106)/20;                  // for 20 cm
        Double_t per_bin_value4 = (26301 - 19857)/20;                  // for 20 cm
        // Double_t Spot_Plastic_1;
        // Double_t Spot_Plastic_2;
        // Double_t Spot_Plastic_3;
        // Double_t Spot_Plastic_4;
        std::vector<Double_t> Spot_Plastic_1;
        std::vector<Double_t> Spot_Plastic_2;
        std::vector<Double_t> Spot_Plastic_3;     
        std::vector<Double_t> Spot_Plastic_4;
        
        // Double_t Spot_DC_X;
        std::vector<double> TAC_D4_CATS1nc;
        bool TAC_TOF = false;
        // double TAC_CATS_PL;
        std::vector<double> TAC_CATS_PL;
        std::vector<unsigned long long> TAC_CATS_PL_TS;

        // double TAC_CATS2_CATS1;
        std::vector<double> TAC_CATS2_CATS1;
        std::vector<unsigned long long> TAC_CATS2_CATS1_TS;

        // double TAC_MMG_CATS1;
        std::vector<double> TAC_MMG_CATS1;
        std::vector<unsigned long long> TAC_MMG_CATS1_TS;

        // double TAC_MMG_CATS2;
        std::vector<double> TAC_MMG_CATS2;
        std::vector<unsigned long long> TAC_MMG_CATS2_TS;

        // double TAC_MMG_EXOGAM;
        std::vector<double> TAC_MMG_EXOGAM;
        std::vector<unsigned long long> TAC_MMG_EXOGAM_TS;

        // double TAC_CATS_EXOGAM;
        std::vector<double> TAC_CATS_EXOGAM;
        std::vector<unsigned long long> TAC_CATS_EXOGAM_TS;

        // double TAC_CATS_HF;
        std::vector<double> TAC_CATS_HF;
        std::vector<unsigned long long> TAC_CATS_HF_TS;

        // double TAC_PL_1;
        std::vector<double> TAC_PL_1;
        std::vector<unsigned long long> TAC_PL_1_TS;

        // double TAC_PL_2;
        std::vector<double> TAC_PL_2;
        std::vector<unsigned long long> TAC_PL_2_TS;

        // double TAC_PL_3;
        std::vector<double> TAC_PL_3;
        std::vector<unsigned long long> TAC_PL_3_TS;

        // double TAC_PL_4;
        std::vector<double> TAC_PL_4;
        std::vector<unsigned long long> TAC_PL_4_TS;

        // double TAC_PL_5;
        std::vector<double> TAC_PL_5;
        std::vector<unsigned long long> TAC_PL_5_TS;    Double_t norm_length;

        // double TAC_D4_CATS1;
        std::vector<double> TAC_D4_CATS1;
        std::vector<unsigned long long> TAC_D4_CATS1_TS;

        // double TAC_DD6_DD4;
        std::vector<double> TAC_DD6_DD4;
        std::vector<unsigned long long> TAC_DD6_DD4_TS;

        // double TAC_CATSD6_1_DD4;
        std::vector<double> TAC_CATSD6_1_DD4;
        std::vector<unsigned long long> TAC_CATSD6_1_DD4_TS;

    private:                                         // functions declaration
         void TreatTAC();

  };
} // namespace user_analysis
#endif

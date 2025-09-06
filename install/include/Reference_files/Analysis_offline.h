#ifndef Analysis_h
#define Analysis_h

#include "NPVUserAnalysis.h"
//#include "NPVDataInput.h"
//#include "NPVDataOutput.h"
#include "ZddDetector.h"
#include "TacDetector.h"
#include "Must2Detector.h"
#include "ExogamDetector.h"
#include "CatsDetector.h"

#include "NPReaction.h"
#include "NPEnergyLoss.h"

#include "NPRootOutput.h"

//#include "GenericEbyEBranchDetector.h"
#include "TH2.h"
#include "Math/Vector3D.h"
#include "TCanvas.h"

using namespace nptool;

namespace user_analysis {
  class Analysis : public nptool::VUserAnalysis {
   public:
    Analysis(){};
    ~Analysis(){};

   public:
    void Init();
    void TreatEvent();
    void SetDataOutput(std::shared_ptr<nptool::VDataOutput>);
    void SetDataInput(std::shared_ptr<nptool::VDataInput>){};
    // if this method return false, the event is discarded
    // Caution : this change the size of the tree,
    // so the friend mecanism no longer work
    bool FillOutputCondition() { return true; };
    void End();
    void Clear();

   private:
    std::shared_ptr<zdd::ZddDetector> zdd;
    //std::shared_ptr<tac::TacDetector> tac;
    std::shared_ptr<exogam::ExogamDetector> exogam;
    //std::shared_ptr<generic_ebye_branch::GenericEbyEBranchDetector> gatconf;
    std::shared_ptr<must2::Must2Detector> must2;
    std::shared_ptr<cats::CatsDetector> cats;
  
   private:
    
    // ----------- Energy Loss Tables ------------
    EnergyLoss d_Si;
    EnergyLoss d_Al;
    EnergyLoss d_CH2;
    EnergyLoss si_CH2;
    EnergyLoss s_CH2;

    // ----------- Reaction list ------------
    Reaction S36pdReaction;
    Reaction Si34pdReaction; 
    Reaction Si34ptReaction; 

    // ----------- Must2 Si and dead layer thickness ------------
    std::map<int, float> si_thickness;
    std::map<int, float> al_thickness;

    // ------------ Settings (fixed) ------------
    double BeamEnergy;
    double TargetThickness;

    // ------------ New branches ------------

    // depend on the reaction reconstruction
    std::vector<double> Ex;
    std::vector<double> thetaParticle;
    std::vector<double> thetaRecoil;
    std::vector<double> phiRecoil;
    std::vector<double> betaRecoil;
    std::vector<double> ELabCorrected;

    double ELab;
    double ThetaLab;
    double ThetaCM_light;
    double ThetaCM_heavy;

    double particleE;
    double ReactionBeta;

    std::vector<double> EDC;
    // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB // std::vector<double> EDC_Method1;
    // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB // std::vector<double> EDC_Method2;
    std::vector<double> M2X;
    std::vector<double> M2Y;
    std::vector<double> M2Z;

    double CsI_EReconstructed;

    /* // ------------ Histograms ------------
    std::map<int, std::map<int, TH2F*>> de_e_histos;
    std::map<int, std::map<int, TH2F*>> csi_calib_histos;
    */
    TFile* outFile;
    
  };
} // namespace user_analysis
#endif

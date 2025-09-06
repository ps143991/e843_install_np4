#ifndef Analysis_h
#define Analysis_h

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
// #include "MyDetector.h"
namespace user_analysis {
  class Analysis : public nptool::VUserAnalysis {
   public:
    Analysis(){};
    ~Analysis(){};

   public:
    void Init();
    void TreatEvent();
    // if this method return false, the event is discarded
    // Caution : this change the size of the tree,
    // so the friend mecanism no longer work
    bool FillOutputCondition() { return true; };

    void SetDataOutput(std::shared_ptr<nptool::VDataOutput>);
    void SetDataInput(std::shared_ptr<nptool::VDataInput>){};
    void End();
    void Clear();

   private:
    void TreatCATS();
    
    
    void TreatEXO();
    void TreatMust2();
    //void TreatMust2();


   private:
    //  std::shared_ptr<my_detector::MyDetector> mydetector;
    std::shared_ptr<zdd::ZddDetector> zdd;
    std::shared_ptr<tac::TacDetector> tac;
    // std::shared_ptr<exogam::ExogamDetector> exogam;
    std::shared_ptr<ebye::EbyEDetector> gatconf;
    std::shared_ptr<cats::CatsDetector> cats;
    std::shared_ptr<must2::Must2Detector> must2;
    Reaction* reaction1 = nullptr;
    Reaction* reaction2 = nullptr;
    private:
    bool bCATS;
    int gf;
    double TOF;
    
    double velocity;

    Double_t CATS1_XX;
    Double_t CATS2_XX;
    Double_t CATS1_YY;
    Double_t CATS2_YY;




    double OriginalBeamEnergy ; // AMEV
    double OriginalBeamEnergync ; // AMEV
    unsigned int GATCONFMASTER;
    //TTreeReaderValue<vector<unsigned int>>* GATCONFMASTER_;
    vector<unsigned long long> GATCONFMASTERTS;
    //TTreeReaderValue<vector<unsigned long long>>* GATCONFMASTERTS_;

    TVector3 HitDirectionMG;
    TVector3 BeamDirection;
    TVector3 BeamImpact;
    TVector3 BeamDirection0;
    TVector3 BeamImpact0;
    // Mugast info
    unsigned short MG_DetM;
    std::vector<double> MG_Ex;
    std::vector<double> MG_Extof;
    std::vector<double> MG_ELab;
    std::vector<double> MG_ELabraw;
    std::vector<double> ELoss_Al;
    std::vector<double> ELoss;
    std::vector<double> ELoss_Target;
    std::vector<double> MG_ThetaLab;
    std::vector<double> MG_PhiLab;
    std::vector<double> MG_Phi0;

    std::vector<double> MG_ThetaCM;
    std::vector<double> MG_X;
    std::vector<double> MG_Y;
    std::vector<double> MG_Z;
    std::vector<double> MG_ThetaLab0;
    std::vector<double> MG_ELab0;
    std::vector<double> MG_Ex0;
    std::vector<double> MG_Ex0tof;
    std::vector<double> MG_ThetaCM0;
    std::vector<double> MG_Ex0nocor;
    std::vector<double> MG_Exnocor;

    // MUST2 info
    unsigned short M2_TelescopeM;
    std::vector<double> M2_Ex;
    std::vector<double> M2_ExNoBeam;
    std::vector<double> M2_ExNoProton;
    std::vector<double> M2_EDC;
    std::vector<double> M2_ELab;
    std::vector<double> M2_ThetaLab;
    std::vector<double> M2_ThetaCM;
    std::vector<double> M2_X;
    std::vector<double> M2_Y;
    std::vector<double> M2_Z;
    std::vector<double> M2_dE;
    std::vector<double> M2_ECsI_from_deltaE ;

    //Mugast2 info
    ///
    double thetalab_tmp2;
    std::vector<double> MG_Ex2;
    double OriginalBeamEnergy2 ; // AMEV
    double ThetaNormalTargetMG2;
    double ThetaMGSurface2;
    double Energy2;
    TVector3 HitDirectionMG2;
    std::vector<double> MG_ThetaLab2;
    std::vector<double> MG_ELab2;
    std::vector<double> MG_ThetaCM2;

    ///
    std::vector<double> EXO_Doppler_dp;
    std::vector<double> EXO_Doppler_pt;
    std::vector<double> EXO_Doppler_p3He;
    std::vector<double> Beta_dp;
    std::vector<double> Beta_pt;
    std::vector<double> Beta_p3He;

  


/*     // ----------- Energy Loss Tables ------------ //PS: not defined properly... giving error
                                                      //FIXME: until reaction Si34ptReaction is defined
    EnergyLoss d_Si;
    EnergyLoss d_Al;
    EnergyLoss d_CH2;
    EnergyLoss si_CH2;
    EnergyLoss s_CH2; */

    // ----------- Reaction list ------------
/*     Reaction S36pdReaction;
    Reaction Si34pdReaction; 
    Reaction Si34ptReaction;  */

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

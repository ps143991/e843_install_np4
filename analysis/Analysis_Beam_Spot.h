#ifndef AnalysisBEAMSPOT_h
#define AnalysisBEAMSPOT_h

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
#include "TVector3.h"


using namespace nptool;

namespace user_analysis {
  class Analysis_Beam_SPOT : public nptool::VUserAnalysis {
   public:
    Analysis_Beam_SPOT(){};
    ~Analysis_Beam_SPOT(){};

   public:
    void Init();
    void TreatEvent();
    bool FillOutputCondition() { return true; };

    std::shared_ptr<tac::TacDetector> tac;
    std::shared_ptr<cats::CatsDetector> cats;
    std::shared_ptr<ebye::EbyEDetector> gatconf;
    std::shared_ptr<zdd::ZddDetector> zdd;
        
    unsigned int GATCONFMASTER;
    unsigned long long GATCONFMASTERTS;



    void SetDataOutput(std::shared_ptr<nptool::VDataOutput>);
    void SetDataInput(std::shared_ptr<nptool::VDataInput>){};
    void End();
    void Clear();

    private:                                        //Variables declaration
        Double_t entriesPS;

        // double TAC_PL_3;
        // Double_t Spot_Plastic_3;
        Double_t Spot_DC_X;

        Double_t CATS1_X;
        Double_t CATS1_Y;
        Double_t CATS1_Z;
        Double_t CATS2_X;
        Double_t CATS2_Y;
        Double_t CATS2_Z;

        Double_t targetX_byCATS;
        Double_t targetY_byCATS;

        Double_t dx_off[2]={+53.055,-54.09 };
        Double_t dx_gain[2]={-0.33057,0.41992};
        Double_t dy_off[2]={-68.3353,76.538};
        Double_t dy_gain[2]={0.4735,-0.43558};

        Double_t Position_unshifted[3] = {6035.5, 6532.5, 8883.5}; //CATS1 z pos, CATS2 z pos, DC z pos
        // position unshifted are positions with additional 7622.6 mm...
        // new positions are: unshifted - 7622.6 mm
        // 6035 -7622.6 = -1587.1
        // 6532 -7622.6 = -1090.6
        // 8883 -7622.6 = 1260.4
        Double_t Position[3] = {-1587.1, -1090.6, 1260.4}; //CATS1 z pos, CATS2 z pos, DC z pos
        // Position[0] = 6035.5 - 7622.6; //CATS1 z pos
        // Position[1] = 6532.5 - 7622.6; //CATS2 z pos
        // Position[2] = 8883.5 - 7622.6; //DC z pos

        Double_t Position_ZDD_unshifted[4] =    {8763.5, 9008.5, 9183.5, 9683.5};                          // Positions of DC1, DC2, IC1, and Plastics
        // Position_ZDD[0]      =    Position[2]-125;                                           // 8883.5 - 125 = 8763.5
        // Position_ZDD[1]      =    Position[2]+125;                                           // 8883.5 + 125 = 9008.5
        // Position_ZDD[2]      =    Position_ZDD[0] + 420;                                     // 8763.5 + 420 = 9183.5
        // Position_ZDD[3]      =    Position_ZDD[2] + 46*2*5 + 40;                             // 9183.5 + 460 + 40 = 9683.5

        //similarly, Position_ZDD are:
        // 8763.5 - 7622.6 = 1140.9
        // 9008.5 - 7622.6 = 1385.9
        // 9183.5 - 7622.6 = 1560.9
        // 9683.5 - 7622.6 = 2060.9

        Double_t Position_ZDD[4] = {1140.9, 1385.9, 1560.9, 2060.9}; // Positions of DC1, DC2, IC1, and Plastics

        Double_t dx_C12;
        Double_t dy_C12;


        Double_t dx_C1_DC;
        Double_t dx_C1_PL;
        Double_t dy_C1_DC;
        Double_t dy_C1_PL;

        Int_t ix;
        Int_t iy;

        Long64_t dt_x;
        Long64_t dt_y;
        // Double_t dt_x;
        // Double_t dt_y ;
        Double_t dl_x ;
        Double_t dl_y ;
        Double_t dl_x1;
        Double_t dl_y1;
        Double_t dl_x2;
        Double_t dl_y2;
        Long64_t dt_x1;

        Double_t cx[2];
        Double_t cy[2];
        Double_t ax[2];
        Double_t ay[2];

        Double_t old_L_Bins[5]={19617,19836,20106,19857,19395}; //lower bins of shifted spectra of TACs
        Double_t L_Bins[5] = {222,441,711,462,0};               //lower bins subtracted by the lowest bin of all low bins of 5 plastics (the 5th one)
        Double_t old_U_Bins[5] = {26541,26322,26052,26301,26763}; //upper bins of shifted spectra of TACs

        Double_t y_value[5]={10.0,5.0,0.0,-5.0,-10.0};
        Double_t perbin_value[5];
        Double_t x_value[5];



    private:                                         // functions declaration
        void Further_Analysis();
      
    private: //PS: added for beam spot analysis
        TH2F* h2d           = nullptr;
        TH2D *dl_dc_0       = nullptr;
        TH2D *dl_dc_1       = nullptr;
        TH2D *dl_dc_2       = nullptr;
        TH2D *dl_dc_3       = nullptr;
        TH2D *hdcxy00       = nullptr;
        TH2D *hdcxy01       = nullptr;
        TH2D *hdcxy10       = nullptr;
        TH2D *hdcxy11       = nullptr;
        TH2D *hdcxy         = nullptr;
        TH2D *hDC_reconst   = nullptr;
        TH2D *test          = nullptr;
        TH1D *hDC1          = nullptr;
        TH1D *hDC2          = nullptr;
        TH1D *hDC3          = nullptr;
        TH1D *hDC0          = nullptr;
        TH1D *hDCpos        = nullptr;
        TH1D *hgatconts     = nullptr;
        TH2F *Trajectory_XZ = nullptr;
        TH2F *Trajectory_YZ = nullptr;
        TH1D *hDC0ts        = nullptr;
        TH1D *hDC1ts        = nullptr;
        TH1D *hDC2ts        = nullptr;
        TH1D *hDC3ts        = nullptr;
        TH1D *hdcnum        = nullptr;
        TH1D *hdcnumx       = nullptr;
        TH1D *hdcnumy       = nullptr;
        TH2F *h2dPL         = nullptr;
        TH2F *h2dPL_reconst = nullptr;
        TH2F *hSpotPl       = nullptr;
        TVector3 BeamImpact;
        TVector3 BeamDirection;

        TFile* outFile = nullptr; //PS: output file for histograms

  };
} // namespace user_analysis
#endif

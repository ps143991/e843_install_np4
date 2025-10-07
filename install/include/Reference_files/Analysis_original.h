#ifndef Analysis_h
#define Analysis_h
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  MUST_AND_ZDD analysis project                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include"Geometry_Clover_Exogam.h"
#include"TMust2Physics.h"
#include"TMugastPhysics.h"
#include"TCATSPhysics.h"
#include "TTACPhysics.h"
#include "TExogamPhysics.h"
#include "NPTimeStamp.h"
#include"NPVAnalysis.h"
#include"TZDDPhysics.h"
#include"NPEnergyLoss.h"
#include"NPFunction.h"
#include"NPReaction.h"
#include"NPOptionManager.h"
#include"RootInput.h"
#include"RootOutput.h"
#include"TInitialConditions.h"
#include "TReactionConditions.h"
#include"NPParticle.h"
#include"NPBeam.h"
#include "TCutG.h"
#include<random>
class Analysis: public NPL::VAnalysis{
public:
    Analysis();
    ~Analysis();

public:
    void SetParticles();
    void SetEnergyLoss();
    void Init();
    bool UnallocateBeforeBuild();
    bool FillOutputCondition();
    bool UnallocateBeforeTreat();
    void TreatEvent();
    void End();
    void ReInit();
    void InitOutputBranch();
    void InitInputBranch();
    //  void TrajectoryReconstruction(); //open for trajectory reconstruction code
    bool CheckGoodEvent();
    // CheckIC is checking that all IC are crossed (E>0) not less and not more than 1 time
    void UnallocateVariables();

    bool CheckIC();
    // CheckPlastics is checking that only and at leat 1 plastic is hit
    bool CheckPlastics();
    // CHecking multiplicity 1 in DC
    bool CheckDC();
    bool CheckExoAddBack(int ExoDet1, int ExoCristal1, int ExoSegment1, float ExoTime1, int ExoDet2, int ExoCristal2, int ExoSegment2, float ExoTime2);
    bool CheckExoDeltaTV(float ExoTime);
    void SetRefTS();
    void ReadConfigTS();
    static NPL::VAnalysis* Construct();
    // TTreeReaderValue<TMust2Data>* r_ReaderEventData;
    TTreeReaderValue<TMugastData>* r_ReaderEventData;

private:
    void TreatCATS();
    void TreatZDD();
    void TreatTAC();
    //void TreatMugast();
    //void TreatMugast2();
    //void TreatEXO();
    //void TreatMust2();
private:
    bool bCATS;
    bool IsPhysics;
private:

    /// Currently only treating multiplicity 1 events
    // ZDD info
    double ZDD_DC_X;
    double ZDD_DC_Y;
    double ZDD_ThetaIC = 30/deg;
    double ZDD_ThetaAfterTarget;
    double ZDD_ThetaAfterTarget_X;
    double ZDD_ThetaLab;
    double ZDD_E_tot;
    double ZDD_E_Plastic;
    double ZDD_dE_tot;
    std::vector<double> ZDD_Corrected_IC_E;

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
    double thetalab_tmp;
    double philab_tmp;
    double OriginalBeamEnergy ; // AMEV
    double OriginalBeamEnergync ; // AMEV
    double FinalBeamEnergy;
    ///Mugast2 info
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

    std::vector<unsigned int> TAC_CATS_PL;
    std::vector<unsigned long long> TAC_CATS_PL_TS;

    std::vector<unsigned int> TAC_CATS2_CATS1;
    std::vector<unsigned long long> TAC_CATS2_CATS1_TS;

    std::vector<unsigned int> TAC_MMG_CATS1;
    std::vector<unsigned long long> TAC_MMG_CATS1_TS;

    std::vector<unsigned int> TAC_MMG_CATS2;
    std::vector<unsigned long long> TAC_MMG_CATS2_TS;

    std::vector<unsigned int> TAC_MMG_EXOGAM;
    std::vector<unsigned long long> TAC_MMG_EXOGAM_TS;

    std::vector<unsigned int> TAC_CATS_EXOGAM;
    std::vector<unsigned long long> TAC_CATS_EXOGAM_TS;

    std::vector<unsigned int> TAC_CATS_HF;
    std::vector<unsigned long long> TAC_CATS_HF_TS;

    std::vector<unsigned int> TAC_PL_1;
    std::vector<unsigned long long> TAC_PL_1_TS;

    std::vector<unsigned int> TAC_PL_2;
    std::vector<unsigned long long> TAC_PL_2_TS;

    std::vector<unsigned int> TAC_PL_3;
    std::vector<unsigned long long> TAC_PL_3_TS;

    std::vector<unsigned int> TAC_PL_4;
    std::vector<unsigned long long> TAC_PL_4_TS;

    std::vector<unsigned int> TAC_PL_5;
    std::vector<unsigned long long> TAC_PL_5_TS;

    std::vector<unsigned int> TAC_D4_CATS1;
    std::vector<unsigned long long> TAC_D4_CATS1_TS;

    std::vector<unsigned int> TAC_DD6_DD4;
    std::vector<unsigned long long> TAC_DD6_DD4_TS;

    std::vector<unsigned int> TAC_CATSD6_1_DD4;
    std::vector<unsigned long long> TAC_CATSD6_1_DD4_TS;

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
    double  IC_ZDD_sum;
    bool TAC_TOF;
    double TAC_D4_CATS1nc;

    int gd;
    int gf;
    double Ed;
    double Time_0_Ref;
    double Channel_Calib;
    double D4_CATS1_L;
    double TOF;
    double velocity;
    std::vector<string> light_particles{"proton", "deuterium"};
    double Si_E_d;
    double CsI_E_d;

    double Etot_d;
    double ThetaLab_d;
    double Ex_d;
    double Ex_dc;

    double Etot_p;
    double ThetaLab_p;
    double Ex_p;
    double Ex_pc;


    double PL_center;
    double PL_centerLog;

    double Beam_E;

    float CATS1_X;
    float CATS2_X;
    float CATS1_Y;
    float CATS2_Y;

    float CATSX_diff;
    float CATSY_diff;

    float Xf;
    float Yf;
    float Tf;
    float Pf;

    vector<unsigned int> GATCONFMASTER;
    TTreeReaderValue<vector<unsigned int>>* GATCONFMASTER_;
    vector<unsigned long long> GATCONFMASTERTS;
    TTreeReaderValue<vector<unsigned long long>>* GATCONFMASTERTS_;

    int Inner6MVM;
    float Inner6MV[48];
    unsigned short Inner6MVN[48];
    unsigned long long Inner6MVTS[48];

    int BGOVM;
    float BGOV[48];
    unsigned short BGOVN[48];

    // Trajectory branches
    /*
 Float_t Plastic_vec[10];
 Float_t IC_vec[10];
  Float_t PlasticRaw[10];
  Float_t IC_ZDDRaw[5];
  // LogicIC accounts for charge deposited in at least 1 IC
  Int_t LogicIC;
  // sumIC is the sums of charge in all IC
  Float_t sumIC;
  // Last IC that received a charge
  Int_t LastIC;
   Long64_t DCRawTS[4];
   ULong64_t DATATRIG_CATSTS;

   Float_t Position[4];
  // Position of CATS 1
  Position[0] = 6035.5;
  // Position of CATS 2
  Position[1] = 6532.5;
  // Position of the Target
  Position[2] = 7622.6;
  // Position of the center of DC
  Position[3] = 8883.5;

  // Distance between first DC and first IC
  Float_t Delta_DC_IC = 420;
  // Distanc between 2 IC
  //Float_t Delta_IC_IC = 46;
  Float_t Delta_IC_IC = 46*2;
  // Distance between last IC and plastics
   /////
 Float_t Position_ZDD[4];
  // Position of first DC
  Position_ZDD[0] = 8500;
  // Position of second DC
  Position_ZDD[1] = 8750;
  // Position of first IC
  Position_ZDD[2] = Position_ZDD[0] + Delta_DC_IC;
  // Position of Plastics
  Position_ZDD[3] = Position_ZDD[2] + Delta_IC_IC*5 + 40;
   Float_t Position_IC[5];
  Position_IC[0] = Position_ZDD[0] + Delta_DC_IC;

 ///////
 */

    //  int Strip_X_M;
    //  float Strip_X_E[48];
    //  float Strip_X_T[48];
    //  unsigned int Strip_X_Nb[48];
    //  unsigned short Strip_X_Det[48];
    //
    //  int Strip_Y_M;
    //  float Strip_Y_E[48];
    //  float Strip_Y_T[48];
    //  unsigned int Strip_Y_Nb[48];
    //  unsigned short Strip_Y_Det[48];

    int DeltaTVM;
    float DeltaTV[48];
    unsigned short DeltaTVN[48];
    unsigned long long DeltaTVTS[48];

    float EnergyDoppler;
    float EnergyAddBackDoppler;
    float EnergyAddBack;
    int ExogamDetNb[3];
    int CristalNb[3];
    int SegmentNb[3];

    std::vector<int> event1;
    std::vector<int> event2;
    int highest_E;

    int OutersVM;
    float OutersV[192];
    unsigned short OutersVN[192];

    int DCRawM;
    unsigned short DCRaw[4];
    unsigned short DCRawNr[4];
    unsigned long long DCRawTS[4];

    unsigned short PlasticRaw[10];
    unsigned long long PlasticRawTS[10];
    float PlasticCal[10];

    float PlasticEner[5];
    int PlasticEnerM;
    unsigned short PlasticEnerN[5];
    unsigned long long PlasticEnerTS[5];
    int PlasticCounter;
    float PlasticThreshold;
    float PlasticEner_tmp;

    unsigned short IC_ZDDRaw[6];
    unsigned long long IC_ZDDRawTS[6];
    float ICCal[4];



    double xtarget;
    double ytarget;
    double IncidentTheta = 0;
    int DetectorNumber  ;
    double  ThetaNormalTarget;
    double ThetaNormalTargetM2;
    double ThetaM2Surface ;
    // std::vector<double> ThetaMGSurface ;
    //std::vector<double> ThetaNormalTargetMG;
    double ThetaNormalTargetMG;
    double ThetaMGSurface ;
    double ThetaNormalTargetMG0;
    double ThetaMGSurface0;
    double Si_E_M2 ;
    double CsI_E_M2  ;
    double Energy ;
    double BeamEnergy;
    double ThetaGDSurface ;
    double Energycor ;
    double Energy0 ;
    double Energycor0 ;
    double Beta;
    double Gamma;
    double Velocity;

    double Drift_Speed;
    double TargetThickness;
    double CorrectedBeamEnergy;
    std::vector<double> IC_Energy;

    TVector3 ZDir;
    TVector3 BeamDirection;
    TVector3 BeamImpact;
    TVector3 BeamDirection0;
    TVector3 BeamImpact0;
    TVector3 HitDirectionMG;
    float EXO_ZDD_A_C;
    float EXO_ZDD_B_C;
    float EXO_ZDD_C_C;
    float EXO_ZDD_D_C;
    int EXO_ZDDRawM;
    unsigned short EXO_ZDDRawNr[4];
    unsigned long long EXO_ZDDRawTS[4];

    float EXO_ZDD[4];

    unsigned long long t_EXO_ZDD[4];

    NPL::Reaction* reaction = new Reaction;
    NPL::Particle* Co_57 = new Particle;
    NPL::Particle* Ni_57 = new Particle;
    NPL::EnergyLoss Beam_Target;
    NPL::EnergyLoss Heavy_Target;
    NPL::EnergyLoss LightTarget;
    std::map<TString, NPL::EnergyLoss> LightTargetmg;
    NPL::EnergyLoss ProtonSi;
    std::vector<NPL::EnergyLoss> Heavy_IC_Gas;
    std::vector<NPL::EnergyLoss> Heavy_IC_Windows;
    std::vector<NPL::EnergyLoss> Heavy_IC_Mylar;
    std::vector<NPL::EnergyLoss> Heavy_DC_Gas;
    NPL::EnergyLoss LightAl;
    NPL::EnergyLoss HeavyMylar;
    std::map<TString, NPL::EnergyLoss> LightAlmg;
    NPL::EnergyLoss LightSi;




    string BeamName = "48Cr";

    int Nb_Mylar_After_IC = 1;
    int Nb_Mylar_Before_IC = 4;
    int Nb_IC = 10;
    double Drift_Chamber_Length = 180*mm;
    double Drift_Chamber_Width = 180*mm;
    double ZDD_R = 1027*mm;

private:
     TMust2Physics* M2;
    TMugastPhysics* MG;
    TCATSPhysics* CATS;
    TZDDPhysics* ZDD;
    TExogamPhysics* Exogam;
    TTACPhysics* TAC;

   // TMugastPhysicsReader* MG_Reader;
     TMust2PhysicsReader* M2_Reader;
    TInitialConditions* InitialConditions;
    TReactionConditions* ReactionConditions;

    std::ifstream ExogamTopo;
    std::ifstream DC_calib;
    std::ifstream Plastic_calib;
    std::ifstream IC_calib;
    std::string ExogamLine;
    std::string DCLine;
    std::string PlasticLine;
    std::string ICLine;

    Int_t Plastic_Nb_tmp;
    Double_t Plastic_peak_tmp,Plastic_pedestal_tmp;
    Double_t Plastic_pedestal[10];
    Double_t Plastic_peak[10];

    Int_t IC_Nb_tmp;
    Double_t IC_peak_tmp,IC_pedestal_tmp;
    Double_t IC_pedestal[4];
    Double_t IC_peak[4];

    int DC_Nr;
    long long dt;
    char DC_XY[1];
    Int_t DC_numb_tmp;
    Double_t off_tmp,cff_tmp,sqr_tmp;
    Double_t off[4];
    Double_t cff[4];
    Double_t sqr[4];

    double DC_X;
    double DC_Y;

    //Char_t* ExogamLine_char[100];


    double Theta_seg;
    double Phi_seg;


    Int_t ExoNumb;
    Int_t Flange_tmp;
    int FlangeNumb[12];



    /////////////Exogam
    Clover_struc Exogam_Clovers_struc[12];


    TCutG* proton_cut[4];
    TCutG* deuteron_cut[4];
    TCutG* triton_cut[4];

    TFile* proton_cut_file[4];
    TFile* deuteron_cut_file[4];
    TFile* triton_cut_file[4];

    Beam* BeamPart = new Beam;
    Particle* HeavyEjectile = new Particle;

    default_random_engine generator;
    CalibrationManager* Cal;

    normal_distribution<double> distribution = normal_distribution<double>(0.0,1.0);
private:
    TimeStamp* TSEvent = new TimeStamp;
};
#endif

#include <iostream>
#include <fstream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TVector3.h"


// (Hugo) This code fills and writes in a root file histograms that give information on the beam trajectory through the detectors.
// These histograms can be shown using the script ShowTrajectories.cxx
// To execute it, in ROOT: .x Trajectory_reconstruction.cxx(NNN) where NNN is the number of the run

void Trajectory_reconstruction(int runNr){
  Float_t PI = 3.1415;

  TChain* Chain=NULL ;
  Chain = new TChain("AD");
//  Chain->Add(Form("./Data_Exp/r%04d_000a*.root",runNr));
  Chain->Add(Form("./rootA/r%04d_000a.root",runNr));
  // Declaring the chains
  Chain->SetBranchStatus("CATS1_X",true);
  Chain->SetBranchStatus("CATS1_Y",true);
  Chain->SetBranchStatus("CATS2_X",true);
  Chain->SetBranchStatus("CATS2_Y",true);

  Chain->SetBranchStatus("Xf",true);
  Chain->SetBranchStatus("Tf",true);
  Chain->SetBranchStatus("Yf",true);
  Chain->SetBranchStatus("Pf",true);


  //Chain->SetBranchStatus("DC_X1_C",true);
  //Chain->SetBranchStatus("DC_X2_C",true);
  //Chain->SetBranchStatus("DC_Y1_C",true);
  //Chain->SetBranchStatus("DC_Y2_C",true);

  Chain->SetBranchStatus("DC_00_C",true);
  Chain->SetBranchStatus("DC_01_C",true);
  Chain->SetBranchStatus("DC_02_C",true);
  Chain->SetBranchStatus("DC_03_C",true);

  //Chain->SetBranchStatus("DATATRIG_CATS1RawTS",true);
  Chain->SetBranchStatus("DATATRIG_CATSTS",true);
  Chain->SetBranchStatus("DATATRIG_CATS",true);
  Chain->SetBranchStatus("GATCONFMASTER",true);
  //Chain->SetBranchStatus("DCX_ZDDRawTS",true);
  //Chain->SetBranchStatus("DCY_ZDDRawTS",true);
  Chain->SetBranchStatus("DCRawTS",true);
  Chain->SetBranchStatus("DCRawM",true);
  Chain->SetBranchStatus("DCRawNr",true);
  Chain->SetBranchStatus("DCRaw",true);

  Chain->SetBranchStatus("TAC_D4_CATS1",true);

  Chain->SetBranchStatus("IC_ZDDRaw", true);
  Chain->SetBranchStatus("PlasticRaw", true);
  Chain->SetBranchStatus("PlasticRawTS", true);
  // Declaring variables for the Chains
  Float_t CATS1_X;
  Float_t CATS1_Y;
  Float_t CATS2_X;
  Float_t CATS2_Y;

  Float_t Xf;
  Float_t Yf;
  Float_t Tf;
  Float_t Pf;

  Float_t DC_00_C;
  Float_t DC_01_C;
  Float_t DC_02_C;
  Float_t DC_03_C;
  UShort_t TAC_D4_CATS1;
  Int_t DCRawM;
  short DCRawNr[4];
  short DCRaw[4];


  // These 2 lists store the 10 IC and Plastic values in a more convenient way
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
  Long64_t PlasticRawTS[10];
  Long64_t DCRawTS[4];
  ULong64_t DATATRIG_CATSTS;
  UShort_t GATCONFMASTER;
  UShort_t DATATRIG_CATS;
  //======================
  // The following parameters are positions of detectors in the experimental Setup
/*
  Float_t Position[4];
  // Position of CATS 1
  Position[0] = 6037.7;
  // Position of CATS 2
  Position[1] = 6534.8;
  // Position of the Target
  Position[2] = 7618.0;
  // Position of the center of DC
  Position[3] = 8623.0;
  */
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


  //Note : in this code reconstruction, I did not account for the fact that IC are tilted.
  // I don't think it changes anything important for the reconstructions I did


  // This angle is used to make cuts in the beam angular distribution after the target. (In the code, the initial angular distribution in CATS is taken into account)
  Float_t Angle_Cut = 0.2 * 2 * PI/180;



  Float_t Position_ZDD[4];
  // Position of first DC
  Position_ZDD[0] = 8500;
  // Position of second DC
  Position_ZDD[1] = 8750;
  // Position of first IC
  Position_ZDD[2] = Position_ZDD[0] + Delta_DC_IC;
  // Position of Plastics
  Position_ZDD[3] = Position_ZDD[2] + Delta_IC_IC*5 + 40;

  // This loop sets position for all IC
/*  Float_t Position_IC[10];
  Position_IC[0] = Position_ZDD[0] + Delta_DC_IC;
  for(int k = 1; k < 10; k++){
    Position_IC[k] = Position_IC[k - 1] + Delta_IC_IC;
  }*/

 Float_t Position_IC[5];
  Position_IC[0] = Position_ZDD[0] + Delta_DC_IC;
  for(int k = 1; k < 5; k++){
    Position_IC[k] = Position_IC[k - 1] + Delta_IC_IC;
  }

    // Set of coefficients used for position extrapolation between CATS and target and between Target and DC
    Float_t ax[2];
    Float_t ay[2];

    // Some logical variables for DC
    Int_t ix = -1;
    Int_t iy = -1;

    Long64_t CATS_TS=0;

    // Some logical variables for Plastics
    Int_t LogicPlastic = 0;
    // Number of Plastic that has been hit
    Int_t NbrPlastic = -1;
    // Maximum charge deposited in Plastic
    Double_t MaxPlastic_E = 0;

  // Delta TimeStamp between CATS1 and DC
  Double_t dt_x = -1.;
  Double_t dt_y = -1.;
  Double_t dt_xpl = -1.;
  Double_t dt_ypl = -1.;
  // Position in DC extracted from position extrapolation and TimeStamp fitting (by Shun Pei's method)
  Double_t dl_x = -1.;
  Double_t dl_y = -1.;

//===============================================
  //DC position parameters

// Shun Pei first parameters  (These do not work for the Tuesday runs)
  //Double_t x_cff[2] = {0.394707,-0.355796};
 // Double_t y_cff[2] = {-0.46312,0.362971};
 // Double_t x_off[2] = {-37.8055-6,87.3719+6};
 // Double_t y_off[2] = {70.1211+5,-70.3519-5};
 // parameters from exp quadratic
/*  Double_t y_cff[2] = {-3.38358,4.4896};
    Double_t x_cff[2] = {-0.483547,-0.0642364};
    Double_t y_off[2] = {330.736,-432.961};
    Double_t x_off[2] = {15.8165, 34.3149};
    Double_t y_sqr[2] = {0.00841051,-0.0118443};
    Double_t x_sqr[2] = {0.00234925,-0.000541164};*/

/*Double_t  x_cff[2] = {-0.782922,3.16369};
Double_t  y_cff[2] = {0.249771,0.315457};
Double_t  x_off[2] = {94.4589,-247.695};
Double_t  y_off[2] = {-20.8379,-27.2687};
Double_t  x_sqr[2] = {0.00131639,-0.0102965};
Double_t  y_sqr[2]= {-0.000655093,-0.000750275};*/

 //My first parameters
  // These are not so good, actually you can delete them
  Double_t x_cff[2] = {-0.340,0.314};
  Double_t y_cff[2] = {0.337,-0.358};
  Double_t x_off[2] = {97.05, -79.44};
  Double_t y_off[2] = {-94.56, 94.93};


  Double_t x_sqr[2] = {0,0};
  Double_t y_sqr[2] = {0,0};


    // These coefficients work super well for the runs 230 - 249
  /*Double_t x_cff[2] = {0.31,-0.56};
  Double_t y_cff[2] = {-0.379,0.46};
  Double_t x_off[2] = {-4.0, 91.9};
  Double_t y_off[2] = {31.7, -60};
  Double_t  x_sqr[2] = {0.00,0.00};
  Double_t  y_sqr[2] = {-0.00,0.00};*/

  // These offset are useless
  //Double_t x_corr[2] = {4.87,36.3};
  //Double_t y_corr[2] = {-36.1, 0.3};

  // Some angles: Phi and Theta are the angle of the beam before target, Phi_p and Theta_p are the angle of the beam after target
  //(the difference between them gives the angle tested for the angular cut)
  Double_t Phi;
  Double_t Phi_p;
  Double_t Theta;
  Double_t Theta_p;


  Chain->SetBranchAddress("CATS1_X",&CATS1_X);
  Chain->SetBranchAddress("CATS1_Y",&CATS1_Y);
  Chain->SetBranchAddress("CATS2_X",&CATS2_X);
  Chain->SetBranchAddress("CATS2_Y",&CATS2_Y);

  Chain->SetBranchAddress("Xf",&Xf);
  Chain->SetBranchAddress("Yf",&Yf);
  Chain->SetBranchAddress("Tf",&Tf);
  Chain->SetBranchAddress("Pf",&Pf);
  Chain->SetBranchAddress("TAC_D4_CATS1",&TAC_D4_CATS1);
  Chain->SetBranchAddress("DC_00_C",&DC_00_C);
  Chain->SetBranchAddress("DC_01_C",&DC_01_C);
  Chain->SetBranchAddress("DC_02_C",&DC_02_C);
  Chain->SetBranchAddress("DC_03_C",&DC_03_C);


  Chain->SetBranchAddress("DCRawTS",&DCRawTS);
  Chain->SetBranchAddress("DCRawM",&DCRawM);
  Chain->SetBranchAddress("DCRaw",&DCRaw);
  Chain->SetBranchAddress("DCRawNr",&DCRawNr);
//Chain->SetBranchAddress("DCY_ZDDRawTS",&DCY_ZDDRawTS);
  Chain->SetBranchAddress("GATCONFMASTER",&GATCONFMASTER);
  Chain->SetBranchAddress("DATATRIG_CATSTS",&DATATRIG_CATSTS);
  Chain->SetBranchAddress("DATATRIG_CATS",&DATATRIG_CATS);
  Chain->SetBranchAddress("IC_ZDDRaw", &IC_ZDDRaw);
  Chain->SetBranchAddress("PlasticRaw", &PlasticRaw);
  Chain->SetBranchAddress("PlasticRawTS", &PlasticRawTS);

    // Trajectories of particles in CATS until DC
    auto *Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 4000, 6000, 10000, 1000, -499, 500 );
    auto *Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 4000, 6000, 10000, 1000, -499, 500 );
    auto *projectionX = new TH1F("target_X_projection","Reconstruction of beam in target (X projection)",1000, -499, 500 );
    auto *projectionY = new TH1F("target_Y_projection","Reconstruction of beam in target (Y projection)",1000, -499, 500 );
    auto *projectionX_trig = new TH1F("target_X_projection_trig","Reconstruction of beam in target (X projection) trig cats",1000, -499, 500 );
    auto *projectionY_trig = new TH1F("target_Y_projection_trig","Reconstruction of beam in target (Y projection) trig cats",1000, -499, 500 );
    auto *target_position = new TH2F("targetXY","Reconstruction of beam in target",1000, -499, 500,1000, -499, 500);
    auto *Xf_trig = new TH2F("projectionX_trig","xf vs gatconf",1000, -499, 500, 256,0,256);
    auto *Xf_datatrigcats = new TH2F("projectionX_datatrigcats","xf vs datatrig",1000, -499, 500, 256,0,256);
    // These histograms store data for all events that are accepted by CATS (Trajectory after Target in IC)
    auto *Trajectory_Tget_IC_XZ = new TH2F("Trajectory_Tget_IC_XZ", "Reconstruction of beam trajectory in IC  (X projection)",
    Position_IC[4]-Position[2] + 200, Position[2] - 100, Position_IC[4] + 100, 1000, -499, 500 );
    auto *Trajectory_Tget_IC_YZ = new TH2F("Trajectory_Tget_IC_YZ", "Reconstruction of beam trajectory in IC  (Y projection)",
    Position_IC[4]-Position[2] + 200, Position[2] - 100, Position_IC[4] + 100, 1000, -499, 500 );
    auto *Trajectory_Tget_IC_XZ_trig = new TH2F("Trajectory_Tget_IC_XZ_trig", "Reconstruction of beam trajectory in IC trigger cats (X projection)",
    Position_IC[4]-Position[2] + 200, Position[2] - 100, Position_IC[4] + 100, 1000, -499, 500 );
    auto *Trajectory_Tget_IC_YZ_trig = new TH2F("Trajectory_Tget_IC_YZ_trig", "Reconstruction of beam trajectory in IC trigger cats (Y projection)",
    Position_IC[4]-Position[2] + 200, Position[2] - 100, Position_IC[4] + 100, 1000, -499, 500 );
    // The name is self explanatory
    auto *Events_CATS = new TH1F("Events_CATS", "Number of Events in CATS", 3, 0, 3);
    auto *Events_CATS_LastIC = new TH1F("Events_CATS_LastIC", "Number of Events in CATS that missed last IC", 3, -1, 3);
    auto *Events_CATS_FirstIC = new TH1F("Events_CATS_FirstIC", "Number of Events in CATS that missed first IC", 3, 0, 3);

    // Same as Trajectory_Tget_IC_XZ but we ask that the position of the beam is inside the Si detector hole
//    auto *Trajectory_cats_target_XZ_trigger = new TH2F("Trajectory_cats_target_XZ_trigger", "Reconstruction of beam trajectory in target (X projection) trigger cats1",  Position_IC[4]-Position[2] + 200, Position[2] - 100, Position_IC[4] + 100, 1000, -499, 500 );
    //auto *Trajectory_cats_target_YZ_trigger = new TH2F("Trajectory_cats_target_YZ_trigger", "Reconstruction of beam trajectory in target (Y projection) trigger cats1", Position_IC[4]-Position[2] + 200, Position[2] - 100, Position_IC[4] + 100, 1000, -499, 500 );

    auto *Trajectory_cats_target_XZ_trigger = new TH2F("Trajectory_cats_target_XZ_trigger", "Reconstruction of beam trajectory in target (X projection) trigger cats1",4000, 6000, 10000, 1000, -499, 500);
    auto *Trajectory_cats_target_YZ_trigger = new TH2F("Trajectory_cats_target_YZ_trigger", "Reconstruction of beam trajectory in target (Y projection) trigger cats1", 4000, 6000, 10000, 1000, -499, 500 );

    // Self explanatory
    auto *Events_CATS_trigger = new TH1F("Events_CATS_trigger", "Number of Events in CATS with trigger cats1", 3, 0, 3);
    auto *Events_CATS_LastIC_Inside = new TH1F("Events_CATS_LastIC_Inside", "Number of Events in CATS that missed last IC inside of the Si", 3, -1, 3);
    auto *Events_CATS_FirstIC_Inside = new TH1F("Events_CATS_FirstIC_Inside", "Number of Events in CATS that missed first IC inside of the Si", 3, 0, 3);

    // These histograms show the correlation of beam position in DC and the Plsatics hit
    auto *Position_X_Plastic = new TH2F("Position_X_Plastic","Position in Plastic scintillators compared to projection of X position from Tget DC extrapoltation",
     2,0,2, 400, -200, 200);
    auto *Position_Y_Plastic = new TH2F("Position_Y_Plastic","Position in Plastic scintillators compared to projection of Y position from Tget DC extrapoltation",
    5,0,5, 400, -200, 200);

    // This histogram stores charge recorded in first IC and is used to fit with a Gaussian (this histogram is modified by the angular cut)
    auto *hRes_IC_1 = new TH1F("hRes_IC_1","Charge deposited in first IC",100,40,140);

    // Log(Qr/Ql) in plastics
    auto *Plastic_Qr_Ql = new TH1F("Plastic_Qr_Ql","Qr_Ql comparison in plastics", 200, -1, 1);

    // Trajectory of the beam after the target with an angular cut
    auto *Trajectory_Angular_Cut_YZ = new TH2F("Trajectory_Angular_Cut_YZ", "Reconstruction of beam trajectory with angular cuts(Y projection)",
    Position_IC[4]-Position[2] + 200, Position[2] - 100, Position_IC[4] + 100, 1000, -499, 500 );
    auto *DC_X_Y = new TH2F("DC_X_Y", "Position in DC", 200, -500,500,200,-500,500);
    auto *DC_X_Y_Time = new TH2F("DC_X_Y_time", "DCRawTS[3]- PlasticRawTS[2]", 500, 0,500,500,00,500);
    auto *angle_dc_target = new TH1F("angle_dc_target","dc target angle distribution", 3600,-180,180);
    auto *angle_dc_target2 = new TH1F("angle_dc_target2","dc target angle distribution2", 3600,-180,180);
    auto *hTOF = new TH1F("TOF","TAC D4 CATS TOF", 10000000,0,1);
    Int_t nentries = Chain->GetEntries();
    std::cout << "TChain contains " << nentries << " events" << std::endl;

    double pourcentage;
    double nentries_test = 1000000;

    int DC_Nr;
    DC_Nr=0;
    double  nextpostX;
    double  nextpostY;

    for (Int_t i = 0; i < nentries; i++) {
        pourcentage = 100.*i/nentries ;
        if (i%100000 == 0) std::cout << "\rEntry " << i << Form("---- Entry processed : %.2f%%",pourcentage) << std::flush;
        Chain->GetEntry(i);

        //////////////////////////////////////////////
        // Initializing Logical parameters + vectors for Plastic and IC values

        ix = -1;
        iy = -1;
        LastIC = -1;

        LogicPlastic = 0;
        NbrPlastic = -1;
        MaxPlastic_E = 0;
        //In the given runs, IC 0, 4, 8 were not working correctly: we give them the value 0
for(Int_t j =0 ; j<5; j++){
IC_vec[j] = IC_ZDDRaw[j];

}

 for(Int_t j =0 ; j<10; j++){
 Plastic_vec[j] = PlasticRaw[j];
  }

       /////////////////////////////////////////////////////////////////
        // First condition: We ask that the event is detected in position in the 2 CATS
double TOF=0;
    if(TAC_D4_CATS1>0){
        TOF= (2759-TAC_D4_CATS1)*0.02542/2147/1e7;
        hTOF->Fill(TOF);
        }

	    if(CATS1_X > -1500 && CATS2_X > -1500 && CATS1_Y > -1500 && CATS2_Y > -1500){

  	    if(DC_02_C > 0 || DC_03_C > 0){
              if(DC_02_C > DC_03_C){
                ix = 0;
              }
              if(DC_03_C>= DC_02_C ){
                ix = 1;
              }
            }

            if(DC_00_C > 0 || DC_01_C > 0){
              if(DC_00_C > DC_01_C){
                iy = 0;
              }
              if(DC_01_C >= DC_00_C){
                iy = 1;
              }
            }

            CATS_TS = DATATRIG_CATSTS;

            // This loop determines in which Plastic the most important charge has been deposited + if any plastic was hit at all
            for(int k = 0; k < 10; k++){
              if(Plastic_vec[k] > 0)
                {
                  if(Plastic_vec[k] > MaxPlastic_E){
                    MaxPlastic_E = Plastic_vec[k];
                    NbrPlastic = k;
                    LogicPlastic = 1;
                  }
                }
                }

            // This cut basically means: CATS TS makes sense, we see something in drift chambers, we see something in the plastics
             Events_CATS->Fill(1);
             if(GATCONFMASTER==16)  Events_CATS_trigger->Fill(1);
             if(CATS_TS > 0 && LogicPlastic == 1  && ix >= 0 && iy >= 0 ){
          //    dt_x = DCRawTS[ix] - DATATRIG_CATSTS;
          //    dt_y = DCRawTS[iy] - DATATRIG_CATSTS;
                dt_x =  DATATRIG_CATSTS - DCRawTS[ix];
                dt_y =  DATATRIG_CATSTS - DCRawTS[iy];
                dl_y =  dt_y * y_cff[iy] + y_off[iy];//+ y_sqr[iy]*dt_y*dt_y;
                dl_x =  dt_x * x_cff[ix] + x_off[ix];//+ x_sqr[ix]*dt_x*dt_x;
                // Compute time difference between CATS and DC (Note: if the coefficients x_off and y_off were computed without correcting the TS difference
                // between CATS and DC, this TS difference should not be corrected here (the correction is taken into account in offset parameters))
                // Compute position in DC using parameters that have to be fitted using Shun Pei's method

                  if(DCRawM <=2){
                    for(int DCmult = 0; DCmult < DCRawM; DCmult++){
                      DC_Nr = DCRawNr[DCmult];


                   // std::cout << " Dc_Nr " << DC_Nr<< "  " << DCmult << " " << DCRawNr[DCmult] <<  "\n";



                      if(DC_Nr > 2 && DCRawTS[DCmult] > 0){
                       // dt_x = DCRawTS[DCmult] - DATATRIG_CATSTS;
                          dt_xpl = DCRawTS[DCmult] -PlasticRawTS[2];
                       // dl_x = dt_x * x_cff[DC_Nr] + x_off[DC_Nr]+ x_sqr[DC_Nr]*dt_x*dt_x; //


                       // std::cout << dt_x << " X " << dl_x << " " << DC_Nr <<" "  << x_cff[DC_Nr] << "  " <<  x_off[DC_Nr]<< " " << x_sqr[DC_Nr]<<"\n";
                       // std::cout << " Test " << DCRawTS[DCmult] << "  " << DATATRIG_CATSTS <<  "\n";
                      }
                      else if(DC_Nr < 2 && DCRawTS[DCmult] > 0){
                      //  dt_y = DCRawTS[DCmult] - DATATRIG_CATSTS;
                          dt_ypl = DCRawTS[DCmult] -PlasticRawTS[2];
                      //  dl_y = dt_y * y_cff[DC_Nr] + y_off[DC_Nr]+ y_sqr[DC_Nr]*dt_y*dt_y; //


                      //  std::cout << dt_y << "Y " << dl_y << " " << DC_Nr <<" "  << y_cff[DC_Nr] << "  " <<  y_off[DC_Nr]<< " " << y_sqr[DC_Nr]<<"\n";
                      //  std::cout << " Test " << DCRawTS[DCmult] << "  " << DATATRIG_CATSTS <<  "\n";
                      }
                    }
                  }


            DC_X_Y_Time->Fill(dt_xpl,dt_ypl);
                DC_X_Y->Fill(dl_x,dl_y);




                // Determination of slope coefficients by linear extrapolation of coordinates in CATS
                ax[0] = (Position[1] - Position[0])/(CATS2_X - CATS1_X); //cats2-cats1
                ay[0] = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
                ax[1] = (Position[3] - Position[2])/(dl_x - Xf); //center of dc-target/
                ay[1] = (Position[3] - Position[2])/(dl_y - Yf);


                // FIlling Plastic/DC position correlation histograms
                if(NbrPlastic < 5)
                  Position_X_Plastic->Fill(0.0, Xf + (Position_ZDD[3] - Position[2])/ax[1]);
                if(NbrPlastic >= 5)
                  Position_X_Plastic->Fill(1.0, Xf + (Position_ZDD[3] - Position[2])/ax[1]);
                for(int j = 0; j<5; j++){
                  if(NbrPlastic%5 == j){
                    Position_Y_Plastic->Fill(NbrPlastic%5, Yf + (Position_ZDD[3] - Position[2])/ay[1]);
                  }
                }

                // FIlling Plastic charge ratio histogram
                for(int j = 0; j < 5; j++){
                  if(Plastic_vec[j] > 0 && Plastic_vec[j+5] > 0){
                    Plastic_Qr_Ql->Fill(TMath::Log(Plastic_vec[j]/Plastic_vec[j+5]));
                  }
                }

                // Determining the last IC that recorded charge deposited
                for(int k= 0; k<5; k++){
                  if(IC_vec[k] > 0)
                    LastIC = k;
                }
                // Computing extrapolations of position and filling it in histograms
                for(int k= Position[0]; k<Position[2]; k++){
                  //  for(int k= Position[0]; k<Position_IC[LastIC]; k++){

                  //  cout<< CATS1_X << CATS1_Y << k << endl;
                   Trajectory_XZ->Fill(k, CATS1_X + (k - Position[0])/ax[0]);
                   Trajectory_YZ->Fill(k, CATS1_Y + (k - Position[0])/ay[0]);
                   projectionX->Fill(CATS1_X + (k - Position[0])/ax[0]);
                   projectionY->Fill(CATS1_Y + (k - Position[0])/ay[0]);
                   Xf_trig->Fill(CATS1_X + (k - Position[0])/ax[0],GATCONFMASTER);
                   Xf_datatrigcats->Fill(CATS1_X + (k - Position[0])/ax[0],DATATRIG_CATS);
                   target_position->Fill(CATS1_X + (k - Position[0])/ax[0],CATS1_Y + (k - Position[0])/ay[0]);
                   if(GATCONFMASTER==16) Trajectory_cats_target_XZ_trigger->Fill(k, CATS1_X + (k - Position[0])/ax[0]);
                  if(GATCONFMASTER==16) Trajectory_cats_target_YZ_trigger->Fill(k, CATS1_Y + (k - Position[0])/ay[0]);
                 if(GATCONFMASTER==16)   projectionX_trig->Fill(CATS1_X + (k - Position[0])/ax[0]);
                if(GATCONFMASTER==16)    projectionY_trig->Fill(CATS1_Y + (k - Position[0])/ay[0]);
                }


                if(LastIC >= 0){
                  for(int k= Position[2]; k< Position_IC[LastIC]; k++){
                   Trajectory_Tget_IC_XZ->Fill(k, Xf + (k - Position[2])/ax[1]);
                  Trajectory_Tget_IC_YZ->Fill(k, Yf + (k - Position[2])/ay[1]);
                   if(GATCONFMASTER==16)   Trajectory_Tget_IC_XZ_trig->Fill(k, Xf + (k - Position[2])/ax[1]);
                if(GATCONFMASTER==16)   Trajectory_Tget_IC_YZ_trig->Fill(k, Yf + (k - Position[2])/ay[1]);
                     nextpostX = Xf + (k - Position[2])/ax[1];
                     nextpostY = Yf + (k - Position[2])/ay[1];
                     TVector3 v1(Xf,Yf,Position[2]);
                     TVector3 v2(Xf + (k - Position[2])/ax[1],Yf + (k - Position[2])/ay[1],Position[3]);
		Double_t a = v1.Angle(v2);
		angle_dc_target->Fill(a*180/PI);
                  }

                  if(LogicPlastic == 1)
                  {
                    for(int k= Position_IC[LastIC]; k< Position_ZDD[3]; k++){
                    // Trajectory_XZ->Fill(k, Xf + (k - Position[2])/ax[1]);
                   //  Trajectory_YZ->Fill(k, Yf + (k - Position[2])/ay[1]);
                   //  Trajectory_XZ->Fill(k, nextpostX + (k - Position_IC[LastIC])/ax[1]);
                   //  Trajectory_YZ->Fill(k, nextpostY + (k - Position_IC[LastIC])/ay[1]);
                    }

                  }
                }

                // These histograms show the trajectory of events after hitting the target by projection of DC position

                   Events_CATS_trigger->Fill(1);

                // Records events that did not reach last IC
                if(LastIC <5)
                {
                  Events_CATS_LastIC->Fill(1);
                }
                // Records events that did not reach first IC
                if(LastIC < 0)
                {
                  Events_CATS_FirstIC->Fill(1);
                }

                // Computing angles for the angular cut
                Phi = TMath::ATan((CATS2_X - CATS1_X)/(Position[1]-Position[0]));
                Theta = TMath::ATan((CATS2_Y - CATS1_Y)/(Position[1]-Position[0]));

                Phi_p = TMath::ATan((dl_x - Xf)/(Position[3]-Position[2]));
                Theta_p = TMath::ATan((dl_y - Yf)/(Position[3]-Position[2]));

                 TVector3 vv1((CATS2_X - CATS1_X),(CATS2_Y - CATS1_Y),(Position[1]-Position[0]));
                     TVector3 vv2((dl_x - Xf),(dl_y - Yf),(Position[3]-Position[2]));
		Double_t aa = vv1.Angle(vv2);

		angle_dc_target2->Fill(aa*180/PI);

                // Angular cut condition
                if( TMath::Abs(Phi_p - Phi) < Angle_Cut && TMath::Abs(Theta_p - Theta) < Angle_Cut){
                  // Filling the charge in first IC with this angular cut
                  hRes_IC_1->Fill(IC_vec[1]);
                  for(int k = Position[2]; k <= Position_IC[LastIC]; k++){
                    Trajectory_Angular_Cut_YZ->Fill(k, Yf + (k - Position[2])/ay[1]);
                    }

                }



            }
        }
    }

  // Fitting this histogram to check resolution (even with small angles anguar cuts, i didnt fit anything better than 9%)
  // Fit range is arbitrary
  hRes_IC_1->Fit("gaus","","",80,100);

// These 2 canvas display trajectories but I coded a dedicated program that plots histograms so these are not necessary
/*
  TCanvas *c0 = new TCanvas("c0", "X trajectory", 600, 600);
  c0->Draw("");
  Trajectory_Tget_IC_XZ->Draw("colz");
//  Trajectory_XZ->Draw("colz");
  c0->Update();


  TCanvas *c1 = new TCanvas("c1", "Y trajectory", 600, 600);
  c1->Draw("");
  Trajectory_Tget_IC_YZ->Draw("colz");
  //Trajectory_YZ->Draw("colz");
  c1->Update();

  */
  //TFile *fout = new TFile(Form("./test.root"), "recreate");
  TFile *fout = new TFile(Form("./Trajectory_reconstruction_r%04d.root",runNr), "recreate");


    // Just writing histgrams in the file
    Trajectory_XZ->Write();
    Trajectory_YZ->Write();
    Trajectory_Tget_IC_XZ->Write();
    Trajectory_Tget_IC_YZ->Write();
    Trajectory_Tget_IC_XZ_trig->Write();
    Trajectory_Tget_IC_YZ_trig->Write();
    Events_CATS->Write();
    Events_CATS_LastIC->Write();
    Events_CATS_FirstIC->Write();

    Trajectory_cats_target_XZ_trigger->Write();
    Trajectory_cats_target_YZ_trigger->Write();

    Events_CATS_trigger->Write();
    Events_CATS_LastIC_Inside->Write();
    Events_CATS_FirstIC_Inside->Write();

    Position_X_Plastic->Write();
    Position_Y_Plastic->Write();

    Plastic_Qr_Ql->Write();
    hRes_IC_1->Write();
    Trajectory_Angular_Cut_YZ->Write();
    DC_X_Y->Write();
        DC_X_Y_Time->Write();
    target_position->Write();
    projectionX->Write();
    projectionY->Write();
    projectionX_trig->Write();
    projectionY_trig->Write();
    Xf_trig->Write();
    Xf_datatrigcats->Write();
    angle_dc_target->Write();
    angle_dc_target2->Write();
    hTOF->Write();
  fout->Write();




}


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


// (Hugo) This code fills and writes in a root file histograms that give information on the beam trajectory through the detectors.
// These histograms can be shown using the script ShowTrajectories.cxx
// To execute it, in ROOT: .x Trajectory_reconstruction.cxx(NNN) where NNN is the number of the run 

void Trajectory_reconstruction(int runNr){
  Float_t PI = 3.1415;

  TChain* Chain=NULL ;
  Chain = new TChain("AD");
  Chain->Add(Form("./Data_Exp/r%04d_000a*.root",runNr));

  // Declaring the chains  
  Chain->SetBranchStatus("CATS1_X",true);
  Chain->SetBranchStatus("CATS1_Y",true);
  Chain->SetBranchStatus("CATS2_X",true);
  Chain->SetBranchStatus("CATS2_Y",true);
  
  Chain->SetBranchStatus("Xf",true);
  Chain->SetBranchStatus("Tf",true);
  Chain->SetBranchStatus("Yf",true);
  Chain->SetBranchStatus("Pf",true);
  
  
  Chain->SetBranchStatus("DC_X1_C",true);
  Chain->SetBranchStatus("DC_X2_C",true);
  Chain->SetBranchStatus("DC_Y1_C",true);
  Chain->SetBranchStatus("DC_Y2_C",true);
  
  Chain->SetBranchStatus("DATATRIG_CATS1RawTS",true);
  Chain->SetBranchStatus("DCX_ZDDRawTS",true);
  Chain->SetBranchStatus("DCY_ZDDRawTS",true);
  
  Chain->SetBranchStatus("IC_00_C", true);
  Chain->SetBranchStatus("IC_01_C", true);
  Chain->SetBranchStatus("IC_02_C", true);
  Chain->SetBranchStatus("IC_03_C", true);
  Chain->SetBranchStatus("IC_04_C", true);
  Chain->SetBranchStatus("IC_05_C", true);
  Chain->SetBranchStatus("IC_06_C", true);
  Chain->SetBranchStatus("IC_07_C", true);
  Chain->SetBranchStatus("IC_08_C", true);
  Chain->SetBranchStatus("IC_09_C", true);
  
  Chain->SetBranchStatus("Plastic_10_C", true);
  Chain->SetBranchStatus("Plastic_01_C", true);
  Chain->SetBranchStatus("Plastic_02_C", true);
  Chain->SetBranchStatus("Plastic_03_C", true);
  Chain->SetBranchStatus("Plastic_04_C", true);
  Chain->SetBranchStatus("Plastic_05_C", true);
  Chain->SetBranchStatus("Plastic_06_C", true);
  Chain->SetBranchStatus("Plastic_07_C", true);
  Chain->SetBranchStatus("Plastic_08_C", true);
  Chain->SetBranchStatus("Plastic_09_C", true);

  Chain->SetBranchStatus("S1_StrRawM", true);
  Chain->SetBranchStatus("S1_SecRawM", true);
  
  // Declaring variables for the Chains  
  Float_t CATS1_X;
  Float_t CATS1_Y;
  Float_t CATS2_X;
  Float_t CATS2_Y;

  Float_t Xf;
  Float_t Yf;
  Float_t Tf;
  Float_t Pf;
  
  Float_t DC_X1_C;
  Float_t DC_X2_C;
  Float_t DC_Y1_C;
  Float_t DC_Y2_C;
  
  Float_t IC_00_C;
  Float_t IC_01_C;
  Float_t IC_02_C;
  Float_t IC_03_C;
  Float_t IC_04_C;
  Float_t IC_05_C;
  Float_t IC_06_C;
  Float_t IC_07_C;
  Float_t IC_08_C;
  Float_t IC_09_C;

  Float_t Plastic_10_C;
  Float_t Plastic_01_C;
  Float_t Plastic_02_C;
  Float_t Plastic_03_C;
  Float_t Plastic_04_C;
  Float_t Plastic_05_C;
  Float_t Plastic_06_C;
  Float_t Plastic_07_C;
  Float_t Plastic_08_C;
  Float_t Plastic_09_C;

  Int_t S1_SecRawM;
  Int_t S1_StrRawM;


  // These 2 lists store the 10 IC and Plastic values in a more convenient way
  Float_t Plastic_vec[10];
  Float_t IC_vec[10];

  // LogicIC accounts for charge deposited in at least 1 IC
  Int_t LogicIC;
  // sumIC is the sums of charge in all IC
  Float_t sumIC;
  // Last IC that received a charge
  Int_t LastIC;

  Long64_t DCX_ZDDRawTS[2];
  Long64_t DCY_ZDDRawTS[2];
  Long64_t DATATRIG_CATS1RawTS;

//======================
  // The following parameters are positions of detectors in the experimental Setup

  Float_t Position[4];
  // Position of CATS 1
  Position[0] = 6037.7;
  // Position of CATS 2
  Position[1] = 6534.8;
  // Position of the Target
  Position[2] = 7618.0;
  // Position of the center of DC
  Position[3] = 8623.0;

  // Distance between first DC and first IC
  Float_t Delta_DC_IC = 420;
  // Distanc between 2 IC
  Float_t Delta_IC_IC = 46;
  // Distance between last IC and plastics


  //Note : in this code reconstruction, I did not account for the fact that IC are tilted.
  // I don't think it changes anything important for the reconstructions I did 


  // Position (in Z) of the Si Detector
  Float_t Position_Si = 8000.4;
  // Angle where Si detector begins
  Float_t Angle_Si = 3.7 * 2 * PI/180;
  // X_Si is the maximum radial position with respect to the Z axis at the position of the Si detector
  Float_t X_Si = TMath::Tan(Angle_Si) * (Position_Si - Position[2]); 
  
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
  Position_ZDD[3] = Position_ZDD[2] + Delta_IC_IC*10 + 40;

  // This loop sets position for all IC
  Float_t Position_IC[10];
  Position_IC[0] = Position_ZDD[0] + Delta_DC_IC; 
  for(int k = 1; k < 10; k++){
    Position_IC[k] = Position_IC[k - 1] + Delta_IC_IC;
  }


    // Set of coefficients used for position extrapolation between CATS and target and between Target and DC
    Float_t ax[2];
    Float_t ay[2];

    // Some logical variables for DC
    Int_t ix = -1;
    Int_t iy = -1;


    Long64_t CATS_TS;

    // Some logical variables for Plastics    
    Int_t LogicPlastic = 0;
    // Number of Plastic that has been hit
    Int_t NbrPlastic = -1; 
    // Maximum charge deposited in Plastic
    Double_t MaxPlastic_E = 0;

  // Delta TimeStamp between CATS1 and DC
  Double_t dt_x = -1.;
  Double_t dt_y = -1.;
  // Position in DC extracted from position extrapolation and TimeStamp fitting (by Shun Pei's method)
  Double_t dl_x = -1.;
  Double_t dl_y = -1.;

//===============================================
  //DC position parameters

// Shun Pei first parameters  (These do not work for the Tuesday runs)
  /*Double_t x_cff[2] = {0.394707,-0.355796};
  Double_t y_cff[2] = {-0.46312,0.362971}; 
  Double_t x_off[2] = {-37.8055-6,87.3719+6};
  Double_t y_off[2] = {70.1211+5,-70.3519-5}; 
  */ 
  
 //My first parameters  
  // These are not so good, actually you can delete them
  Double_t x_cff[2] = {0.288,-0.547};
  Double_t y_cff[2] = {-0.368,0.315}; 
  Double_t x_off[2] = {-1.9, 91.9};
  Double_t y_off[2] = {31.2, -43.5}; 
  //Double_t x_corr[2] = {0,0};
  //Double_t y_corr[2] = {0,0};


    // These coefficients work super well for the runs 230 - 249
  /*Double_t x_cff[2] = {0.31,-0.56};
  Double_t y_cff[2] = {-0.379,0.46}; 
  Double_t x_off[2] = {-4.0, 91.9};
  Double_t y_off[2] = {31.7, -60};*/ 
  // These offset are useless
  /*Double_t x_corr[2] = {4.87,36.3};
  Double_t y_corr[2] = {-36.1, 0.3};*/

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
  
  Chain->SetBranchAddress("DC_X1_C",&DC_X1_C);
  Chain->SetBranchAddress("DC_X2_C",&DC_X2_C);
  Chain->SetBranchAddress("DC_Y1_C",&DC_Y1_C);
  Chain->SetBranchAddress("DC_Y2_C",&DC_Y2_C);


  Chain->SetBranchAddress("DCX_ZDDRawTS",&DCX_ZDDRawTS);
  Chain->SetBranchAddress("DCY_ZDDRawTS",&DCY_ZDDRawTS);

  Chain->SetBranchAddress("DATATRIG_CATS1RawTS",&DATATRIG_CATS1RawTS);



  Chain->SetBranchAddress("IC_00_C", &IC_00_C);
  Chain->SetBranchAddress("IC_01_C", &IC_01_C);
  Chain->SetBranchAddress("IC_02_C", &IC_02_C);
  Chain->SetBranchAddress("IC_03_C", &IC_03_C);
  Chain->SetBranchAddress("IC_04_C", &IC_04_C);
  Chain->SetBranchAddress("IC_05_C", &IC_05_C);
  Chain->SetBranchAddress("IC_06_C", &IC_06_C);
  Chain->SetBranchAddress("IC_07_C", &IC_07_C);
  Chain->SetBranchAddress("IC_08_C", &IC_08_C);
  Chain->SetBranchAddress("IC_09_C", &IC_09_C);


  Chain->SetBranchAddress("Plastic_10_C", &Plastic_10_C);
  Chain->SetBranchAddress("Plastic_01_C", &Plastic_01_C);
  Chain->SetBranchAddress("Plastic_02_C", &Plastic_02_C);
  Chain->SetBranchAddress("Plastic_03_C", &Plastic_03_C);
  Chain->SetBranchAddress("Plastic_04_C", &Plastic_04_C);
  Chain->SetBranchAddress("Plastic_05_C", &Plastic_05_C);
  Chain->SetBranchAddress("Plastic_06_C", &Plastic_06_C);
  Chain->SetBranchAddress("Plastic_07_C", &Plastic_07_C);
  Chain->SetBranchAddress("Plastic_08_C", &Plastic_08_C);
  Chain->SetBranchAddress("Plastic_09_C", &Plastic_09_C);


  Chain->SetBranchAddress("S1_StrRawM", &S1_StrRawM);
  Chain->SetBranchAddress("S1_SecRawM", &S1_SecRawM);

    // Trajectories of particles in CATS until DC
    auto *Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 4000, 6000, 10000, 1000, -499, 500 );
    auto *Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 4000, 6000, 10000, 1000, -499, 500 );
    
    // These histograms store data for alla events that are accepted by CATS (Trajectory after Target in IC)
    auto *Trajectory_Tget_IC_XZ = new TH2F("Trajectory_Tget_IC_XZ", "Reconstruction of beam trajectory in IC (X projection)", 
    Position_IC[9]-Position[2] + 200, Position[2] - 100, Position_IC[9] + 100, 1000, -499, 500 );
    auto *Trajectory_Tget_IC_YZ = new TH2F("Trajectory_Tget_IC_YZ", "Reconstruction of beam trajectory in IC (Y projection)", 
    Position_IC[9]-Position[2] + 200, Position[2] - 100, Position_IC[9] + 100, 1000, -499, 500 );

    // The name is self explanatory
    auto *Events_CATS = new TH1F("Events_CATS", "Number of Events in CATS", 3, 0, 3);
    auto *Events_CATS_LastIC = new TH1F("Events_CATS_LastIC", "Number of Events in CATS that missed last IC", 3, -1, 3);
    auto *Events_CATS_FirstIC = new TH1F("Events_CATS_FirstIC", "Number of Events in CATS that missed first IC", 3, 0, 3);

    // Same as Trajectory_Tget_IC_XZ but we ask that the position of the beam is inside the Si detector hole
    auto *Trajectory_Tget_IC_XZ_Inside = new TH2F("Trajectory_Tget_IC_XZ_Inside", "Reconstruction of beam trajectory in IC (X projection) inside of the Si", 
    Position_IC[9]-Position[2] + 200, Position[2] - 100, Position_IC[9] + 100, 1000, -499, 500 );
    auto *Trajectory_Tget_IC_YZ_Inside = new TH2F("Trajectory_Tget_IC_YZ_Inside", "Reconstruction of beam trajectory in IC (Y projection) inside of the Si", 
    Position_IC[9]-Position[2] + 200, Position[2] - 100, Position_IC[9] + 100, 1000, -499, 500 );

    // Self explanatory
    auto *Events_CATS_Inside = new TH1F("Events_CATS_Inside", "Number of Events in CATS inside of Si", 3, 0, 3);
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
    Position_IC[9]-Position[2] + 200, Position[2] - 100, Position_IC[9] + 100, 1000, -499, 500 );
    auto *DC_X_Y = new TH2F("DC_X_Y", "Position in DC", 200, -50,50,200,-50,50); 
  
  Int_t nentries = Chain->GetEntries();
  cout << "TChain contains " << nentries << " events" << endl;
  
  double pourcentage;
//  double nentries_test = 1000000;
  
  
  
    for (Int_t i = 0; i < nentries; i++) {
        pourcentage = 100.*i/nentries ;
        if (i%100000 == 0) cout << "\rEntry " << i << Form("---- Entry processed : %.2f%%",pourcentage) << flush;
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

        IC_vec[0] = 0;              //IC_00_C;
        IC_vec[1] = IC_01_C;
        IC_vec[2] = IC_02_C;
        IC_vec[3] = IC_03_C;
        IC_vec[4] = 0;              //IC_04_C;
        IC_vec[5] = IC_05_C;
        IC_vec[6] = IC_06_C;
        IC_vec[7] = IC_07_C;
        IC_vec[8] = 0;              //IC_08_C;
        IC_vec[9] = IC_09_C;



        Plastic_vec[0] = Plastic_01_C;    
        Plastic_vec[1] = Plastic_02_C;    
        Plastic_vec[2] = Plastic_03_C;    
        Plastic_vec[3] = Plastic_04_C;    
        Plastic_vec[4] = Plastic_05_C;    
        Plastic_vec[5] = Plastic_06_C;    
        Plastic_vec[6] = Plastic_07_C;    
        Plastic_vec[7] = Plastic_08_C;    
        Plastic_vec[8] = Plastic_09_C;    
        Plastic_vec[9] = Plastic_10_C;    
        
        
        /////////////////////////////////////////////////////////////////
        // First condition: We ask that the event is detected in position in the 2 CATS 


	    if(CATS1_X > -1500 && CATS2_X > -1500 && CATS1_Y > -1500 && CATS2_Y > -1500){
            
            /*if( DC_X1_C > 0){
                ix = 0;
            }
            else if( DC_X2_C > 0){
                ix = 1;
            }    
            if( DC_Y1_C > 0){
                iy = 0;
            }
            else if( DC_Y2_C > 0){
                iy = 1;
            }
            */
            // Asking where the charge in DC is the highest to determine which DC have been crossd

            if(DC_X1_C > 0 || DC_X2_C > 0){
              if(DC_X1_C > DC_X2_C){
                ix = 0;
              }
              if(DC_X2_C >= DC_X1_C){
                ix = 1;
              } 
            }    

            if(DC_Y1_C > 0 || DC_Y2_C > 0){
              if(DC_Y1_C > DC_Y2_C){
                iy = 0;
              }
              if(DC_Y2_C >= DC_Y1_C){
                iy = 1;
              } 
            }    

            CATS_TS = DATATRIG_CATS1RawTS;

            // This loop determines in which Plastic the most important charge as been deposited + if any plastic was hit at all                        
            for(int k = 0; k < 10; k++)
              if(Plastic_vec[k] > 0)
                {
                  if(Plastic_vec[k] > MaxPlastic_E){
                    MaxPlastic_E = Plastic_vec[k];
                    NbrPlastic = k;
                    LogicPlastic = 1;
                  }
                }

            // This cut basically means: CATS TS makes sense, we see something in drift chambers, we see something in the plastics and we did not hit the Si detector;
            if(CATS_TS > 0 && ix >= 0 && iy >= 0 && LogicPlastic == 1 && S1_SecRawM == 0 && S1_StrRawM == 0){
              
                Events_CATS->Fill(1);

                // Compute time difference between CATS and DC (Note: if the coefficients x_off and y_off were computed without correcting the TS difference
                // between CATS and DC, this TS difference should not be corrected here (the correction is taken into account in offset parameters))
	              dt_x = CATS_TS  -  DCX_ZDDRawTS[ix];
	              dt_y = CATS_TS -  DCY_ZDDRawTS[iy];
                

                // Compute position in DC using parameters that have to be fitted using Shun Pei's method
	              dl_x = dt_x * x_cff[ix] + x_off[ix]; //+ x_corr[ix];
	              dl_y = dt_y * y_cff[iy] + y_off[iy]; //+ y_corr[iy];
            
                DC_X_Y->Fill(dl_x,dl_y);



                // Determination of slope coefficients by linear extrapolation of coordinates in CATS
                ax[0] = (Position[1] - Position[0])/(CATS2_X - CATS1_X);
                ay[0] = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
                ax[1] = (Position[3] - Position[2])/(dl_x - Xf);
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
                for(int k= 0; k<10; k++){
                  if(IC_vec[k] > 0)
                    LastIC = k;
                }
                // Computing extrapolations of position and filling it in histograms                
                for(int k= Position[0]; k<Position[2]; k++){
                   Trajectory_XZ->Fill(k, CATS1_X + (k - Position[0])/ax[0]); 
                   Trajectory_YZ->Fill(k, CATS1_Y + (k - Position[0])/ay[0]); 
                }

                if(LastIC >= 0){
                  for(int k= Position[2]; k< Position_IC[LastIC]; k++){
                     Trajectory_XZ->Fill(k, Xf + (k - Position[2])/ax[1]); 
                     Trajectory_YZ->Fill(k, Yf + (k - Position[2])/ay[1]); 
                  }

                  if(LogicPlastic == 1)
                  {
                    for(int k= Position_IC[LastIC]; k< Position_ZDD[3]; k++){
                     Trajectory_XZ->Fill(k, Xf + (k - Position[2])/ax[1]); 
                     Trajectory_YZ->Fill(k, Yf + (k - Position[2])/ay[1]); 
                    }

                  }
                }
                
                // These histograms show the trajectory of events after hitting the target by projection of DC position 
                for(int k = Position[2]; k <= Position_IC[LastIC]; k++)
                {
                  Trajectory_Tget_IC_XZ->Fill(k, Xf + (k - Position[2])/ax[1]);
                  Trajectory_Tget_IC_YZ->Fill(k, Yf + (k - Position[2])/ay[1]);
                }

                // Records events that did not reach last IC
                if(LastIC < 9)
                {
                  Events_CATS_LastIC->Fill(1);  
                }
                // Records events that did not reach first IC
                if(LastIC < 0)
                {
                  Events_CATS_FirstIC->Fill(1);
                }

                // This cut checks if the event goes inside the hole of the Si detector
                if((TMath::Sqrt(TMath::Power(Xf + (Position_Si -Position[2])/ax[1], 2) +TMath::Power(Yf + (Position_Si -Position[2])/ay[1], 2)) ) < X_Si){

                  
                  Events_CATS_Inside->Fill(1);

                  // Same as before but only events inside the Si hole
                  for(int k = Position[2]; k < Position_IC[LastIC]; k++){
                    Trajectory_Tget_IC_XZ_Inside->Fill(k, Xf + (k - Position[2])/ax[1]);
                    Trajectory_Tget_IC_YZ_Inside->Fill(k, Yf + (k - Position[2])/ay[1]);
                  }

                  if(LastIC < 9){
                    Events_CATS_LastIC_Inside->Fill(1);  
                  }
                  if(LastIC < 0){
                    Events_CATS_FirstIC_Inside->Fill(1);
                    }
                }

                // Computing angles for thz angular cut
                Phi = TMath::ATan((CATS2_X - CATS1_X)/(Position[1]-Position[0]));
                Theta = TMath::ATan((CATS2_Y - CATS1_Y)/(Position[1]-Position[0]));
                
                Phi_p = TMath::ATan((dl_x - Xf)/(Position[3]-Position[2]));
                Theta_p = TMath::ATan((dl_y - Yf)/(Position[3]-Position[2]));
                
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

    Events_CATS->Write();
    Events_CATS_LastIC->Write();
    Events_CATS_FirstIC->Write();

    Trajectory_Tget_IC_XZ_Inside->Write();
    Trajectory_Tget_IC_YZ_Inside->Write();

    Events_CATS_Inside->Write();
    Events_CATS_LastIC_Inside->Write();
    Events_CATS_FirstIC_Inside->Write();

    Position_X_Plastic->Write();
    Position_Y_Plastic->Write();  

    Plastic_Qr_Ql->Write();
    hRes_IC_1->Write();
    Trajectory_Angular_Cut_YZ->Write();
    DC_X_Y->Write();
  fout->Write();

    

    
}
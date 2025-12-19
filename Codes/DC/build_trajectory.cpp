#include "NPApplication.h"
using namespace nptool;
#include "NPDetectorManager.h"
#include "NPFunction.h"
#include "ZddPhysics.h"
#include "CatsPhysics.h"
// #include "VDetector.h"
// #include "NPRootPlugin.h"
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2F.h>

#include <iostream>
using namespace zdd;
using namespace std;
using namespace cats;
// fit_erf_line.C
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCutG.h"
#include "TVector3.h"

TChain* tree = NULL;
TChain* tree2 = NULL;
TCanvas* c1 = NULL;
TCanvas *cmask = NULL;
TCanvas *ccats = NULL;

TCutG *cut_left;
TCutG *cut_right;
TCutG *cut_up;
TCutG *cut_down;
TCutG *cut_cats1_right;
TCutG *cut_cats1_left;
TCutG *cut_cats1_top;
TCutG *cut_cats1_down;

void loadFILES() {
  tree = new TChain("PhysicsTree");
//   tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/525_further_shift1.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/525_further_shift_2_full.root");
}
void loadCUTS(){
  TFile *cut_L = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_left.root","READ");
  TFile *cut_R = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_right.root","READ");
  TFile *cut_U = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_up.root","READ");
  TFile *cut_D = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_down.root","READ");
  TFile *cats1_right  = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_cats1_right.root","READ");
  TFile *cats1_left  = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_cats1_left.root","READ");
  TFile *cats1_top = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_cats1_top.root","READ");
  TFile *cats1_down = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_cats1_down.root","READ");

  // Load your cuts here if needed
  cut_left = (TCutG*)cut_L->FindObjectAny("cut_theau");
  cut_right = (TCutG*)cut_R->FindObjectAny("cut_theau2");
  cut_up = (TCutG*)cut_U->FindObjectAny("cut_up");
  cut_down = (TCutG*)cut_D->FindObjectAny("cut_down");
  cut_cats1_right = (TCutG*)cats1_right->FindObjectAny("cut_cats1_right");
  cut_cats1_left = (TCutG*)cats1_left->FindObjectAny("CUTG");
  cut_cats1_top = (TCutG*)cats1_top->FindObjectAny("cut_cats1_top");
  cut_cats1_down = (TCutG*)cats1_down->FindObjectAny("cut_cats1_down");
}

void build_trajectory(){

    ///////////////////////////////////////////////////////main functions////////////////////////////////////////////////////////
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    loadCUTS();
    Long64_t tot_entries= tree->GetEntries();
    cout<<"Total Entries in tree1: "<<tot_entries<<endl;

    TTreeReader reader(tree);
    TTreeReaderValue<ZddPhysics> phy_zdd_r(reader, "zdd");
    TTreeReaderValue<unsigned int> GATCONF_r(reader, "GATCONF");
    TTreeReaderValue<unsigned long long> GATCONFTS_r(reader, "GATCONFTS");
    TTreeReaderValue<CatsPhysics> phy_cats_r(reader, "cats");


    ///////////////////////////////////////////////////////output file def////////////////////////////////////////////////////////

    TFile* outputFile = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/macro_output/build_trajectory.root", "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        outputFile->Close();
        return;
    }
    TTree* outputTree = new TTree("t", "Tree");


    ///////////////////////////////////////////////////////variable define////////////////////////////////////////////////////////
    
    Double_t Position[3] = {-1587.1, -1090.1, 1260.9}; //CATS1 z pos, CATS2 z pos, DC z pos
    Double_t Position_ZDD[4] = {1260.9+10, 1260.9+30, 1560.9, 2060.9}; // Positions of DC1 (1 cm more), DC2 (3 cm more ), IC1, and Plastics
    Double_t Target_Z = 0;
    Double_t Position_Mask[2] = {-1732.1,-1235.1}; // CATS1 X position, CATS2 X position
    Double_t pos_mask_1_X; 
    Double_t pos_mask_2_X; 
    Double_t pos_mask_1_Y; 
    Double_t pos_mask_2_Y; 
    Double_t ay;
    Double_t ax;
    Double_t CATS1_X;
    Double_t CATS1_Y;
    Double_t CATS1_Z;
    Double_t CATS2_X;
    Double_t CATS2_Y;
    Double_t CATS2_Z;
    Double_t theta = -50;
    Double_t theta_x = -50;
    Double_t theta_y = -50;


    Double_t TS_sub1, TS_sub2;
    outputTree->Branch("TS_DC1_sub", &TS_sub1, "TS_DC1_sub/D");
    outputTree->Branch("TS_DC2_sub", &TS_sub2, "TS_DC2_sub/D");
    TH1 *h1 = new TH1D("h1", "DC 1 Time Subtraction", 800, -600, 200);
    TH1 *h2 = new TH1D("h2", "DC 2 Time Subtraction", 800, -600, 200);


    TH2F *hcats_recons_np4 = new TH2F("hcats_recons_np4", "CATS Reconstructed Position by nptool", 1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS  = new TH2F("hcats_recons_PS", "Beam Spot on Target mannual reconstruction",          1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS2  = new TH2F("hcats_recons_PS2", "Beam Spot on DC1 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS3  = new TH2F("hcats_recons_PS3", "Beam Spot on DC2 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS4  = new TH2F("hcats_recons_PS4", "Beam Spot on Plastics plane",  1800, -30, 30, 1800, -30, 30);

    TH2F *Hhcats_recons_PS  = new TH2F("Hhcats_recons_PS", "gated Beam Spot on Target mannual reconstruction",          1800, -30, 30, 1800, -30, 30);
    TH2F *Hhcats_recons_PS2  = new TH2F("Hhcats_recons_PS2", "gated Beam Spot on DC1 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *Hhcats_recons_PS3  = new TH2F("Hhcats_recons_PS3", "gated Beam Spot on DC2 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *Hhcats_recons_PS4  = new TH2F("Hhcats_recons_PS4", "gated Beam Spot on Plastics plane",  1800, -30, 30, 1800, -30, 30);

    TH2F *Lhcats_recons_PS   = new TH2F("Lhcats_recons_PS",  "left gated Beam Spot on Target mannual reconstruction",          1800, -30, 30, 1800, -30, 30);
    TH2F *Lhcats_recons_PS2  = new TH2F("Lhcats_recons_PS2", "left gated Beam Spot on DC1 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *Lhcats_recons_PS3  = new TH2F("Lhcats_recons_PS3", "left gated Beam Spot on DC2 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *Lhcats_recons_PS4  = new TH2F("Lhcats_recons_PS4", "left gated Beam Spot on Plastics plane",  1800, -30, 30, 1800, -30, 30);

    TH2F *Ucats_recons_PS   = new TH2F("Ucats_recons_PS",  "up gated Beam Spot on Target mannual reconstruction",          1800, -30, 30, 1800, -30, 30);
    TH2F *Ucats_recons_PS2  = new TH2F("Ucats_recons_PS2", "up gated Beam Spot on DC1 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *Ucats_recons_PS3  = new TH2F("Ucats_recons_PS3", "up gated Beam Spot on DC2 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *Ucats_recons_PS4  = new TH2F("Ucats_recons_PS4", "up gated Beam Spot on Plastics plane",  1800, -30, 30, 1800, -30, 30);

    TH2F *Dcats_recons_PS   = new TH2F("Dcats_recons_PS",  "down gated Beam Spot on Target mannual reconstruction",          1800, -30, 30, 1800, -30, 30);
    TH2F *Dcats_recons_PS2  = new TH2F("Dcats_recons_PS2", "down gated Beam Spot on DC1 plane",       1800, -30, 30, 1800, -30, 30);    
    TH2F *Dcats_recons_PS3  = new TH2F("Dcats_recons_PS3", "down gated Beam Spot on DC2 plane",       1800, -30, 30, 1800, -30, 30);
    TH2F *Dcats_recons_PS4  = new TH2F("Dcats_recons_PS4", "down gated Beam Spot on Plastics plane",  1800, -30, 30, 1800, -30, 30);


    TH2F* Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 381, -1700, 2100, 1000, -499, 500 );
    TH2F* Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 381, -1700, 2100, 1000, -499, 500 );
    TH2F* Trajectory_XZ_left_cut = new TH2F("Trajectory_XZ_left_cut", "Reconstruction of beam trajectory in detectors left cut (X projection)", 381, -1700, 2100, 1000, -499, 500 );
    TH2F* Trajectory_YZ_left_cut = new TH2F("Trajectory_YZ_left_cut", "Reconstruction of beam trajectory in detectors left cut (Y projection)", 381, -1700, 2100, 1000, -499, 500 );
    TH2F* Trajectory_XZ_right_cut = new TH2F("Trajectory_XZ_right_cut", "Reconstruction of beam trajectory in detectors right cut (X projection)", 381, -1700, 2100, 1000, -499, 500 );
    TH2F* Trajectory_YZ_right_cut = new TH2F("Trajectory_YZ_right_cut", "Reconstruction of beam trajectory in detectors right cut (Y projection)", 381, -1700, 2100, 1000, -499, 500 );

    TH2F* h_Mask_1 = new TH2F("h_Mask_1", "Mask 1 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* h_Mask_2 = new TH2F("h_Mask_2", "Mask 2 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* hcats_1 = new TH2F("hcats_1", "CATS 1 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* hcats_2 = new TH2F("hcats_2", "CATS 2 Position", 1800, -30, 30, 1800, -30, 30);

    TH1F *h_theta = new TH1F("h_theta", "Beam angle w.r.t z axis", 200, -5, 5);

    TH1F *h_theta_x = new TH1F("h_theta_x", "Beam angle in X projection w.r.t z axis", 200, -5, 5);
    TH1F *h_theta_y = new TH1F("h_theta_y", "Beam angle in Y projection w.r.t z axis", 200, -5, 5);
    

    ///////////////////////////////////////////////////////      code     ////////////////////////////////////////////////////////

    while(reader.Next()){
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }
        theta = -50;
        theta_x = -50;
        theta_y = -50;
        CATS1_X = -1000.0;
        CATS1_Y = -1000.0;
        CATS1_Z = -1000.0;
        CATS2_X = -1000.0;
        CATS2_Y = -1000.0;
        CATS2_Z = -1000.0;
        ay = -1000.0;
        ax = -1000.0;
        pos_mask_1_X = -1000.0;
        pos_mask_2_X = -1000.0;
        pos_mask_1_Y = -1000.0;
        pos_mask_2_Y = -1000.0;

        if (*GATCONF_r == 4) {
            if(phy_cats_r->PositionOnTargetY > -200 && phy_cats_r->PositionOnTargetX > -200){


///////////////////////////////////////////DEFINING CATS POSITIONS/////////////////////////////////////////////////////

                Double_t targetX_byCATS = phy_cats_r->PositionOnTargetX;
                Double_t targetY_byCATS = phy_cats_r->PositionOnTargetY;


                if(phy_cats_r->DetNumber.size()==2){
                    if(phy_cats_r->DetNumber[0] == 1){
                        CATS1_X =  phy_cats_r->PositionX[0];
                        CATS1_Y =  phy_cats_r->PositionY[0];
                        CATS1_Z =  phy_cats_r->PositionZ[0];
                    }
                    if(phy_cats_r->DetNumber[1] == 2){
                        CATS2_X =  phy_cats_r->PositionX[1]/*  + 3.3 */;
                        CATS2_Y =  phy_cats_r->PositionY[1];
                        CATS2_Z =  phy_cats_r->PositionZ[1];
                    }
                }
///////////////////////////////////////////DEFINING CATS POSITIONS/////////////////////////////////////////////////////




///////////////////////////////////////////DEFINING SLOPES//////////////////////////////////////////////////////////
                hcats_recons_np4 -> Fill(targetX_byCATS,targetY_byCATS);                    

                ay = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
                ax = (Position[1] - Position[0])/(CATS2_X - CATS1_X);
///////////////////////////////////////////DEFINING SLOPES//////////////////////////////////////////////////////////


///////////////////////////////////////////EXTRAPOLATING MASKS//////////////////////////////////////////////////////////

                pos_mask_1_X = CATS1_X + ( Position_Mask[0] - Position[0])/ax;
                pos_mask_2_X = CATS1_X + ( Position_Mask[1] - Position[0])/ax;
                pos_mask_1_Y = CATS1_Y + ( Position_Mask[0] - Position[0])/ay;
                pos_mask_2_Y = CATS1_Y + ( Position_Mask[1] - Position[0])/ay;

                h_Mask_1 -> Fill(pos_mask_1_X,pos_mask_1_Y);
                h_Mask_2 -> Fill(pos_mask_2_X,pos_mask_2_Y);

///////////////////////////////////////////EXTRAPOLATING MASKS//////////////////////////////////////////////////////////



///////////////////////////////////////////PLAYGROUND//////////////////////////////////////////////////////////
                hcats_1->Fill(CATS1_X, CATS1_Y);
                hcats_2->Fill(CATS2_X,CATS2_Y);
                if(phy_zdd_r->PL_E.size() ==2) {
                    hcats_recons_PS->Fill(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);    //mannual reconstruction of beam spot on target
                    hcats_recons_PS2->Fill(CATS1_X + ( Position_ZDD[0] - Position[0])/ax,CATS1_Y + (Position_ZDD[0] - Position[0])/ay);   //reconstruction of beam spot on DC1 plane
                    hcats_recons_PS3->Fill(CATS1_X + ( Position_ZDD[1] - Position[0])/ax,CATS1_Y + (Position_ZDD[1] - Position[0])/ay);   //reconstruction of beam spot on DC2 planes
                    hcats_recons_PS4->Fill(CATS1_X + ( Position_ZDD[3] - Position[0])/ax,CATS1_Y + (Position_ZDD[3] - Position[0])/ay);   //reconstruction of beam spot on Plastics
                    
                }

                    if(cut_right->IsInside(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay) ){
                        Hhcats_recons_PS->Fill(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);
                        Hhcats_recons_PS2->Fill(CATS1_X + ( Position_ZDD[0] - Position[0])/ax,CATS1_Y + (Position_ZDD[0] - Position[0])/ay);   //reconstruction of beam spot on DC1 plane
                        Hhcats_recons_PS3->Fill(CATS1_X + ( Position_ZDD[1] - Position[0])/ax,CATS1_Y + (Position_ZDD[1] - Position[0])/ay);   //reconstruction of beam spot on DC2 planes
                        Hhcats_recons_PS4->Fill(CATS1_X + ( Position_ZDD[3] - Position[0])/ax,CATS1_Y + (Position_ZDD[3] - Position[0])/ay);   //reconstruction of beam spot on Plastics
                        
                    }

                    if(cut_left->IsInside(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay) ){
                        Lhcats_recons_PS->Fill(CATS1_X  + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);
                        Lhcats_recons_PS2->Fill(CATS1_X + ( Position_ZDD[0] - Position[0])/ax,CATS1_Y + (Position_ZDD[0] - Position[0])/ay);   //reconstruction of beam spot on DC1 plane
                        Lhcats_recons_PS3->Fill(CATS1_X + ( Position_ZDD[1] - Position[0])/ax,CATS1_Y + (Position_ZDD[1] - Position[0])/ay);   //reconstruction of beam spot on DC2 planes
                        Lhcats_recons_PS4->Fill(CATS1_X + ( Position_ZDD[3] - Position[0])/ax,CATS1_Y + (Position_ZDD[3] - Position[0])/ay);   //reconstruction of beam spot on Plastics
                        
                    }

                    if(cut_up->IsInside(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay) ){
                        Ucats_recons_PS->Fill(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);
                        Ucats_recons_PS2->Fill(CATS1_X + ( Position_ZDD[0] - Position[0])/ax,CATS1_Y + (Position_ZDD[0] - Position[0])/ay);   //reconstruction of beam spot on DC1 plane
                        Ucats_recons_PS3->Fill(CATS1_X + ( Position_ZDD[1] - Position[0])/ax,CATS1_Y + (Position_ZDD[1] - Position[0])/ay);   //reconstruction of beam spot on DC2 planes
                        Ucats_recons_PS4->Fill(CATS1_X + ( Position_ZDD[3] - Position[0])/ax,CATS1_Y + (Position_ZDD[3] - Position[0])/ay);   //reconstruction of beam spot on Plastics
                    }

                    if(cut_down->IsInside(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay) ){
                        Dcats_recons_PS->Fill(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);
                        Dcats_recons_PS2->Fill(CATS1_X + ( Position_ZDD[0] - Position[0])/ax,CATS1_Y + (Position_ZDD[0] - Position[0])/ay);   //reconstruction of beam spot on DC1 plane
                        Dcats_recons_PS3->Fill(CATS1_X + ( Position_ZDD[1] - Position[0])/ax,CATS1_Y + (Position_ZDD[1] - Position[0])/ay);   //reconstruction of beam spot on DC2 planes
                        Dcats_recons_PS4->Fill(CATS1_X + ( Position_ZDD[3] - Position[0])/ax,CATS1_Y + (Position_ZDD[3] - Position[0])/ay);   //reconstruction of beam spot on Plastics
                    }

///////////////////////////////////////////PLAYGROUND//////////////////////////////////////////////////////////


///////////////////////////////////////////TRAJECTORIES//////////////////////////////////////////////////////////

                for(int k= Position[0]; k<Position_ZDD[3]; k+=5){
                    Trajectory_XZ->Fill(k, CATS1_X + (k - Position[0])/ax);
                    Trajectory_YZ->Fill(k, CATS1_Y + (k - Position[0])/ay);
                }
                TVector3 beam_dir(CATS2_X - CATS1_X, CATS2_Y - CATS1_Y, CATS2_Z - CATS1_Z);
                //beam angle with respect to z axis in degrees
                theta = beam_dir.Theta()*TMath::RadToDeg();
                // theta = beam_dir.Theta() - TMath::Pi()/2; //angle
                h_theta->Fill(theta);
                theta_x = std::atan2(CATS2_X - CATS1_X, CATS2_Z - CATS1_Z)*TMath::RadToDeg();   h_theta_x->Fill(theta_x);
                theta_y = std::atan2(CATS2_Y - CATS1_Y, CATS2_Z - CATS1_Z)*TMath::RadToDeg();   h_theta_y->Fill(theta_y);


                if(cut_up->IsInside(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay) ){
                  if(cut_cats1_down->IsInside(CATS1_X,CATS1_Y)){
                    for(int k= Position[0]; k<Position_ZDD[0]; k+=5){
                        Trajectory_XZ_left_cut->Fill(k, CATS1_X + (k - Position[0])/ax);
                        Trajectory_YZ_left_cut->Fill(k, CATS1_Y + (k - Position[0])/ay);
                    }
                  }
                }
                if(cut_down->IsInside(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay) ){
                  if(cut_cats1_top->IsInside(CATS1_X,CATS1_Y)){
                    for(int k= Position[0]; k<Position_ZDD[0]; k+=5){
                        Trajectory_XZ_right_cut->Fill(k, CATS1_X + (k - Position[0])/ax);
                        Trajectory_YZ_right_cut->Fill(k, CATS1_Y + (k - Position[0])/ay);
                    }
                  }
                }

///////////////////////////////////////////TRAJECTORIES//////////////////////////////////////////////////////////



            }
            outputTree->Fill();


        }
    }
    TCanvas *spot_compare = new TCanvas("spot_compare", "CATS Reconstructed Position vs Beam Spot on Target", 1700, 950);

    spot_compare->Divide(2,2);
    spot_compare->cd(1);
    hcats_recons_PS->SetTitle("Beam Spot on Target");
    hcats_recons_PS->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_PS->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_PS->Draw("COLZ");
    cut_left->Draw("same");
    cut_right->Draw("same");
    cut_up->Draw("same");
    cut_down->Draw("same");

    spot_compare->cd(2);
    hcats_recons_PS4->SetTitle("Beam Spot on Plastics plane");
    hcats_recons_PS4->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_PS4->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_PS4->Draw("COLZ");


    spot_compare->cd(3);
    hcats_recons_PS2->SetTitle("Beam Spot on DC1 plane");
    hcats_recons_PS2->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_PS2->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_PS2->Draw("COLZ");

    spot_compare->cd(4);
    hcats_recons_PS3->SetTitle("Beam Spot on DC2 plane");
    hcats_recons_PS3->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_PS3->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_PS3->Draw("COLZ");


    spot_compare->Update();

    TCanvas *spot_compare_gated = new TCanvas("spot_compare_gated", "CATS Reconstructed Position vs Beam Spot on Target (gated)", 1700, 950);
    spot_compare_gated->Divide(2,2);
    spot_compare_gated->cd(1);
    Hhcats_recons_PS->SetTitle("gated Beam Spot on Target");
    Hhcats_recons_PS->GetXaxis()->SetTitle("X Position (mm)");
    Hhcats_recons_PS->GetYaxis()->SetTitle("Y Position (mm)");
    Hhcats_recons_PS->Draw("COLZ");
    cut_right->Draw("same");
    spot_compare_gated->cd(2);
    Hhcats_recons_PS4->SetTitle("cut right gated Beam Spot on Plastics plane");
    Hhcats_recons_PS4->GetXaxis()->SetTitle("X Position (mm)");
    Hhcats_recons_PS4->GetYaxis()->SetTitle("Y Position (mm)");
    Hhcats_recons_PS4->Draw("COLZ");    
    spot_compare_gated->cd(3);
    Hhcats_recons_PS2->SetTitle("cut right gated Beam Spot on DC1 plane");
    Hhcats_recons_PS2->GetXaxis()->SetTitle("X Position (mm)");
    Hhcats_recons_PS2->GetYaxis()->SetTitle("Y Position (mm)");
    Hhcats_recons_PS2->Draw("COLZ");
    spot_compare_gated->cd(4);
    Hhcats_recons_PS3->SetTitle("cut right gated Beam Spot on DC2 plane");
    Hhcats_recons_PS3->GetXaxis()->SetTitle("X Position (mm)");
    Hhcats_recons_PS3->GetYaxis()->SetTitle("Y Position (mm)");
    Hhcats_recons_PS3->Draw("COLZ");
    spot_compare_gated->Update();

    TCanvas *spot_compare_gated_left = new TCanvas("spot_compare_gated_left", "CATS Reconstructed Position vs Beam Spot on Target (left gated)", 1700, 950);
    spot_compare_gated_left->Divide(2,2);
    spot_compare_gated_left->cd(1);
    Lhcats_recons_PS->SetTitle("left gated Beam Spot on Target");
    Lhcats_recons_PS->GetXaxis()->SetTitle("X Position (mm)");
    Lhcats_recons_PS->GetYaxis()->SetTitle("Y Position (mm)");
    Lhcats_recons_PS->Draw("COLZ");
    cut_left->Draw("same");
    spot_compare_gated_left->cd(2);
    Lhcats_recons_PS4->SetTitle("cut left gated Beam Spot on Plastics plane");
    Lhcats_recons_PS4->GetXaxis()->SetTitle("X Position (mm)");
    Lhcats_recons_PS4->GetYaxis()->SetTitle("Y Position (mm)");
    Lhcats_recons_PS4->Draw("COLZ");
    spot_compare_gated_left->cd(3);
    Lhcats_recons_PS2->SetTitle("cut left gated Beam Spot on DC1 plane");
    Lhcats_recons_PS2->GetXaxis()->SetTitle("X Position (mm)");
    Lhcats_recons_PS2->GetYaxis()->SetTitle("Y Position (mm)");
    Lhcats_recons_PS2->Draw("COLZ");    
    spot_compare_gated_left->cd(4);
    Lhcats_recons_PS3->SetTitle("cut left gated Beam Spot on DC2 plane");
    Lhcats_recons_PS3->GetXaxis()->SetTitle("X Position (mm)");
    Lhcats_recons_PS3->GetYaxis()->SetTitle("Y Position (mm)");
    Lhcats_recons_PS3->Draw("COLZ");
    spot_compare_gated_left->Update();

    TCanvas *spot_compare_gated_up = new TCanvas("spot_compare_gated_up", "CATS Reconstructed Position vs Beam Spot on Target (up gated)", 1700, 950);
    spot_compare_gated_up->Divide(2,2);
    spot_compare_gated_up->cd(1);
    Ucats_recons_PS->SetTitle("up gated Beam Spot on Target");
    Ucats_recons_PS->GetXaxis()->SetTitle("X Position (mm)");
    Ucats_recons_PS->GetYaxis()->SetTitle("Y Position (mm)");
    Ucats_recons_PS->Draw("COLZ");
    cut_up->Draw("same");
    spot_compare_gated_up->cd(2);
    Ucats_recons_PS4->SetTitle("cut up gated Beam Spot on Plastics plane");
    Ucats_recons_PS4->GetXaxis()->SetTitle("X Position (mm)");
    Ucats_recons_PS4->GetYaxis()->SetTitle("Y Position (mm)");
    Ucats_recons_PS4->Draw("COLZ");
    spot_compare_gated_up->cd(3);
    Ucats_recons_PS2->SetTitle("cut up gated Beam Spot on DC1 plane");
    Ucats_recons_PS2->GetXaxis()->SetTitle("X Position (mm)");
    Ucats_recons_PS2->GetYaxis()->SetTitle("Y Position (mm)");
    Ucats_recons_PS2->Draw("COLZ");    
    spot_compare_gated_up->cd(4);
    Ucats_recons_PS3->SetTitle("cut up gated Beam Spot on DC2 plane");
    Ucats_recons_PS3->GetXaxis()->SetTitle("X Position (mm)");
    Ucats_recons_PS3->GetYaxis()->SetTitle("Y Position (mm)");
    Ucats_recons_PS3->Draw("COLZ");
    spot_compare_gated_up->Update();    

    TCanvas *spot_compare_gated_down = new TCanvas("spot_compare_gated_down", "CATS Reconstructed Position vs Beam Spot on Target (down gated)", 1700, 950);
    spot_compare_gated_down->Divide(2,2);
    spot_compare_gated_down->cd(1);
    Dcats_recons_PS->SetTitle("down gated Beam Spot on Target");        
    Dcats_recons_PS->GetXaxis()->SetTitle("X Position (mm)");
    Dcats_recons_PS->GetYaxis()->SetTitle("Y Position (mm)");
    Dcats_recons_PS->Draw("COLZ");
    cut_down->Draw("same");
    spot_compare_gated_down->cd(2);
    Dcats_recons_PS4->SetTitle("cut down gated Beam Spot on Plastics plane");
    Dcats_recons_PS4->GetXaxis()->SetTitle("X Position (mm)");
    Dcats_recons_PS4->GetYaxis()->SetTitle("Y Position (mm)");
    Dcats_recons_PS4->Draw("COLZ");
    spot_compare_gated_down->cd(3);
    Dcats_recons_PS2->SetTitle("cut down gated Beam Spot on DC1 plane");
    Dcats_recons_PS2->GetXaxis()->SetTitle("X Position (mm)");
    Dcats_recons_PS2->GetYaxis()->SetTitle("Y Position (mm)");
    Dcats_recons_PS2->Draw("COLZ");    
    spot_compare_gated_down->cd(4);
    Dcats_recons_PS3->SetTitle("cut down gated Beam Spot on DC2 plane");
    Dcats_recons_PS3->GetXaxis()->SetTitle("X Position (mm)");
    Dcats_recons_PS3->GetYaxis()->SetTitle("Y Position (mm)");
    Dcats_recons_PS3->Draw("COLZ");
    spot_compare_gated_down->Update();



    TCanvas *trajectory = new TCanvas("trajectory", "CATS Reconstructed Trajectory Till Plastics", 1700, 950);
    trajectory->Divide(1,2);
    trajectory->cd(1);
    Trajectory_XZ->SetTitle("Reconstruction of beam trajectory in detectors (X projection)");
    Trajectory_XZ->GetXaxis()->SetTitle("Z Position (mm)");
    Trajectory_XZ->GetYaxis()->SetTitle("X Position (mm)");
    Trajectory_XZ->Draw("COLZ");

    trajectory->cd(2);
    Trajectory_YZ->SetTitle("Reconstruction of beam trajectory in detectors (Y projection)");
    Trajectory_YZ->GetXaxis()->SetTitle("Z Position (mm)");
    Trajectory_YZ->GetYaxis()->SetTitle("Y Position (mm)");
    Trajectory_YZ->Draw("COLZ");

    trajectory->Update();

    TCanvas *trajectory_left = new TCanvas("trajectory_left", "CATS Reconstructed Trajectory Till DC1 (left cut)", 1700, 950);
    trajectory_left->Divide(1,2);
    trajectory_left->cd(1);
    Trajectory_XZ_left_cut->SetTitle("Reconstruction of beam trajectory in detectors left cut (X projection)");
    Trajectory_XZ_left_cut->GetXaxis()->SetTitle("Z Position (mm)");
    Trajectory_XZ_left_cut->GetYaxis()->SetTitle("X Position (mm)");
    Trajectory_XZ_left_cut->Draw("COLZ");
    trajectory_left->cd(2);
    Trajectory_YZ_left_cut->SetTitle("Reconstruction of beam trajectory in detectors left cut (Y projection)");
    Trajectory_YZ_left_cut->GetXaxis()->SetTitle("Z Position (mm)");
    Trajectory_YZ_left_cut->GetYaxis()->SetTitle("Y Position (mm)");
    Trajectory_YZ_left_cut->Draw("COLZ");
    trajectory_left->Update();

    TCanvas *trajectory_right = new TCanvas("trajectory_right", "CATS Reconstructed Trajectory Till DC1 (right cut)", 1700, 950);
    trajectory_right->Divide(1,2);
    trajectory_right->cd(1);
    Trajectory_XZ_right_cut->SetTitle("Reconstruction of beam trajectory in detectors right cut (X projection)");
    Trajectory_XZ_right_cut->GetXaxis()->SetTitle("Z Position (mm)");
    Trajectory_XZ_right_cut->GetYaxis()->SetTitle("X Position (mm)");
    Trajectory_XZ_right_cut->Draw("COLZ");
    trajectory_right->cd(2);
    Trajectory_YZ_right_cut->SetTitle("Reconstruction of beam trajectory in detectors right cut (Y projection)");
    Trajectory_YZ_right_cut->GetXaxis()->SetTitle("Z Position (mm)");
    Trajectory_YZ_right_cut->GetYaxis()->SetTitle("Y Position (mm)");
    Trajectory_YZ_right_cut->Draw("COLZ");
    trajectory_right->Update();

/*     cmask = new TCanvas("cmask", "Mask Positions", 1700, 950);
    cmask->Divide(2,1);
    cmask->cd(1);
    h_Mask_1->SetTitle("Mask 1 Position");
    h_Mask_1->GetXaxis()->SetTitle("X Position (mm)");
    h_Mask_1->GetYaxis()->SetTitle("Y Position (mm)");
    h_Mask_1->Draw("COLZ");

    cmask->cd(2);
    h_Mask_2->SetTitle("Mask 2 Position");
    h_Mask_2->GetXaxis()->SetTitle("X Position (mm)");
    h_Mask_2->GetYaxis()->SetTitle("Y Position (mm)");
    h_Mask_2->Draw("COLZ");

    cmask->Update(); */


    TCanvas *theta_canvas = new TCanvas("theta_canvas", "Beam Angle Distribution", 800, 800);
    h_theta->SetTitle("Beam Angle Distribution w.r.t Z axis; Angle (degrees); Counts");
    h_theta->Draw();

    TCanvas *theta_canvas_xy = new TCanvas("theta_canvas_xy", "Beam Angle Distribution in X and Y projections", 1600, 800);
    theta_canvas_xy->Divide(2,1);
    theta_canvas_xy->cd(1);
    h_theta_x->SetTitle("Beam Angle Distribution in X projection w.r.t Z axis; Angle (degrees); Counts");
    h_theta_x->Draw();
    theta_canvas_xy->cd(2);
    h_theta_y->SetTitle("Beam Angle Distribution in Y projection w.r.t Z axis; Angle (degrees); Counts");
    h_theta_y->Draw();
    theta_canvas_xy->Update();


    ccats = new TCanvas("ccats", "CATS Positions", 1700, 950);
    ccats->Divide(2,1);
    ccats->cd(1);
    hcats_1->SetTitle("CATS 1 Position");
    hcats_1->GetXaxis()->SetTitle("X Position (mm)");
    hcats_1->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_1->Draw("COLZ");
    cut_cats1_right->Draw("same");
    cut_cats1_left->Draw("same");
    cut_cats1_down->Draw("same");
    cut_cats1_top->Draw("same");

    ccats->cd(2);
    hcats_2->SetTitle("CATS 2 Position");
    hcats_2->GetXaxis()->SetTitle("X Position (mm)");
    hcats_2->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_2->Draw("COLZ");

    ccats->Update();

    //write and close output file
    outputFile->cd();
    outputTree->Write();
    h1->Write();
    h2->Write();
    hcats_recons_np4->Write();
    hcats_recons_PS->Write();
    hcats_recons_PS2->Write();
    hcats_recons_PS3->Write();
    Trajectory_XZ->Write();
    Trajectory_YZ->Write();
    h_Mask_1->Write();
    h_Mask_2->Write();
    hcats_1->Write();
    hcats_2->Write();
    // outputFile->Close();

}

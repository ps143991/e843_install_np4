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
#include "TLine.h"
#include "TText.h"


TChain* tree = NULL;
TCanvas* c1 = NULL;
TCanvas *cmask = NULL;
TCanvas *ccats = NULL;

void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/558.root");

}

Double_t cut_midY = 0;
Double_t cut_midX = 0;


void confirming_DC_by_beamspot(){

    ///////////////////////////////////////////////////////main functions////////////////////////////////////////////////////////
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    Long64_t tot_entries= tree->GetEntries();
    cout<<"Total Entries: "<<tot_entries<<endl;

    TTreeReader reader(tree);
    TTreeReaderValue<ZddPhysics> phy_zdd_r(reader, "zdd");
    TTreeReaderValue<unsigned int> GATCONF_r(reader, "GATCONF");
    TTreeReaderValue<unsigned long long> GATCONFTS_r(reader, "GATCONFTS");
    TTreeReaderValue<CatsPhysics> phy_cats_r(reader, "cats");
    TTreeReaderArray<unsigned int> DC_E1_r(reader,"DC_1");
    TTreeReaderArray<unsigned int> DC_E2_r(reader,"DC_2");
    TTreeReaderArray<unsigned int> DC_E3_r(reader,"DC_3");
    TTreeReaderArray<unsigned int> DC_E4_r(reader,"DC_4");


    ///////////////////////////////////////////////////////output file def////////////////////////////////////////////////////////

    TFile* outputFile = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/macro_output/confirming_DC.root", "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        outputFile->Close();
        return;
    }
    TTree* outputTree = new TTree("t", "Tree");


    ///////////////////////////////////////////////////////variable define////////////////////////////////////////////////////////
    
    Double_t Position[3] = {-1587.1, -1090.6, 1260.4}; //CATS1 z pos, CATS2 z pos, DC z pos
    Double_t Position_ZDD[4] = {1140.9, 1385.9, 1560.9, 2060.9}; // Positions of DC1(||), DC2(=), IC1, and Plastics
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

    Double_t TS_sub1, TS_sub2;
    outputTree->Branch("TS_DC1_sub", &TS_sub1, "TS_DC1_sub/D");
    outputTree->Branch("TS_DC2_sub", &TS_sub2, "TS_DC2_sub/D");
    TH1 *h1 = new TH1D("h1", "DC 1 Time Subtraction", 800, -600, 200);
    TH1 *h2 = new TH1D("h2", "DC 2 Time Subtraction", 800, -600, 200);


    TH2F *hcats_recons_np4 = new TH2F("hcats_recons_np4", "CATS Reconstructed Position from np4", 1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS  = new TH2F("hcats_recons_PS", "Beam Spot on Target manual",          1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS2  = new TH2F("hcats_recons_PS2", "Beam Spot on Target2",       1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS3  = new TH2F("hcats_recons_PS3", "Beam Spot on Target3",       1800, -30, 30, 1800, -30, 30);

    TH2F* Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 380, -1700, 2100, 1000, -50, 50 );
    TH2F* Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 380, -1700, 2100, 1000, -50, 50 );
    TH2F* h_Mask_1 = new TH2F("h_Mask_1", "Mask 1 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* h_Mask_2 = new TH2F("h_Mask_2", "Mask 2 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* hcats_1 = new TH2F("hcats_1", "CATS 1 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* hcats_2 = new TH2F("hcats_2", "CATS 2 Position", 1800, -30, 30, 1800, -30, 30);

    TH1F* h_dc1 = new TH1F("h_dc1", "DC 1 Energy Spectrum", 800, -4000, 4000);
    TH1F* h_dc2 = new TH1F("h_dc2", "DC 2 Energy Spectrum", 800, -4000, 4000);
    TH1F* h_dc3 = new TH1F("h_dc3", "DC 3 Energy Spectrum", 800, -4000, 4000);
    TH1F* h_dc4 = new TH1F("h_dc4", "DC 4 Energy Spectrum", 800, -4000, 4000);

    TH1F* h_dc1_cond = new TH1F("h_dc1_cond", "DC 1 Energy Spectrum with CATS Y>0", 800, -4000, 4000);
    TH1F* h_dc2_cond = new TH1F("h_dc2_cond", "DC 2 Energy Spectrum with CATS Y>0", 800, -4000, 4000);
    TH1F* h_dc3_cond = new TH1F("h_dc3_cond", "DC 3 Energy Spectrum with CATS X>0", 800, -4000, 4000);
    TH1F* h_dc4_cond = new TH1F("h_dc4_cond", "DC 4 Energy Spectrum with CATS X>0", 800, -4000, 4000);

    ///////////////////////////////////////////////////////      code     ////////////////////////////////////////////////////////
    long long entry_limit = 1000000;
    long long entry_count = 0;

    while(reader.Next() /* && entry_count < entry_limit */){
        // if (++entry_count > entry_limit) break;
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }
        entry_count++;
        CATS1_X = -300.0;
        CATS1_Y = -300.0;
        CATS1_Z = -300.0;
        CATS2_X = -300.0;
        CATS2_Y = -300.0;
        CATS2_Z = -300.0;
        ay = -1000.0;
        ax = -1000.0;
        pos_mask_1_X = -300.0;
        pos_mask_2_X = -300.0;
        pos_mask_1_Y = -300.0;
        pos_mask_2_Y = -300.0;

        // if (*GATCONF_r == 32) {
            if(phy_cats_r->PositionOnTargetY > -200 && phy_cats_r->PositionOnTargetX > -200){
                Double_t targetX_byCATS = phy_cats_r->PositionOnTargetX;
                Double_t targetY_byCATS = phy_cats_r->PositionOnTargetY;


                hcats_recons_np4 -> Fill(targetX_byCATS,targetY_byCATS);
                
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


                ay = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
                ax = (Position[1] - Position[0])/(CATS2_X - CATS1_X);




                if(phy_zdd_r->PL_E.size() ==2) {
                    hcats_recons_PS->Fill(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);
                }

                if(DC_E1_r.GetSize()>0){
                    h_dc1->Fill(DC_E1_r[0]);
                    if(targetY_byCATS > cut_midY){
                        h_dc1_cond -> Fill(DC_E1_r[0]);
                    }
                }
                if(DC_E2_r.GetSize()>0){
                    h_dc2->Fill(-static_cast<int>(DC_E2_r[0]));
                    if(targetY_byCATS > cut_midY){
                        h_dc2_cond -> Fill(-static_cast<int>(DC_E2_r[0]));
                    }
                }
                if(DC_E3_r.GetSize()>0){
                    h_dc3->Fill(-static_cast<int>(DC_E3_r[0]));
                    if(targetX_byCATS > cut_midY){
                        h_dc3_cond -> Fill(-static_cast<int>(DC_E3_r[0]));
                    }
                }
                if(DC_E4_r.GetSize()>0){
                    h_dc4->Fill(DC_E4_r[0]);
                    if(targetX_byCATS > cut_midX){
                        h_dc4_cond -> Fill(DC_E4_r[0]);
                    }
                }
                

            outputTree->Fill();


        // }
    }
}

                TText *text1 = new TText(25,0,"Right");
                TText *text2 = new TText(-25,0,"Left");
                TText *text3 = new TText(0,25,"Up");
                TText *text4 = new TText(0,-25,"Down");


                TLine *line_middle_horiz = new TLine(-30,cut_midY,30,cut_midY);
                line_middle_horiz->SetLineColor(kRed);
                line_middle_horiz->SetLineStyle(2);
                line_middle_horiz->SetLineWidth(3);
                TLine *line_middle_verti = new TLine(cut_midX,-30,cut_midX,30);
                line_middle_verti->SetLineColor(kRed);
                line_middle_verti->SetLineStyle(2);
                line_middle_verti->SetLineWidth(3);

    TCanvas *spot_compare = new TCanvas("spot_compare", "CATS Reconstructed Position vs Beam Spot on Target", 1700, 950);

    spot_compare->Divide(1,2);
    spot_compare->cd(1);
    hcats_recons_np4->SetTitle("CATS Reconstructed Position");
    hcats_recons_np4->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_np4->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_np4->Draw("COLZ");

                text1->Draw("same");
                text2->Draw("same");
                text3->Draw("same");
                text4->Draw("same");
                line_middle_horiz->Draw("same");
                line_middle_verti->Draw("same");

    spot_compare->cd(2);
    hcats_recons_PS->SetTitle("Beam Spot on Target");
    hcats_recons_PS->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_PS->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_PS->Draw("COLZ");

   

                text1->Draw("same");
                text2->Draw("same");
                text3->Draw("same");
                text4->Draw("same");
                line_middle_horiz->Draw("same");
                line_middle_verti->Draw("same"); 



    spot_compare->Update();




    // TCanvas *dc1_dc2 = new TCanvas("dc1_dc2", "DC 1 and DC2 Energy Spectrum", 1700, 950);
    // h_dc1->Draw();
    // h_dc2->SetLineColor(kRed);
    // h_dc2->Draw("same");

    TCanvas *dc3_dc4 = new TCanvas("dc3_dc4", "DC 3 and DC4 Energy Spectrum", 1700, 950);
    h_dc3->SetLineWidth(2);
    h_dc3->Draw();
    h_dc4->SetLineColor(kRed);
    h_dc4->SetLineWidth(2);
    h_dc4->Draw("same");
    h_dc3_cond->SetLineColor(kBlue);
    h_dc3_cond->SetLineStyle(2);
    h_dc3_cond->SetLineWidth(2);
    h_dc3_cond->Draw("same");
    h_dc4_cond->SetLineColor(kGreen+2);
    h_dc4_cond->SetLineStyle(2);
    h_dc4_cond->SetLineWidth(2);
    h_dc4_cond->Draw("same");

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

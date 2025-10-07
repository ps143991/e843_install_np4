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
    // tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatA.txt.root");
    // tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatB.txt.root");
    // tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatC.txt.root");
    // tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatD.txt.root");
    // tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatE.txt.root");

}


void confirming_DC(){

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

    Double_t TS_sub1, TS_sub2;
    outputTree->Branch("TS_DC1_sub", &TS_sub1, "TS_DC1_sub/D");
    outputTree->Branch("TS_DC2_sub", &TS_sub2, "TS_DC2_sub/D");
    TH1 *h1 = new TH1D("h1", "DC 1 Time Subtraction", 800, -600, 200);
    TH1 *h2 = new TH1D("h2", "DC 2 Time Subtraction", 800, -600, 200);


    TH2F *hcats_recons_np4 = new TH2F("hcats_recons_np4", "CATS Reconstructed Position", 1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS  = new TH2F("hcats_recons_PS", "Beam Spot on Target",          1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS2  = new TH2F("hcats_recons_PS2", "Beam Spot on Target2",       1800, -30, 30, 1800, -30, 30);
    TH2F *hcats_recons_PS3  = new TH2F("hcats_recons_PS3", "Beam Spot on Target3",       1800, -30, 30, 1800, -30, 30);

    TH2F* Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 380, -1700, 2100, 1000, -50, 50 );
    TH2F* Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 380, -1700, 2100, 1000, -50, 50 );
    TH2F* h_Mask_1 = new TH2F("h_Mask_1", "Mask 1 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* h_Mask_2 = new TH2F("h_Mask_2", "Mask 2 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* hcats_1 = new TH2F("hcats_1", "CATS 1 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* hcats_2 = new TH2F("hcats_2", "CATS 2 Position", 1800, -30, 30, 1800, -30, 30);

    

    ///////////////////////////////////////////////////////      code     ////////////////////////////////////////////////////////

    auto *canv_indiv = new TCanvas("canv_indiv", "trajectory event by event", 1600, 1000);
    canv_indiv -> Divide(1,2);

    while(reader.Next()){
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }
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

                pos_mask_1_X = CATS1_X + ( Position_Mask[0] - Position[0])/ax;
                pos_mask_2_X = CATS1_X + ( Position_Mask[1] - Position[0])/ax;
                pos_mask_1_Y = CATS1_Y + ( Position_Mask[0] - Position[0])/ay;
                pos_mask_2_Y = CATS1_Y + ( Position_Mask[1] - Position[0])/ay;

                h_Mask_1 -> Fill(pos_mask_1_X,pos_mask_1_Y);
                h_Mask_2 -> Fill(pos_mask_2_X,pos_mask_2_Y);

                hcats_1->Fill(CATS1_X, CATS1_Y);
                hcats_2->Fill(CATS2_X,CATS2_Y);
                // if(phy_zdd_r->PL_E.size() ==2) {
                //     hcats_recons_PS->Fill(CATS1_X + ( Target_Z - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);
                //     hcats_recons_PS2->Fill(CATS1_X + ( (Target_Z + 500) - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);
                //     hcats_recons_PS3->Fill(CATS1_X + ( (Target_Z + 300) - Position[0])/ax,CATS1_Y + (Target_Z - Position[0])/ay);   
                // }
                Trajectory_XZ->Reset();
                Trajectory_YZ->Reset();
                for(int k= Position[0]; k<Position_ZDD[0]; k+=5){
                    Trajectory_XZ->Fill(k, CATS1_X + (k - Position_ZDD[0])/ax);
                }
                for(int k= Position[0]; k<Position_ZDD[1]; k+=5){
                    Trajectory_YZ->Fill(k, CATS1_Y + (k - Position_ZDD[1])/ay);
                }

                canv_indiv->cd(1);  
                                    Trajectory_XZ->SetMarkerSize(1.0);
                                    Trajectory_XZ->SetMarkerStyle(20);    // Common filled circle
                                    Trajectory_XZ->Draw("SCAT");
                                    //a horizontal dashed line at y = 0 in each trajectory... from -1700 to 2100
                                    TLine *line1= new TLine(-1700,0,2100,0);
                                    line1->SetLineColor(kRed);
                                    line1->SetLineStyle(2);
                                    line1->SetLineWidth(3);
                                    line1->Draw("same");
                                    if(DC_E3_r.GetSize()==1){
                                        Double_t DC_E3 = DC_E3_r[0];
                                        if (DC_E3 >0) {
                                            TText *text3 = new TText(1000,-40,"DC3 Hit");
                                            text3->SetTextColor(kRed);
                                            text3->SetTextSize(0.05);
                                            text3->Draw("same");
                                        }
                                    }
                                    if(DC_E4_r.GetSize()==1){
                                        Double_t DC_E4 = DC_E4_r[0];
                                        if (DC_E4 >0) {
                                            TText *text4 = new TText(1000,+40,"DC4 Hit");
                                            text4->SetTextColor(kRed);
                                            text4->SetTextSize(0.05);
                                            text4->Draw("same");
                                        }
                                    }
                                    
                    canv_indiv->cd(2);
                    Trajectory_YZ->SetMarkerSize(1.0);
                    Trajectory_YZ->SetMarkerStyle(20);    // Common filled circle
                    Trajectory_YZ->Draw("SCAT");
                                    //a horizontal dashed line at y = 0 in each trajectory... from -1700 to 2100
                                    TLine *line2= new TLine(-1700,0,2100,0);
                                    line2->SetLineColor(kRed);
                                    line2->SetLineStyle(2);
                                    line2->SetLineWidth(3);
                                    line2->Draw("same");
                                    
                                    if(DC_E1_r.GetSize()==1){
                                        Double_t DC_E1 = DC_E1_r[0];
                                        if (DC_E1 >0) {
                                            TText *text1 = new TText(1000,40,"DC1 Hit");
                                            text1->SetTextColor(kRed);
                                            text1->SetTextSize(0.05);
                                            text1->Draw("same");
                                        }
                                    }
                                    if(DC_E2_r.GetSize()==1){
                                        Double_t DC_E2 = DC_E2_r[0];
                                        if (DC_E2 >0) {
                                            TText *text2 = new TText(1000,-40,"DC2 Hit");
                                            text2->SetTextColor(kRed);
                                            text2->SetTextSize(0.05);
                                            text2->Draw("same");
                                        }
                                    }
                    canv_indiv->Update();
                    canv_indiv->WaitPrimitive();


            outputTree->Fill();


        // }
    }
}
    TCanvas *spot_compare = new TCanvas("spot_compare", "CATS Reconstructed Position vs Beam Spot on Target", 1700, 950);

    spot_compare->Divide(2,2);
    spot_compare->cd(1);
    hcats_recons_np4->SetTitle("CATS Reconstructed Position");
    hcats_recons_np4->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_np4->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_np4->Draw("COLZ");

    spot_compare->cd(2);
    hcats_recons_PS->SetTitle("Beam Spot on Target");
    hcats_recons_PS->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_PS->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_PS->Draw("COLZ");

    spot_compare->cd(3);
    hcats_recons_PS2->SetTitle("Beam Spot on Target +500mm");
    hcats_recons_PS2->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_PS2->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_PS2->Draw("COLZ");

    spot_compare->cd(4);
    hcats_recons_PS3->SetTitle("Beam Spot on Target +300mm");
    hcats_recons_PS3->GetXaxis()->SetTitle("X Position (mm)");
    hcats_recons_PS3->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_recons_PS3->Draw("COLZ");


    spot_compare->Update();


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

    cmask = new TCanvas("cmask", "Mask Positions", 1700, 950);
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

    cmask->Update();

    ccats = new TCanvas("ccats", "CATS Positions", 1700, 950);
    ccats->Divide(2,1);
    ccats->cd(1);
    hcats_1->SetTitle("CATS 1 Position");
    hcats_1->GetXaxis()->SetTitle("X Position (mm)");
    hcats_1->GetYaxis()->SetTitle("Y Position (mm)");
    hcats_1->Draw("COLZ");

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

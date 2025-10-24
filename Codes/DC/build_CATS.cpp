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
using namespace cats;
using namespace zdd;
using namespace std;
// fit_erf_line.C
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TEllipse.h"


TChain* tree = NULL;
TCanvas* c1 = NULL;
TCanvas* c2 = NULL;
TCanvas* c3 = NULL;
TCanvas* c4 = NULL;
TCanvas* c5 = NULL;
TCanvas* c6 = NULL;
TCanvas* c7 = NULL;


struct Circle {
  double x, y;   // center
  double r;      // radius
};
struct CircleGroup {
  std::vector<Circle> cs;
  // Draw all (as circles)
  void Draw(int lineColor=kBlue, int lineWidth=2, int fillStyle=0) const {
    for (auto& c: cs){
      auto *e = new TEllipse(c.x, c.y, c.r, c.r);
      e->SetFillStyle(fillStyle);
      e->SetLineColor(lineColor);
      e->SetLineWidth(lineWidth);
      e->Draw("same");
    }
  }

};

void target_mask(){
//   TCanvas *c = new TCanvas("c","Grouped circles", 700, 700);

    gPad->cd();
//   c->DrawFrame(-20, -20, 20, 20, "Grouped circles;X;Y");

  // Build a group: 1 big circle with 3 inner circles
  CircleGroup G;
  G.cs.push_back({-2.4,-2.1,13}); // outer
  G.cs.push_back({-3.6,8.8,0.2});   // top left
  G.cs.push_back({-2.4,-2.1,0.2});  // central
  G.cs.push_back({-1.5,-10.1,0.2}); // bottop right
  G.cs.push_back({-7.37, -2.6471, 0.2}); // left middle
  G.cs.push_back({2.57, -1.5528, 0.2});  // right middle
  G.cs.push_back({-12.34, -3.194, 0.2}); // far left
  G.cs.push_back({7.54, -1.006, 0.2});   // far right
  G.cs.push_back({-2.947, 2.87, 0.2});    // top middle
  G.cs.push_back({-2.071, -5.082, 0.2});   // bottom middle
  G.cs.push_back({0.63114, 0.10651, 0.2});    // identifier hole
//   G.cs.push_back({ 0, 0, 7});
//   G.cs.push_back({-3, 2, 2});
//   G.cs.push_back({ 4,-1, 1.2});

  // Draw original
  G.Draw(kBlack, 2);

//   c->Update();
}


void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("../../data/analysed/525.root");
}
void loadMask(){
    TFile *maskfile = new TFile("./target_mask.cpp","READ");
    // CatsMask* DC_mask = (CatsMask*)maskfile->Get("DC_mask");
    // nptool::DetectorManager::getInstance()->GetDetector<CatsPhysics>("Cats")->SetMask(DC_mask);
}



void build_CATS(){
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    loadMask();
    cout<<"Total Entries: "<<tree->GetEntries()<<endl;
    TTreeReader reader(tree);
    TTreeReaderValue<ZddPhysics> phy_zdd_r(reader, "zdd");

    TTreeReaderValue<unsigned int> GATCONF_r(reader, "GATCONF");
    TTreeReaderValue<unsigned long long> GATCONFTS_r(reader, "GATCONFTS");
    TTreeReaderArray<unsigned long long> DC_TS1_r(reader,"DC_TS1");
    TTreeReaderArray<unsigned long long> DC_TS2_r(reader,"DC_TS2");
    TTreeReaderArray<unsigned long long> DC_TS3_r(reader,"DC_TS3");
    TTreeReaderArray<unsigned long long> DC_TS4_r(reader,"DC_TS4");
    TTreeReaderValue<CatsPhysics> phy_cats_r(reader, "cats");


    // TFile* outputFile = new TFile("./../../data/analysed/Build_DC_output_cats_shifted_4_with_C2X_m_1.root", "RECREATE");
    TFile* outputFile = new TFile("./../../data/analysed/test2.root", "RECREATE");

    if (!outputFile || outputFile->IsZombie()) {
        outputFile->Close();
        return;
    }
    TTree* outputTree = new TTree("t", "Tree");

    Double_t TS_sub1, TS_sub2, TS_sub3, TS_sub4;
    outputTree->Branch("TS_DC1_sub", &TS_sub1, "TS_DC1_sub/D");
    outputTree->Branch("TS_DC2_sub", &TS_sub2, "TS_DC2_sub/D");
    outputTree->Branch("TS_DC3_sub", &TS_sub3, "TS_DC3_sub/D");
    outputTree->Branch("TS_DC4_sub", &TS_sub4, "TS_DC4_sub/D");
    TH1 *h1 = new TH1D("h1", "DC 1 Time Subtraction", 800, -600, 200);
    TH1 *h2 = new TH1D("h2", "DC 2 Time Subtraction", 800, -600, 200);
    TH1 *h3 = new TH1D("h3", "DC 3 Time Subtraction", 800, -600, 200);
    TH1 *h4 = new TH1D("h4", "DC 4 Time Subtraction", 800, -600, 200);
    TH1 *h1s = new TH1D("h1s", "DC 1 Time Subtraction cut", 400, -200, 200);
    TH1 *h2s = new TH1D("h2s", "DC 2 Time Subtraction cut", 400, -200, 200);
    TH1 *h1s_opposite = new TH1D("h1s_opposite", "DC 1 Time Subtraction cut opposite", 400, -200, 200);
    TH1 *h2s_opposite = new TH1D("h2s_opposite", "DC 2 Time Subtraction cut opposite", 400, -200, 200);

    TH1 *hadded12 = new TH1D("hadded12", "DC 1 and DC 2 Time Subtraction added", 400, -200, 200);
    TH1 *hadded21 = new TH1D("hadded21", "DC 2 and DC 1 Time Subtraction added", 400, -200, 200);

    TH2F *h_y_cats_cons = new TH2F("h_y_cats_cons", "CATS Y vs TS", 400,-200,200,400, -200, 200);
    TH2F *h_x_cats_cons = new TH2F("h_x_cats_cons", "CATS X vs TS", 400,-200,200,400, -200, 200);

    TH2F *hcats_recons_PS  = new TH2F("hcats_recons_PS", "Beam Spot on Target",          2400, -40, 40, 2400, -40, 40);

    TH2F* Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 380, -1700, 2100, 1000, -500, 500 );
    TH2F* Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 380, -1700, 2100, 1000, -500, 500 );

    TH2F *h_cats1xcats1y = new TH2F("h_cats1xcats1y", "CATS1 X vs CATS1 Y", 2400, -40, 40, 2400, -40, 40);
    TH2F *h_cats2xcats2y = new TH2F("h_cats2xcats2y", "CATS2 X vs CATS2 Y", 2400, -40, 40, 2400, -40, 40);

        TH2F *h_cats1xcats2x = new TH2F("h_cats1xcats2x", "CATS1 X vs CATS2 X", 2400, -40, 40, 2400, -40, 40);
        TH2F *h_cats1ycats2y = new TH2F("h_cats1ycats2y", "CATS1 Y vs CATS2 Y", 2400, -40, 40, 2400, -40, 40);

    TH2F *h_target = new TH2F("h_target","target reconstruction with cats", 2400, -40, 40, 2400, -40, 40);
    TH2F *h_DC1 = new TH2F("h_DC1","DC1 reconstruction with cats", 2400, -40, 40, 2400, -40, 40);
    TH2F *h_DC2 = new TH2F("h_DC2","DC2 reconstruction with cats", 2400, -40, 40, 2400, -40, 40);



    Double_t Position[3] = {-1587.1, -1090.1, 1260.9}; //CATS1 z pos, CATS2 z pos, DC z pos
    Double_t Position_ZDD[4] = {1260.9+10, 1260.9+30, 1560.9, 2060.9}; // Positions of DC1 (1 cm more), DC2 (3 cm more ), IC1, and Plastics
    Double_t Target_Z = 0;
    Double_t x_cats_consDC1 = 0;
    Double_t y_cats_consDC1 = 0;
    Double_t target_cons_X = 0;
    Double_t target_cons_y = 0;
    Double_t x_cats_consDC2 = 0;
    Double_t y_cats_consDC2 = 0;


    // Long64_t tot_entries = tree->GetEntries();
    // Long64_t entry_count = 0;
    // Long64_t max_entries = 5000000; // set your desired limit
    // if(max_entries >= tot_entries) {max_entries = tot_entries;}

    while(reader.Next()){
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }
        // if (entry_count > max_entries) break;

        if (*GATCONF_r == 4) {
            if(phy_cats_r->PositionOnTargetY > -200 && phy_cats_r->PositionOnTargetX > -200){
              
              if(phy_zdd_r->PL_E.size()>0){
                Double_t targetX_byCATS = phy_cats_r->PositionOnTargetX;
                Double_t targetY_byCATS = phy_cats_r->PositionOnTargetY;

                Double_t CATS1_X =  phy_cats_r->PositionX[0];
                Double_t CATS1_Y =  phy_cats_r->PositionY[0];
                Double_t CATS1_Z =  phy_cats_r->PositionZ[0];
                Double_t CATS2_X =  (phy_cats_r->PositionX[1])    /*  - 10 */; // Shift CATS2 X position by 1 cm
                Double_t CATS2_Y =  phy_cats_r->PositionY[1];
                Double_t CATS2_Z =  phy_cats_r->PositionZ[1];

                Double_t ay = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
                Double_t ax = (Position[1] - Position[0])/(CATS2_X - CATS1_X);

                y_cats_consDC1 = CATS1_Y + ( Position_ZDD[0] - Position[0])/ay;      //dc1 const y
                x_cats_consDC1 = CATS1_X + ( Position_ZDD[0] - Position[0])/ax;      //dc1 const x

                y_cats_consDC2 = CATS1_Y + ( Position_ZDD[1] - Position[0])/ay;      //dc2 const y
                x_cats_consDC2 = CATS1_X + ( Position_ZDD[1] - Position[0])/ax;      //dc2 const x


                target_cons_X = CATS1_X + ( Target_Z - Position[0])/ax;           //target const x
                target_cons_y = CATS1_Y + ( Target_Z - Position[0])/ay;           //target const y

                for(int k= Position[0]; k<Position_ZDD[3]; k+=10){
                    Trajectory_XZ->Fill(k, CATS1_X + (k - Position[0])/ax);
                    Trajectory_YZ->Fill(k, CATS1_Y + (k - Position[0])/ay);
                }
                unsigned long long cats2_ts = (unsigned long long)(*GATCONFTS_r);
                // cout<<"DC_TS1_r: "<<DC_TS1_r.GetSize()<<" DC_TS2_r: "<<DC_TS2_r.GetSize()<<" DC_TS3_r: "<<DC_TS3_r.GetSize()<<" DC_TS4_r: "<<DC_TS4_r.GetSize()<<endl;
                if(DC_TS1_r.GetSize()==1){
                    unsigned long long dc_1_ts  = (unsigned long long)(DC_TS1_r[0]);
                    TS_sub1 = static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_1_ts);
                    h1->Fill(TS_sub1);
                }
                if(DC_TS2_r.GetSize()==1){
                    unsigned long long dc_2_ts  = (unsigned long long)(DC_TS2_r[0]);
                    TS_sub2 =  static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_2_ts);
                    h2->Fill(TS_sub2);
                }
                if(DC_TS3_r.GetSize()==1){
                    unsigned long long dc_3_ts  = (unsigned long long)(DC_TS3_r[0]);
                    TS_sub3 = static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_3_ts);
                    h3->Fill(TS_sub3);
                }
                if(DC_TS4_r.GetSize()==1){
                    unsigned long long dc_4_ts  = (unsigned long long)(DC_TS4_r[0]);
                    TS_sub4 =  static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_4_ts);
                    h4->Fill(TS_sub4);
                }

                h_target->Fill(target_cons_X,target_cons_y);
                h_DC1->Fill(x_cats_consDC1,y_cats_consDC1);
                h_DC2->Fill(x_cats_consDC2,y_cats_consDC2);

                outputTree->Fill();
                // cout<<"************************************************"<<endl;
              }
            }
        }
    }
    // c1 = new TCanvas("c1", "DC12 Time Subtraction", 800, 600);
    // c2 = new TCanvas("c2", "DC34 Time Subtraction", 800, 600);

    // c1->Divide(1,2);
    // c1->cd(1);
    // h1->SetLineColor(kBlue);
    // h1->Draw(); 
    // fit_erf_line(h1, -600, 200);
    // c1->cd(2);
    // h2->SetLineColor(kBlue);
    // h2->Draw();
    // fit_erf_line(h2, -600, 200);
    
    // c2->Divide(1,2);
    // c2->cd(1);
    // h3->SetLineColor(kBlue);
    // h3->Draw();
    // fit_erf_line(h3, -600, 200);
    // c2->cd(2);
    // h4->SetLineColor(kBlue);
    // h4->Draw();
    // fit_erf_line(h4, -600, 200);
    // c1->Update();
    // c2->Update();
    c3 = new TCanvas("c3", "DC Time Subtraction Added", 800, 600);
    c3->Divide(2,2);
    c3->cd(1);
    h1s->SetLineColor(kRed);
    h1s->Draw();
    h2s->SetLineColor(kBlue);
    h2s->Draw("same");
    c3->cd(3);
    hadded12->SetLineColor(kRed);
    hadded12->Draw();
    c3->cd(2);
    h1s_opposite->SetLineColor(kRed+1);
    h1s_opposite->Draw();
    h2s_opposite->SetLineColor(kBlue+1);
    h2s_opposite->Draw("same");
    c3->cd(4);
    hadded21->SetLineColor(kBlue);
    hadded21->Draw();
    c3->cd(4);
    // hadded12->Fit("gaus","R");
    c3->Update();
    c4 = new TCanvas("c4", "recons", 800, 600);
    c4->Divide(1,2);
    c4->cd(1);
    h_x_cats_cons->Draw("COLZ");
    c4->cd(2);
    h_y_cats_cons->Draw("COLZ");
    c5 = new TCanvas("c5", "Trajectory XZ", 800, 600);
    c5->Divide(1,2);
    c5->cd(1);
    Trajectory_XZ->Draw("COLZ");
    c5->cd(2);
    Trajectory_YZ->Draw("COLZ");
    

    // c1->Write();
    // c2->Write();
    c3->Write();
    c4->Write();
    c5->Write();

    h1->Write();
    h2->Write();
    h3->Write();
    h4->Write();
    h1s->Write();
    h2s->Write();
    h1s_opposite->Write();
    h2s_opposite->Write();
    hadded12->Write();
    hadded21->Write();

    h_y_cats_cons->Write();
    h_x_cats_cons->Write();

    Trajectory_XZ->Write();
    Trajectory_YZ->Write();

    c6 = new TCanvas("c6", "CATS1 vs CATS2", 800, 600);
    c6->Divide(2,2);
    c6->cd(1);
    h_cats1xcats1y->Draw("COLZ");
    c6->cd(2);
    h_cats2xcats2y->Draw("COLZ");
    c6->cd(3);
    h_cats1xcats2x->Draw("COLZ");
    c6->cd(4);

    h_cats1ycats2y->Draw("COLZ");

    // h_cats1cats2->Write();
    c6->Update();


    c7 = new TCanvas("c7","Target Reconstructed", 800,600);
    c7->Divide(2,2);
    c7->cd(1);
    h_target->Draw("colz");
    h_target->Rebin2D(3,3);
    target_mask();
    c7->cd(2);
    // target_mask();
    c7->cd(3);
    h_DC1->Draw("colz");
    c7->cd(4);
    h_DC2->Draw("colz");

    
    

    outputTree->Write();


}
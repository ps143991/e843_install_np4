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
#include "TText.h"
#include "TVector.h"
#include "TVector3.h"
#include "TCutG.h"


TChain* tree = NULL;
TChain* tree_mask1 = NULL;
TChain* tree_mask2 = NULL;
TCanvas* c1 = NULL;
TCanvas* c2 = NULL;
TCanvas* c3 = NULL;
TCanvas* c4 = NULL;
TCanvas* c5 = NULL;
TCanvas* c6 = NULL;
TCanvas* c7 = NULL;
TCutG *cut_hole1;
TCutG *cut_hole2;
TCutG *cut_hole3;
TCutG *cut_hole4;

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

  //Double_t holes_Target_X[11] = {-2.4, -3.6, -2.4, -1.5, -7.37, 2.57, -12.34, 7.54, -2.947, -2.071, -0.62586};
  //changing the values to positive in x
  Double_t holes_Target_X[11] = {2.4, 3.6, 2.4, 1.5, 7.37, -2.57, 12.34, -7.54, 2.947, 2.071, 0.62586};
  
  Double_t holes_Target_Y[11] = {-2.1, 8.8, -2.1, -10.1, -2.6471, -1.5528, -3.194, -1.006, 2.87, -5.082, 0.102821};

  enum { OUTER=0, TOP_LEFT, CENTER, BOTTOM_RIGHT, LEFT_MIDDLE, RIGHT_MIDDLE, FAR_LEFT, FAR_RIGHT, TOP_MIDDLE, BOTTOM_MIDDLE, IDENTIFIER_HOLE };

void target_mask(){
//   TCanvas *c = new TCanvas("c","Grouped circles", 700, 700);

    gPad->cd();
//   c->DrawFrame(-20, -20, 20, 20, "Grouped circles;X;Y");

  // Build a group: 1 big circle with 3 inner circles
  CircleGroup G;

  G.cs.push_back({holes_Target_X[0], holes_Target_Y[0], 13});     // outer
  G.cs.push_back({holes_Target_X[1], holes_Target_Y[1], 0.2});    // top left
  G.cs.push_back({holes_Target_X[2], holes_Target_Y[2], 0.2});    // central
  G.cs.push_back({holes_Target_X[3], holes_Target_Y[3], 0.2});    // bottop right
  G.cs.push_back({holes_Target_X[4], holes_Target_Y[4], 0.2});    // left middle
  G.cs.push_back({holes_Target_X[5], holes_Target_Y[5], 0.2});    // right middle
  G.cs.push_back({holes_Target_X[6], holes_Target_Y[6], 0.2});    // far left
  G.cs.push_back({holes_Target_X[7], holes_Target_Y[7], 0.2});    // far right
  G.cs.push_back({holes_Target_X[8], holes_Target_Y[8], 0.2});    // top middle
  G.cs.push_back({holes_Target_X[9], holes_Target_Y[9], 0.2});    // bottom middle
  G.cs.push_back({holes_Target_X[10], holes_Target_Y[10], 0.2});  // identifier hole
//   G.cs.push_back({ 0, 0, 7});
//   G.cs.push_back({-3, 2, 2});
//   G.cs.push_back({ 4,-1, 1.2});

  // Draw original
  G.Draw(kRed, 5);

//   c->Update();
}

void loadMask1file(){
  tree_mask1 = new TChain("PhysicsTree");
  // tree_mask1->Add("../../data/analysed/504.root");
  // tree_mask1->Add("../../data/analysed/504_further_shift1.root");
  // tree_mask1->Add("../../data/analysed/504_further_shift_2.root");
  tree_mask1->Add("../../data/analysed/504_cats_mask.root");


}
void loadMask2file(){
  tree_mask2 = new TChain("PhysicsTree");
  // tree_mask2->Add("../../data/analysed/505.root");
  // tree_mask2->Add("../../data/analysed/505_further_shift1.root");
  tree_mask2->Add("../../data/analysed/505_cats_mask.root");

}


void loadMask(){
    // TFile *maskfile = new TFile("./target_mask.cpp","READ");
    // CatsMask* DC_mask = (CatsMask*)maskfile->Get("DC_mask");
    // nptool::DetectorManager::getInstance()->GetDetector<CatsPhysics>("Cats")->SetMask(DC_mask);
}






void build_CATS_masks_2(){
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadMask();
    loadMask1file();
    loadMask2file();
    cout<<"Total Entries in mask1 RUN: "<<tree_mask1->GetEntries()<<endl;
    cout<<"Total Entries in mask2 RUN: "<<tree_mask2->GetEntries()<<endl;



    TTreeReader reader_mask1(tree_mask1);
    TTreeReaderValue<CatsPhysics> phy_cats_r_mask1(reader_mask1, "cats");
    TTreeReaderValue<ZddPhysics> phy_zdd_r_mask1(reader_mask1, "zdd");
    TTreeReaderValue<unsigned int> GATCONF_r_mask1(reader_mask1, "GATCONF");
    TTreeReaderValue<unsigned long long> GATCONFTS_r_mask1(reader_mask1, "GATCONFTS");

    TTreeReader reader_mask2(tree_mask2);
    TTreeReaderValue<CatsPhysics> phy_cats_r_mask2(reader_mask2, "cats");
    TTreeReaderValue<ZddPhysics> phy_zdd_r_mask2(reader_mask2, "zdd");
    TTreeReaderValue<unsigned int> GATCONF_r_mask2(reader_mask2, "GATCONF");
    TTreeReaderValue<unsigned long long> GATCONFTS_r_mask2(reader_mask2, "GATCONFTS");


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


    TH2F *hcats_recons_PS  = new TH2F("hcats_recons_PS", "Beam Spot on Target",          2400, -40, 40, 2400, -40, 40);
    TH2F *h_cats1xcats2x = new TH2F("h_cats1xcats2x", "CATS1 X vs CATS2 X", 2400, -40, 40, 2400, -40, 40);
    TH2F *h_cats1ycats2y = new TH2F("h_cats1ycats2y", "CATS1 Y vs CATS2 Y", 2400, -40, 40, 2400, -40, 40);

    TH2F *h_target = new TH2F("h_target","target reconstruction with cats", 1200, -20, 20, 1200, -20, 20);
    TH2F *h_DC1 = new TH2F("h_DC1","DC1 reconstruction with cats", 2400, -40, 40, 2400, -40, 40);
    TH2F *h_DC2 = new TH2F("h_DC2","DC2 reconstruction with cats", 2400, -40, 40, 2400, -40, 40);
    TH2F* h_Mask_1 = new TH2F("h_Mask_1", "Mask 1 Position", 1800, -30, 30, 1800, -30, 30);
    TH2F* h_Mask_2 = new TH2F("h_Mask_2", "Mask 2 Position", 1800, -30, 30, 1800, -30, 30);

    TH2F* Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 381, -1700, 2100, 1000, -499, 500 );
    TH2F* Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 381, -1700, 2100, 1000, -499, 500 );

    TH1F *h_theta = new TH1F("h_theta", "Beam angle w.r.t z axis", 200, -5, 5);

    TH2F *h_hole1 = new TH2F("h_hole1", "CATS Target Hole 1", 2400, -40, 40, 2400, -40, 40);
    TH2F *h_hole2 = new TH2F("h_hole2", "CATS Target Hole 2", 2400, -40, 40, 2400, -40, 40);
    TH2F *h_hole3 = new TH2F("h_hole3", "CATS Target Hole 3", 2400, -40, 40, 2400, -40, 40);
    TH2F *h_hole4 = new TH2F("h_hole4", "CATS Target Hole 4", 2400, -40, 40, 2400, -40, 40);

    Double_t c1x = 0.0;
    Double_t c1y = 0.0;
    Double_t c1z = 0.0;
    Double_t c2x = 0.0;
    Double_t c2y = 0.0;
    Double_t c2z = 0.0;
    // Double_t c1x =  0.2085;
    // Double_t c1y =  0.1257;
    // Double_t c1z = 0.0;
    // Double_t c2x =  -0.7906;
    // Double_t c2y =  -0.4743;
    // Double_t c2z = 0.0;



    Double_t Position[3] = {-1587.1+c1z, -1090.1+c2z, 1260.9}; //CATS1 z pos, CATS2 z pos, DC z pos
    Double_t Position_ZDD[4] = {1260.9+10, 1260.9+30, 1560.9, 2060.9}; // Positions of DC1 (1 cm more), DC2 (3 cm more ), IC1, and Plastics
    Double_t Position_Mask[2] = {-1732.1,-1235.1}; // CATS1 X position, CATS2 X position
    Double_t pos_mask_1_X; 
    Double_t pos_mask_2_X; 
    Double_t pos_mask_1_Y; 
    Double_t pos_mask_2_Y; 
    Double_t Target_Z = 0;
    Double_t x_cats_consDC1 = 0;
    Double_t y_cats_consDC1 = 0;
    Double_t target_cons_X = 0;
    Double_t target_cons_y = 0;
    Double_t x_cats_consDC2 = 0;
    Double_t y_cats_consDC2 = 0;
    Double_t theta = -50;

  auto rot = [&](double x, double y, double cx, double cy, double th){
  double X = x - cx, Y = y - cy;
  double xr =  X*std::cos(th) - Y*std::sin(th);
  double yr =  X*std::sin(th) + Y*std::cos(th);
  return std::pair<double,double>(xr+cx, yr+cy);
  };


    // Long64_t tot_entries = tree->GetEntries();
    // Long64_t entry_count = 0;
    // Long64_t max_entries = 5000000; // set your desired limit
    // if(max_entries >= tot_entries) {max_entries = tot_entries;}

    while(reader_mask1.Next()){
      
        if(reader_mask1.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader_mask1.GetCurrentEntry()) / tree_mask1->GetEntries()) * 100.0 << "%" << endl;
        }
        // if (entry_count > max_entries) break;

        pos_mask_1_X = -1000.0;
        pos_mask_2_X = -1000.0;
        pos_mask_1_Y = -1000.0;
        pos_mask_2_Y = -1000.0;


        if (*GATCONF_r_mask1 == 16) {
            if(phy_cats_r_mask1->PositionOnTargetY > -200 && phy_cats_r_mask1->PositionOnTargetX > -200){
              
              if(phy_zdd_r_mask1->PL_E.size()>0){
                Double_t targetX_byCATS = phy_cats_r_mask1->PositionOnTargetX;
                Double_t targetY_byCATS = phy_cats_r_mask1->PositionOnTargetY;

                Double_t CATS1_X =  c1x + (phy_cats_r_mask1->PositionX[0]);     // shifts
                Double_t CATS1_Y =  c1y + (phy_cats_r_mask1->PositionY[0]);
                Double_t CATS1_Z =  c1z + (phy_cats_r_mask1->PositionZ[0]);
                Double_t CATS2_X =  c2x + (phy_cats_r_mask1->PositionX[1]);
                Double_t CATS2_Y =  c2y + (phy_cats_r_mask1->PositionY[1]);
                Double_t CATS2_Z =  c2z + (phy_cats_r_mask1->PositionZ[1]);


                Double_t ay = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
                Double_t ax = (Position[1] - Position[0])/(CATS2_X - CATS1_X);

                y_cats_consDC1 = CATS1_Y + ( Position_ZDD[0] - Position[0])/ay;      //dc1 const y
                x_cats_consDC1 = CATS1_X + ( Position_ZDD[0] - Position[0])/ax;      //dc1 const x

                y_cats_consDC2 = CATS1_Y + ( Position_ZDD[1] - Position[0])/ay;      //dc2 const y
                x_cats_consDC2 = CATS1_X + ( Position_ZDD[1] - Position[0])/ax;      //dc2 const x
                target_cons_X = CATS1_X + ( Target_Z - Position[0])/ax;           //target const x
                target_cons_y = CATS1_Y + ( Target_Z - Position[0])/ay;           //target const y

                ///////////////////////////////////////////EXTRAPOLATING MASKS//////////////////////////////////////////////////////////

                pos_mask_1_X = CATS1_X + ( Position_Mask[0] - Position[0])/ax;
                pos_mask_1_Y = CATS1_Y + ( Position_Mask[0] - Position[0])/ay;


                h_Mask_1 -> Fill(pos_mask_1_X,pos_mask_1_Y);

///////////////////////////////////////////EXTRAPOLATING MASKS//////////////////////////////////////////////////////////

              }
            }
          }
        }

    cout<<"&&&&&&&&&&&&&&&&&&&&& Finished Mask 1 RUN  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& "<<endl;

    while (reader_mask2.Next()){
      
        if(reader_mask2.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader_mask2.GetCurrentEntry()) / tree_mask2->GetEntries()) * 100.0 << "%" << endl;
        }
        // if (entry_count > max_entries) break;

        pos_mask_1_X = -1000.0;
        pos_mask_2_X = -1000.0;
        pos_mask_1_Y = -1000.0;
        pos_mask_2_Y = -1000.0;
        if (*GATCONF_r_mask2 == 32) {
            if(phy_cats_r_mask2->PositionOnTargetY > -200 && phy_cats_r_mask2->PositionOnTargetX > -200){
              
              if(phy_zdd_r_mask2->PL_E.size()>0){
                Double_t targetX_byCATS = phy_cats_r_mask2->PositionOnTargetX;
                Double_t targetY_byCATS = phy_cats_r_mask2->PositionOnTargetY;

                Double_t CATS1_X =  c1x + (phy_cats_r_mask2->PositionX[0]);     // shifts
                Double_t CATS1_Y =  c1y + (phy_cats_r_mask2->PositionY[0]);
                Double_t CATS1_Z =  c1z + (phy_cats_r_mask2->PositionZ[0]);
                Double_t CATS2_X =  c2x + (phy_cats_r_mask2->PositionX[1]);
                Double_t CATS2_Y =  c2y + (phy_cats_r_mask2->PositionY[1]);
                Double_t CATS2_Z =  c2z + (phy_cats_r_mask2->PositionZ[1]);

                Double_t ay = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
                Double_t ax = (Position[1] - Position[0])/(CATS2_X - CATS1_X);

                y_cats_consDC1 = CATS1_Y + ( Position_ZDD[0] - Position[0])/ay;      //dc1 const y
                x_cats_consDC1 = CATS1_X + ( Position_ZDD[0] - Position[0])/ax;      //dc1 const x

                y_cats_consDC2 = CATS1_Y + ( Position_ZDD[1] - Position[0])/ay;      //dc2 const y
                x_cats_consDC2 = CATS1_X + ( Position_ZDD[1] - Position[0])/ax;      //dc2 const x
                target_cons_X = CATS1_X + ( Target_Z - Position[0])/ax;           //target const x
                target_cons_y = CATS1_Y + ( Target_Z - Position[0])/ay;           //target const y

                ///////////////////////////////////////////EXTRAPOLATING MASKS////////////////////////////////////////////////////////// 

                pos_mask_2_X = CATS1_X + ( Position_Mask[1] - Position[0])/ax;
                pos_mask_2_Y = CATS1_Y + ( Position_Mask[1] - Position[0])/ay;
                h_Mask_2 -> Fill(pos_mask_2_X,pos_mask_2_Y);

                ///////////////////////////////////////////EXTRAPOLATING MASKS//////////////////////////////////////////////////////////
              }
            }
          }
        }
    cout<<"&&&&&&&&&&&&&&&&&&&&& Finished Mask 2 RUN  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& "<<endl;

        



    






    TCanvas *theta_canvas = new TCanvas("theta_canvas", "Beam Angle Distribution", 800, 800);
    h_theta->SetTitle("Beam Angle Distribution w.r.t Z axis; Angle (degrees); Counts");
    h_theta->Draw();

    

    
    TCanvas *cmask = new TCanvas("cmask", "Mask Positions", 1600, 800);
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

    TCanvas *mask_projections = new TCanvas("mask_projections", "Mask Projections", 1600, 800);
    TH2F *h_mask1_center = (TH2F*)h_Mask_1->Clone("h_mask1_center");
    h_mask1_center->GetXaxis()->SetRangeUser(-1.5,1.5);
    h_mask1_center->GetYaxis()->SetRangeUser(-1.5,1.5);
    TH1D* proj_M1_X = h_mask1_center-> ProjectionX();
    TH1D* proj_M1_Y = h_mask1_center-> ProjectionY();
    

    TH2F *h_mask2_center = (TH2F*)h_Mask_2->Clone("h_mask2_center");
    h_mask2_center->GetXaxis()->SetRangeUser(-1.5,1.5);
    h_mask2_center->GetYaxis()->SetRangeUser(-2.5,0);
    TH1D* proj_M2_X = h_mask2_center-> ProjectionX();
    TH1D* proj_M2_Y = h_mask2_center-> ProjectionY();

    mask_projections->Divide(2,2);

    ///////////////////////////////////////////////////////// 1X /////////////////////////////////////////////////////////
    mask_projections->cd(1);
    proj_M1_X->SetTitle("Mask 1 X Projection");
    proj_M1_X->GetXaxis()->SetTitle("X Position (mm)");
    proj_M1_X->GetYaxis()->SetTitle("Counts");
    proj_M1_X->Draw();
    
    TText *text1 = new TText(-1.2, proj_M1_X->GetMaximum()*0.8, "Surveyor: X = 0.0 mm");
    text1->SetTextColor(kGreen+2);
    text1->Draw("same");

    proj_M1_X->Fit("gaus","","",-1.5,1.5);
    TF1 *Xm1 = (TF1*)proj_M1_X->GetFunction("gaus");
    TText *text1b = new TText(-1.2, proj_M1_X->GetMaximum()*0.6, Form("Fitted Mean = %.3f mm", Xm1->GetParameter(1)));
    text1b->SetTextColor(kBlue+2);
    text1b->Draw("same");

    //////////////////////////////////////////////////////// 1Y /////////////////////////////////////////////////////////

    mask_projections->cd(3);
    proj_M1_Y->SetTitle("Mask 1 Y Projection");
    proj_M1_Y->GetXaxis()->SetTitle("Y Position (mm)");
    proj_M1_Y->GetYaxis()->SetTitle("Counts");
    proj_M1_Y->Draw();


    TText *text2 = new TText(-1.2, proj_M1_Y->GetMaximum()*0.8, "Surveyor: Y = +0.2 mm");
    text2->SetTextColor(kGreen+2);
    text2->Draw("same");    
    
    proj_M1_Y->Fit("gaus","","",-1.5,1.5);
    TF1 *Ym1 = (TF1*)proj_M1_Y->GetFunction("gaus");
    TText *text2b = new TText(-1.2, proj_M1_Y->GetMaximum()*0.6, Form("Fitted Mean = %.3f mm", Ym1->GetParameter(1)));
    text2b->SetTextColor(kBlue+2);
    text2b->Draw("same");

    ///////////////////////////////////////////////////////// 2X /////////////////////////////////////////////////////////
    mask_projections->cd(2);
    proj_M2_X->SetTitle("Mask 2 X Projection"); 
    proj_M2_X->GetXaxis()->SetTitle("X Position (mm)");
    proj_M2_X->GetYaxis()->SetTitle("Counts");
    proj_M2_X->Draw();

    TText *text3 = new TText(-1.2, proj_M2_X->GetMaximum()*0.8, "Surveyor: X = -0.1 mm");
    text3->SetTextColor(kGreen+2);
    text3->Draw("same");

    proj_M2_X->Fit("gaus","","",-1.5,1.5);
    TF1 *Xm2 = (TF1*)proj_M2_X->GetFunction("gaus");
    TText *text3b = new TText(-1.2, proj_M2_X->GetMaximum()*0.6, Form("Fitted Mean = %.3f mm", Xm2->GetParameter(1)));
    text3b->SetTextColor(kBlue+2);
    text3b->Draw("same");

    ///////////////////////////////////////////////////////// 2Y /////////////////////////////////////////////////////////
    mask_projections->cd(4);
    proj_M2_Y->SetTitle("Mask 2 Y Projection");
    proj_M2_Y->GetXaxis()->SetTitle("Y Position (mm)");
    proj_M2_Y->GetYaxis()->SetTitle("Counts");
    proj_M2_Y->Draw(); 
    TText *text4 = new TText(-2.2, proj_M2_Y->GetMaximum()*0.8, "Surveyor: Y = -1.1 mm"); 
    text4->SetTextColor(kGreen+2);
    text4->Draw("same");

    proj_M2_Y->Fit("gaus","","",-2.5,0);
    TF1 *Ym2 = (TF1*)proj_M2_Y->GetFunction("gaus");
    TText *text4b = new TText(-2.2, proj_M2_Y->GetMaximum()*0.6, Form("Fitted Mean = %.3f mm", Ym2->GetParameter(1)));
    text4b->SetTextColor(kBlue+2);
    text4b->Draw("same");

    mask_projections->Update(); 

    const double z1 = -1587.1, z2 = -1090.1;
    const double zm1 = -1732.1, zm2 = -1235.1;

    // Blend factors
    const double a1 = (zm1 - z1)/(z2 - z1);  // ≈ -0.2917505030
    const double a2 = (zm2 - z1)/(z2 - z1);  // ≈  0.7082494970
    const double den = (a2 - a1);            // = 1.0 for these numbers

    // Surveyed central-hole coords
    const double S1x = 0.0,  S1y =  0.2;
    const double S2x = -0.1,  S2y = -1.1;

    // Fitted means from your projections (already in your code)
    const double M1x = Xm1->GetParameter(1);
    const double M1y = Ym1->GetParameter(1);
    const double M2x = Xm2->GetParameter(1);
    const double M2y = Ym2->GetParameter(1);

    // Deltas we want the translations to correct
    const double d1x = S1x - M1x;
    const double d1y = S1y - M1y;
    const double d2x = S2x - M2x;
    const double d2y = S2y - M2y;

    // Solve for translations of each CATS (per axis)
    const double c1x_new = (a2*d1x - a1*d2x) / den;
    const double c2x_new = ((1.0 - a1)*d2x - (1.0 - a2)*d1x) / den;
    const double c1y_new = (a2*d1y - a1*d2y) / den;
    const double c2y_new = ((1.0 - a1)*d2y - (1.0 - a2)*d1y) / den;

    std::cout << std::fixed << std::setprecision(4)
              << "CATS translations (mm): "
              << " c1x=" << c1x_new << " c1y=" << c1y_new
              << " c2x=" << c2x_new << " c2y=" << c2y_new << std::endl;




// // deltas = surveyed - measured centroids (mm)
// auto solX = solve_axis_balanced(d1x, d2x, diff_avg_hole_X);
// cout<<"Solution X: c1 = "<<solX.c1<<", c2 = "<<solX.c2<<", t = "<<solX.t<<endl;
// auto solY = solve_axis_balanced(d1y, d2y, diff_avg_hole_Y);
// cout<<"Solution Y: c1 = "<<solY.c1<<", c2 = "<<solY.c2<<", t = "<<solY.t<<endl;

/* // Write these into your analysis:
// Apply CATS geometry translations (kept run-invariant)
CATS1_X_corr = base_CATS1_X + solX.c1;
CATS2_X_corr = base_CATS2_X + solX.c2;
CATS1_Y_corr = base_CATS1_Y + solY.c1;
CATS2_Y_corr = base_CATS2_Y + solY.c2;

// At the target plane (z=0), add the per-run offset:
X_target_final = X_target_reco + solX.t;
Y_target_final = Y_target_reco + solY.t;
 */



}
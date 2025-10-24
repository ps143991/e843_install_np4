/*****************************************************************************
---*
 * Comment:                                                                  *
 * Improvements to be made: (1) the exact distance of DC1 and DC2
 * (2) the window of Drift time to be taken
 * (3) does the beam spend enough time in the target for physics runs to alter
 *  the time difference giving us fake drift time?
 * (4) The data of the drift time could be randomized before inverting and joining
 * 
 * 
 * Find the resolution of the drift chambers...
 *                                               
 * I leave the v5 of this script with a working spot along with randomization of 5ns                                    *
 *****************************************************************************/



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
#include <TProfile.h>
#include <TCutG.h>
#include <TGraph.h>

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
#include "TRandom3.h"


TRandom3 rng(12345);
inline double dither10ns_tri(TRandom3& r){                        //a trianugular dithering function to dither within +/- 5 ns
  return r.Uniform(-5.0, 5.0) - r.Uniform(-5.0, 5.0);
}

// model: DC(t) = C0*(1 + erf((t - mu)/(sigma*sqrt(2)))) + a0 + a1*t
double ErfLine(double *x, double *p){
  const double t = x[0];
  const double C0 = p[0];
  const double mu = p[1];
  const double sigma = fabs(p[2]); // keep it positive
  const double a0 = p[3];
  const double a1 = p[4];
  return C0*(1.0 + TMath::Erf((t - mu)/(sigma*TMath::Sqrt2()))) + a0 + a1*t;
}

// quick helper to get an average over a range of bins
static double mean_in_range(TH1 *h, int b1, int b2){
  double sum=0, w=0;
  for(int b=b1;b<=b2;++b){ double y=h->GetBinContent(b); sum+=y; w+=1; }
  return (w>0? sum/w : 0.0);
}

// ---- TH1 version ----
void fit_erf_line(TH1 *h, double xmin=0, double xmax=0){
  if(!h){ printf("No histogram!\n"); return; }
  if(xmin==0 && xmax==0){ xmin=h->GetXaxis()->GetXmin(); xmax=h->GetXaxis()->GetXmax(); }

  // initial guesses
  int nbin = h->GetNbinsX();
  int nedge = std::max(1, nbin/10);
  double y_lo = mean_in_range(h, 1, nedge);
  double y_hi = mean_in_range(h, nbin-nedge+1, nbin);

  double C0_guess = 0.5*(y_hi - y_lo);
  double a1_guess = 0.0; // baseline slope (adjust if you know there's tilt)
  double a0_guess = y_lo - a1_guess * h->GetXaxis()->GetBinCenter(nedge/2>0?nedge/2:1);

  // crude mu,sigma: middle of range and a small fraction of range
  double mu_guess = 0.5*(xmin+xmax);
  double sigma_guess = 0.05*(xmax-xmin);

  // slightly smarter mu: find first bin that exceeds y_lo + C0
  double half = y_lo + C0_guess;
  for(int b=1;b<=nbin;++b){
    if(h->GetBinContent(b) > half){ mu_guess = h->GetBinCenter(b); break; }
  }

  TF1 *f = new TF1("f_erf_line", ErfLine, xmin, xmax, 5);
  f->SetParNames("C0","mu","sigma","a0","a1");
  // f->SetParameters(C0_guess, mu_guess, sigma_guess, a0_guess, a1_guess);
  f->SetParameters(3279,-154,1.72,349,1.98);
  f->SetParLimits(2, (xmax-xmin)/1e4, (xmax-xmin)); // sigma in a sensible range

  h->Fit(f, "R"); // fit in range

  // // draw
  // TCanvas *c = new TCanvas("c_erf_line","erf+line fit",900,600);
  // h->Draw();
  // f->SetLineWidth(3);
  // f->Draw("same");
}

// ---- TGraph version ----
void fit_erf_line_graph(TGraph *g, double xmin=0, double xmax=0){
  if(!g){ printf("No graph!\n"); return; }
  if(xmin==0 && xmax==0){
    double x,y; g->GetPoint(0,x,y);
    xmin = x; g->GetPoint(g->GetN()-1,x,y); xmax = x;
    if(xmin>xmax) std::swap(xmin,xmax);
  }

  // simple guesses from endpoints
  double xl,yl, xr,yr;
  g->GetPoint(0,xl,yl); g->GetPoint(g->GetN()-1,xr,yr);
  double y_lo = yl, y_hi = yr;
  if(y_hi<y_lo){ std::swap(y_lo,y_hi); }

  double C0_guess = 0.5*(y_hi - y_lo);
  double a1_guess = 0.0;
  double a0_guess = y_lo - a1_guess*xl;
  double mu_guess = 0.5*(xmin+xmax);
  double sigma_guess = 0.05*(xmax-xmin);

  TF1 *f = new TF1("f_erf_line_g", ErfLine, xmin, xmax, 5);
  f->SetParNames("C0","mu","sigma","a0","a1");
  f->SetParameters(C0_guess, mu_guess, sigma_guess, a0_guess, a1_guess);
  f->SetParLimits(2, (xmax-xmin)/1e4, (xmax-xmin));

  g->Fit(f, "R");

  TCanvas *c = new TCanvas("c_erf_line_g","erf+line fit (graph)",900,600);
  g->SetMarkerStyle(20);
  g->Draw("AP");
  f->SetLineWidth(3);
  f->Draw("same");
}




TChain* tree = NULL;
TCanvas* c1 = NULL;
TCanvas* c2 = NULL;
TCanvas* c3 = NULL;
TCanvas* c4 = NULL;
TCanvas* c5 = NULL;
TCanvas* c6 = NULL;
TCanvas* c7 = NULL;


TCutG *cut_left;
TCutG *cut_right;
TCutG *cut_up;
TCutG *cut_down;
TCutG *cut_cats1_right;
TCutG *cut_cats1_left;
TCutG *cut_cats1_top;
TCutG *cut_cats1_down;
TCutG* cut_dc1;
TCutG* cut_dc2;
TCutG* cut_dc3;
TCutG* cut_dc4;
TCutG* cut_dc5;


void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/558.root");
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

  TFile *dc1_cut = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/dc1.root","READ");
  TFile *dc2_cut = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/dc2.root","READ");
  TFile *dc3_cut = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/dc3.root","READ");
  TFile *dc4_cut = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/dc4.root","READ");

  TFile *dc5_cut = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/cuts/cut_rect.root","READ");

  // Load your cuts here if needed
  cut_left = (TCutG*)cut_L->FindObjectAny("cut_theau");
  cut_right = (TCutG*)cut_R->FindObjectAny("cut_theau2");
  cut_up = (TCutG*)cut_U->FindObjectAny("cut_up");
  cut_down = (TCutG*)cut_D->FindObjectAny("cut_down");
  cut_cats1_right = (TCutG*)cats1_right->FindObjectAny("cut_cats1_right");
  cut_cats1_left = (TCutG*)cats1_left->FindObjectAny("CUTG");
  cut_cats1_top = (TCutG*)cats1_top->FindObjectAny("cut_cats1_top");
  cut_cats1_down = (TCutG*)cats1_down->FindObjectAny("cut_cats1_down");

  cut_dc1 = (TCutG*)dc1_cut->FindObjectAny("CUTG");
  cut_dc2 = (TCutG*)dc2_cut->FindObjectAny("CUTG");
  cut_dc3 = (TCutG*)dc3_cut->FindObjectAny("CUTG");
  cut_dc4 = (TCutG*)dc4_cut->FindObjectAny("CUTG");
  cut_dc5 = (TCutG*)dc5_cut->FindObjectAny("CUTG");
}



void build_DC_5(){
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    loadCUTS();
    cout<<"Total Entries in tree1: "<<tree->GetEntries()<<endl;

    TTreeReader reader(tree);
    TTreeReaderValue<ZddPhysics> phy_zdd_r(reader, "zdd");

    TTreeReaderValue<unsigned int> GATCONF_r(reader, "GATCONF");
    TTreeReaderValue<unsigned long long> GATCONFTS_r(reader, "GATCONFTS");
    TTreeReaderArray<unsigned long long> DC_TS1_r(reader,"DC_TS1");
    TTreeReaderArray<unsigned long long> DC_TS2_r(reader,"DC_TS2");
    TTreeReaderArray<unsigned long long> DC_TS3_r(reader,"DC_TS3");
    TTreeReaderArray<unsigned long long> DC_TS4_r(reader,"DC_TS4");
    TTreeReaderArray<unsigned int> DC_E1_r(reader,"DC_1");
    TTreeReaderArray<unsigned int> DC_E2_r(reader,"DC_2");
    TTreeReaderArray<unsigned int> DC_E3_r(reader,"DC_3");
    TTreeReaderArray<unsigned int> DC_E4_r(reader,"DC_4");

    TTreeReaderValue<CatsPhysics> phy_cats_r(reader, "cats");


    // TFile* outputFile = new TFile("./../../data/analysed/Build_DC_output_cats_shifted_4_with_C2X_m_1.root", "RECREATE");
    TFile* outputFile = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/macro_output/test.root", "RECREATE");

    if (!outputFile || outputFile->IsZombie()) {
        outputFile->Close();
        return;
    }
    TTree* outputTree = new TTree("t", "Tree");

    Double_t TS_sub1, TS_sub2, TS_sub3, TS_sub4;

    Double_t TS_sub1_adj, TS_sub2_adj, TS_sub3_adj, TS_sub4_adj;
    Double_t TS_sub1_adj_d, TS_sub2_adj_d, TS_sub3_adj_d, TS_sub4_adj_d;

    bool dc1_flag, dc2_flag, dc3_flag, dc4_flag;
    
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
    TH1 *h3s_opposite = new TH1D("h3s_opposite", "DC 3 Time Subtraction cut opposite", 400, -200, 200);
    TH1 *h4s_opposite = new TH1D("h4s_opposite", "DC 4 Time Subtraction cut opposite", 400, -200, 200);

    TH1 *hdc1 = new TH1D("hdc1", "DC 1 Energy", 400, -200, 200);
    TH1 *hdc2 = new TH1D("hdc2", "DC 2 Energy", 400, -200, 200);
    TH1 *hdc3 = new TH1D("hdc3", "DC 3 Energy", 400, -200, 200);
    TH1 *hdc4 = new TH1D("hdc4", "DC 4 Energy", 400, -200, 200);
    
    TH1 *hadded12 = new TH1D("hadded12", "DC 1 and DC 2 Time Subtraction added", 400, -200, 200);
    TH1 *hadded34 = new TH1D("hadded34", "DC 3 and DC 4 Time Subtraction added", 400, -200, 200);

    TH2F *h_y_cats_cons = new TH2F("h_y_cats_cons", "CATS Y vs TS", 4000,-200,200,400, -200, 200);
    TH2F *h_x_cats_cons = new TH2F("h_x_cats_cons", "CATS X vs TS", 4000,-200,200,400, -200, 200);

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

    TH2F *h_only_dc2 = new TH2F("h_only_dc2","DC2 reconstruction with cats only dc2", 400,-200,200,400, -200, 200);


    TH2F* Trajectory_XZ_left_cut = new TH2F("Trajectory_XZ_left_cut", "Reconstruction of beam trajectory in detectors left cut (X projection)", 381, -1700, 2100, 1000, -249, 250 );
    TH2F* Trajectory_YZ_up_cut = new TH2F("Trajectory_YZ_left_cut", "Reconstruction of beam trajectory in detectors left cut (Y projection)", 381, -1700, 2100, 1000, -249, 250 );
    TH2F* Trajectory_XZ_right_cut = new TH2F("Trajectory_XZ_right_cut", "Reconstruction of beam trajectory in detectors right cut (X projection)", 381, -1700, 2100, 1000, -249, 250 );
    TH2F* Trajectory_YZ_down_cut = new TH2F("Trajectory_YZ_right_cut", "Reconstruction of beam trajectory in detectors right cut (Y projection)", 381, -1700, 2100, 1000, -249, 250 );

    TH2F* plot_cons_vs_drift = new TH2F("plot_cons_vs_drift", "CATS Y at DC2 vs TS DC2", 800,-600,200,400, -200, 200);
    TH1F* hEdc1 = new TH1F("hEdc1", "DC1 Energy", 400, -4000, 4000);
    TH1F* hEdc2 = new TH1F("hEdc2", "DC2 Energy", 400, -4000, 4000);
    TH1F* hEdc3 = new TH1F("hEdc3", "DC3 Energy", 400, -4000, 4000);
    TH1F* hEdc4 = new TH1F("hEdc4", "DC4 Energy", 400, -4000, 4000);
    TH2F* hNEw3 = new TH2F("hNEw3", "TS DC3 sub vs CATS1 X at DC3", 200,-200,0, 2000, -200, 200);
    TH2F* hNEw4 = new TH2F("hNEw4", "TS DC4 sub vs CATS1 X at DC4", 200,-200,0, 2000, -200, 200);
    TH2F* hNEw1 = new TH2F("hNEw1", "TS DC1 sub vs CATS1 Y at DC1", 200,-200,0, 2000, -200, 200);
    TH2F* hNEw2 = new TH2F("hNEw2", "TS DC2 sub vs CATS1 Y at DC2", 200,-200,0, 2000, -200, 200);

    TH2F* hNEw3_gated = new TH2F("hNEw3_gated", "gated TS DC3 sub vs CATS1 X at DC3", 200,-200,0, 2000, -200, 200);
    TH2F* hNEw4_gated = new TH2F("hNEw4_gated", "gated TS DC4 sub vs CATS1 X at DC4", 200,-200,0, 2000, -200, 200);
    TH2F* hNEw1_gated = new TH2F("hNEw1_gated", "gated TS DC1 sub vs CATS1 Y at DC1", 200,-200,0, 2000, -200, 200);
    TH2F* hNEw2_gated = new TH2F("hNEw2_gated", "gated TS DC2 sub vs CATS1 Y at DC2", 200,-200,0, 2000, -200, 200);

    TH2F* dc_spot = new TH2F("dc_spot", "DC spot", 800,-40,40,800,-40,40);
    TH2F* h_DC1_hit = new TH2F("h_DC1_hit","DC1 hit positions", 800,-40,40,800,-40,40);
    TH2F* h_yplus = new TH2F("hplus_y", "DC2 TS plus CATS1 Y at DC1", 400,-200,200,2000,-200,200);
    TH2F* h_yminus = new TH2F("hminus_y", "DC1 TS plus CATS1 Y at DC1", 400,-200,200,2000,-200,200);
    TH2F* h_xplus = new TH2F("hplus_x", "DC2 TS plus CATS1 X at DC1", 400,-200,200,2000,-200,200);
    TH2F* h_xminus = new TH2F("hminus_x", "DC1 TS plus CATS1 X at DC1", 400,-200,200,2000,-200,200);
    TH2F* h_horizontal = new TH2F("hhorizontal", "CATS1 X at DC1 vs CATS1 Y at DC1", 400,-200,200,2000,-200,200);
    TH2F* h_vertical = new TH2F("hvertical", "CATS1 Y at DC1 vs CATS1 X at DC1", 400,-200,200,2000,-200,200);


    
    Double_t Position[3] = {-1587.1, -1090.1, 1260.9}; //CATS1 z pos, CATS2 z pos, DC z pos
    Double_t Position_ZDD[4] = {1260.9+10, 1260.9+30, 1560.9, 2060.9}; // Positions of DC1 (1 cm more), DC2 (3 cm more ), IC1, and Plastics

    Double_t DC_quad_coeff[4][2]={{-0.392921,-58.8316}, {0.419384,68.0564}, {0.39777,65.3414}, {-0.452811,-67.5281}}; //{slope, intercept} for DC1, DC2, DC3, DC4
    Double_t DC_plane_coeff[2][2]={{0.44121, 1.76974}, {0.471454,1.69337}}; //{slope, intercept} for DC12, DC34
    Double_t DC_drif_cutoffs[4]={-154.077,-157.186,-157.019,-151.89}; // Drift time cutoffs for DC1, DC2, DC3, DC4
    // Double_t cutoffs[4] = {-154,-157,-157, -152}; // Integer cutoffs for easier handling in cuts for DC1, DC2, DC3, DC4
    Double_t cutoffs[4] = {-152,-155,-155, -150}; // Integer cutoffs for easier handling in cuts for DC1, DC2, DC3, DC4

    Double_t Target_Z = 0;
    Double_t x_cats_consDC1 = 0;
    Double_t y_cats_consDC1 = 0;
    Double_t target_cons_X = 0;
    Double_t target_cons_y = 0;
    Double_t x_cats_consDC2 = 0;
    Double_t y_cats_consDC2 = 0;
    unsigned int DC_E1 = 0;
    unsigned int DC_E2 = 0;
    unsigned int DC_E3 = 0;
    unsigned int DC_E4 = 0;


    // Long64_t tot_entries = tree->GetEntries();
    // Long64_t entry_count = 0;
    // Long64_t max_entries = 5000000; // set your desired limit
    // if(max_entries >= tot_entries) {max_entries = tot_entries;}
    long long event_counter = 0;

    while(reader.Next() /* && event_counter < 3000000 */) {
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }
        TS_sub1 = -1000; TS_sub1_adj = -1000;  dc1_flag = false;
        TS_sub2 = -1000; TS_sub2_adj = -1000;  dc2_flag = false;
        TS_sub3 = -1000; TS_sub3_adj = -1000;  dc3_flag = false;
        TS_sub4 = -1000; TS_sub4_adj = -1000;  dc4_flag = false;
        // if (entry_count > max_entries) break;

        if (/* *GATCONF_r == 1 || *GATCONF_r == 2 ||*GATCONF_r == 16 || */*GATCONF_r == 32) {
            if(phy_cats_r->PositionOnTargetY > -200 && phy_cats_r->PositionOnTargetX > -200){
              
              if(phy_zdd_r->PL_E.size()>0){

                // if(DC_E1_r.GetSize()==1){
                //     DC_E1 = DC_E1_r[0];
                // }


                Double_t targetX_byCATS = phy_cats_r->PositionOnTargetX;
                Double_t targetY_byCATS = phy_cats_r->PositionOnTargetY;

                Double_t CATS1_X =  phy_cats_r->PositionX[0];
                Double_t CATS1_Y =  phy_cats_r->PositionY[0];
                Double_t CATS1_Z =  phy_cats_r->PositionZ[0];
                Double_t CATS2_X =  (phy_cats_r->PositionX[1])     - 1; // Shift CATS2 X position by 1 cm
                Double_t CATS2_Y =  phy_cats_r->PositionY[1]; // Shift CATS2 Y position by -0.3 cm
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
                    TS_sub1_adj = TS_sub1 - cutoffs[0];
                    TS_sub1_adj_d = TS_sub1_adj + dither10ns_tri(rng);
                }
                if(DC_TS2_r.GetSize()==1){
                    unsigned long long dc_2_ts  = (unsigned long long)(DC_TS2_r[0]);
                    TS_sub2 =  static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_2_ts);
                    h2->Fill(TS_sub2);
                    TS_sub2_adj = TS_sub2 - cutoffs[1];
                    TS_sub2_adj_d = TS_sub2_adj + dither10ns_tri(rng);
                }
                if(DC_TS3_r.GetSize()==1){
                    unsigned long long dc_3_ts  = (unsigned long long)(DC_TS3_r[0]);
                    TS_sub3 = static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_3_ts);
                    h3->Fill(TS_sub3);
                    TS_sub3_adj = TS_sub3 - cutoffs[2];
                    TS_sub3_adj_d = TS_sub3_adj + dither10ns_tri(rng);
                }
                if(DC_TS4_r.GetSize()==1){
                    unsigned long long dc_4_ts  = (unsigned long long)(DC_TS4_r[0]);
                    TS_sub4 =  static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_4_ts);
                    h4->Fill(TS_sub4);
                    TS_sub4_adj = TS_sub4 - cutoffs[3];
                    TS_sub4_adj_d = TS_sub4_adj + dither10ns_tri(rng);
                }
                /* if(phy_zdd_r->DC_TS.size()>2){
                 
                  cout<<"*************************  "<<"DC_E size: "<<phy_zdd_r->DC_E.size()<<"*************************"<<endl;
                  for(int ll = 0; ll<phy_zdd_r->DC_TS.size(); ll++){
                      cout<<"for DC_N: "<<phy_zdd_r->DC_Nbr[ll]<< " TimeStamps are: "<<phy_zdd_r->DC_TS[ll]<<endl;
                  }
                } */


                if(TS_sub1_adj>=(-10) || TS_sub2_adj>=(-10)){   //excluding noise
                    h1s_opposite->Fill(-TS_sub1_adj);                           //adjusted time to center at 0
                    h2s_opposite->Fill(TS_sub2_adj);                            //adjusted time to center at 0
                    hadded12->Add(h1s_opposite, h2s_opposite, 1.0, 1.0);               //added time spectra
                    
                    h_yplus->Fill( TS_sub2_adj,CATS1_Y + ( Position_ZDD[1] - Position[0])/ay);
                    h_yminus->Fill(-TS_sub1_adj,CATS1_Y + ( Position_ZDD[1] - Position[0])/ay);
                }
                if(TS_sub3_adj>=(-10) || TS_sub4_adj>=(-10)){   //excluding noise
                    h3s_opposite->Fill(TS_sub3_adj);                           //adjusted time to center at 0
                    h4s_opposite->Fill(-TS_sub4_adj);                         //adjusted time to center at 0
                    hadded34->Add(h3s_opposite, h4s_opposite, 1.0, 1.0);               //added time spectra

                    h_xplus->Fill(TS_sub3_adj,CATS1_X + ( Position_ZDD[0] - Position[0])/ax);
                    h_xminus->Fill(-TS_sub4_adj,CATS1_X + ( Position_ZDD[0] - Position[0])/ax);
                }

                    h_cats1xcats1y->Fill(CATS1_X, CATS1_Y);                         //CATS1 X:Y
                    h_cats2xcats2y->Fill(CATS2_X, CATS2_Y);                         //CATS2 X:Y

                    h_cats1xcats2x->Fill(CATS1_X,CATS2_X);
                    h_cats1ycats2y->Fill(CATS1_Y,CATS2_Y);
                    
                    h_target->Fill(target_cons_X,target_cons_y);
                    h_DC1->Fill(x_cats_consDC1,y_cats_consDC1);
                    h_DC2->Fill(x_cats_consDC2,y_cats_consDC2);

                  if(TS_sub3_adj>=(-10) && TS_sub3_adj<= 90)
                  {
                    hNEw3->Fill(TS_sub3, CATS1_X + ( Position_ZDD[0] - Position[0])/ax);
                    dc3_flag = true;
                  }
                  if(TS_sub4_adj>=(-10) && TS_sub4_adj<= 90)
                  {
                    hNEw4->Fill(TS_sub4, CATS1_X + ( Position_ZDD[0] - Position[0])/ax);
                    dc4_flag = true;
                  }
                  if(TS_sub1_adj>=(-10) && TS_sub1_adj<= 90)
                  {
                    hNEw1->Fill(TS_sub1, CATS1_Y + ( Position_ZDD[1] - Position[0])/ay);
                    dc1_flag = true;
                  }
                  if(TS_sub2_adj>=(-10) && TS_sub2_adj<= 90)
                  {
                    hNEw2->Fill(TS_sub2, CATS1_Y + ( Position_ZDD[1] - Position[0])/ay);
                    dc2_flag = true;
                  }
                  // if(cut_dc5->IsInside(x_cats_consDC1, y_cats_consDC1)){
                  // the 4 quadrants
                  if(dc3_flag && dc2_flag){dc_spot->Fill(  TS_sub3_adj_d*DC_plane_coeff[1][0] + DC_plane_coeff[1][1],    TS_sub2_adj_d*DC_plane_coeff[0][0] + DC_plane_coeff[0][1]);
                  // cout<<"(x,y) is: ("<<TS_sub3_adj_d*DC_plane_coeff[1][0] + DC_plane_coeff[1][1]<<","<<  TS_sub2_adj_d*DC_plane_coeff[0][0] + DC_plane_coeff[0][1]<<")"<<endl;
                    // h_DC1_hit->Fill(x_cats_consDC1, y_cats_consDC1);
                  }  //quad 1
                  if(dc4_flag && dc2_flag){dc_spot->Fill(-(TS_sub4_adj_d*DC_plane_coeff[1][0] + DC_plane_coeff[1][1]),   TS_sub2_adj_d*DC_plane_coeff[0][0] + DC_plane_coeff[0][1]);
                  // cout<<"(x,y) is: ("<<-(TS_sub4_adj_d*DC_plane_coeff[1][0] + DC_plane_coeff[1][1])<<","<<  TS_sub2_adj_d*DC_plane_coeff[0][0] + DC_plane_coeff[0][1]<<")"<<endl;
                    // h_DC1_hit->Fill(x_cats_consDC1, y_cats_consDC1);
                  }  //quad 2
                  if(dc4_flag && dc1_flag){dc_spot->Fill(-(TS_sub4_adj_d*DC_plane_coeff[1][0] + DC_plane_coeff[1][1]), -(TS_sub1_adj_d*DC_plane_coeff[0][0] + DC_plane_coeff[0][1]));
                  // cout<<"(x,y) is: ("<<-(TS_sub4_adj_d*DC_plane_coeff[1][0] + DC_plane_coeff[1][1])<<","<<  -(TS_sub1_adj_d*DC_plane_coeff[0][0] + DC_plane_coeff[0][1])<<")"<<endl;
                    // h_DC1_hit->Fill(x_cats_consDC1, y_cats_consDC1);
                  }  //quad 3
                  if(dc3_flag && dc1_flag){dc_spot->Fill(  TS_sub3_adj_d*DC_plane_coeff[1][0] + DC_plane_coeff[1][1],  -(TS_sub1_adj_d*DC_plane_coeff[0][0] + DC_plane_coeff[0][1]));
                  // cout<<"(x,y) is: ("<<TS_sub3_adj_d*DC_plane_coeff[1][0] + DC_plane_coeff[1][1]<<","<<   -(TS_sub1_adj_d*DC_plane_coeff[0][0] + DC_plane_coeff[0][1])<<")"<<endl;
                    // h_DC1_hit->Fill(x_cats_consDC1, y_cats_consDC1);
                  }  //quad 4
                // }

                outputTree->Fill();
                // cout<<"************************************************"<<endl;  
              }
            }
        }
        event_counter++;
    }
    event_counter = 0;


  
    c1 = new TCanvas("c1", "DC1234 Time Subtraction", 800, 600);

    c1->Divide(2,2);
    c1->cd(1);
    gPad->SetLogy();
    h1->SetLineColor(kBlue);
    h1->Draw(); 
    fit_erf_line(h1, -170, -147);          //fitting the drift time spectra
    c1->cd(2);
    gPad->SetLogy();
    h2->SetLineColor(kBlue);
    h2->Draw();
    fit_erf_line(h2, -170, -153);
    c1->cd(3);
    gPad->SetLogy();  
    h3->SetLineColor(kBlue);
    h3->Draw();
    fit_erf_line(h3, -170, -150);
    c1->cd(4);
    gPad->SetLogy();
    h4->SetLineColor(kBlue);
    h4->Draw();
    fit_erf_line(h4, -168, -144);
    h1->GetXaxis()->SetRangeUser(-180,-100);
    h2->GetXaxis()->SetRangeUser(-180,-100);
    h3->GetXaxis()->SetRangeUser(-180,-100);
    h4->GetXaxis()->SetRangeUser(-180,-100);
    c1->Update();


    c3 = new TCanvas("c3", "DC Time Subtraction Added", 800, 600);
    c3->Divide(2,2);
    c3->cd(1);
    gPad->SetLogy();
    h1s_opposite->SetLineColor(kRed+1);
    h1s_opposite->Draw();
    c3->cd(2);
    gPad->SetLogy();
    h2s_opposite->SetLineColor(kBlue+1);
    h2s_opposite->Draw();
    c3->cd(3);
    gPad->SetLogy();
    h3s_opposite->SetLineColor(kRed+1);
    h3s_opposite->Draw();
    c3->cd(4);
    gPad->SetLogy(); 
    h4s_opposite->SetLineColor(kBlue+1);
    h4s_opposite->Draw();

    


    c3->Update();

    TCanvas *c8 = new TCanvas("c8", "DC Energy Spectra", 800, 600);
    c8->Divide(2,2);
    c8->cd(1);
    gPad->SetLogy();
    h1s_opposite->SetLineColor(kRed+1);
    h1s_opposite->Draw();
    h2s_opposite->SetLineColor(kBlue+1);
    h2s_opposite->Draw("same");
    c8->cd(2);
    gPad->SetLogy();
    hadded12->SetLineColor(kMagenta);
    hadded12->Draw();
    c8->cd(3);
    gPad->SetLogy();
    h3s_opposite->SetLineColor(kRed+1);
    h3s_opposite->Draw();
    h4s_opposite->SetLineColor(kBlue+1);
    h4s_opposite->Draw("same");
    c8->cd(4);
    gPad->SetLogy(); 
    hadded34->SetLineColor(kMagenta);
    hadded34->Draw();
    c8->Update(); 


    TCanvas *c11 = new TCanvas("c11", "DC construction y", 800, 600);
    c11->Divide(2,2);
    c11->cd(1);
    h_yplus->Draw("COLZ");
    c11->cd(2);
    h_yminus->Draw("COLZ");
    c11->Update();


    TCanvas *c17 = new TCanvas("c17", "DC3 and DC4 Time Subtraction Added", 800, 600);
    c17->Divide(1,2);
    c17->cd(1);

    h_vertical->Add(h_yplus, h_yminus, 1.0, 1.0);
    h_vertical->SetTitle("DC2 TS minus DC1 TS vs CATS1 Y at DC1");
    h_vertical->GetXaxis()->SetTitle("DC2 TS minus DC1 TS (ns)"); 
    h_vertical->GetYaxis()->SetTitle("CATS1 Y at DC1 (mm)");
    h_vertical->GetYaxis()->SetRangeUser(-50,50);
    h_vertical->Draw("COLZ");
    TProfile *prof_v = h_vertical->ProfileX();
    // prof_v->SetLineColor(kRed);
    // prof_v->SetLineWidth(2);
    // prof_v->Draw("E1 same");
    prof_v->Fit("pol1","R0","",-50,50);
    TF1 *fit_v = prof_v->GetFunction("pol1");
    fit_v->SetLineColor(kRed);
    fit_v->SetLineWidth(2);
    fit_v->Draw("same");
    c17->Update();
    
    c17->cd(2);
    h_horizontal->Add(h_xplus, h_xminus, 1.0, 1.0);
    h_horizontal->SetTitle("DC3 TS minus DC4 TS vs CATS1 X at DC1");
    h_horizontal->GetXaxis()->SetTitle("DC3 TS minus DC4 TS (ns)");
    h_horizontal->GetYaxis()->SetTitle("CATS1 X at DC1 (mm)");
    h_horizontal->GetYaxis()->SetRangeUser(-50,50);
    h_horizontal->Draw("COLZ");
    TProfile *prof_h = h_horizontal->ProfileX();
    // prof_h->SetLineColor(kRed);
    // prof_h->SetLineWidth(2);
    // prof_h->Draw("E1 same");
    prof_h->Fit("pol1","R0","",-50,50);
    TF1 *fit_h = prof_h->GetFunction("pol1");
    fit_h->SetLineColor(kRed);
    fit_h->SetLineWidth(2);
    fit_h->Draw("same");
    c17->Update();



    c5 = new TCanvas("c5", "Trajectory XZ YZ", 800, 600);
    c5->Divide(1,2);
    c5->cd(1);
    Trajectory_XZ->Draw("COLZ");
    c5->cd(2);
    Trajectory_YZ->Draw("COLZ");
    

    // c1->Write();
    // c2->Write();
    c3->Write();
    // c4->Write();
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
    // hadded21->Write();

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

    cut_left->Draw("same");
    cut_right->Draw("same");
    c7->cd(3);
    h_DC1->Draw("colz");
    c7->cd(4);
    h_DC2->Draw("colz");



    TCanvas *c13 = new TCanvas("c13", "raw dist. vs drift_time", 1200,900);
    c13->Divide(2,2);
    c13->cd(1);
    hNEw3->SetTitle("TS DC3 sub vs CATS1 X at DC3 left");
    hNEw3->GetXaxis()->SetTitle("TS DC3 sub (ns)");
    hNEw3->GetYaxis()->SetTitle("CATS1 X at DC3 (mm)");
    hNEw3->Draw("COLZ");
    TProfile *p3 = hNEw3->ProfileX();
    p3->SetLineColor(kRed);
    p3->SetLineWidth(2);
    p3->Draw("E1 same");

    c13->cd(2);
    hNEw4->SetTitle("TS DC4 sub vs CATS1 X at DC4 right");
    hNEw4->GetXaxis()->SetTitle("TS DC4 sub (ns)");
    hNEw4->GetYaxis()->SetTitle("CATS1 X at DC4 (mm)");
    hNEw4->Draw("COLZ");  
    TProfile *p4 = hNEw4->ProfileX();
    p4->SetLineColor(kRed);
    p4->SetLineWidth(2);
    p4->Draw("E1 same");
    c13->cd(3);
    hNEw1->SetTitle("TS DC1 sub vs CATS1 Y at DC1 bottom");
    hNEw1->GetXaxis()->SetTitle("TS DC1 sub (ns)");
    hNEw1->GetYaxis()->SetTitle("CATS1 Y at DC1 (mm)");
    hNEw1->Draw("COLZ");
    TProfile *p1 = hNEw1->ProfileX();
    p1->SetLineColor(kRed);
    p1->SetLineWidth(2);
    p1->Draw("E1 same");   
    c13->cd(4);
    hNEw2->SetTitle("TS DC2 sub vs CATS1 Y at DC2 top");
    hNEw2->GetXaxis()->SetTitle("TS DC2 sub (ns)");
    hNEw2->GetYaxis()->SetTitle("CATS1 Y at DC2 (mm)");
    hNEw2->Draw("COLZ");
    TProfile *p2 = hNEw2->ProfileX();
    p2->SetLineColor(kRed); 
    p2->SetLineWidth(2);
    p2->Draw("E1 same");
    c13->Update();  


    hNEw3_gated = (TH2F*)hNEw3->Clone("hNEw3_gated");hNEw3_gated -> Reset("ICES");
    hNEw4_gated = (TH2F*)hNEw4->Clone("hNEw4_gated");hNEw4_gated -> Reset("ICES");
    hNEw1_gated = (TH2F*)hNEw1->Clone("hNEw1_gated");hNEw1_gated -> Reset("ICES");
    hNEw2_gated = (TH2F*)hNEw2->Clone("hNEw2_gated");hNEw2_gated -> Reset("ICES");

    for(int i=1; i<=hNEw3->GetNbinsX(); i++){
      for(int j=1; j<=hNEw3->GetNbinsY(); j++){
        if(cut_dc3->IsInside(hNEw3->GetXaxis()->GetBinCenter(i), hNEw3->GetYaxis()->GetBinCenter(j))){
          hNEw3_gated->SetBinContent(i,j, hNEw3->GetBinContent(i,j));
        }
      }
    }
    for(int i=1; i<=hNEw4->GetNbinsX(); i++){
      for(int j=1; j<=hNEw4->GetNbinsY(); j++){
        if(cut_dc4->IsInside(hNEw4->GetXaxis()->GetBinCenter(i), hNEw4->GetYaxis()->GetBinCenter(j))){
          hNEw4_gated->SetBinContent(i,j, hNEw4->GetBinContent(i,j));
        }
      }
    }
    for(int i=1; i<=hNEw1->GetNbinsX(); i++){
      for(int j=1; j<=hNEw1->GetNbinsY(); j++){
        if(cut_dc1->IsInside(hNEw1->GetXaxis()->GetBinCenter(i), hNEw1->GetYaxis()->GetBinCenter(j))){
          hNEw1_gated->SetBinContent(i,j, hNEw1->GetBinContent(i,j));
        }
      }
    }
    for(int i=1; i<=hNEw2->GetNbinsX(); i++){
      for(int j=1; j<=hNEw2->GetNbinsY(); j++){
        if(cut_dc2->IsInside(hNEw2->GetXaxis()->GetBinCenter(i), hNEw2->GetYaxis()->GetBinCenter(j))){
          hNEw2_gated->SetBinContent(i,j, hNEw2->GetBinContent(i,j));
        }
      }
    }

    TF1* line1 = new TF1("line1","[0]*x+[1]", -200, 0);
    TCanvas *c14 = new TCanvas("c14", "gated dist. vs drift_time", 1200,900);
    c14->Divide(2,2);
    c14->cd(1);
    hNEw3_gated->SetTitle("gated TS DC3 sub vs CATS1 X at DC3 left");
    hNEw3_gated->GetXaxis()->SetTitle("TS DC3 sub (ns)");
    hNEw3_gated->GetYaxis()->SetTitle("CATS1 X at DC3 (mm)");

    TProfile *p3g = hNEw3_gated->ProfileX();
    p3g->SetLineColor(kRed);
    p3g->SetLineWidth(2);
    p3g->Fit("line1","R");
    hNEw3_gated->Draw("COLZ");
    p3g->Draw("same");

    c14->cd(2);
    hNEw4_gated->SetTitle("gated TS DC4 sub vs CATS1 X at DC4 right");
    hNEw4_gated->GetXaxis()->SetTitle("TS DC4 sub (ns)");
    hNEw4_gated->GetYaxis()->SetTitle("CATS1 X at DC4 (mm)");
    TProfile *p4g = hNEw4_gated->ProfileX();
    p4g->SetLineColor(kRed);
    p4g->SetLineWidth(2);
    p4g->Fit("line1","R");
    hNEw4_gated->Draw("COLZ");  
    p4g->Draw("same");
    c14->cd(3);
    hNEw1_gated->SetTitle("gated TS DC1 sub vs CATS1 Y at DC1 bottom");
    hNEw1_gated->GetXaxis()->SetTitle("TS DC1 sub (ns)");
    hNEw1_gated->GetYaxis()->SetTitle("CATS1 Y at DC1 (mm)");
    TProfile *p1g = hNEw1_gated->ProfileX();
    p1g->SetLineColor(kRed);
    p1g->SetLineWidth(2);
    p1g->Fit("line1","R");
    hNEw1_gated->Draw("COLZ");

    p1g->Draw("same");   
    c14->cd(4);
    hNEw2_gated->SetTitle("gated TS DC2 sub vs CATS1 Y at DC2 top");
    hNEw2_gated->GetXaxis()->SetTitle("TS DC2 sub (ns)");
    hNEw2_gated->GetYaxis()->SetTitle("CATS1 Y at DC2 (mm)");
    TProfile *p2g = hNEw2_gated->ProfileX();
    p2g->SetLineColor(kRed); 
    p2g->SetLineWidth(2);
    p2g->Fit("line1","R");
    hNEw2_gated->Draw("COLZ");

    p2g->Draw("same");
    c14->Update();

    TCanvas *c15 = new TCanvas("c15", "DC spot", 800,600);
    dc_spot->SetTitle("DC spot");
    dc_spot->GetXaxis()->SetTitle("X Position (mm)");
    dc_spot->GetYaxis()->SetTitle("Y Position (mm)");
    dc_spot->Draw("COLZ");
    c15->Update();

    TCanvas *c16 = new TCanvas("c16", "DC1 hit positions", 800,600);
    h_DC1_hit->SetTitle("DC1 hit positions");
    h_DC1_hit->GetXaxis()->SetTitle("X Position (mm)");
    h_DC1_hit->GetYaxis()->SetTitle("Y Position (mm)");
    h_DC1_hit->Draw("COLZ");
    c16->Update();

    // outputTree->Write();


}
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
  f->SetParameters(C0_guess, mu_guess, sigma_guess, a0_guess, a1_guess);
  f->SetParLimits(2, (xmax-xmin)/1e4, (xmax-xmin)); // sigma in a sensible range

  h->Fit(f, "R"); // fit in range

  // draw
  TCanvas *c = new TCanvas("c_erf_line","erf+line fit",900,600);
  h->Draw();
  f->SetLineWidth(3);
  f->Draw("same");
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


TCutG *cut_right;
TCutG *cut_left;
TCutG *cut_top;
TCutG *cut_bottom;
TCutG *cut_y1y2;

void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/558.root");
}

void loadCUTS(){
  TFile *cut_L = new TFile("/home/sharmap/Workplace/np4/e843/Codes/DC/cut_left.root","READ");
  TFile *cut_R = new TFile("/home/sharmap/Workplace/np4/e843/Codes/DC/cut_right.root","READ");
  TFile *cut_T = new TFile("/home/sharmap/Workplace/np4/e843/Codes/DC/cut_top.root","READ");
  TFile *cut_B = new TFile("/home/sharmap/Workplace/np4/e843/Codes/DC/cut_bottom.root","READ");
  TFile *file_cut_y1y2 = new TFile("/home/sharmap/Workplace/np4/e843/Codes/DC/catsy1y2.root","READ");

  // Load your cuts here if needed
  cut_left = (TCutG*)cut_L->FindObjectAny("CUTG");
  cut_right = (TCutG*)cut_R->FindObjectAny("CUTG");
  cut_top = (TCutG*)cut_T->FindObjectAny("CUTG");
  cut_bottom = (TCutG*)cut_B->FindObjectAny("CUTG");

  cut_y1y2 = (TCutG*)file_cut_y1y2->FindObjectAny("CUTG");
}



void build_DC_2(){
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    loadCUTS();
    cout<<"Total Entries: "<<tree->GetEntries()<<endl;
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

    TH1 *hdc1 = new TH1D("hdc1", "DC 1 Energy", 400, -200, 200);
    TH1 *hdc2 = new TH1D("hdc2", "DC 2 Energy", 400, -200, 200);
    TH1 *hdc3 = new TH1D("hdc3", "DC 3 Energy", 400, -200, 200);
    TH1 *hdc4 = new TH1D("hdc4", "DC 4 Energy", 400, -200, 200);
    
    TH1 *hadded12 = new TH1D("hadded12", "DC 1 and DC 2 Time Subtraction added", 400, -200, 200);
    TH1 *hadded21 = new TH1D("hadded21", "DC 2 and DC 1 Time Subtraction added", 400, -200, 200);

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



    
    Double_t Position[3] = {-1587.1, -1090.1, 1260.9}; //CATS1 z pos, CATS2 z pos, DC z pos
    Double_t Position_ZDD[4] = {1260.9+10, 1260.9+30, 1560.9, 2060.9}; // Positions of DC1 (1 cm more), DC2 (3 cm more ), IC1, and Plastics
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

    while(reader.Next()/*  && event_counter < 500000 */) {
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }
        // if (entry_count > max_entries) break;

        if (*GATCONF_r == 1 || *GATCONF_r == 2 ||*GATCONF_r == 16 ||*GATCONF_r == 32) {
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
                Double_t CATS2_X =  (phy_cats_r->PositionX[1])    /*  - 10 */; // Shift CATS2 X position by 1 cm
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
                    if(cut_top->IsInside(target_cons_X, target_cons_y))
                      hdc1 ->Fill(-TS_sub1-153);
                }
                if(DC_TS2_r.GetSize()==1){
                    unsigned long long dc_2_ts  = (unsigned long long)(DC_TS2_r[0]);
                    TS_sub2 =  static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_2_ts);
                    h2->Fill(TS_sub2);
                    if(cut_top->IsInside(target_cons_X, target_cons_y))
                      hdc2 ->Fill(TS_sub2+158);
                }
                if(DC_TS3_r.GetSize()==1){
                    unsigned long long dc_3_ts  = (unsigned long long)(DC_TS3_r[0]);
                    TS_sub3 = static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_3_ts);
                    h3->Fill(TS_sub3);
                    if(cut_right->IsInside(target_cons_X, target_cons_y))
                      hdc3 ->Fill(TS_sub3);
                }
                if(DC_TS4_r.GetSize()==1){
                    unsigned long long dc_4_ts  = (unsigned long long)(DC_TS4_r[0]);
                    TS_sub4 =  static_cast<Long64_t>(cats2_ts) - static_cast<Long64_t>(dc_4_ts);
                    h4->Fill(TS_sub4);
                    if(cut_right->IsInside(target_cons_X, target_cons_y))
                      hdc4 ->Fill(TS_sub4);
                }

                if(TS_sub1>(-154) && TS_sub2>(-160)){
                    h1s->Fill(TS_sub1+153);                                     //drift time spectra
                    h2s->Fill(-TS_sub2-158);                                    //drift time spectra

                    h1s_opposite->Fill(-TS_sub1-153);                           //adjusted time spectra for reference
                    h2s_opposite->Fill(TS_sub2+158);
                    

                    Double_t TS12 = TS_sub1 - TS_sub2 - 5;
                    Double_t TS21 = TS_sub2 - TS_sub1 + 5;

                    hadded12->Fill(TS12);                                         //added the adjusted drift time spectra
                    hadded21->Fill(TS21);                       
                    // if(cut_y1y2->IsInside(CATS1_Y, CATS2_Y)) {               
                      h_y_cats_cons->Fill(y_cats_consDC1, TS21);
                    // }                       //drawing cats cons vs drift time y
                    h_x_cats_cons->Fill(x_cats_consDC1, TS21);                       //drawing cats cons vs drift time x
                    h_cats1xcats1y->Fill(CATS1_X, CATS1_Y);                         //CATS1 X:Y
                    h_cats2xcats2y->Fill(CATS2_X, CATS2_Y);                         //CATS2 X:Y

                    h_cats1xcats2x->Fill(CATS1_X,CATS2_X);
                    h_cats1ycats2y->Fill(CATS1_Y,CATS2_Y);
                    
                    h_target->Fill(target_cons_X,target_cons_y);
                    h_DC1->Fill(x_cats_consDC1,y_cats_consDC1);
                    h_DC2->Fill(x_cats_consDC2,y_cats_consDC2);
                }

                outputTree->Fill();
                // cout<<"************************************************"<<endl;
              }
            }
        }
        event_counter++;
    }
    c1 = new TCanvas("c1", "DC12 Time Subtraction", 800, 600);
    c2 = new TCanvas("c2", "DC34 Time Subtraction", 800, 600);

    c1->Divide(1,2);
    c1->cd(1);
    h1->SetLineColor(kBlue);
    h1->Draw(); 
    // fit_erf_line(h1, -600, 200);
    c1->cd(2);
    h2->SetLineColor(kBlue);
    h2->Draw();
    // fit_erf_line(h2, -600, 200);
    
    c2->Divide(1,2);
    c2->cd(1);
    h3->SetLineColor(kBlue);
    h3->Draw();
    // fit_erf_line(h3, -600, 200);
    c2->cd(2);
    h4->SetLineColor(kBlue);
    h4->Draw();
    // fit_erf_line(h4, -600, 200);
    c1->Update();
    c2->Update();
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
    hdc1->SetLineColor(kGreen);
    hdc1->Draw("same");
    hdc2->SetLineColor(kOrange);
    hdc2->Draw("same");
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
    cut_bottom->Draw("same");
    cut_top->Draw("same");
    cut_left->Draw("same");
    cut_right->Draw("same");
    c7->cd(3);
    h_DC1->Draw("colz");
    c7->cd(4);
    h_DC2->Draw("colz");

    
    

    outputTree->Write();


}
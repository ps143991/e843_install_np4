// TripleAlphaSatellites.C
#include <iostream>

#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"

using namespace std;

TFile* f;
TH1D* hh1;
// --------------------------------------------------------
// Per-source models: exactly your functions
// (intensities hard-coded as ratios)
// --------------------------------------------------------

Double_t gausss(Double_t *x, Double_t *par)
{
  // [0] : constant
  // [1] : mean
  // [2] : sigma

  Double_t arg = 0;

  if (par[2] != 0) {
    arg = (x[0] - par[1]) / par[2];
  }
  else
    std::cout << "Attention, sigma est nul !" << std::endl;

  Double_t gaus = par[0] * TMath::Exp(-0.5 * arg * arg);

  return gaus;
}

Double_t source_Pu(Double_t *x, Double_t *par)
{
  // [0] : constant (amplitude of main peak)
  // [1] : position peak1
  // [2] : position peak2
  // [3] : position peak3
  // [4] : sigma

  Double_t arg1 = 0;  
  Double_t arg2 = 0;
  Double_t arg3 = 0;

  if (par[4] != 0) {
    arg1 = (x[0] - par[1]) / par[4];
    arg2 = (x[0] - par[2]) / par[4];
    arg3 = (x[0] - par[3]) / par[4];
  }
  else
    std::cout << "Attention, sigma est nul !" << std::endl;

  Double_t gaus1 =            par[0]        * TMath::Exp(-0.5 * arg1 * arg1);
  Double_t gaus2 = 17.11/70.77 * par[0]      * TMath::Exp(-0.5 * arg2 * arg2);
  Double_t gaus3 = 11.94/70.77 * par[0]      * TMath::Exp(-0.5 * arg3 * arg3);
  Double_t fitval = gaus1 + gaus2 + gaus3;

  return fitval;
}

Double_t source_Am(Double_t *x, Double_t *par)
{
  // [0] : constant
  // [1] : position peak1
  // [2] : position peak2
  // [3] : position peak3
  // [4] : sigma

  Double_t arg1 = 0;
  Double_t arg2 = 0;
  Double_t arg3 = 0;

  if (par[4] != 0) {
    arg1 = (x[0] - par[1]) / par[4];
    arg2 = (x[0] - par[2]) / par[4];
    arg3 = (x[0] - par[3]) / par[4];
  }
  else
    std::cout << "Attention, sigma est nul !" << std::endl;

  Double_t gaus1 =            par[0]        * TMath::Exp(-0.5 * arg1 * arg1);
  Double_t gaus2 = 13.1/84.8 * par[0]      * TMath::Exp(-0.5 * arg2 * arg2);
  Double_t gaus3 =  1.66/84.8 * par[0]      * TMath::Exp(-0.5 * arg3 * arg3);
  Double_t fitval = gaus1 + gaus2 + gaus3;

  return fitval;
}   

Double_t source_Cm(Double_t *x, Double_t *par)
{
  // [0] : constant
  // [1] : position peak1
  // [2] : position peak2
  // [3] : sigma

  Double_t arg1 = 0;
  Double_t arg2 = 0;

  if (par[3] != 0) {
    arg1 = (x[0] - par[1]) / par[3];
    arg2 = (x[0] - par[2]) / par[3];
  }
  else
    std::cout << "Attention, sigma est nul !" << std::endl;

  Double_t gaus1 =            par[0]        * TMath::Exp(-0.5 * arg1 * arg1);
  Double_t gaus2 = 23.6/76.4 * par[0]      * TMath::Exp(-0.5 * arg2 * arg2);
  Double_t fitval = gaus1 + gaus2;

  return fitval;
}

// --------------------------------------------------------
// Main function you will call from ROOT
//
// h          : your ADC histogram (already filled)
// a          : linear calibration slope (MeV / channel)
//             (sign also encodes X/Y as in your original code)
// meanPu     : initial guess for main Pu peak (ADC channels)
// meanAm     : initial guess for main Am peak
// meanCm     : initial guess for main Cm peak
// sigmaPu    : initial sigma guess for Pu (ADC channels)
// sigmaAm    : initial sigma guess for Am
// sigmaCm    : initial sigma guess for Cm
//
// It will:
//  - fit Pu, Am, Cm regions with your satellite models
//  - print fitted means & sigmas
//  - compute & print total intensities for each nucleus
// --------------------------------------------------------

void load_file(){
    f = new TFile("/home/sharmap/Workplace/np4/e843/data/run573/Tele_1_X.root","READ");
}



void function_(TH1 *h, double meanPu, double meanAm, double meanCm,
                    double sigmaPu, double sigmaAm, double sigmaCm)
{

  
  cout<<"Debug: meanPu "<< meanPu<<" meanAm "<< meanAm<<" meanCm "<< meanCm<<endl;

  if (!h) {
    std::cout << "FitTripleAlphaSources: histogram pointer is null\n";
    return;
  }
  double a = 0;
  a = (5.8 - 5.15)/(meanCm - meanPu)  ; // MeV / channel calibration slope


  const double keVtoMeV = 1.0 / 1000.0;

  // -------------------------------
  // Define fit windows around each main peak
  // Your original code: asymmetry depends on sign of 'a'
  // -------------------------------
  double linf1, lsup1, linf2, lsup2, linf3, lsup3;

  if (a > 0) { // Y case in your code
    linf1 = meanPu - 18; lsup1 = meanPu + 13; 
    linf2 = meanAm - 18; lsup2 = meanAm + 13; 
    linf3 = meanCm - 18; lsup3 = meanCm + 13; 
  }
  else {       // X case
    lsup1 = meanPu + 18; linf1 = meanPu - 13;
    lsup2 = meanAm + 18; linf2 = meanAm - 13;
    lsup3 = meanCm + 18; linf3 = meanCm - 13;
  }

  // Use a typical bin width for integral conversion
  double binWidth = h->GetXaxis()->GetBinWidth(1);

  // ======================================================
  // 1) Pu fit
  // ======================================================
  TF1 *Pu = new TF1("fit_sat_Pu", source_Pu, linf1, lsup1, 5);
  Pu->SetParNames("ConstPu", "Mean1_Pu", "Mean2_Pu", "Mean3_Pu", "SigmaPu");

  // satellite positions in ADC channels, using your formula: mean - dE/a
  double m1_Pu = meanPu;
  double m2_Pu = meanPu - 12.4 * keVtoMeV / a;
  double m3_Pu = meanPu - 51.6 * keVtoMeV / a;

  Pu->SetParameters(150.0, m1_Pu, m2_Pu, m3_Pu, sigmaPu);

  Pu->SetParLimits(1, meanPu - 5*sigmaPu, meanPu + 5*sigmaPu);  
  Pu->SetParLimits(2, m2_Pu - 10,        m2_Pu + 10);           
  Pu->SetParLimits(3, m3_Pu - 10,        m3_Pu + 10);           
  Pu->SetParLimits(4, sigmaPu * 0.3,     sigmaPu * 3.0);
  Pu->SetLineColor(kBlue);
  h->Fit(Pu, "RQ");

  double meanPu_fit   = Pu->GetParameter(1);
  double sigmaPu_fit  = Pu->GetParameter(4);
  double intPu_counts = Pu->Integral(linf1, lsup1) / binWidth;

  // ======================================================
  // 2) Am fit
  // ======================================================
  TF1 *Am = new TF1("fit_sat_Am", source_Am, linf2, lsup2, 5);
  Am->SetParNames("ConstAm", "Mean1_Am", "Mean2_Am", "Mean3_Am", "SigmaAm");


  double m1_Am = meanAm;                        
  double m2_Am = meanAm - 43.2 * keVtoMeV / a;  
  double m3_Am = meanAm - 98.4 * keVtoMeV / a;  

  Am->SetParameters(150.0, m1_Am, m2_Am, m3_Am, sigmaAm);

  Am->SetParLimits(1, meanAm - 5*sigmaAm, meanAm + 5*sigmaAm);  
  Am->SetParLimits(2, m2_Am -1,        m2_Am + 5);          
  Am->SetParLimits(3, m3_Am - 7,        m3_Am + 4);        
  Am->SetParLimits(4, 2.1, 2.5);
  Am->SetLineColor(kBlue);
  h->Fit(Am, "RQ+");

  double meanAm_fit   = Am->GetParameter(1);
  double sigmaAm_fit  = Am->GetParameter(4);
  double intAm_counts = Am->Integral(linf2, lsup2) / binWidth;

  // ======================================================
  // 3) Cm fit
  // ======================================================
  TF1 *Cm = new TF1("fit_sat_Cm", source_Cm, linf3, lsup3, 4);
  Cm->SetParNames("ConstCm", "Mean1_Cm", "Mean2_Cm", "SigmaCm");

  double m1_Cm = meanCm;
  double m2_Cm = meanCm - 43.1 * keVtoMeV / a;

  Cm->SetParameters(150.0, m1_Cm, m2_Cm, sigmaCm);

  Cm->SetParLimits(1, meanCm - 5*sigmaCm, meanCm + 5*sigmaCm); 
  Cm->SetParLimits(2, m2_Cm - 10,        m2_Cm + 10);
  Cm->SetParLimits(3, sigmaCm * 0.3,     sigmaCm * 3.0);        

  Cm->SetLineColor(kBlue);
  h->Fit(Cm, "RQ+");

  double meanCm_fit   = Cm->GetParameter(1);
  double sigmaCm_fit  = Cm->GetParameter(3);
  double intCm_counts = Cm->Integral(linf3, lsup3) / binWidth;

  // ======================================================
  // Print results
  // ======================================================
  std::cout << "\n===== Triple-alpha satellite fit results =====\n";
  std::cout << "Pu: mean = " << meanPu_fit
            << ", sigma = " << sigmaPu_fit
            << ", total counts (Pu group) = " << intPu_counts << "\n";

  std::cout << "Am: mean = " << meanAm_fit
            << ", sigma = " << sigmaAm_fit
            << ", total counts (Am group) = " << intAm_counts << "\n";

  std::cout << "Cm: mean = " << meanCm_fit
            << ", sigma = " << sigmaCm_fit
            << ", total counts (Cm group) = " << intCm_counts << "\n";

  std::cout << "=============================================\n";


  cout<<"PU1: "<< Pu->GetParameter(3) << " PU2: "<< Pu->GetParameter(2) << " PU3: "<< Pu->GetParameter(1) <<endl;
  cout<<"AM1: "<< Am->GetParameter(3) << " AM2: "<< Am->GetParameter(2) << " AM3: "<< Am->GetParameter(1) <<endl;
  cout<<"CM1: "<< Cm->GetParameter(2) << " CM2: "<< Cm->GetParameter(1) <<endl;

  // also draw the individual source contributions and the sattelites
  Pu->SetLineColor(kBlue);
  Am->SetLineColor(kBlue);
  Cm->SetLineColor(kBlue);


  TF1 *pu_sat1 = new TF1("pu_sat1", gausss, linf1, lsup1, 3);
  pu_sat1->SetParameters(Pu->GetParameter(0), Pu->GetParameter(1), Pu->GetParameter(4));
  pu_sat1->SetLineColor(kRed);
  pu_sat1->Draw("same");
  TF1 *pu_sat2 = new TF1("pu_sat2", gausss, linf1, lsup1, 3);
  pu_sat2->SetParameters(Pu->GetParameter(0)*17.11/70.77, Pu->GetParameter(2), Pu->GetParameter(4));
  pu_sat2->SetLineColor(kRed);
  pu_sat2->Draw("same");
  TF1 *pu_sat3 = new TF1("pu_sat3", gausss, linf1, lsup1, 3);
  pu_sat3->SetParameters(Pu->GetParameter(0)*11.94/70.77, Pu->GetParameter(3), Pu->GetParameter(4));
  pu_sat3->SetLineColor(kRed);
  pu_sat3->Draw("same");

  TF1 *am_sat1 = new TF1("am_sat1", gausss, linf2, lsup2, 3);
  am_sat1->SetParameters(Am->GetParameter(0), Am->GetParameter(1), Am->GetParameter(4));
  am_sat1->SetLineColor(kRed);
  am_sat1->Draw("same");
  TF1 *am_sat2 = new TF1("am_sat2", gausss, linf2, lsup2, 3);
  am_sat2->SetParameters(Am->GetParameter(0)*13.1/84.8, Am->GetParameter(2), Am->GetParameter(4));
  am_sat2->SetLineColor(kRed);
  am_sat2->Draw("same");
  TF1 *am_sat3 = new TF1("am_sat3", gausss, linf2, lsup2, 3);
  am_sat3->SetParameters(Am->GetParameter(0)*1.66/84.8, Am->GetParameter(3), Am->GetParameter(4));
  am_sat3->SetLineColor(kRed);
  am_sat3->Draw("same");

  TF1 *cm_sat1 = new TF1("cm_sat1", gausss, linf3, lsup3, 3);
  cm_sat1->SetParameters(Cm->GetParameter(0), Cm->GetParameter(1), Cm->GetParameter(3));
  cm_sat1->SetLineColor(kRed);
  cm_sat1->Draw("same");
  TF1 *cm_sat2 = new TF1("cm_sat2", gausss, linf3, lsup3, 3);
  cm_sat2->SetParameters(Cm->GetParameter(0)*23.6/76.4, Cm->GetParameter(2), Cm->GetParameter(3));
  cm_sat2->SetLineColor(kRed);
  cm_sat2->Draw("same");
  
}
void calibrator_3_alpha(){
  load_file();
  f->cd();
  TCanvas* c1 = (TCanvas*)f->Get("c_T1_X_06");
  if(!c1){
      std::cout<<"Canvas not found!"<<std::endl;
      return;
  }
  c1->cd();
  // hh1 = (TH1D*)c1->Get("h_Tele_1_X_strip_066");
  //retrieve the histogram from the canvas defined as OBJ: TH1D

  hh1 = (TH1D*)c1->FindObject("h_T1_X_strip_080");


  if(!hh1){
      std::cout<<"Histogram not found!"<<std::endl;
      return;
  }
  TCanvas* c2 = new TCanvas("c2","Fit Triple Alpha Sources",800,600);
  c2->cd();
  hh1->Draw();
  function_(hh1, 8797.7, 8840, 8876.79, 3.24, 3.38, 3.86);
}

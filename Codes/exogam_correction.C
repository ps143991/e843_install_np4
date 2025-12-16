#include <iostream>
using namespace std;

// a simple script to  draw the resolution of gamma ray  energy after Doppler correction with EXOGAM as a function of angle and beta values
// the formula used for Doppler correction is : E_corrected = E_measured * (1 - beta * cos(theta)) / sqrt(1 - beta^2)
// to know the uncertainity propagation, we derive the formula with respect to theta and beta
// dE_corrected/dtheta = E_measured * beta * sin(theta) / sqrt(1 - beta^2)
// dE_corrected/dbeta = E_measured * (cos(theta) - beta) / (1 - beta^2)/(1-beta*cos(theta))
// then the


// DopplerResolutionStudy.C
// Plots Doppler-corrected gamma-ray resolution contributions vs angle for given E0, beta, σθ, σβ, and detector FWHM.
// Author: ChatGPT (for EXOGAM clovers @ 14 cm; highlights 90° and 135°)

#include "TCanvas.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TMath.h"
#include "TGraph.h"
#include <iostream>
#include <iomanip>


inline double BetaFromEperU(double E_per_u_MeV){
  const double mc2 = 931.494;
  const double gamma = 1.0 + E_per_u_MeV/mc2;
  return std::sqrt(1.0 - 1.0/(gamma*gamma));
}

// --------------- USER PARAMETERS (EDIT THESE) -----------------
static const double E0_keV        = 915;   // True gamma energy [keV] after perfect Doppler correction
static const double beta_value          = 0.18;    // Beam/recoil speed / c used in correction
// static const double sigmaTheta_deg= 8.0;      // Effective ANGULAR RMS uncertainty [degrees]
static const double sigmabeta_value     = 0.003;   // RMS uncertainty on beta_value (dimensionless)
static const double FWHM_det_keV  = 2.4667;      // Intrinsic detector FWHM at this energy [keV], pre-correction
// Optional quick helper: estimate σθ from segmentation and distance
// (Uncomment & use to replace sigmaTheta_deg if desired.)
static const double distance_mm   = 140.0;    // target-to-crystal face distance [mm] ~14 cm
static const double seg_pitch_mm  = 27.5;     // rear 2x2 segment pitch ~ 45 mm / 2
static const double sigmaTheta_deg= (seg_pitch_mm/(TMath::Sqrt(12.0)*distance_mm))*TMath::RadToDeg();

// Angles to annotate for your geometry:
static const double markTheta1_deg = 90.0  ;
static const double markTheta2_deg = 135.0 ;

// Plot range (degrees)
static const double thetaMin = 0.1;
static const double thetaMax = 179.9;
// -------------------------------------------------------------

namespace {
  constexpr double kDeg2Rad = TMath::Pi()/180.0;
  constexpr double kRad2Deg = 180.0/TMath::Pi();
  constexpr double kF2S = 1.0/2.354820045; // FWHM -> sigma

  inline double gamma_from_beta_value(double b){ return 1.0/TMath::Sqrt(1.0 - b*b); }

  // Angular contribution FWHM(θ): from σθ
  double fAng_impl(double theta_deg, double E0, double b, double sigmaTheta_deg){
    const double th = theta_deg*kDeg2Rad;
    const double denom = 1.0 - b*TMath::Cos(th);
    const double deriv = E0 * b * TMath::Sin(th) / denom; // dE_corr/dθ  [keV/rad]
    const double sigma_theta_rad = sigmaTheta_deg * kDeg2Rad;
    const double sigma_keV = TMath::Abs(deriv) * sigma_theta_rad;
    return 2.354820045 * sigma_keV; // FWHM
  }

  // beta_value contribution FWHM(θ): from σβ
  double fbeta_value_impl(double theta_deg, double E0, double b, double sigmaB){
    const double th = theta_deg*kDeg2Rad;
    const double denom = 1.0 - b*TMath::Cos(th);
    const double term  = ( b/(1.0 - b*b) ) - ( TMath::Cos(th)/denom ); // bracket
    const double deriv = E0 * term; // dE_corr/dβ [keV]
    const double sigma_keV = TMath::Abs(deriv) * sigmaB;
    return 2.354820045 * sigma_keV; // FWHM
  }

  // Detector contribution FWHM(θ): intrinsic FWHM on E_meas scaled by γ(1 - β cosθ) through the correction
  double fDet_impl(double theta_deg, double b, double FWHM_det){
    const double th = theta_deg*kDeg2Rad;
    const double gam = gamma_from_beta_value(b);
    return FWHM_det * gam * (1.0 - b*TMath::Cos(th)); // FWHM
  }

  // Total FWHM in quadrature
  double fTot_impl(double theta_deg, double E0, double b, double sTh_deg, double sB, double FWHM_det){
    const double fA = fAng_impl(theta_deg, E0, b, sTh_deg) * kF2S; // to sigma
    const double fB = fbeta_value_impl(theta_deg, E0, b, sB)    * kF2S;
    const double fD = fDet_impl(theta_deg, b, FWHM_det)   * kF2S;
    const double sigma_tot = TMath::Sqrt(fA*fA + fB*fB + fD*fD);
    return 2.354820045 * sigma_tot; // back to FWHM
  }

  // Wrappers for TF1
  double fAng_TF1(double *x, double *par){
    return fAng_impl(x[0], par[0], par[1], par[2]);
  }
  double fbeta_value_TF1(double *x, double *par){
    return fbeta_value_impl(x[0], par[0], par[1], par[3]);
  }
  double fDet_TF1(double *x, double *par){
    return fDet_impl(x[0], par[1], par[4]);
  }
  double fTot_TF1(double *x, double *par){
    return fTot_impl(x[0], par[0], par[1], par[2], par[3], par[4]);
  }
}

void exogam_correction(){
  gStyle->SetOptStat(0);

  // Parameters vector for TF1 (we'll use a common 5-par array):
  // par[0]=E0_keV, par[1]=beta_value, par[2]=sigmaTheta_deg, par[3]=sigmabeta_value, par[4]=FWHM_det_keV
  double par[5] = {E0_keV, beta_value, sigmaTheta_deg, sigmabeta_value, FWHM_det_keV};
  // Define TF1s
  TF1 *fAng = new TF1("fAng",  fAng_TF1,  thetaMin, thetaMax, 5);
  TF1 *fBet = new TF1("fbeta_value", fbeta_value_TF1, thetaMin, thetaMax, 5);
  TF1 *fDet = new TF1("fDet",  fDet_TF1,  thetaMin, thetaMax, 5);
  TF1 *fTot = new TF1("fTot",  fTot_TF1,  thetaMin, thetaMax, 5);
  for(int i=0;i<5;i++){ fAng->SetParameter(i,par[i]); fBet->SetParameter(i,par[i]); fDet->SetParameter(i,par[i]); fTot->SetParameter(i,par[i]); }

  fAng->SetTitle("Doppler-corrected Resolution Contributions vs #theta;#theta (deg);FWHM (keV)");
  fAng->SetLineColor(kOrange+7);
  fAng->SetLineWidth(3);

  fBet->SetLineColor(kAzure+2);
  fBet->SetLineWidth(3);

  fDet->SetLineColor(kGreen+2);
  fDet->SetLineWidth(3);

  fTot->SetLineColor(kRed+1);
  fTot->SetLineWidth(4);

  // Draw
  TCanvas *c = new TCanvas("c","Doppler Resolution Study", 1000, 700);
  c->SetGrid();
  fAng->SetMaximum( TMath::Max( fTot->GetMaximum(thetaMin,thetaMax), 1.2*fDet->GetMaximum(thetaMin,thetaMax) ) );
  fAng->Draw();
  fBet->Draw("same");
  fDet->Draw("same");
  fTot->Draw("same");

  // Legend
  auto *leg = new TLegend(0.55,0.70,0.88,0.88);
  leg->AddEntry(fTot,"Total (quadrature)","l");
  leg->AddEntry(fAng,"Angle term","l");
  leg->AddEntry(fBet,"#beta_value term","l");
  leg->AddEntry(fDet,"Detector term","l");
  leg->SetBorderSize(0);
  leg->Draw();

  // Mark your clover angles
  TLine *L1 = new TLine(markTheta1_deg, 0, markTheta1_deg, 40);
  TLine *L2 = new TLine(markTheta2_deg, 0, markTheta2_deg, 40);
  L1->SetLineStyle(2); L2->SetLineStyle(2);
  L1->SetLineColor(kGray+2); L2->SetLineColor(kGray+2);
  L1->Draw(); L2->Draw();

  // Print numeric values at 90° and 135°
  auto printAt = [&](double th){
    
    double F_A = fAng_impl(th, E0_keV, beta_value, sigmaTheta_deg);
    double F_B = fbeta_value_impl(th, E0_keV, beta_value, sigmabeta_value);
    double F_D = fDet_impl(th, beta_value, FWHM_det_keV);
    double F_T = fTot_impl(th, E0_keV, beta_value, sigmaTheta_deg, sigmabeta_value, FWHM_det_keV);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "θ = " << th << "° :  FWHM_total=" << F_T
              << " keV   (angle " << F_A << ",  beta_value " << F_B << ",  det " << F_D << ")\n";
  };
  std::cout << "Inputs: E0="<<E0_keV<<" keV,  beta_value="<<beta_value
            <<",  sigmaTheta="<<sigmaTheta_deg<<" deg,  sigmabeta_value="<<sigmabeta_value
            <<",  FWHM_det="<<FWHM_det_keV<<" keV\n";
  printAt(markTheta1_deg);
  printAt(markTheta2_deg);

  cout<<"The total FWHM with quad of 90 and 135 is: "<<TMath::Sqrt((TMath::Power((fTot_impl(markTheta1_deg,E0_keV,beta_value,sigmaTheta_deg,sigmabeta_value,FWHM_det_keV)),2)*8 + 4*TMath::Power((fTot_impl(markTheta2_deg,E0_keV,beta_value,sigmaTheta_deg,sigmabeta_value,FWHM_det_keV)),2))/12.0)<<endl;
  //total = sqrt((FWHM1^2 * 8 + FWHM2^2 *4)/12)
  // On-canvas labels
  TLatex t; t.SetNDC(); t.SetTextSize(0.032);
  t.DrawLatex(0.15,0.92,Form("E_{0}=%.0f keV,  #beta_value=%.3f,  #sigma_{#theta}=%.2f^{#circ},  #sigma_{#beta_value}=%.3g,  FWHM_{det}=%.1f keV",
                             E0_keV,beta_value,sigmaTheta_deg,sigmabeta_value,FWHM_det_keV));

  c->Update();
}
// End of DopplerResolutionStudy.C
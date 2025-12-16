// DopplerResolutionStudyPercent.C
// Plots Doppler-corrected resolution contributions vs angle WITH Y-AXIS IN PERCENT.
// Includes angle term, beta_value term, detector term, and total (quadrature), all as FWHM%.
//
// Note: Kinematic terms scale ∝ E0, so (ΔE/E)% ~ const for them.
//       Intrinsic detector term usually follows FWHM^2 = a^2 + bE + cE^2, so % varies with E.

#include "TCanvas.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLine.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TMath.h"
#include <iostream>
#include <iomanip>

// ------------------ USER PARAMETERS ------------------
static const double E0_keV         = 1000.0;   // True gamma energy [keV] after perfect correction
static const double beta_value           = 0.3;    // β used in correction
static const double sigmaTheta_deg = 8.0;      // angular RMS [deg]
static const double sigmabeta_value      = 0.03;   // β RMS (dimensionless)

// Choose detector resolution model:
// 0 = constant FWHM_det_const_keV at the detector (pre-correction, at E_meas)
// 1 = quadratic model: FWHM^2 = a2 + b1*E + c2*E^2  (E in keV)
static const int    DET_MODEL = 1;

// If DET_MODEL==0:
static const double FWHM_det_const_keV = 100.0;

// If DET_MODEL==1 (typical ballpark numbers—replace with your calibration):
// a2: electronics/position term (keV^2), b1: Fano/stat term (keV), c2: charge collection (dimensionless)
// Example: a≈1.2 keV → a2≈1.44; b1≈0.0025 keV; c2≈(1–3)×10^-6 (tune to your crystals)
static const double det_a2 = 1.44;        // keV^2
static const double det_b1 = 0.0025;      // keV
static const double det_c2 = 1.0e-6;      // dimensionless

// Geometry markers (your clovers)
static const double markTheta1_deg = 79.2;
static const double markTheta2_deg = 100.8;

// Plot domain
static const double thetaMin = 0.1, thetaMax = 179.9;
// ----------------------------------------------------

namespace {
  constexpr double kDeg2Rad = TMath::Pi()/180.0;
  constexpr double kF2S = 1.0/2.354820045;

  inline double gamma_from_beta_value(double b){ return 1.0/TMath::Sqrt(1.0 - b*b); }

  // Intrinsic detector FWHM at the detector, as a function of E_meas (keV)
  double FWHM_det_intrinsic(double E_meas_keV){
    if(DET_MODEL==0) return FWHM_det_const_keV;
    // Quadratic model: FWHM^2 = a2 + b1*E + c2*E^2
    double v = det_a2 + det_b1*E_meas_keV + det_c2*E_meas_keV*E_meas_keV;
    return (v>0.0) ? TMath::Sqrt(v) : 0.0;
  }

  // Angular contribution (absolute FWHM in keV)
  double FWHM_ang_keV(double theta_deg, double E0, double b, double sTh_deg){
    const double th = theta_deg*kDeg2Rad;
    const double denom = 1.0 - b*TMath::Cos(th);
    const double deriv = E0 * b * TMath::Sin(th) / denom; // dE_corr/dθ [keV/rad]
    const double sigma_theta_rad = sTh_deg * kDeg2Rad;
    const double sigma_keV = TMath::Abs(deriv) * sigma_theta_rad;
    return /* 2.354820045 * */ sigma_keV;
  }

  // beta_value contribution (absolute FWHM in keV)
  double FWHM_beta_value_keV(double theta_deg, double E0, double b, double sB){
    const double th = theta_deg*kDeg2Rad;
    const double denom = 1.0 - b*TMath::Cos(th);
    const double term  = ( b/(1.0 - b*b) ) - ( TMath::Cos(th)/denom ); // d ln(E_corr)/dβ bracket
    const double deriv = E0 * term; // dE_corr/dβ [keV]
    const double sigma_keV = TMath::Abs(deriv) * sB;
    return /* 2.354820045 * */ sigma_keV;
  }

  // Detector contribution after correction (absolute FWHM in keV)
  double FWHM_det_corr_keV(double theta_deg, double E0, double b){
    const double th = theta_deg*kDeg2Rad;
    const double gam = gamma_from_beta_value(b);
    const double fac = gam * (1.0 - b*TMath::Cos(th));
    // E_meas at the crystal:
    const double E_meas = E0 / fac;
    const double F_det  = FWHM_det_intrinsic(E_meas);
    return F_det * fac; // scales through the correction
  }

  // Total absolute FWHM (keV), in quadrature of sigmas
  double FWHM_total_keV(double theta_deg, double E0, double b, double sTh_deg, double sB){
    const double fA = FWHM_ang_keV(theta_deg,E0,b,sTh_deg) /* * kF2S */;
    const double fB = FWHM_beta_value_keV(theta_deg,E0,b,sB)    /*  * kF2S */;
    const double fD = FWHM_det_corr_keV(theta_deg,E0,b)   /*  * kF2S */;
    return /* 2.354820045 * */ TMath::Sqrt(fA*fA + fB*fB + fD*fD);
  }

  // Percent helpers
  double pct(double x_keV, double E0){ return (E0>0)? 100.0 * x_keV / E0 : 0.0; }
  double FWHM_ang_pct (double th, double E0, double b, double sTh){ return pct(FWHM_ang_keV (th,E0,b,sTh), E0); }
  double FWHM_beta_value_pct(double th, double E0, double b, double sB ){ return pct(FWHM_beta_value_keV(th,E0,b,sB ), E0); }
  double FWHM_det_pct (double th, double E0, double b            ){ return pct(FWHM_det_corr_keV(th,E0,b), E0); }
  double FWHM_tot_pct (double th, double E0, double b, double sTh, double sB){
    return pct(FWHM_total_keV(th,E0,b,sTh,sB), E0);
  }

  // TF1 wrappers (return PERCENT)
  double fAngP (double *x, double *p){ return FWHM_ang_pct (x[0], p[0], p[1], p[2]); }
  double fBetP (double *x, double *p){ return FWHM_beta_value_pct(x[0], p[0], p[1], p[3]); }
  double fDetP (double *x, double *p){ return FWHM_det_pct (x[0], p[0], p[1]);       }
  double fTotP (double *x, double *p){ return FWHM_tot_pct (x[0], p[0], p[1], p[2], p[3]); }
}

void DopplerResolutionStudyPercent(){
  gStyle->SetOptStat(0);

  // par[0]=E0_keV, par[1]=beta_value, par[2]=sigmaTheta_deg, par[3]=sigmabeta_value
  double par[4] = {E0_keV, beta_value, sigmaTheta_deg, sigmabeta_value};

  TF1 *fAng = new TF1("fAng_pct", fAngP, thetaMin, thetaMax, 4);
  TF1 *fBet = new TF1("fbeta_value_pct",fBetP, thetaMin, thetaMax, 4);
  TF1 *fDet = new TF1("fDet_pct", fDetP, thetaMin, thetaMax, 4);
  TF1 *fTot = new TF1("fTot_pct", fTotP, thetaMin, thetaMax, 4);
  for(int i=0;i<4;i++){ fAng->SetParameter(i,par[i]); fBet->SetParameter(i,par[i]); fDet->SetParameter(i,par[i]); fTot->SetParameter(i,par[i]); }

  fAng->SetTitle("Doppler-corrected Resolution vs #theta;#theta (deg);FWHM (%)");
  fAng->SetLineColor(kOrange+7); fAng->SetLineWidth(3);
  fBet->SetLineColor(kAzure+2);  fBet->SetLineWidth(3);
  fDet->SetLineColor(kGreen+2);  fDet->SetLineWidth(3);
  fTot->SetLineColor(kRed+1);    fTot->SetLineWidth(4);

  TCanvas *c = new TCanvas("c_pct","Doppler Resolution (Percent)", 1000,700);
  c->SetGrid();
  fAng->Draw();
  fBet->Draw("same");
  fDet->Draw("same");
  fTot->Draw("same");

  auto *leg = new TLegend(0.55,0.70,0.88,0.88);
  leg->AddEntry(fTot,"Total (quadrature)","l");
  leg->AddEntry(fAng,"Angle term","l");
  leg->AddEntry(fBet,"#beta_value term","l");
  leg->AddEntry(fDet,"Detector term","l");
  leg->SetBorderSize(0);
  leg->Draw();

  // Mark your clover rings
  TLine *L1 = new TLine(markTheta1_deg, 0, markTheta1_deg, c->GetUymax());
  TLine *L2 = new TLine(markTheta2_deg, 0, markTheta2_deg, c->GetUymax());
  L1->SetLineStyle(2); L2->SetLineStyle(2);
  L1->SetLineColor(kGray+2); L2->SetLineColor(kGray+2);
  L1->Draw(); L2->Draw();

  // Console printouts at 90° and 135°
  auto printAt = [&](double th){
    double pA = FWHM_ang_pct (th,E0_keV,beta_value,sigmaTheta_deg);
    double pB = FWHM_beta_value_pct(th,E0_keV,beta_value,sigmabeta_value);
    double pD = FWHM_det_pct (th,E0_keV,beta_value);
    double pT = FWHM_tot_pct (th,E0_keV,beta_value,sigmaTheta_deg,sigmabeta_value);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "θ="<<th<<"° : Total="<<pT<<" %  (angle "<<pA<<" %,  beta_value "<<pB<<" %,  det "<<pD<<" %)\n";
  };
  std::cout << "Inputs: E0="<<E0_keV<<" keV, beta_value="<<beta_value
            <<", sigmaTheta="<<sigmaTheta_deg<<" deg, sigmabeta_value="<<sigmabeta_value
            <<", DET_MODEL="<<DET_MODEL<<"\n";
  printAt(markTheta1_deg);
  printAt(markTheta2_deg);

  // On-plot parameter label
  TLatex t; t.SetNDC(); t.SetTextSize(0.032);
  t.DrawLatex(0.12,0.92,Form("E_{0}=%.0f keV,  #beta_value=%.3f,  #sigma_{#theta}=%.2f^{#circ},  #sigma_{#beta_value}=%.3g",
                             E0_keV,beta_value,sigmaTheta_deg,sigmabeta_value));
  if(DET_MODEL==0){
    t.DrawLatex(0.12,0.87,Form("Detector: constant FWHM_{det}=%.2f keV (pre-correction)",FWHM_det_const_keV));
  } else {
    t.DrawLatex(0.12,0.87,Form("Detector: FWHM^{2}=a^{2}+bE+cE^{2},  a^{2}=%.3g, b=%.3g, c=%.3g",
                               det_a2,det_b1,det_c2));
  }

  c->Update();
}

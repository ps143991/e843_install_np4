// script for extrapolating and calculating gamma ray efficiency from clover detectors

#include <TCanvas.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TCutG.h>
#include <TGraph.h>
#include <iostream>
using namespace std;
// fit_erf_line.C
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMultiGraph.h"



Double_t total_energy[7]={121.8,244.7,344.3,778.9,964.1,1112.1,1408.0}; 


/***********************************************************************************************************/
//taken from efficiencyofexogam.ods -> sheet1 for time = 4098 seconds, distance = 14 cm, activity = 9920.0335
///shifting it to gamma_efficiency_total.C
/***********************************************************************************************************/




Double_t total[8] = {5.01316507753849, 7.43409025606563, 7.74895150814504, 6.09878303400616, 5.80635368729799, 5.43603128199012, 4.88351957307112};


Double_t Fit1(Double_t *x, Double_t *par){
        Double_t arg1     = TMath::Log(0.01*x[0]);          // in keV
        Double_t arg2     = TMath::Log(0.001*x[0]);          // in keV

            Double_t fit_val = TMath::Exp(TMath::Power((TMath::Power((par[0]+par[1]*arg1+par[2]*arg1*arg1),
                                (-1.0*par[3]))+TMath::Power((par[4]+par[5]*arg2+par[6]*arg2*arg2),(-1.0*par[3]))),(-1.0*1/par[3])))/1000.;
        return fit_val;
}

void gamma_efficiency_total(){

    // total intrinsic efficiency fit
    TGraph *gr_eff_tot = new TGraph(7, total_energy, total);
//set the x axis range of the graph and the fit to [0,3000]
gr_eff_tot->GetXaxis()->SetLimits(0,3000);
gr_eff_tot->GetYaxis()->SetLimits(0,10);

    gr_eff_tot-> Draw("A*");
    TF1 *eff_curve_tot = new TF1("eff_curve_tot",Fit1,0.,2500.,7); //option 7 for 7 parameterss
    gr_eff_tot->GetXaxis()->SetRangeUser(0,3000);
    gr_eff_tot->GetYaxis()->SetRangeUser(0,10);

    eff_curve_tot->SetParameter(0,5.);
    eff_curve_tot->SetParameter(1,3.);
    eff_curve_tot->SetParameter(2,-1.);
    eff_curve_tot->SetParameter(3,7.);
    eff_curve_tot->SetParameter(4,6.);
    eff_curve_tot->SetParameter(5,-0.5);
    eff_curve_tot->SetParameter(6,1.);
    gr_eff_tot->Fit("eff_curve_tot","IER+");
    gr_eff_tot->SetMarkerStyle(4);
    gr_eff_tot->SetTitle("Total Efficiency Vs Energy(keV);Energy;Total Efficiency");
    gStyle->SetOptFit(0111);  //showing the parameters on graph
    gr_eff_tot->Draw("AP");
    // TText *t = new TText(Form("Clover 2"));
    Double_t eff_at_unknown_enrg_tot; Double_t energy_tot = 2033.0;
    eff_at_unknown_enrg_tot= eff_curve_tot->Eval(energy_tot);
    cout<<"your total eff. is: "<<eff_at_unknown_enrg_tot<<endl;
}
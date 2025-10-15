// script for extrapolating and calculating gamma ray efficiency from clover detectors
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
#include "TStyle.h"
#include "TGraph.h"
#include "TMultiGraph.h"


Double_t Energy[8] = {1408, 1332, 1172, 964, 778, 344, 244, 121};
Double_t CL2[8] = {0.291265175569599, 0.289707355989666, 0.327418233582857, 0.361658270798636, 0.434824988773592, 0.744223972176433, 0.850018929386871, 0.740814378333925};
Double_t CL3[8] = {0.297418517141083, 0.305577601612272, 0.335725043949674, 0.375248901297621, 0.441661906421678, 0.679621038910337, 0.688884249903554, 0.378360298651839};
Double_t CL4[8] = {0.289167688174318, 0.298466877467297, 0.323071080650195, 0.368371451045775, 0.427363451660383, 0.73047039440765, 0.823037458890521, 0.582474961469538};
Double_t CL5[8] = {0.28002302994154, 0.277006646882597, 0.305474467026309, 0.350495611486885, 0.404242917179855, 0.655705521732256, 0.650846696237207, 0.429040593422529};
Double_t CL6[8] = {0.271163044687617, 0.270076794701998, 0.292969866863077, 0.324509442083089, 0.379074531345937, 0.564212137854637, 0.605931578951285, 0.413037711485387};
Double_t CL7[8] = {0.25998663981278, 0.236613006627839, 0.275857641758227, 0.303882573272623, 0.350239611458119, 0.512222489479593, 0.566479875569851, 0.41349501320597};
Double_t CL8[8] = {0.239744165411022, 0.229391595269636, 0.238865791917132, 0.283331443557619, 0.311953823666379, 0.415867550595083, 0.396646402502025, 0.171143965361501};
Double_t CL9[8] = {0.281618570948069, 0.290889615332936, 0.312028696255796, 0.336251291706882, 0.370637258493185, 0.460212114343388, 0.449862785221997, 0.252254351182786};
Double_t CL10[8] = {0.299420823012467, 0.299207763334766, 0.326604232293986, 0.367323880515577, 0.416910269340794, 0.580041722752041, 0.523587397740668, 0.040301555203668};
Double_t CL11[8] = {0.313150148097596, 0.312038502420921, 0.348710307371965, 0.385860888662904, 0.443242697380472, 0.670919001283531, 0.730409700871017, 0.500317334122005};
Double_t CL12[8] = {0.258679834689587, 0.247608752975502, 0.281468000019132, 0.29264939741228, 0.349701713331915, 0.523752123418604, 0.567334214090896, 0.387038957801552};
Double_t CL13[8] = {0.296005987226274, 0.298032351941048, 0.328357080284712, 0.354423011228094, 0.401300869986638, 0.59115775527918, 0.631591985885584, 0.395418800369715};

Double_t  CL_all[12][8];
void fill_CL_all(){
for(int i=0; i<8; i++){
    CL_all[0][i] = CL2[i];
    CL_all[1][i] = CL3[i];
    CL_all[2][i] = CL4[i];
    CL_all[3][i] = CL5[i];
    CL_all[4][i] = CL6[i];
    CL_all[5][i] = CL7[i];
    CL_all[6][i] = CL8[i];
    CL_all[7][i] = CL9[i];
    CL_all[8][i] = CL10[i];
    CL_all[9][i] = CL11[i];
    CL_all[10][i] = CL12[i];
    CL_all[11][i] = CL13[i];
}}

Double_t Fit1(Double_t *x, Double_t *par){
        Double_t arg1     = TMath::Log(0.01*x[0]);          // in keV
        Double_t arg2     = TMath::Log(0.001*x[0]);          // in keV

            Double_t fit_val = TMath::Exp(TMath::Power((TMath::Power((par[0]+par[1]*arg1+par[2]*arg1*arg1),(-1.0*par[3]))+TMath::Power((par[4]+par[5]*arg2+par[6]*arg2*arg2),(-1.0*par[3]))),(-1.0*1/par[3])))/1000.;
        return fit_val;
}

void gamma_efficiency()
{
    fill_CL_all();
    TGraph *gr_eff1 = new TGraph(8, Energy, CL_all[0]);
    gr_eff1-> Draw("A*");

    TF1 *eff_curve = new TF1("eff_curve",Fit1,100.,1500.,7); //option 7 for 7 parameterss
    eff_curve->SetParameter(0,5.);
    eff_curve->SetParameter(1,3.);
    eff_curve->SetParameter(2,-1.);
    eff_curve->SetParameter(3,7.);
    eff_curve->SetParameter(4,6.);
    eff_curve->SetParameter(5,-0.5);
    eff_curve->SetParameter(6,1.);
    gr_eff1->Fit("eff_curve","IER+");
    gr_eff1->SetMarkerStyle(4);
    gr_eff1->SetTitle("Efficiency Vs Energy(keV);Energy;Relative Efficiency");
    gStyle->SetOptFit(0111);  //showing the parameters on graph
    gr_eff1->Draw("AP");
    TText *t = new TText(Form("Clover 2"));
    Double_t eff_at_unknown_enrg; Double_t energy = 1112.0;
    eff_at_unknown_enrg= eff_curve->Eval(energy);
    cout<<"your eff. is: "<<eff_at_unknown_enrg<<endl;
}
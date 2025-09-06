#include <iostream>
#include <fstream>
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"

using namespace std;
void cosmic_track_breakdown()
//void IC_PL(int runNr)
{
  TChain* Chain=NULL ;
  Chain = new TChain("AD");
  Chain->Add(Form("../r0031_000a.root"));  

  Chain->SetBranchStatus("PlasticRaw",true);
  Chain->SetBranchStatus("PlasticRawTS",true);

  Short_t PlasticRaw[10];
  Long64_t PlasticRawTS[10];
    Float_t PL_thr = 10.;
  Long64_t PL_TS=-1000;
  Short_t PL_Q_off[10]={8320,8650,8290,8520,7800,8010,7850,7810,8770,8190};
  Double_t Qgain[10] = {3.3415,1.0,0.96738,2.27838,1.08208,1.58414,1.01826,1.784784,1.191078,1.89953};
  Double_t Qgm[10] = {};
  Double_t Q[5];
  Double_t Qr[5];
  Double_t cff_XQr[5]={9.12591,8.422069,8.201449,8.8511,6.7612};
  Double_t XQr[5];
  Double_t YPL[5]={8.0,4.0,0.0,-4.0,-8.0};


   TH2F *hpl = new TH2F("hpl", "PL energy",12,-1.5,10.5,6400,1,64001);
  TH2F *hpla = new TH2F("hpla", "PL energy all",12,-1.5,10.5,6400,1,64001);
  TH1F *hplm = new TH1F("hplm", "PL Mul",12,-1.5,10.5);
    TH2F *hpqr = new TH2F("hpqr", "PL energy ratio",5,-0.5,4.5,400,-2.0,2.0);
  TH2F *hpxqr = new TH2F("hpxqr", "PL X energy ratio",5,-0.5,4.5,400,-20.0,20.0);



    Chain->SetBranchAddress("PlasticRaw",&PlasticRaw);
  Chain->SetBranchAddress("PlasticRawTS",&PlasticRawTS);
  TF1 *f1 = new TF1("f1","pol1");
  TF1 *f2 = new TF1("f2","pol1");
  TF1 *f3 = new TF1("f3","pol1");
  TCanvas * c = new TCanvas("c", "c", 800, 800);
  Int_t nentries = Chain->GetEntries();
  cout << "TChain contains " << nentries << " events" << endl;
  
  double pourcentage;
  //      for (Int_t i = 0; i < 100000; i++) {
  for (Int_t i = 0; i < 10000; i++) {
    pourcentage = 100.*i/nentries ;
    if (i%100000 == 0) cout << "\rEntry " << i << Form("---- Entry processed : %.2f%%",pourcentage) << flush;
    Chain->GetEntry(i);    
    
    Int_t PL_M = 0;
    Int_t PL_all = 0; 
    for (Int_t j = 0; j < 10; j++) {
      if( PlasticRawTS[j] > 0 && PlasticRaw[j] > PL_thr ){
	hpl -> Fill(j,PlasticRaw[j]);
	PL_M++;
	Qgm[j] = (PlasticRaw[j]-PL_Q_off[j])*Qgain[j];
      }
    }
    cout<<"debug: the value of pl_m is: "<<PL_M<<"entry #: "<<i+1<<endl;
  

hplm -> Fill(PL_M);
if (PL_M==10){PL_all =1;}
if(PL_all == 1){
      for (Int_t j = 0; j < 10; j++) {
	hpla -> Fill(j,Qgm[j]);
      }
      TGraph *gr = new TGraph();

      gr->SetMarkerStyle(20);
      gr->SetMarkerSize(1);
      gr->SetMarkerColor(1);
      for (Int_t j = 0; j < 5; j++) {
	Q[j] = sqrt(Qgm[j]*Qgm[j+5]);
    Qr[j] = log(Qgm[j]/Qgm[j+5]);
    XQr[j] = cff_XQr[j] * Qr[j] *2;
    hpqr -> Fill(j,Qr[j]);
    hpxqr -> Fill(j,XQr[j]);
    gr -> SetPoint(j,XQr[j],YPL[j]); 
    }

    gr->Draw("AP");
    c->Update();
      c->WaitPrimitive();

      }}


  TFile *fout = new TFile(Form("./test_cosmic_breakdown.root"), "recreate");
  hpl->Write();
  hplm->Write();
  hpla->Write();
  fout->Close();
  hpqr->Write();
  hpxqr->Write();
  }
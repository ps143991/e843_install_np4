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
void cosmic_track()
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

//  Short_t PL_Q_off[10]={3929,4310,3934,4150,3758,3633,3589,3428,4464,91};
//  Double_t Qgain[10] = {0.6743207,0.4801790,0.7115280,0.4832440,0.6622707,1.1101672,0.2938989,0.3822262,0.8872156,0.5244723};
  
  TH1F *hplm = new TH1F("hplm", "PL Mul",12,-1.5,10.5);
  TH2F *hpl = new TH2F("hpl", "PL energy",12,-1.5,10.5,6400,1,64001);
  TH2F *hpla = new TH2F("hpla", "PL energy all",12,-1.5,10.5,6400,1,64001);
  TH2F *hpqr = new TH2F("hpqr", "PL energy ratio",5,-0.5,4.5,400,-2.0,2.0);
  TH2F *hpxqr = new TH2F("hpxqr", "PL X energy ratio",5,-0.5,4.5,400,-20.0,20.0);
  TH1F *hchi2 = new TH1F("hchi2", "chi2",2000,-1,199);
  TH1F *hpos = new TH1F("hpos", "position",500,-50,50);
  TH1F *hang = new TH1F("hang", "angle",500,-10,10);
  TH2F *hpa = new TH2F("hpa", "pos angle",200,-50,50,200,-10,10);

  TH2F *hcpos = new TH2F("hcpos", "pos chi2",200,-50,50,200,0.0,200.0);
  TH2F *hcang = new TH2F("hcang", "ang chi2",200,-10,10,200,0.0,200.0);

  TH2F *hqc0 = new TH2F("hqc0", "PL0 E chi2",200,0,5000,200,0.0,200.0);
  TH2F *hqc1 = new TH2F("hqc1", "PL1 E chi2",200,0,5000,200,0.0,200.0);
  TH2F *hqc2 = new TH2F("hqc2", "PL2 E chi2",200,0,5000,200,0.0,200.0);
  TH2F *hqc3 = new TH2F("hqc3", "PL3 E chi2",200,0,5000,200,0.0,200.0);
  TH2F *hqc4 = new TH2F("hqc4", "PL4 E chi2",200,0,5000,200,0.0,200.0);

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
  for (Int_t i = 0; i < nentries; i++) {
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

    hplm -> Fill(PL_M);
    if(PL_M == 10){
      PL_all = 1;
    }
      

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



	//	cout << Qr[j] << " " << PlasticRaw[j] << " " << PlasticRaw[j+5] << endl;
	//	cout << Qr[j] << " " << PlasticRaw[j] << " " << PlasticRaw[j+5] << endl;
      }

      gr -> Fit("f1","Q","");
      gr -> Draw("AP");
      Double_t pos = f1->GetParameter(0);
      Double_t angle = f1->GetParameter(1);
      Double_t chi2 = f1->GetChisquare();
      f2 -> SetRange(-20,20);
      f3 -> SetRange(-20,20);
      f2 -> SetParameters(pos+2,angle);
      f3 -> SetParameters(pos-2,angle);
      f2 -> Draw("same");
      f3 -> Draw("same");
      hpos -> Fill(pos);
      hang -> Fill(angle);
      hchi2 -> Fill(chi2);
      hpa -> Fill(pos,angle);

      hcpos -> Fill(pos,chi2);
      hcang -> Fill(angle,chi2);

      hqc0 -> Fill(Q[0],chi2);
      hqc1 -> Fill(Q[1],chi2);
      hqc2 -> Fill(Q[2],chi2);
      hqc3 -> Fill(Q[3],chi2);
      hqc4 -> Fill(Q[4],chi2);
      c->Update();
      c->WaitPrimitive();
      

    }


    
  }
        
  TFile *fout = new TFile(Form("./test_cosmic_charge.root"), "recreate");
  //  TFile *fout = new TFile(Form("./FileOut/DC_CATS_r%04d.root",runNr), "recreate");
  

  hpl->Write();
  hpla->Write();
  hplm->Write();
  hpqr->Write();
  hpxqr->Write();
  hpos->Write();
  hang->Write();
  hcpos->Write();
  hcang->Write();
  hpa->Write();
  hchi2->Write();
  hqc0->Write();
  hqc1->Write();
  hqc2->Write();
  hqc3->Write();
  hqc4->Write();
  fout->Write();

  // connect the TMust2Data branch                                                                                      
//  Chain->SetBranchStatus("MUST2",true);
//  //  Chain->SetBranchStatus("fMM*",true);                                                                              
//  TMust2Data *rawMust2 = new TMust2Data();
//  Chain->SetBranchAddress("MUST2",&rawMust2);


}

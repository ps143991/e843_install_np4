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
void kcosmic_track_PS()
//void IC_PL(int runNr)
{
  TChain* Chain=NULL ;
  Chain = new TChain("AD");
  Chain->Add(Form("./r0031_000a.root"));  

  Chain->SetBranchStatus("PlasticRaw",true);
  Chain->SetBranchStatus("PlasticRawTS",true);

  Chain->SetBranchStatus("TAC_PL_1",true);
  Chain->SetBranchStatus("TAC_PL_2",true);
  Chain->SetBranchStatus("TAC_PL_3",true);
  Chain->SetBranchStatus("TAC_PL_4",true);
  Chain->SetBranchStatus("TAC_PL_5",true);

  Short_t PlasticRaw[10];
  Long64_t PlasticRawTS[10];
  Float_t PL_thr = 10.;
  Long64_t PL_TS=-1000;
  Short_t TAC_PL_1;
  Short_t TAC_PL_2;
  Short_t TAC_PL_3;
  Short_t TAC_PL_4;
  Short_t TAC_PL_5;

  Short_t PL_Q_off[10]={8332,8660,8290,8537,7815,8020,7842,7810,8779,8205};         //fitting with gaussian and subtracting 3 sigmas    //8537 can be 
  Double_t Qgain[10] = {3.3415,1.0,0.96738,2.27838,1.08208,1.58414,1.01826,1.784784,1.191078,1.89953};
  Double_t Qgm[10] = {};

  Double_t Q[5];
  Double_t Qr[5];
  Double_t off_XQr[5]={-0.211832,0.259716,0.97132,-0.664074,0.148121};
  Double_t cff_XQr[5]={14.7462,15.9757,14.0053,15.221,13.1269};
//  Double_t cff_XQr[5]={9.12591,8.422069,8.201449,8.8511,6.7612};
//  Double_t off_XQr[5]={-2.88074e-01,6.98845e-02,7.09333e-01,-6.41932e-01,9.27271e-02};
  Double_t XQr[5];
  Double_t YPL[5]={8.0,4.0,0.0,-4.0,-8.0};

  Double_t TAC_cal[5];
  Double_t cff_Xt0[5]={2.24321e+04,2.29791e+04,2.30800e+04,2.18917e+04,2.26320e+04};
  Double_t cff_Xt1[5]={2.4078399e-02,2.639752919e-02,2.626510e-02,2.427714792e-02,2.3818219e-02};
  Double_t off_Xt[5]={5.35791e-02,-9.85831e-03,-9.71661e-02,1.49988e-02,3.67712e-02};


//  Short_t PL_Q_off[10]={3929,4310,3934,4150,3758,3633,3589,3428,4464,91};
//  Double_t Qgain[10] = {0.6743207,0.4801790,0.7115280,0.4832440,0.6622707,1.1101672,0.2938989,0.3822262,0.8872156,0.5244723};
  
  TH1F *hplm = new TH1F("hplm", "PL Mul",12,-1.5,10.5);
  TH2F *hpl = new TH2F("hpl", "PL energy",12,-1.5,10.5,6400,1,64001);
  TH2F *hpla = new TH2F("hpla", "PL energy all",12,-1.5,10.5,6400,1,64001);
  TH2F *hpqr = new TH2F("hpqr", "PL energy ratio",5,-0.5,4.5,400,-2.0,2.0);
  TH2F *hpxqr = new TH2F("hpxqr", "PL X energy ratio",5,-0.5,4.5,400,-20.0,20.0);
  TH1F *hchi2 = new TH1F("hchi2", "chi2 energy ",2000,-1,199);
  TH1F *hchi2t = new TH1F("hchi2t", "chi2 time",2000,-1,199);
  TH1F *hpos = new TH1F("hpos", "position energy",500,-50,50);
  TH1F *hang = new TH1F("hang", "angle energy",500,-10,10);
  TH2F *hpa = new TH2F("hpa", "pos angle energy",200,-50,50,200,-10,10);
  TH1F *hpost = new TH1F("hpost", "position time",500,-50,50);
  TH1F *hangt = new TH1F("hangt", "angle time",500,-10,10);
  TH2F *hpat = new TH2F("hpat", "pos angle time",200,-50,50,200,-10,10);

  TH2F *het1 = new TH2F("het1", "PL1 energy time",200,-20,20,200,-20,20);
  TH2F *het2 = new TH2F("het2", "PL2 energy time",200,-20,20,200,-20,20);
  TH2F *het3 = new TH2F("het3", "PL3 energy time",200,-20,20,200,-20,20);
  TH2F *het4 = new TH2F("het4", "PL4 energy time",200,-20,20,200,-20,20);
  TH2F *het5 = new TH2F("het5", "PL5 energy time",200,-20,20,200,-20,20);

  TH2F *hpp = new TH2F("hpp", "pos energy time",200,-50,50,200,-50,50);
  TH2F *haa = new TH2F("haa", "ang energy time",200,-10,10,200,-10,10);

  TH1F *hres = new TH1F("hres", "res energy",200,-20,20);
  TH1F *hrest = new TH1F("hrest", "res time",200,-20,20);

  TH2F *hresi = new TH2F("hresi", "res energy i",5,-0.5,4.5,200,-20,20);
  TH2F *hresti = new TH2F("hresti", "res time i",5,-0.5,4.5,200,-20,20);

  TH2F *hcpos = new TH2F("hcpos", "pos chi2",200,-50,50,200,0.0,200.0);
  TH2F *hcang = new TH2F("hcang", "ang chi2",200,-10,10,200,0.0,200.0);

  TH2F *hqc0 = new TH2F("hqc0", "PL0 E angle",200,0,5000,200,-2.0,2.0);
  TH2F *hqc1 = new TH2F("hqc1", "PL1 E angle",200,0,5000,200,-2.0,2.0);
  TH2F *hqc2 = new TH2F("hqc2", "PL2 E angle",200,0,5000,200,-2.0,2.0);
  TH2F *hqc3 = new TH2F("hqc3", "PL3 E angle",200,0,5000,200,-2.0,2.0);
  TH2F *hqc4 = new TH2F("hqc4", "PL4 E angle",200,0,5000,200,-2.0,2.0);

  TH1F *hplmt = new TH1F("hplmt", "PL Mul time",7,-1.5,5.5);
  TH2F *hplt = new TH2F("hplt", "PL time",7,-1.5,5.5,6400,1,64001);
  TH2F *hpxt = new TH2F("hpxt", "PL X time",5,-0.5,4.5,400,-20.0,20.0);

  Chain->SetBranchAddress("PlasticRaw",&PlasticRaw);
  Chain->SetBranchAddress("PlasticRawTS",&PlasticRawTS);

  Chain->SetBranchAddress("TAC_PL_1",&TAC_PL_1);
  Chain->SetBranchAddress("TAC_PL_2",&TAC_PL_2);
  Chain->SetBranchAddress("TAC_PL_3",&TAC_PL_3);
  Chain->SetBranchAddress("TAC_PL_4",&TAC_PL_4);
  Chain->SetBranchAddress("TAC_PL_5",&TAC_PL_5);

  TF1 *f1 = new TF1("f1","pol1");
  TF1 *f2 = new TF1("f2","pol1");
  TF1 *f3 = new TF1("f3","pol1");
  //  TCanvas * c = new TCanvas("c", "c", 800, 800);
  TCanvas * c = new TCanvas("c", "c", 800, 1600);
  c->Divide(1,2);
  Int_t nentries = Chain->GetEntries();
  cout << "TChain contains " << nentries << " events" << endl;
  TGraph *gr0 = new TGraph();
  gr0 -> SetPoint(0,-50,12); 
  gr0 -> SetPoint(1,50,-12); 
  
  double pourcentage;
  //      for (Int_t i = 0; i < 100000; i++) {
  for (Int_t i = 0; i < nentries; i++) {
    pourcentage = 100.*i/nentries ;
    if (i%100000 == 0) cout << "\rEntry " << i << Form("---- Entry processed : %.2f%%",pourcentage) << flush;
    Chain->GetEntry(i);    
    
    Double_t par_e[2]={-1000.,-1000.};
    Double_t par_t[2]={-1000.,-1000.};
    Int_t PL_M = 0;
    Int_t PL_Mt = 0;
    Int_t PL_all = 0;

    TAC_cal[0] = TAC_PL_1;
    TAC_cal[1] = TAC_PL_2;
    TAC_cal[2] = TAC_PL_3;
    TAC_cal[3] = TAC_PL_4;
    TAC_cal[4] = TAC_PL_5;

    for (Int_t j = 0; j < 5; j++) {
      if(TAC_cal[j]>0){
	hplt -> Fill(j,TAC_cal[j]);
	TAC_cal[j]  = (TAC_cal[j]-cff_Xt0[j])*cff_Xt1[j]/5 + off_Xt[j];
	PL_Mt++;
	//	hpxt-> Fill(j,TAC_cal[j]);
      }
    }

    if(PL_Mt==5){
      for (Int_t j = 0; j < 5; j++) {
	hpxt-> Fill(j,TAC_cal[j]);
      }
    }

    hplmt -> Fill(PL_Mt);
    
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
      

    if(PL_all == 1 && PL_Mt==5){
      for (Int_t j = 0; j < 10; j++) {
	hpla -> Fill(j,Qgm[j]);
      }
      for (Int_t j = 0; j < 5; j++) {
	Q[j] = sqrt(Qgm[j]*Qgm[j+5]);	
      }

      TGraph *gr = new TGraph();
      gr->SetMarkerStyle(20);
      gr->SetMarkerSize(2);
      gr->SetMarkerColor(1);
      Int_t npt = 0;
      for (Int_t j = 0; j < 5; j++) {
	if(abs(TAC_cal[j])<12){
	  gr -> SetPoint(npt,YPL[j],TAC_cal[j]); 
	  npt++;
	}	
      }
	//	cout << Qr[j] << " " << PlasticRaw[j] << " " << PlasticRaw[j+5] << endl;

      if(npt>2){
	gr -> Fit("f1","QR","");
	c->cd(1);
	gr0 -> GetXaxis()->SetRangeUser(-12,12);
	gr0 -> Draw("AP");
	f1 -> SetRange(-20,20);
	gr -> Draw("P");
	f1 -> Draw("same");
	Double_t post = f1->GetParameter(0);
	Double_t anglet = f1->GetParameter(1);
	Double_t chi2t = f1->GetChisquare();	

	par_t[0] = post;
	par_t[1] = anglet;

	for (Int_t k = 0; k < 5; k++) {
	  
	  if(abs(TAC_cal[k])<12){
	    Double_t x0 = YPL[k];
	    Double_t y0 = TAC_cal[k];
	    Double_t res = (anglet*x0 - y0 + post)/sqrt(anglet*anglet + 1);
	    hrest->Fill(res);
	    hresti->Fill(k,res);
	  }
	}
	hpost -> Fill(post);
	hangt -> Fill(anglet);
	hchi2t -> Fill(chi2t);
	hpat -> Fill(post,anglet);
	hqc0 -> Fill(Q[0],anglet);
	hqc1 -> Fill(Q[1],anglet);
	hqc2 -> Fill(Q[2],anglet);
	hqc3 -> Fill(Q[3],anglet);
	hqc4 -> Fill(Q[4],anglet);
      }
      
      TGraph *gr2 = new TGraph();
      gr2->SetMarkerStyle(22);
      gr2->SetMarkerSize(2);
      gr2->SetMarkerColor(1);
      Int_t np = 0;

      for (Int_t j = 0; j < 5; j++) {
	Qr[j] = log(Qgm[j]/Qgm[j+5]);	
	XQr[j] = cff_XQr[j] * Qr[j] + off_XQr[j] ;
	hpqr -> Fill(j,Qr[j]);
	hpxqr -> Fill(j,XQr[j]);
	
	if(abs(XQr[j])<12){
	  gr2 -> SetPoint(np,YPL[j],XQr[j]); 
	  np++;
	}	
      }
      
      het1->Fill(XQr[0],TAC_cal[0]);
      het2->Fill(XQr[1],TAC_cal[1]);
      het3->Fill(XQr[2],TAC_cal[2]);
      het4->Fill(XQr[3],TAC_cal[3]);
      het5->Fill(XQr[4],TAC_cal[4]);
//      het1->Fill(XQr[0],TAC_cal[0]);
//      het2->Fill(XQr[1],TAC_cal[1]);
//      het3->Fill(XQr[2],TAC_cal[2]);
//      het4->Fill(XQr[3],TAC_cal[3]);
//      het5->Fill(XQr[4],TAC_cal[4]);

      if(np>2){
	gr2 -> Fit("f2","QR","");
	c->cd(2);
	gr0 -> GetXaxis()->SetRangeUser(-12,12);
	gr0 -> Draw("AP");
	gr2 -> Draw("P");
	f2 -> SetRange(-20,20);
	f2 -> Draw("same");
	Double_t pos = f2->GetParameter(0);
	Double_t angle = f2->GetParameter(1);
	Double_t chi2 = f2->GetChisquare();

	par_e[0] = pos;
	par_e[1] = angle;

	for (Int_t k = 0; k < 5; k++) {
	  if(abs(XQr[k])<10.1){
	    Double_t x0 = YPL[k];
	    Double_t y0 = XQr[k];
	    Double_t res = (angle*x0 - y0 + pos)/sqrt(angle*angle + 1);
	    hres->Fill(res);
	    hresi->Fill(k,res);
	  }
	}
	hpos -> Fill(pos);
	hang -> Fill(angle);
	hchi2 -> Fill(chi2);
	hpa -> Fill(pos,angle);
      }


      if(par_e[0] > -1000. && par_t[0]){
	hpp->Fill(par_e[0],par_t[0]);
	haa->Fill(par_e[1],par_t[1]);
      }
      c->Update();
      //c->WaitPrimitive();
    }


    
  }
        
  TFile *fout = new TFile(Form("./PS_cosmic_by_energy.root"), "recreate");
  //  TFile *fout = new TFile(Form("./FileOut/DC_CATS_r%04d.root",runNr), "recreate");
  

  hpl->Write();
  hplt->Write();
  hpxt->Write();
  hpla->Write();
  hplm->Write();
  hplmt->Write();
  hpqr->Write();
  hpxqr->Write();
 
  hpp->Write();
  haa->Write();

  hpos->Write();
  hang->Write();
  hpost->Write();
  hangt->Write();

  hres->Write();
  hrest->Write();
  hresi->Write();
  hresti->Write();

  het1->Write();
  het2->Write();
  het3->Write();
  het4->Write();
  het5->Write();

  hcpos->Write();
  hcang->Write();
  hpa->Write();
  hchi2->Write();
  hpat->Write();
  hchi2t->Write();
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

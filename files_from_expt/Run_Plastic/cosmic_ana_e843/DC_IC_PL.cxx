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
void DC_IC_PL()
//void IC_PL(int runNr)
{
  TChain* Chain=NULL ;
  Chain = new TChain("AD");
  //  Chain->Add(Form("../RootA/r0536_000a.root"));  
//  Chain->Add(Form("../RootA/r0585_*.root"));  
//  Chain->Add(Form("../RootA/r0586_*.root"));  
//  Chain->Add(Form("../RootA/r0587_*.root"));  
//  Chain->Add(Form("../RootA/r0588_*.root"));  
//  Chain->Add(Form("../RootA/r0590_*.root"));  
//  Chain->Add(Form("../RootA/r0613_*.root"));  
//  Chain->Add(Form("../RootA/r0668_000a.root"));  
//  Chain->Add(Form("../RootA/r0679_000a.root"));  
//  Chain->Add(Form("../RootA/r1100_000a.root"));  
  Chain->Add(Form("../RootA/r1131_000a.root"));  

  Chain->SetBranchStatus("Xf",true);
  Chain->SetBranchStatus("Tf",true);
  Chain->SetBranchStatus("Yf",true);
  Chain->SetBranchStatus("Pf",true);
  
  Chain->SetBranchStatus("IC_ZDDRaw",true);
  Chain->SetBranchStatus("IC_ZDDRawTS",true);

  Chain->SetBranchStatus("PlasticRaw",true);
  Chain->SetBranchStatus("PlasticRawTS",true);

  Chain->SetBranchStatus("DC_ZDDRaw",true);
  Chain->SetBranchStatus("DC_ZDDRawTS",true);

  Chain->SetBranchStatus("DATATRIG_CATS1",true);
  Chain->SetBranchStatus("DATATRIG_CATS1TS",true);
  
  Chain->SetBranchStatus("CHIO_LISE_C",true);
  Chain->SetBranchStatus("T_CATSD4_HF_C",true);
  //  Chain->SetBranchStatus("TAC_CATS_PL_C",true);
//  Chain->SetBranchStatus("TAC_CATS_HF_C",true);

  Short_t IC_ZDDRaw[5];
  Short_t PlasticRaw[10];

  Long64_t IC_ZDDRawTS[5];
  Long64_t PlasticRawTS[10];

  Short_t DC_ZDDRaw[4];

  Long64_t DC_ZDDRawTS[4];

  Short_t DATATRIG_CATS1;
  Long64_t DATATRIG_CATS1TS;

  Float_t PL_thr = 100.;
  Float_t CHIO_LISE_C;
  Float_t T_CATSD4_HF_C;

  Float_t Xf;
  Float_t Yf;
  Float_t Tf;
  Float_t Pf;
  
  //  Short_t PL_Q_off[10]={3920,4338,3984,4162,3678,3830,3602,3464,4461,0};

  Short_t TAC_CATS_PL;
  Short_t TAC_CATS_HF;
    
  Long64_t PL_TS_R=-1000;
  Long64_t PL_TS_L=-1000;
  Long64_t PL_TS=-1000;

  Short_t PL_Q_off[10]={3929,4310,3934,4150,3758,3633,3589,3428,4464,91};
  Double_t Qgain[10] = {0.6743207,0.4801790,0.7115280,0.4832440,0.6622707,1.1101672,0.2938989,0.3822262,0.8872156,0.5244723};

  Double_t dc_cff[4] = {0.391901,-0.403834,-0.465996,0.414837};

  Double_t dc_off[4] = {-43.7747,42.9135,55.4839,-39.414};
  //  Double_t dc_off[4] = {-44.7747,46.9135,55.4839,-39.414};

  //  Double_t dc_off[4] = {-45.7747,47.9135,58.4839,-42.414};
//  Double_t x_off[2] = {-47.4595,44.1302};
//  Double_t y_off[2] = {51.1405,-36.6443}; 

  
  //  Double_t Qrgain[5] = {1.0,1.0,1.0,1.0,1.0};
  //  Double_t Qrgain[5] = { 1.56492e+00,7.03634e-01,7.14069e-01,1.62899e+00,7.79732e-01};

  
  TH1F *hplm = new TH1F("hplm", "PL Mul",12,-1.5,10.5);
  TH1F *hicm = new TH1F("hicm", "IC Mul",7,-1.5,5.5);  

  TH2F *hplq = new TH2F("hplq", "PL Q all",12,-1.5,10.5,3000,0,3000);
  TH2F *hplqm = new TH2F("hplqm", "PL Q lr",7,-1.5,5.5,3000,0,3000);
  TH2F *hplqr = new TH2F("hplqr", "PL Q ratio",7,-1.5,5.5,2000,-2,2);
  TH2F *hic = new TH2F("hic", "IC all",7,-1.5,5.5,2000,0,2000);

  TH2F *hede = new TH2F("hede", "IC vs PL",600,0,3000,400,0,8000);
  TH2F *hede0 = new TH2F("hede0", "IC vs PL0",600,0,3000,400,0,8000);
  TH2F *hede1 = new TH2F("hede1", "IC vs PL1",600,0,3000,400,0,8000);
  TH2F *hede2 = new TH2F("hede2", "IC vs PL2",600,0,3000,400,0,8000);
  TH2F *hede3 = new TH2F("hede3", "IC vs PL3",600,0,3000,400,0,8000);
  TH2F *hede4 = new TH2F("hede4", "IC vs PL4",600,0,3000,400,0,8000);

  TH2F *hede2k1 = new TH2F("hede2k1", "IC vs PL2 Si",600,0,3000,400,0,8000);
  TH2F *hede2k2 = new TH2F("hede2k2", "IC vs PL2 Al",600,0,3000,400,0,8000);

  TH2F *hplx0 = new TH2F("hplx0", "DCX0 vs PL r",400,-2,2,250,50,300);
  TH2F *hplx1 = new TH2F("hplx1", "DCX1 vs PL r",400,-2,2,250,50,300);
  TH2F *hply0 = new TH2F("hply0", "DCY0 vs PL id",5,-0.5,4.5,250,50,300);
  TH2F *hply1 = new TH2F("hply1", "DCY1 vs PL id",5,-0.5,4.5,250,50,300);  

  TH1F *hdcx0 = new TH1F("hdcx0", "DCX0",250,50,300);
  TH1F *hdcx1 = new TH1F("hdcx1", "DCX1",250,50,300);
  TH1F *hdcy0 = new TH1F("hdcy0", "DCY0",250,50,300);
  TH1F *hdcy1 = new TH1F("hdcy1", "DCY1",250,50,300);

  TH2F *hdcxy00 = new TH2F("hdcxy00", "DCX0 Y0",250,50,300,250,50,300);
  TH2F *hdcxy10 = new TH2F("hdcxy10", "DCX1 Y0",250,50,300,250,50,300);
  TH2F *hdcxy01 = new TH2F("hdcxy01", "DCX0 Y1",250,50,300,250,50,300);
  TH2F *hdcxy11 = new TH2F("hdcxy11", "DCX1 Y1",250,50,300,250,50,300);

  TH2F *hdcxy = new TH2F("hdcxy", "DCXY",200,-100,100,200,-100,100);
  TH2F *hdcxys1 = new TH2F("hdcxys1", "DCXY at S1",200,-100,100,200,-100,100);
  TH2F *hdcxypl = new TH2F("hdcxypl", "DCXY at PL",300,-150,150,300,-150,150);

  TH2F *hdcxpl = new TH2F("hdcxpl", "DCX at PL Qr",400,-2,2,600,-150,150);
  TH2F *hdcypl = new TH2F("hdcypl", "DCX at PL id",5,-0.5,4.5,600,-150,150);
  TH1F *hdcxplm = new TH1F("hdcxplm", "DCY at PL Qr mod",500,-30,30);
  TH1F *hdcyplm = new TH1F("hdcyplm", "DCY at PL id mod",600,-5,5);

  TH2F *hede0g = new TH2F("hede0g", "IC vs PL0 g",600,0,3000,400,0,8000);
  TH2F *hede1g = new TH2F("hede1g", "IC vs PL1 g",600,0,3000,400,0,8000);
  TH2F *hede2g = new TH2F("hede2g", "IC vs PL2 g",600,0,3000,400,0,8000);
  TH2F *hede3g = new TH2F("hede3g", "IC vs PL3 g",600,0,3000,400,0,8000);
  TH2F *hede4g = new TH2F("hede4g", "IC vs PL4 g",600,0,3000,400,0,8000);

  TH2F *hbpid = new TH2F("hbpid", "Beam PID",500,160,210,300,2,8);
  TH2F *hbpidk1 = new TH2F("hbpidk1", "Beam PID Si",500,160,210,300,2,8);
  TH2F *hbpidk2 = new TH2F("hbpidk2", "Beam PID Al",500,160,210,300,2,8);
  
  Chain->SetBranchAddress("Xf",&Xf);
  Chain->SetBranchAddress("Yf",&Yf);
  Chain->SetBranchAddress("Tf",&Tf);
  Chain->SetBranchAddress("Pf",&Pf);

  Chain->SetBranchAddress("IC_ZDDRaw",&IC_ZDDRaw);
  Chain->SetBranchAddress("IC_ZDDRawTS",&IC_ZDDRawTS);

  Chain->SetBranchAddress("PlasticRaw",&PlasticRaw);
  Chain->SetBranchAddress("PlasticRawTS",&PlasticRawTS);

  Chain->SetBranchAddress("DC_ZDDRaw",&DC_ZDDRaw);
  Chain->SetBranchAddress("DC_ZDDRawTS",&DC_ZDDRawTS);

  Chain->SetBranchAddress("DATATRIG_CATS1",&DATATRIG_CATS1);
  Chain->SetBranchAddress("DATATRIG_CATS1TS",&DATATRIG_CATS1TS);

  Chain->SetBranchAddress("CHIO_LISE_C",&CHIO_LISE_C);
  Chain->SetBranchAddress("T_CATSD4_HF_C",&T_CATSD4_HF_C);
  
//  Chain->SetBranchAddress("TAC_CATS_PL_C",&TAC_CATS_PL_C);
//  Chain->SetBranchAddress("TAC_CATS_HF_C",&TAC_CATS_HF_C);

  //   TCanvas * c = new TCanvas("c", "c", 800, 800);
  Int_t nentries = Chain->GetEntries();
  cout << "TChain contains " << nentries << " events" << endl;
  
  double pourcentage;
  //      for (Int_t i = 0; i < 100000; i++) {
        for (Int_t i = 0; i < nentries; i++) {
    pourcentage = 100.*i/nentries ;
    if (i%100000 == 0) cout << "\rEntry " << i << Form("---- Entry processed : %.2f%%",pourcentage) << flush;
    Chain->GetEntry(i);    

    //    cout << CHIO_LISE_C << " " << T_CATSD4_HF_C <<endl;
    hbpid->Fill(T_CATSD4_HF_C,CHIO_LISE_C);

    Int_t IC_M = 0;
    Int_t PL_M = 0;  
    Int_t ir = -1;
    Int_t il = -1;
    Double_t Qr = -1000.;
    Double_t Q = -1000.;  
  
    Int_t ix = -1;
    Int_t iy = -1;

    Double_t dt_x = -1.;
    Double_t dt_y = -1.;
    Double_t dl_x = -1.;
    Double_t dl_y = -1.;
    
    Double_t PL_R = -1000.;
    Double_t PL_L = -1000.;

    Double_t IC_sum = 0;

    Long64_t CATS_TS=-1000;
    
    for (Int_t j = 0; j < 5; j++) {
      if( PlasticRawTS[j] > 0 && PlasticRaw[j] > 0 &&  (PlasticRaw[j]-PL_Q_off[j])*Qgain[j] >=PL_thr ){
	PL_R = (PlasticRaw[j]-PL_Q_off[j])*Qgain[j];
	PL_TS_R=PlasticRawTS[j];
	if(PL_R>=PL_thr){
	  ir = j;
	  PL_M++;
	}
	hplq -> Fill(j,PL_R);
      }
    }
    for (Int_t j = 5; j < 10; j++) {
      if( PlasticRawTS[j] > 0 && PlasticRaw[j] > 0 && (PlasticRaw[j]-PL_Q_off[j])*Qgain[j] >=PL_thr ){
	PL_L = (PlasticRaw[j]-PL_Q_off[j])*Qgain[j];
	PL_TS_L = PlasticRawTS[j];
	if(PL_L>=PL_thr){
	  il = j;
	  PL_M++;
	}
	hplq -> Fill(j,PL_L);
      }
    }                  

    //  if(PL_M!=0){
//    if(PL_M==3){
//      //    if(PL_M!=2 && PL_M!=0){
//      cout <<  "Mul " << PL_M <<endl;
//      for (Int_t j = 0; j < 10; j++) {
//	if( (PlasticRaw[j]-PL_Q_off[j])*Qgain[j] > PL_thr){
//	  //	if( PlasticRawTS[j] > 0 && PlasticRaw[j] > 0 ){
//	  cout << j << " " << ( PlasticRaw[j]-PL_Q_off[j])*Qgain[j] << "   "<<  PlasticRaw[j] <<endl;
//	  
//	}
//      }
//    cout <<  "-------------" <<  endl;
//    }                  


    Int_t IC_tmp = 0;
    for (Int_t j = 0; j < 5; j++) {
      if( IC_ZDDRawTS[j] > 0 && IC_ZDDRaw[j] > 0 ){
	IC_M++;
	IC_sum += IC_ZDDRaw[j];
	hic ->Fill(j,IC_ZDDRaw[j]);
      }
    }

    if( PL_M == 2 && il - ir == 5 ){
      Q = sqrt(PL_R*PL_L);
      Qr = log(PL_R/PL_L);
      hplqm -> Fill(ir,Q);
      hplqr -> Fill(ir,Qr);
      if(IC_M==5){
	hede->Fill(Q,IC_sum);
	if(ir == 0){
	  hede0->Fill(Q,IC_sum);
	}
	if(ir == 1){
	  hede1->Fill(Q,IC_sum);
	}
	if(ir == 2){
	  hede2->Fill(Q,IC_sum);

	  //	  if(T_CATSD4_HF_C>189.5 && T_CATSD4_HF_C<191.5 && CHIO_LISE_C >4.4 && CHIO_LISE_C < 5.1){
	  if(T_CATSD4_HF_C>187.3 && T_CATSD4_HF_C<189.6 && CHIO_LISE_C >4.4 && CHIO_LISE_C < 5.1){
	    hbpidk1->Fill(T_CATSD4_HF_C,CHIO_LISE_C);
	    hede2k1->Fill(Q,IC_sum);
	  }
	  //	  else if(T_CATSD4_HF_C>192.2 && T_CATSD4_HF_C<193.9 && CHIO_LISE_C >4.0 && CHIO_LISE_C < 4.4){
	  else if(T_CATSD4_HF_C>190.1 && T_CATSD4_HF_C<191.9 && CHIO_LISE_C >3.8 && CHIO_LISE_C < 4.3){
	    hbpidk2->Fill(T_CATSD4_HF_C,CHIO_LISE_C);
	    hede2k2->Fill(Q,IC_sum);
	  }

	}
	if(ir == 3){
	  hede3->Fill(Q,IC_sum);
	}
	if(ir == 4){
	  hede4->Fill(Q,IC_sum);
	}
      }

    }
    hplm->Fill(PL_M); 
    hicm->Fill(IC_M);


    //    if(1){
    //    if( PL_M == 2 && il - ir == 5 ){
    if(DATATRIG_CATS1TS >0 ){
      CATS_TS = DATATRIG_CATS1TS;
      
      if( DC_ZDDRaw[0] > 0){
	ix = 0;
	dt_x = CATS_TS - DC_ZDDRawTS[0];
	hplx0->Fill(Qr,dt_x);
	hdcx0->Fill(dt_x);      
      }
      else if( DC_ZDDRaw[1] > 0){
	ix = 1;
	dt_x = CATS_TS - DC_ZDDRawTS[1];
	hplx1->Fill(Qr,dt_x);
	hdcx1->Fill(dt_x);      
      }    
      if( DC_ZDDRaw[2] > 0){
	iy = 0;
	dt_y = CATS_TS - DC_ZDDRawTS[2];
	hply0->Fill(ir,dt_y);
	hdcy0->Fill(dt_y);      
      }
      else if( DC_ZDDRaw[3] > 0){
	iy = 1;
	dt_y = CATS_TS - DC_ZDDRawTS[3];
	hply1->Fill(ir,dt_y);
	hdcy1->Fill(dt_y);      
      }
	//      }

      //      cout << ix << " " << iy << endl;
      if(ix == 0 && iy ==0){
	hdcxy00->Fill(dt_x,dt_y);      
      }
      else if(ix == 1 && iy ==0){
	hdcxy10->Fill(dt_x,dt_y);      
      }
      else if(ix == 0 && iy ==1){
	hdcxy01->Fill(dt_x,dt_y);      
      }
      else if(ix == 1 && iy ==1){
	hdcxy11->Fill(dt_x,dt_y);      
      }
      
      if(ix >= 0 && iy >=0){
        Double_t ran1 = rand() % 100000001;
        Double_t ran2 = rand() % 100000001;
        ran1 = ran1/100000000-0.5;
	ran2 = ran2/100000000-0.5;
	dt_x = dt_x + ran1;
	dt_y = dt_y + ran2;

	//	cout << dt_x << endl;

	dl_x = dt_x*dc_cff[ix] + dc_off[ix];
	dl_y = dt_y*dc_cff[iy+2] + dc_off[iy+2];
	hdcxy->Fill(dl_x,dl_y);

	if(Xf != 0.0 && Yf != 0.0){
	  Double_t a0x = Xf;
	  Double_t a0y = Yf;
	  Double_t a1x = (dl_x-Xf)/(880-0.0);
	  Double_t a1y = (dl_y-Yf)/(1130-0.0);
	  
	  Double_t X_PL = a0x + 1821.8 * a1x;
	  Double_t Y_PL = a0y + 1821.8 * a1y;
	  Double_t X_S1 = a0x + 382.4 * a1x;
	  Double_t Y_S1 = a0y + 382.4 * a1y;

	  hdcxys1->Fill(X_S1,Y_S1);
	  hdcxypl->Fill(X_PL,Y_PL);
	  hdcxpl->Fill(Qr,X_PL);
	  hdcypl->Fill(ir,Y_PL);

	  
	  Double_t X_PLm = X_PL-(-3.40459+Qr*180.284);
	  //	  hdcxplm->Fill(Qr,X_PLm);
	  hdcxplm->Fill(X_PLm);

	  Double_t Y_PLm = (Y_PL+(ir-2)*40-2.5)/22.5;
	  //	  hdcyplm->Fill(ir,Y_PLm);
	  hdcyplm->Fill(Y_PLm);

	  if(IC_M==5 && abs(X_PLm) <= 18 && abs(Y_PLm) <= 1){
	    if(ir == 0){
	      hede0g->Fill(Q,IC_sum);
	    }
	    if(ir == 1){
	      hede1g->Fill(Q,IC_sum);
	    }
	    if(ir == 2){
	      hede2g->Fill(Q,IC_sum);
	    }
	    if(ir == 3){
	      hede3g->Fill(Q,IC_sum);
	    }
	    if(ir == 4){
	      hede4g->Fill(Q,IC_sum);
	    }
	  }
	  
	  
	}
      }
      
    }    

    
  }
        
  TFile *fout = new TFile(Form("./IC_test10.root"), "recreate");
  //  TFile *fout = new TFile(Form("./FileOut/DC_CATS_r%04d.root",runNr), "recreate");
  
  hplm->Write(); 
  hicm->Write();

  hplq->Write();
  hplqm->Write();
  hplqr->Write();
  hic->Write();

  hede->Write();
  hede0->Write();
  hede1->Write();
  hede2->Write();
  hede3->Write();
  hede4->Write();

  hplx0->Write();
  hplx1->Write();
  hply0->Write();
  hply1->Write();

  hdcx0->Write();
  hdcx1->Write();
  hdcy0->Write();
  hdcy1->Write();

  hdcxy00->Write();
  hdcxy10->Write();
  hdcxy01->Write();
  hdcxy11->Write();
  hdcxy->Write();
  hdcxys1->Write();
  hdcxypl->Write();
  hdcxpl->Write();
  hdcxplm->Write();
  hdcypl->Write();
  hdcyplm->Write();
  
  hede0g->Write();
  hede1g->Write();
  hede2g->Write();
  hede3g->Write();
  hede4g->Write();

  hbpid->Write();

  hbpidk1->Write();
  hede2k1->Write();
  hbpidk2->Write();
  hede2k2->Write();

  fout->Write();

  // connect the TMust2Data branch                                                                                      
//  Chain->SetBranchStatus("MUST2",true);
//  //  Chain->SetBranchStatus("fMM*",true);                                                                              
//  TMust2Data *rawMust2 = new TMust2Data();
//  Chain->SetBranchAddress("MUST2",&rawMust2);


}

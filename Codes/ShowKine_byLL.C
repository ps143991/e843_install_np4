#include "/home/umugast/.local/nptool/default/physics-essential/sources/NPReaction.h"

using namespace nptool;

TCutG* cut_d=NULL;
TCutG* cut_p=NULL;
TCutG* cut_Target=NULL;
TCutG* cut_Si=NULL;
TCutG* cut_dEE=NULL;
TCutG* cut_34Si=NULL;
TCutG* cut_highZ=NULL;
TCutG* cut_38CaIn=NULL;
TCutG* cut_background=NULL;
TChain* chain=NULL ;
TCanvas* c1 = NULL;
TCanvas* c2 = NULL;
TCanvas* c3 = NULL;



void LoadChain(){
  chain = new TChain("PhysicsTree");

  chain->Add("../output/analysis/run_0120_0.root"); 
  chain->Add("../output/analysis/run_0123_0.root"); 
  chain->Add("../output/analysis/run_0124_0.root"); 
  chain->Add("../output/analysis/run_0125_0.root"); 
  chain->Add("../output/analysis/run_0131_0.root"); 
  chain->Add("../output/analysis/run_0135_0.root"); 
  chain->Add("../output/analysis/run_0136_0.root"); 
  chain->Add("../output/analysis/run_0137_0.root"); 
  chain->Add("../output/analysis/run_0138_0.root"); 
  chain->Add("../output/analysis/run_0140_0.root");
  chain->Add("../output/analysis/run_0141_0.root"); 
  chain->Add("../output/analysis/run_0142_0.root"); 
  chain->Add("../output/analysis/run_0143_0.root");
  chain->Add("../output/analysis/run_0144_0.root"); 
  chain->Add("../output/analysis/run_0145_0.root");


  cout << "Total Entry : " << chain->GetEntries() << endl;

}


////////////////////////////////////////////////////////////////////////////////
void LoadCuts(){

  //LOuis cuts
  
  TFile* File_cut_d = new TFile("./cut_d.root","READ");
  cut_d = (TCutG*) File_cut_d->FindObjectAny("cut_d");

  TFile* File_cut_p = new TFile("./cut_p.root","READ");
  cut_p = (TCutG*) File_cut_p->FindObjectAny("cut_p");


  TFile* File_cut_SiOut = new TFile("./cut_Si_ZDD.root","READ");
  cut_Si = (TCutG*) File_cut_SiOut->FindObjectAny("cut_Si");

  TFile* File_cut_highZ = new TFile("./cut_highZ.root","READ");
  cut_highZ = (TCutG*) File_cut_highZ->FindObjectAny("cut_highZ");


  TFile* File_cut_dEE = new TFile("./cut_dEE.root","READ");
  cut_dEE = (TCutG*) File_cut_dEE->FindObjectAny("cut_dEE");



  TFile* File_cut_34SiOut = new TFile("./cut_34Si.root","READ");
  cut_34Si = (TCutG*) File_cut_34SiOut->FindObjectAny("cut_34Si");


}

void ShowKine(){

	LoadChain();
	LoadCuts();
	
  //TCanvas *c = new TCanvas;
  //chain->Draw("ELab_corrected:ThetaLab>>h(200,0,60,200,0,50)","cut_d && abs(TAC_CATS_PL-9980)<100 && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
  
  //Reaction Si34pdReaction = Reaction("34Si(p,d)33Si@1700MeV"); // Take into a
	//Si34pdReaction.SetExcitationHeavy(0);
	//TGraph* Kine3 = Si34pdReaction.GetKinematicLine3(); 
	//Kine3->SetLineWidth(2);
	//Kine3->SetLineColor(kGreen-3);
	//Kine3->Draw("c same");
/*
  TCanvas *c1 = new TCanvas;
	chain->Draw("Ex[0]>>h1(100,-10,20)","cut_d && abs(TAC_CATS_PL-9980)<100 && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");

//  TCanvas *c3 = new TCanvas;
//	chain->Draw("ThetaCM>>h1(100,0,180)","abs(Ex)<2 && cut_d && cut_Si && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");


*/


  TCanvas *c1 = new TCanvas;
	chain->Draw("ICSum:PL_E>>h11(1000,20000,40000,1000,0,5000)","ICSumMult==4","colz");
  cut_dEE->Draw("same");

  TCanvas *c11 = new TCanvas;
	chain->Draw("ICSum:TAC_CATS_PL>>h(1000,5000,15000,10000,100,5000)","ICSumMult==4","colz");
  cut_Si->Draw("same");

  TCanvas *c2 = new TCanvas;
  c2->Divide(2,2);
  c2->cd(1);
	//chain->Draw("Ex[0]>>hh1(100,-10,20)","TelescopeNumber==1 && cut_d && cut_Si && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
	//chain->Draw("Ex[0]>>hh1(300,-10,20)","TelescopeNumber==1 && cut_d && abs(TAC_CATS_PL-9980)<60 && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
  chain->Draw("Ex[0]>>hh1(300,-10,20)","TelescopeNumber==1 && PL_E>30000 && PL_E<33000 && cut_d && abs(TAC_D4_CATS1_L-26365)<635 && abs(PL_TS-M2_TS-17)<8","colz");
  TH1F* hhh1 = (TH1F*) gDirectory->FindObjectAny("hh1");
  c2->cd(2);
	//chain->Draw("Ex[0]-0.5>>hh2(300,-10,20)","TelescopeNumber==2 && cut_d && abs(TAC_CATS_PL-9980)<60 && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
  chain->Draw("Ex[0]-0.5>>hh2(300,-10,20)","TelescopeNumber==2 && PL_E>30000 && PL_E<33000 && cut_d && abs(TAC_D4_CATS1_L-26365)<635 && abs(PL_TS-M2_TS-17)<8","colz");
  TH1F* hhh2 = (TH1F*) gDirectory->FindObjectAny("hh2");
  c2->cd(3);
	//chain->Draw("Ex[0]+2>>hh3(300,-10,20)","TelescopeNumber==3 && cut_d  && abs(TAC_CATS_PL-9980)<60 && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
  chain->Draw("Ex[0]+2>>hh3(300,-10,20)","TelescopeNumber==3 && PL_E>30000 && PL_E<33000 && cut_d && abs(TAC_D4_CATS1_L-26365)<635 && abs(PL_TS-M2_TS-17)<8","colz");
  TH1F* hhh3 = (TH1F*) gDirectory->FindObjectAny("hh3");
  c2->cd(4);
	//chain->Draw("Ex[0]+3>>hh4(300,-10,20)","TelescopeNumber==4 && cut_d  && abs(TAC_CATS_PL-9980)<60 && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
  chain->Draw("Ex[0]+3>>hh4(300,-10,20)","TelescopeNumber==4 && PL_E>30000 && PL_E<33000 && cut_d && abs(TAC_D4_CATS1_L-26365)<635 && abs(PL_TS-M2_TS-17)<8","colz");
  TH1F* hhh4 = (TH1F*) gDirectory->FindObjectAny("hh4");
  TCanvas *c3 = new TCanvas;

  TH1F* hhhh4 = new TH1F("hhhh4","hhhh4",300,-10,20);
  hhhh4->Add(hhh1);
  hhhh4->Add(hhh2);
  hhhh4->Add(hhh3);
  hhhh4->Add(hhh4);
  hhhh4->Draw();

/*
  TCanvas *c4 = new TCanvas;
	//chain->Draw("Exo_EAddBack:Ex>>hexoex(100,-10,20,1000,0,5000)","cut_d && abs(TAC_CATS_PL-9980)<100  && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
	chain->Draw("Exo_EAddBack:Ex>>hexoex(100,-10,20,1000,0,5000)","cut_d && Si_E@.size()==1","colz");
//	chain->Draw("Exo_EAddBack>>hexo(7000,0,7000)","","colz");

  TCanvas *c5 = new TCanvas;
	//chain->Draw("Exo_EAddBack>>hexo1(7000,0,7000)","cut_d && cut_Si && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
	chain->Draw("Exo_EAddBack>>hexo2(7000,0,7000)","cut_p &&  Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");


  TCanvas *c6 = new TCanvas;
	//chain->Draw("Ex[0]>>hgd(100,-10,20)","abs(Exo_EAddBack-1010)<5 && cut_d && cut_Si && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
	chain->Draw("Exo_EAddBack>>hexo1(7000,0,7000)","abs(Ex-4)<8 && cut_d && abs(TAC_CATS_PL-9980)<100 && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");

  //TCanvas *c7 = new TCanvas;
	//chain->Draw("Ex[0]>>hgd(100,-10,20)","abs(Exo_EAddBack-1010)<5 && cut_d && cut_Si && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
	//chain->Draw("Ex>>hEx(100,-10,20)","abs(Exo_EAddBack-1010)<10 && cut_d && abs(TAC_CATS_PL-9980)<100 && Si_E@.size()==1 && (M2_TS-PL_TS)<-5 && -25<(M2_TS-PL_TS)","colz");
*/

}

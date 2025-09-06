#include "TCanvas.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TMath.h"
#include "TGraph.h"
#include "TPaveText.h"
//#include "response_doubexp.h"
{
  Double_t r0;
  Double_t r1;
  Double_t r2;
  Double_t r3;
  Double_t r4;
  Double_t r5;
  Double_t r6;
  Double_t r7;
  Double_t r8;
  Double_t r9;
  Double_t r10;
  Double_t b10;
  Double_t r11;
  Double_t r12;
 // Double_t r10;
  Double_t rr1;
  Double_t rr2;
  Double_t rr3;
  Double_t rr4;
  Double_t rr5;
  Double_t rr6;
  Double_t rr7;
  Double_t rr8;
  Double_t rr9;
  Double_t rr10;
  Double_t er1;
  Double_t er2;
  Double_t er3;
  Double_t er4;
  Double_t er5;
  Double_t er6;
  Double_t er7;
  Double_t er8;
  Double_t er9;
  Double_t er10;
  Double_t m1;
 // gROOT->ProcessLine( ".L response_doubexp.h" );
  gStyle->SetOptFit(1111);
  //****************************************************************************
  
  TFile *peak1 = new TFile("./2dhgs.root");
  TH1F *hpeak1=new TH1F();
  h0t->ProjectionY("htemp",100,160);  
  htemp->Copy(*hpeak1);
  

  TFile *peak2 = new TFile("./2dh2278.root");
  TH1F *hpeak2=new TH1F();
  h0t->ProjectionY("htemp",144,186);
  htemp->Copy(*hpeak2);
  
   TFile *peak3 = new TFile("./2dh2555.root");
  TH1F *hpeak3=new TH1F();
  h0t->ProjectionY("htemp",144,186);
  htemp->Copy(*hpeak3);
  
  TFile *peak4 = new TFile("./2dh2878.root");
  TH1F *hpeak4=new TH1F();
  h0t->ProjectionY("htemp",144,186);
  htemp->Copy(*hpeak4);
   
  TFile *gamma = new TFile("../hAllthetamore.root");
  TH1F *hgamma=new TH1F("hEx","hEx",500,-5,20); 
  hgamma = (TH1F*)gamma->Get("ex"); 
  hgamma->GetXaxis()->SetTitle("Energy /MeV");
  hgamma->GetYaxis()->SetTitle("Counts/ 50 keV");
  
 
  //*****************************************************************************
TGraph *peak1g = new TGraph(hpeak1);
TGraph *peak2g = new TGraph(hpeak2);
TGraph *peak3g = new TGraph(hpeak3);
TGraph *peak4g = new TGraph(hpeak4);
gROOT->ProcessLine( ".L response_doubexp.h" );
  //****************************what is DSF ADD************************************************
  
 double fitmin= -5.0;
 double fitmax=10.0;
  TF1 *whole = new TF1("whole",land_res,-5,10,8);
  
  whole->SetParName(0,"pk1 amp");
  whole->SetParName(1,"pk1 offset");
  whole->SetParName(2,"pk2 amp.");
  whole->SetParName(3,"pk2 offset");
  whole->SetParName(4,"pk3 amp.");
  whole->SetParName(5,"pk3 offset");
  whole->SetParName(6,"pk4 amp.");
  whole->SetParName(7,"pk4 offset");
  

 whole->SetParameter(0,50); //peak1-170
 whole->SetParameter(1,0);//peak1-
 whole->SetParameter(2,50); //peak1-170
 whole->SetParameter(3,0);//peak1-
 whole->SetParameter(4,50); //peak2-120
 whole->SetParameter(5,0);//peak2-
 whole->SetParameter(6,50); //peak3-200
 whole->SetParameter(7,0);//peak3

whole->SetParLimits(0,0,5000); //peak1-170
whole->SetParLimits(1,-0.5,0.5);//peak1-

 whole->SetParLimits(2,0,3000);//peak1
 whole->SetParLimits(3,-0.5,0.5);//peak1
 whole->SetParLimits(4,0,3000);//peak2
 whole->SetParLimits(5,-0.5,0.5);//peak2
 whole->SetParLimits(6,0,3000);//peak3 
 whole->SetParLimits(7,-0.5,0.5);//peak3
 
   whole->SetLineColor( kBlue+2 );


 whole->SetLineWidth( 2 );
  whole->SetNpx(5000);
  rr1 = whole->GetParError(1);
  rr2 = whole->GetParError(3);
  rr3 = whole->GetParError(5);
  rr4 = whole->GetParError(7);
  
  TFitResultPtr r = hgamma->Fit(whole,"S ,R, E");
//     TFitResultPtr r = hgamma->Fit(whole,"VLER");
  TPaveStats *st = (TPaveStats*)hgamma->GetListOfFunctions()->FindObject("stats");
  
//  st->SetX1NDC(0.73);
//  st->SetY1NDC(0.12);
//  st->SetY2NDC(0.90);
//  st->SetX2NDC(1);
  
  gStyle->SetOptFit(1111);
//r->Print("V");
 

  
   whole->Draw("same");
   
  TF1 * peak1f= new TF1( "peak1f", resp1,-5,20,2);  
  peak1f->SetParameters(whole->GetParameter(0),whole->GetParameter(1) );
  peak1f->SetLineColor(kRed);
  peak1f->SetLineWidth(2);
  peak1f->SetLineStyle(1);
  r1=peak1f->Integral(-5,20);
  peak1f->Draw("same"); 
  peak1f->SetNpx(5000);
  //m1=whole->GetMean(6);
  gMinuit->GetParameter(1,rr1,er1);
  
  TF1 * peak2f= new TF1( "peak2f", resp2,10,1000,2);
  peak2f->SetParameters(whole->GetParameter(2), whole->GetParameter(3));
  peak2f->SetLineColor(kRed);
  peak2f->SetLineWidth(2);
  peak2f->SetLineStyle(1);
  r2=peak2f->Integral(0,1000);
   peak2f->Draw("same");
  //m2=peak2f->GetParameter("Mean");
  gMinuit->GetParameter(3,rr2,er2);
    peak2f->SetNpx(5000);
    
TF1 * peak3f= new TF1( "peak3f", resp3,100,1000,2);
  peak3f->SetParameters(whole->GetParameter(4),whole->GetParameter(5));
 peak3f->SetLineColor(kRed);
  peak3f->SetLineWidth( 2 );
  peak3f->SetLineStyle(1);
  peak3f->Draw("same");
  r3=peak3f->Integral(0,1000);
  //m3=peak3f->GetParameter("Mean");
  gMinuit->GetParameter(5,rr3,er3);
      peak3f->SetNpx(5000);
      
  TF1 * peak4f= new TF1( "peak4f", resp4,100,1000,2);
  peak4f->SetParameters(whole->GetParameter(6),whole->GetParameter(7));
  peak4f->SetLineColor(kRed);
  peak4f->SetLineWidth( 2 );
  peak4f->SetLineStyle(1);
  peak4f->Draw("same");
  r4=peak4f->Integral(0,1000);
  //m4=peak4f->GetParameter("Mean");
  gMinuit->GetParameter(7,rr4,er4);
                 peak4f->SetNpx(5000);

  b10=whole->GetChisquare();
  r11=whole->GetNDF();
  r12=b10/r11;

  printf("peak1f->Integral(0,2000)  = %g\n",r1);
  printf("peak1f->Offset = %g error = %g\n",rr1,er1);  
  printf("peak2f->Integral(0,2000)  = %g\n",r2);
  printf("peak2f->Offset = %g error = %g\n",rr2,er2); 
  printf("peak3f->Integral(0,2000)  = %g\n",r3);  
  printf("peak3f->Offset = %g error = %g\n",rr3,er3);
  printf("peak4f->Integral(0,2000)  = %g\n",r4);  
  printf("peak4f->Offset = %g error = %g\n",rr4,er4);
  printf("backg->Integral(min,max)  = %g\n",r0);  
  printf("whole->chi2 = %g\n",b10);
  printf("whole->ndf = %g\n",r11);
  printf("whole->chi2/ndf = %g\n",r12);
  //Draw
  TCanvas *ooo=new TCanvas("ooo","Fitting");

   ooo->SetFillColor(0);
  ostringstream os;
  os << "p1 No.= " << r1<<"/10kev"<<"   "<<"p2 No.="<<r2<<"/10kev"<<"   "<<"p3 No.= "<<r3<<"/10kev"<<"   "<<"p4 No.="<<r4<<"/10kev"<<"   "<<"range"<<fitmin<<" _ "<<fitmax;
  TPaveLabel *p = new TPaveLabel(0.05,0.05,0.95,0.13,os.str().data(),"br");
  p->SetFillColor(0);
  p->Draw();
  TPad *c1 = new TPad("c1", "c1",0.02,0.12,0.98,0.98);
  c1->Draw();
  c1->SetFillColor(0);
  c1->cd(); 
  
 

 //hgamman->Draw();
  hgamma->SetMarkerStyle(27);
  hgamma->Draw("E1, same");
  whole->Draw("same");

 

  peak1f->Draw("same");
  peak2f->Draw("same");
  peak3f->Draw("same");
  peak4f->Draw("same");
 
  TLegend *legend = new TLegend(0.28,0.90,0.55,0.63);
  legend->SetTextFont(32);
  legend->SetTextSize(0.04);
  legend->AddEntry(hgamma,"Exp_data","lpe");
  legend->AddEntry(peak1f,"p1-130kev","l");
  legend->AddEntry(peak2f,"p2-176kev","l"); 
  legend->AddEntry(peak3f,"p3-205kev","l");
  legend->AddEntry(peak4f,"p4-235kev","l");
  legend->Draw();
  ooo->SetLogy(0);
  
 //ooo->SaveAs("singles/fit_results/ppnwoAid144_land8_j15.jpg");
 ooo->SaveAs("testresponsefit.root");
 
}

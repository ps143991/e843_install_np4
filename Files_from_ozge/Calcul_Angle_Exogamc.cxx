

/* Compile as follow:

root -l
root [0] .L Calcul_Angle_Exogam.cxx++
root [1] AngleExogam(15) //will calculate the 3 first flange


*/

//#define MAURYCYFLAG  //this flag is important if the output will be used for the On line ECC.f code from Maurycy. Because I start phi from the top and he start
// from left middle of the array. This flag allows the translation

#include "Geometry_Clover_Exogam.h"

    TH2F *Map_clo = new TH2F ("Map_clo","Map_clo",50,0.,200.,100,-200.,200.);
    TH2F *Map_cris = new TH2F ("Map_cris","Map_cris",50,0.,200.,100,-200.,200.);
    TH2F *Map_seg = new TH2F ("Map_seg","Map_seg",50,0.,200.,100,-200.,200.);
    
    TH1F *seg = new TH1F ("seg","seg",400,0.,200.);
    TH1F *radius = new TH1F("radius","radius",200,0,200);

    TH2F *Map_cloC = new TH2F ("Map_cloC","Map_cloC",100,-500.,500.,100,-500.,500.);
    TH2F *Map_crisC = new TH2F ("Map_crisC","Map_crisC",100,-500.,500.,100,-500.,500.);
    TH2F *Map_segC = new TH2F ("Map_segC","Map_segC",100,-500.,500.,100,-500.,500.);
    
    TH3F *Map_cloC3 = new TH3F ("Map_cloC3","Map_cloC3",100,-500.,500.,100,-500.,500.,100,-500.,500.);
    TH3F *Map_crisC3 = new TH3F ("Map_crisC3","Map_crisC3",100,-500.,500.,100,-500.,500.,100,-500.,500.);
    TH3F *Map_segC3 = new TH3F ("Map_segC3","Map_segC3",200,-500.,500.,100,-500.,500.,200,-200.,200.);

void AngleExogam( int nb1, int nb2){
struct Clover_struc Gamma;
int i,j,k;
char name1[300];
char name2[300];
char name3[200];
char name4[200];
char name5[200];
char STOP[1];
TDatime date;
gSystem->Exec("rm Output_*");
sprintf(name1,"Output_Clover_Angle_%s.dat",date.AsString());
sprintf(name2,"Output_Crystal_Angle_%s.dat",date.AsString());
sprintf(name3,"Output_Segment_Angle_%s.dat",date.AsString());
sprintf(name4,"Output_Segment_Position_%s.dat",date.AsString());
sprintf(name5,"Output_Clover_Position_%s.dat",date.AsString());

FILE *clover;
clover = fopen(name1,"w");

FILE *crystal;
crystal = fopen(name2,"w");

FILE *crystalCartesien;
crystalCartesien = fopen(name5,"w");

FILE *segment;
segment = fopen(name3,"w");

FILE *segmentCartesien;
segmentCartesien = fopen(name4,"w");



printf("Output are %s %s %s \n",name1,name2,name3);
   	gROOT->Reset();
        gStyle->SetFrameFillColor(0);
        gStyle->SetFrameFillStyle(6);
        gStyle->SetFrameBorderMode(0);
        gStyle->SetFrameBorderSize(0);
        gStyle->SetCanvasBorderMode(0);
        gStyle->SetCanvasBorderSize(0);
        gStyle->SetCanvasColor(0);
        gStyle->SetPadColor(0);
        gStyle->SetPadBorderMode(0);
        gStyle->SetPadBorderSize(0);
        gStyle->SetOptLogz(0);
        gStyle->SetPalette(1,0);
	gStyle->SetOptStat(0);
    	gStyle->SetOptTitle(0);
    fprintf(segment,"!  2022\n");
    fprintf(segment,"!\n");
    fprintf(segment,"!\n");
    fprintf(segment,"!\n");
    fprintf(segment,"!\n");
    fprintf(segmentCartesien,"!  2022\n");
    fprintf(segmentCartesien,"!\n");
    fprintf(segmentCartesien,"! Seg 0 1 2 3\n");
    fprintf(segmentCartesien,"!  X Y Z -  X Y Z - etc ... in mm\n");
    fprintf(segmentCartesien,"!  \n");
    if(nb1<1 || nb1>17 || nb2<1 || nb2>17 || nb1>nb2){
    	printf("problem");
	//gets(STOP);
    
    }
	// MAURYCY ANGLE TRANSLATE
	for(k=nb1;k<=nb2;k++){
		
		Gamma=Ask_For_Angles(k);
		
		
		Map_clo->Fill(Gamma.Theta_Clover*180.0/(TMath::Pi()),Gamma.Phi_Clover*180.0/(TMath::Pi()));
		Map_cloC->Fill(Gamma.Z_Clover,-1.0*Gamma.Y_Clover); // the -1 is just for display convention
			
			
			#ifdef MAURYCYFLAG
			if(Gamma.Phi_Clover*180.0/(TMath::Pi())>=0){Gamma.Phi_Clover=Gamma.Phi_Clover*180.0/(TMath::Pi())+90.;}
			else if(Gamma.Phi_Clover*180.0/(TMath::Pi())<0){Gamma.Phi_Clover=Gamma.Phi_Clover*180.0/(TMath::Pi())+90.;}
			#endif
			
			#ifndef MAURYCYFLAG
			Gamma.Phi_Clover=Gamma.Phi_Clover*180.0/(TMath::Pi());
			#endif
			
			fprintf(clover," %f %f !Flange %d \n",Gamma.Theta_Clover*180.0/(TMath::Pi()),Gamma.Phi_Clover,k );
		      fprintf(crystalCartesien," %f %f !flange%d \n",Gamma.X_Clover,Gamma.Y_Clover,k );
		
		for(i=0;i<4;i++){
			Map_cris->Fill(Gamma.Theta_Crystal[i]*180.0/(TMath::Pi()),Gamma.Phi_Crystal[i]*180.0/(TMath::Pi()));
			Map_crisC->Fill(Gamma.Z_Crystal[i],Gamma.Y_Crystal[i]*-1.);
			
			#ifdef MAURYCYFLAG
			if(Gamma.Phi_Crystal[i]*180.0/(TMath::Pi())>=0){Gamma.Phi_Crystal[i]=Gamma.Phi_Crystal[i]*180.0/(TMath::Pi())+90.;}
			else if(Gamma.Phi_Crystal[i]*180.0/(TMath::Pi())<0){Gamma.Phi_Crystal[i]=Gamma.Phi_Crystal[i]*180.0/(TMath::Pi())+90.;}
			#endif
			
			#ifndef MAURYCYFLAG
			Gamma.Phi_Crystal[i]=Gamma.Phi_Crystal[i]*180.0/(TMath::Pi());
			#endif
			
			fprintf(crystal," %f %f !flange%d \n",Gamma.Theta_Crystal[i]*180.0/(TMath::Pi()),Gamma.Phi_Crystal[i],k );
			//fprintf(crystalCartesien," %f %f !flange%d \n",Gamma.X_Crystal[i],Gamma.Y_Crystal[i],k );
			
			for(j=0;j<4;j++){
				seg->Fill(Gamma.Theta_Crystal_Seg[i][j]*180.0/(TMath::Pi()));
				Map_seg->Fill(Gamma.Theta_Crystal_Seg[i][j]*180.0/(TMath::Pi()),Gamma.Phi_Crystal_Seg[i][j]*180.0/(TMath::Pi()));
				
				#ifdef MAURYCYFLAG
				if(Gamma.Phi_Crystal_Seg[i][j]*180.0/(TMath::Pi())>=0){Gamma.Phi_Crystal_Seg[i][j]=Gamma.Phi_Crystal_Seg[i][j]*180.0/(TMath::Pi())+90.;}
				else if(Gamma.Phi_Crystal_Seg[i][j]*180.0/(TMath::Pi())<0){Gamma.Phi_Crystal_Seg[i][j]=Gamma.Phi_Crystal_Seg[i][j]*180.0/(TMath::Pi())+90.;}
				#endif
				
				#ifndef MAURYCYFLAG
				Gamma.Phi_Crystal_Seg[i][j]=Gamma.Phi_Crystal_Seg[i][j]*180.0/(TMath::Pi());
				#endif
				
				fprintf(segment," %f %f  ",Gamma.Theta_Crystal_Seg[i][j]*180.0/(TMath::Pi()),Gamma.Phi_Crystal_Seg[i][j]);
				fprintf(segmentCartesien," %f %f %f ",Gamma.X_Crystal_Seg[i][j],Gamma.Y_Crystal_Seg[i][j],Gamma.Z_Crystal_Seg[i][j]);
				
				Map_segC->Fill(Gamma.Z_Crystal_Seg[i][j],Gamma.Y_Crystal_Seg[i][j]*-1.0);
				Map_segC3->Fill(Gamma.Z_Crystal_Seg[i][j],Gamma.Y_Crystal_Seg[i][j]*-1.0,Gamma.X_Crystal_Seg[i][j],1);
				cout<< k <<" "<< Gamma.X_Crystal_Seg[i][j]<<" " <<Gamma.Y_Crystal_Seg[i][j]<<" " <<Gamma.Z_Crystal_Seg[i][j]<<endl;
				radius->Fill(sqrt(pow(Gamma.Z_Crystal_Seg[i][j],2)+pow(Gamma.Y_Crystal_Seg[i][j],2)+pow(Gamma.X_Crystal_Seg[i][j],2)));
			}
			fprintf(segment,"!--> flange %d \n",k);
			fprintf(segmentCartesien,"!--> flange %d \n",k);
		}
	}
	
	
	
	
	
	TCanvas *can2=new TCanvas("EXOGAM","EXOGAM",150,150,900,700);
	can2->Divide(2,2);
	can2->cd(1);
    	Map_clo->Draw("COLZ");
	Map_clo->GetXaxis()->SetTitle("Theta lab") ;
	Map_clo->GetYaxis()->SetTitle("Projected phi lab") ;
	can2->cd(2);
	seg->Draw();
	seg->GetXaxis()->SetTitle("Theta lab") ;
 	can2->cd(3);
   	Map_cris->Draw("COLZ");
	Map_cris->GetXaxis()->SetTitle("Theta lab") ;
	Map_cris->GetYaxis()->SetTitle("Projected phi lab") ;
 	can2->cd(4);
   	Map_seg->Draw("COLZ");
  	Map_seg->GetXaxis()->SetTitle("Theta lab") ;
	Map_seg->GetYaxis()->SetTitle("Projected phi lab") ;
	
	TCanvas *can1=new TCanvas("EXOGAMC","EXOGAMC",310,310,910,710);
	can1->Divide(2,2);
	can1->cd(1);
	Map_cloC->Draw("COLZ");
	Map_cloC->GetXaxis()->SetTitle("Beam Axis") ;
	Map_cloC->GetYaxis()->SetTitle("Horizontal plane") ;
	can1->cd(2);
	radius->Draw("");
 	can1->cd(3);
   	Map_crisC->Draw("COLZ");
	Map_crisC->GetXaxis()->SetTitle("Beam Axis") ;
	Map_crisC->GetYaxis()->SetTitle("Horizontal plane") ;
 	can1->cd(4);
   	Map_segC->Draw("COLZ");
  	Map_segC->GetXaxis()->SetTitle("Beam Axis") ;
	Map_segC->GetYaxis()->SetTitle("Horizontal plane") ;
	 TArrow *arrow = new TArrow(0.,0.,400.,0,0.02,">");
	   // TLatex *   tex = new TLatex(-328.6015,137.5647,"Fl1");
 /*  tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(-104.7417,296.463,"Fl2");
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(149.9244,246.4395,"Fl3");
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(264.9349,114.0242,"Fl4");
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(283.1241,-140.1948,"Fl5");
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(32.86015,-336.1876,"Fl6");
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(-250.5586,-268.5087,"FL7");
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(-346.3599,-151.7177,"FL8");
   tex->SetLineWidth(2);
   tex->Draw();
	*/ 
	 
	 
	 
   	arrow->SetFillColor(1);
   	arrow->SetFillStyle(1001);
	arrow->Draw();
		TCanvas *can3=new TCanvas("EXOGAMCs","EXOGAMCs",110,110,910,710);
		can3->cd(1);
		Map_segC3->Draw("box");
	Map_segC3->GetXaxis()->SetTitle("Beam Axis") ;
	Map_segC3->GetXaxis()->SetTitleOffset(1.5);
	Map_segC3->GetYaxis()->SetTitleOffset(1.5);
	Map_segC3->GetZaxis()->SetTitleOffset(1.5);
	Map_segC3->GetZaxis()->SetTitle("Altitude") ;
	Map_segC3->GetYaxis()->SetTitle("Dispersive plan") ;
	
	 
	fclose(clover);
	fclose(crystal);
fclose(crystalCartesien);
	fclose(segment); 
	fclose(segmentCartesien);
	


}

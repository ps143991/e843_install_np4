#include "response_fitPS.h"
#include "TFile.h"
#include "TH1.h"
#include "TFractionFitter.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"

TH1F* peak1g = nullptr;
TH1F* peak2g = nullptr;
TH1F* peak3g = nullptr;
TH1F* peak4g = nullptr;
TH1F* peak5g = nullptr;
TH1F* peak6g = nullptr;
TH1F* bc = nullptr;

using namespace std;
void response_fitPS_v1()
{
    const int angmin = 130;
    const int angmax = 140;
    const int projXmin = angmin - 100;
    const int projXmax = angmax - 100;

    TFile* file1 = TFile::Open("simulation9mm/hhgs.root");
    TFile* file2 = TFile::Open("simulation9mm/hh915.root");
    TFile* file3 = TFile::Open("simulation9mm/hh1816.root");
    TFile* file4 = TFile::Open("simulation9mm/hh2552.root");
    TFile* file5 = TFile::Open("simulation9mm/hh2880.root");
    TFile* file6 = TFile::Open("simulation9mm/hh4000.root");
    TFile* datafile = TFile::Open("../hAllthetamore4kev.root");
    TFile* carbonfile = TFile::Open("../hAllthetamore4kev_carbon2.root");
    

    if (!datafile || datafile->IsZombie()) {
        printf("Error: Data ROOT file could not be opened.\n");
        return;
    }
    if (!carbonfile || carbonfile->IsZombie()) {
        printf("Error: Carbon ROOT file could not be opened.\n");
        return;
    }
    TH2F* hhdata = (TH2F*)datafile->Get("extheta");
    if (!hhdata) {
        printf("Error: 2D Data histogram not found.\n");
        return;
    }

    TH2F* carbon_2d = (TH2F*)carbonfile->Get("extheta");
    if (!carbon_2d) {
        printf("Error: 2D Carbon histogram not found.\n");
        return;
    }

    if (!file1 || file1->IsZombie() || !file2 || file2->IsZombie() ||
        !file3 || file3->IsZombie() || !file4 || file4->IsZombie() ||
        !file5 || file5->IsZombie() || !file6 || file6->IsZombie() ) {
        printf("Error: One or more response ROOT files could not be opened.\n");
        return;
    }
    TH1F* carbon = (TH1F*)carbonfile->Get("ex");
    TH2F* hh1 = (TH2F*)file1->Get("hh0");
    TH2F* hh2 = (TH2F*)file2->Get("hh0");
    TH2F* hh3 = (TH2F*)file3->Get("hh0");
    TH2F* hh4 = (TH2F*)file4->Get("hh0");
    TH2F* hh5 = (TH2F*)file5->Get("hh0");
    TH2F* hh6 = (TH2F*)file6->Get("hh0");
    TH1F* h_data = new TH1F();
    TH1F* carbon_2d_projected = new TH1F();
    hhdata->ProjectionY("hdatatemp", projXmin, projXmax)->Copy(*h_data);
    carbon_2d->ProjectionY("carbon_temp", projXmin, projXmax)->Copy(*carbon_2d_projected);
    //h_data ->SetLineColor(kBlack+2);
    //h_data ->Draw();

    //added 2d histograms

    peak1g = new TH1F(); hh1->ProjectionY("htemp1", projXmin, projXmax)->Copy(*peak1g); peak1g->Rebin(2); peak1g->Smooth(2);
    peak2g = new TH1F(); hh2->ProjectionY("htemp2", projXmin, projXmax)->Copy(*peak2g); peak2g->Rebin(2); peak2g->Smooth(2);
    peak3g = new TH1F(); hh3->ProjectionY("htemp3", projXmin, projXmax)->Copy(*peak3g); peak3g->Rebin(2); peak3g->Smooth(2);
    peak4g = new TH1F(); hh4->ProjectionY("htemp4", projXmin, projXmax)->Copy(*peak4g); peak4g->Rebin(2); peak4g->Smooth(2);
    peak5g = new TH1F(); hh5->ProjectionY("htemp5", projXmin, projXmax)->Copy(*peak5g); peak5g->Rebin(2); peak5g->Smooth(2);
    peak6g = new TH1F(); hh6->ProjectionY("htemp6", projXmin, projXmax)->Copy(*peak6g); peak6g->Rebin(2); peak6g->Smooth(2);

    //copied the projection of simulated histograms and smoothed them
/* 
    fit_rangeMIN = -2;
    fit_rangeMAX = 5; */
    TCanvas* c2 = new TCanvas("c2", "carbon", 800, 600);
    // carbon->Rebin(7);
    // carbon->Draw("");
    carbon_2d_projected->Rebin(7);
    carbon_2d_projected->Draw();

    TF1* landau_carbon = new TF1("landau_carbon", left_skewed_landau, -5, 15, 3);
    landau_carbon->SetParameters(14, 8, 1.2);
    carbon_2d_projected->Fit(landau_carbon, "R");
    landau_carbon->SetLineColor(kRed+1);
    landau_carbon->Draw("same");
    
    TF1* carbon_func = new TF1("carbon_func", left_skewed_landau, -5, 15, 3);
    carbon_func->SetParameters(landau_carbon->GetParameter(0), landau_carbon->GetParameter(1), landau_carbon->GetParameter(2));
    carbon_func->SetLineColor(kGreen+ 1);
    carbon_func->Draw("same");



    TF1* whole = new TF1("f1", doubexp, fit_rangeMIN, fit_rangeMAX, 15);
    // TF1* whole = new TF1("f1",Res_carbon , -5, -0.5, 2);


    whole->SetParName(0,"pk1 amp");
    whole->SetParName(1,"pk1 offset");
    whole->SetParName(2,"pk2 amp.");
    whole->SetParName(3,"pk2 offset");
    whole->SetParName(4,"pk3 amp.");
    whole->SetParName(5,"pk3 offset");
    whole->SetParName(6,"pk4 amp.");
    whole->SetParName(7,"pk4 offset");
    whole->SetParName(8,"pk5 amp.");
    whole->SetParName(9,"pk5 offset");
    whole->SetParName(10,"pk6 amp.");
    whole->SetParName(11,"pk6 offset");
    whole->SetParName(12,"bckg amplitude");
    whole->SetParName(13,"MPV");
    whole->SetParName(14,"sigma");

    //the fit function is made of interpolated peaks from simulation
    for (int i = 0; i < 12; i += 2) {
    whole->SetParameter(i,500);
    whole->SetParameter(i + 1,0);
    whole->SetParLimits(i, 0, 50000);
    whole->SetParLimits(i + 1, -0.5, 0.2);
    }                                       
/*     whole->FixParameter(12, 140);          // background amplitude                   //parameters for angle 130 to 180
    whole->FixParameter(13, 8);          // MPV
    whole->FixParameter(14, 1.2);             // sigma */
    //set the parameters of the fit function  

    //whole->SetParameter(12, 17.4);        // background amplitude
    //whole->FixParameter(13, 7.32);        // MPV
    //whole->FixParameter(14, 4);           // sigma
    whole->SetLineColor(kBlack );
    // whole->SetFillColor(kBlack);
    // whole->SetFillStyle(3003);
    whole->SetLineWidth( 2 );
    whole->SetLineStyle(2);
    whole->SetNpx(5000);
    h_data->SetLineColor(kGray+1);
    TCanvas* c3 = new TCanvas("c3", "Fit result", 800, 600);
    //gStyle->SetOptStat(1);
    //gStyle->SetOptFit(1);
    h_data->Fit(whole,"R");             // fitting happens here
    whole->Draw("same");




    /***********************************************************************************************************/
    /***********************************************************************************************************/
    /********************************************Drawing functions on TOP***************************************/
    /***********************************************************************************************************/
    /***********************************************************************************************************/



    TF1* bcf = new TF1("bcf", left_skewed_landau, fit_rangeMIN, 12, 3);
    bcf->SetParameters(whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14) );
    bcf->SetLineColor(kRed+ 1);
    bcf->Draw("same");

    ///////////////////////////////Drawing fit functions from baseline////////////////////////////////////

    TF1* peak1f = new TF1("peak1f", resp1, fit_rangeMIN, fit_rangeMAX, 2);
    peak1f->SetParameters(whole->GetParameter(0), whole->GetParameter(1));
    peak1f->SetLineColor(kYellow);
    peak1f->SetFillColor(kYellow);
    peak1f->SetFillStyle(3003);
    //peak1f->Draw("same");    

    TF1* peak2f = new TF1("peak2f", resp2, fit_rangeMIN, fit_rangeMAX, 2);
    peak2f->SetParameters(whole->GetParameter(2), whole->GetParameter(3));
    peak2f->SetLineColor(kGreen);
    peak2f->SetFillColor(kGreen);
    peak2f->SetFillStyle(3003);
    //peak2f->Draw("same");//

    TF1* peak3f = new TF1("peak3f", resp3, fit_rangeMIN, fit_rangeMAX, 2);
    peak3f->SetParameters(whole->GetParameter(4), whole->GetParameter(5));
    peak3f->SetLineColor(kBlue);
    peak3f->SetFillColor(kBlue);
    peak3f->SetFillStyle(3003);
    // peak3f->Draw("same");

    TF1* peak4f = new TF1("peak4f", resp4, fit_rangeMIN, fit_rangeMAX, 2);
    peak4f->SetParameters(whole->GetParameter(6), whole->GetParameter(7));
    peak4f->SetLineColor(kOrange+6);
    peak4f->SetFillColor(kOrange + 6);
    peak4f->SetFillStyle(3003);
    // peak4f->Draw("same");

    TF1* peak5f = new TF1("peak5f", resp5, fit_rangeMIN, fit_rangeMAX, 2);
    peak5f->SetParameters(whole->GetParameter(8), whole->GetParameter(9));
    peak5f->SetLineColor(kViolet + 3);    
    peak5f->SetFillColor(kViolet + 3);
    peak5f->SetFillStyle(3003);
    // peak5f->Draw("same");

    TF1* peak6f = new TF1("peak6f", resp6, fit_rangeMIN, fit_rangeMAX, 2);
    peak6f->SetParameters(whole->GetParameter(10), whole->GetParameter(11));
    peak6f->SetLineColor(kCyan + 1);
    peak6f->SetFillColor(kCyan + 1);
    peak6f->SetFillStyle(3003);
    // peak6f->Draw("same");


    ///////////////////////////////Drawing fit functions on background////////////////////////////////////

    peak1ft->SetParameters(whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak1ft->SetLineColor(kMagenta);
    peak1ft->SetFillColor(kMagenta);
    peak1ft->SetFillStyle(0); // No fill, just line
    peak1ft->Draw("same");

    peak2ft->SetParameters(whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak2ft->SetLineColor(kGreen);
    peak2ft->SetFillColor(kGreen);
    peak2ft->SetFillStyle(0); // No fill, just line
    peak2ft->Draw("same");

    peak3ft->SetParameters(whole->GetParameter(4), whole->GetParameter(5), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak3ft->SetLineColor(kBlack);
    peak3ft->SetFillColor(kBlack);
    peak3ft->SetFillStyle(0); // No fill, just line
    peak3ft->Draw("same");

    peak4ft->SetParameters(whole->GetParameter(6), whole->GetParameter(7), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak4ft->SetLineColor(kBlue);
    peak4ft->SetFillColor(kBlue);
    peak4ft->SetFillStyle(0); // No fill, just line
    peak4ft->Draw("same");

    peak5ft->SetParameters(whole->GetParameter(8), whole->GetParameter(9), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak5ft->SetLineColor(kOrange+6);
    peak5ft->SetFillColor(kOrange+6);
    peak5ft->SetFillStyle(0); // No fill, just line
    peak5ft->Draw("same");

    peak6ft->SetParameters(whole->GetParameter(10), whole->GetParameter(11), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak6ft->SetLineColor(kCyan + 1);
    peak6ft->SetFillColor(kCyan + 1);
    peak6ft->SetFillStyle(0); // No fill, just line
    peak6ft->Draw("same");

    
    // Extract integrals using histogram method

    double r1 = peak1g->Integral(peak1g->FindBin(-5 - whole->GetParameter(1)), peak1g->FindBin(20 - whole->GetParameter(1))) * whole->GetParameter(0) / 10000.0;
    double r2 = peak2g->Integral(peak2g->FindBin(-5 - whole->GetParameter(3)), peak2g->FindBin(20 - whole->GetParameter(3))) * whole->GetParameter(2) / 10000.0;
    double r3 = peak3g->Integral(peak3g->FindBin(-5 - whole->GetParameter(5)), peak3g->FindBin(20 - whole->GetParameter(5))) * whole->GetParameter(4) / 10000.0;
    double r4 = peak4g->Integral(peak4g->FindBin(-5 - whole->GetParameter(7)), peak4g->FindBin(20 - whole->GetParameter(7))) * whole->GetParameter(6) / 10000.0;
    double r5 = peak5g->Integral(peak5g->FindBin(-5 - whole->GetParameter(9)), peak5g->FindBin(20 - whole->GetParameter(9))) * whole->GetParameter(8) / 10000.0;
    double r6 = peak6g->Integral(peak6g->FindBin(-5 - whole->GetParameter(11)), peak6g->FindBin(20 - whole->GetParameter(11))) * whole->GetParameter(10) / 10000.0;
  
    // Output integrals
    printf("Integrals: R1=%.2f, R2=%.2f, R3=%.2f, R4=%.2f, R5=%.2f , R6=%.2f\n", r1, r2, r3, r4, r5, r6);

    // Save fit parameters and integrals to CSV
        FILE* csv = fopen(Form("csv/fit_results NC_ang%dto%d.csv", angmin, angmax), "w");
        if (csv) {
            fprintf(csv, "Parameter,Value");
            for (int i = 0; i < 18; ++i) {
                fprintf(csv, "par[%d],%f", i, whole->GetParameter(i));
            }
            fprintf(csv, "Integral,R1,R2,R3,R4,R5,R6");
            fprintf(csv, "Integral,%f,%f,%f,%f,%f,%f", r1, r2, r3, r4, r5, r6);
            fclose(csv);
        } else {
       printf("Error: Could not write CSV file.");
        }
/* 	 TLegend* legend = new TLegend(0.15, 0.45, 0.45, 0.95);
          legend->SetHeader("Fit Components in keV", "C");
          legend->AddEntry((TObject*)nullptr, Form("ThetaLab: %d °", (angmin + angmax)/2), "");
          legend->SetTextSize(0.03);
          legend->SetBorderSize(1);
          legend->AddEntry(peak1f, "g.s", "l");
          legend->AddEntry(peak2f, "915", "l");
          legend->AddEntry(peak3f, "1816", "l");
          legend->AddEntry(peak4f, "2552", "l");
          legend->AddEntry(peak5f, "2886", "l");
          legend->AddEntry(peak6f, "4000", "l"); */
        // legend->AddEntry(bcf, "landau", "l");
        //  legend->AddEntry(peak8f, "Response 8", "l");
          //    legend->Draw();         //Dont want to draw the legent right now:: PS
 /*        c3->SaveAs(Form("pdf/exfit_ang%dto%d.pdf", angmin, angmax));
        c3->SaveAs(Form("pngout/exfit_ang%dto%d.png", angmin, angmax));
        c3->SaveAs(Form("rootout/exfit_ang%dto%d.root", angmin, angmax)); */

    // Clean up
    // delete whole;
    /*file1->Close();
    file2->Close();
    file3->Close();
    file4->Close();
    file5->Close();
    file6->Close();
    datafile->Close(); */
}


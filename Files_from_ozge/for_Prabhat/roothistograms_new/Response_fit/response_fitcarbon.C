#include "response_fitcarbon.h"
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
TH1F* peak7g = nullptr;
TH1F* peak8g = nullptr;
TH1F* carbon = nullptr;

void response_fitcarbon()
{
    const int angmin = 100;
    const int angmax = 180;
    const int projXmin = angmin - 100;
    const int projXmax = angmax - 100;

    TFile* file1 = TFile::Open("simulation9mm/hhgs.root");
    TFile* file2 = TFile::Open("simulation9mm/hh915.root");
    TFile* file3 = TFile::Open("simulation9mm/hh1816.root");
    TFile* file4 = TFile::Open("simulation9mm/hh2552.root");
    TFile* file5 = TFile::Open("simulation9mm/hh2880.root");
    TFile* file6 = TFile::Open("simulation9mm/hh4000.root");
    TFile* file7 = TFile::Open("simulation9mm/hh6000.root");
    TFile* file8 = TFile::Open("simulation9mm/hh7000.root");
    TFile* fcarbon = TFile::Open("../hAllthetamore4kev_carbon2.root");

    if (!file1 || file1->IsZombie() || !file2 || file2->IsZombie() ||
        !file3 || file3->IsZombie() || !file4 || file4->IsZombie() ||
        !file5 || file5->IsZombie() || !file6 || file6->IsZombie() ||
        !file7 || file7->IsZombie() || !file8 || file8->IsZombie() ) {
        printf("Error: One or more response ROOT files could not be opened.\n");
        return;
    }

    TH2F* hh1 = (TH2F*)file1->Get("hh0");
    TH2F* hh2 = (TH2F*)file2->Get("hh0");
    TH2F* hh3 = (TH2F*)file3->Get("hh0");
    TH2F* hh4 = (TH2F*)file4->Get("hh0");
    TH2F* hh5 = (TH2F*)file5->Get("hh0");
    TH2F* hh6 = (TH2F*)file6->Get("hh0");
    TH2F* hh7 = (TH2F*)file7->Get("hh0");
    TH2F* hh8 = (TH2F*)file8->Get("hh0");

    peak1g = new TH1F(); hh1->ProjectionY("htemp1", projXmin, projXmax)->Copy(*peak1g); peak1g->Rebin(2);peak1g->Smooth(2);
    peak2g = new TH1F(); hh2->ProjectionY("htemp2", projXmin, projXmax)->Copy(*peak2g); peak2g->Rebin(2); peak2g->Smooth(2);
    peak3g = new TH1F(); hh3->ProjectionY("htemp3", projXmin, projXmax)->Copy(*peak3g); peak3g->Rebin(2);peak3g->Smooth(2);
    peak4g = new TH1F(); hh4->ProjectionY("htemp4", projXmin, projXmax)->Copy(*peak4g); peak4g->Rebin(2); peak4g->Smooth(2);
    peak5g = new TH1F(); hh5->ProjectionY("htemp5", projXmin, projXmax)->Copy(*peak5g); peak5g->Rebin(2);peak5g->Smooth(2);
    peak6g = new TH1F(); hh6->ProjectionY("htemp6", projXmin, projXmax)->Copy(*peak6g); peak6g->Rebin(2);peak6g->Smooth(2);
    peak7g = new TH1F(); hh7->ProjectionY("htemp7", projXmin, projXmax)->Copy(*peak7g); peak7g->Rebin(2);peak7g->Smooth(2);
    peak8g = new TH1F(); hh8->ProjectionY("htemp8", projXmin, projXmax)->Copy(*peak8g); peak8g->Rebin(2);peak8g->Smooth(2);
    carbon = dynamic_cast<TH1F*>(fcarbon->Get("ex")); carbon->Smooth(2); carbon->Rebin(2);

    TFile* datafile = TFile::Open("../hAllthetamore4kev.root");
    if (!datafile || datafile->IsZombie()) {
        printf("Error: Data ROOT file could not be opened.\n");
        return;
    }
    TH2F* hhdata = (TH2F*)datafile->Get("extheta");
    if (!hhdata) {
        printf("Error: 2D Data histogram not found.\n");
        return;
    }
    TH1F* h_data = new TH1F();
    hhdata->ProjectionY("hdatatemp", projXmin, projXmax)->Copy(*h_data);
    TF1* whole = new TF1("f1", doubexp, -5, 0, 18);
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
    whole->SetParName(12,"pk7 amp.");
    whole->SetParName(13,"pk7 offset");
    whole->SetParName(14,"pk8 amp.");
    whole->SetParName(15,"pk8 offset");
   whole->SetParName(16,"carbon amp.");
   whole->SetParName(17,"carbon offset");

    for (int i = 0; i < 18; i += 2) {
        whole->SetParameter(i,500);
        whole->SetParameter(i + 1,0);
        whole->SetParLimits(i, 0, 50000);
        whole->SetParLimits(i + 1, -0.5, 0.5);
    }
   // whole->FixParameter(16,9.71274e+02);
   // whole->FixParameter(17,-1.35375e-01);
    whole->SetLineColor( kBlack );
    whole->SetLineWidth( 2 );
    whole->SetLineStyle(1);
    whole->SetNpx(5000);
    
   TCanvas* c1 = new TCanvas("c1", "Fit result", 800, 600);
    //gStyle->SetOptStat(1);
    //gStyle->SetOptFit(1);
    h_data->Fit(whole,"R");
    //h_data->SetLineColor(kMagenta);
    //whole->SetLineColor(kSpring+6);
    whole->Draw("same");


    TF1* peak1f = new TF1("peak1f", resp1, -1, 10, 2);
    peak1f->SetParameters(whole->GetParameter(0), whole->GetParameter(1));
    peak1f->SetLineColor(kRed);
    peak1f->Draw("same");

    TF1* peak2f = new TF1("peak2f", resp2, -0, 10, 2);
    peak2f->SetParameters(whole->GetParameter(2), whole->GetParameter(3));
    peak2f->SetLineColor(kGreen);
    peak2f->Draw("same");

    TF1* peak3f = new TF1("peak3f", resp3, -0, 10, 2);
    peak3f->SetParameters(whole->GetParameter(4), whole->GetParameter(5));
    peak3f->SetLineColor(kMagenta);
    peak3f->Draw("same");

    TF1* peak4f = new TF1("peak4f", resp4, -0, 10, 2);
    peak4f->SetParameters(whole->GetParameter(6), whole->GetParameter(7));
    peak4f->SetLineColor(kOrange+7);
    peak4f->Draw("same");

    TF1* peak5f = new TF1("peak5f", resp5, -0, 10, 2);
    peak5f->SetParameters(whole->GetParameter(8), whole->GetParameter(9));
    peak5f->SetLineColor(kViolet + 3);
    peak5f->Draw("same");

    TF1* peak6f = new TF1("peak6f", resp6, -0, 10, 2);
    peak6f->SetParameters(whole->GetParameter(10), whole->GetParameter(11));
    peak6f->SetLineColor(kCyan + 1);
    peak6f->Draw("same");

    TF1* peak7f = new TF1("peak7f", resp7, -0, 10, 2);
    peak7f->SetParameters(whole->GetParameter(12), whole->GetParameter(13));
    peak7f->SetLineColor(kMagenta + 3);
  peak7f->Draw("same");

    TF1* peak8f = new TF1("peak8f", resp8, -0, 10, 2);
    peak8f->SetParameters(whole->GetParameter(14), whole->GetParameter(15));
    peak8f->SetLineColor(kSpring+5);
    peak8f->Draw("same");

    TF1* carbonf = new TF1("carbonf", Res_carbon, -5, 15, 2);
    carbonf->SetParameters(whole->GetParameter(16), whole->GetParameter(17));
    carbonf->SetLineColor(kMagenta + 2);
    carbonf->Draw("same");

    // Extract integrals using histogram method

    double r1 = peak1g->Integral(peak1g->FindBin(-5 - whole->GetParameter(1)), peak1g->FindBin(20 - whole->GetParameter(1))) * whole->GetParameter(0) / 10000.0;
    double r2 = peak2g->Integral(peak2g->FindBin(-5 - whole->GetParameter(3)), peak2g->FindBin(20 - whole->GetParameter(3))) * whole->GetParameter(2) / 10000.0;
    double r3 = peak3g->Integral(peak3g->FindBin(-5 - whole->GetParameter(5)), peak3g->FindBin(20 - whole->GetParameter(5))) * whole->GetParameter(4) / 10000.0;
    double r4 = peak4g->Integral(peak4g->FindBin(-5 - whole->GetParameter(7)), peak4g->FindBin(20 - whole->GetParameter(7))) * whole->GetParameter(6) / 10000.0;
    double r5 = peak5g->Integral(peak5g->FindBin(-5 - whole->GetParameter(9)), peak5g->FindBin(20 - whole->GetParameter(9))) * whole->GetParameter(8) / 10000.0;
    double r6 = peak6g->Integral(peak6g->FindBin(-5 - whole->GetParameter(11)), peak3g->FindBin(20 - whole->GetParameter(11))) * whole->GetParameter(10) / 10000.0;
    double r7 = peak7g->Integral(peak7g->FindBin(-5 - whole->GetParameter(13)), peak4g->FindBin(20 - whole->GetParameter(13))) * whole->GetParameter(12) / 10000.0;
    double r8 = peak8g->Integral(peak8g->FindBin(-5 - whole->GetParameter(15)), peak5g->FindBin(20 - whole->GetParameter(15))) * whole->GetParameter(14) / 10000.0;
    // Output integrals
    printf("Integrals: R1=%.2f, R2=%.2f, R3=%.2f, R4=%.2f, R5=%.2f , R6=%.2f, R7=%.2f, R8=%.2f\n", r1, r2, r3, r4, r5, r6, r7 ,r8);

    // Save fit parameters and integrals to CSV
        FILE* csv = fopen(Form("fit_results_C_ang%dto%d.csv", angmin, angmax), "w");
        if (csv) {
            fprintf(csv, "Parameter,Value");
            for (int i = 0; i < 18; ++i) {
                fprintf(csv, "par[%d],%f", i, whole->GetParameter(i));
            }
            fprintf(csv, "Integral,R1,R2,R3,R4,R5,R6,R7,R8");
            fprintf(csv, "Integral,%f,%f,%f,%f,%f,%f,%f,%f", r1, r2, r3, r4, r5, r6, r7, r8);
            fclose(csv);
        } else {
       printf("Error: Could not write CSV file.");
        }
        TLegend* legend = new TLegend(0.15, 0.65, 0.45, 0.95);
          legend->SetHeader("Fit Components in keV", "C");
          legend->SetTextSize(0.03);
          legend->SetBorderSize(1);
          legend->AddEntry(peak1f, "g.s", "l");
          legend->AddEntry(peak2f, "915", "l");
          legend->AddEntry(peak3f, "1816", "l");
          legend->AddEntry(peak4f, "2552", "l");
          legend->AddEntry(peak5f, "2886", "l");
          legend->AddEntry(peak6f, "4000", "l");
          legend->AddEntry(carbonf, "carbon", "l");
         // legend->AddEntry(peak7f, "Response 7", "l");
        //  legend->AddEntry(peak8f, "Response 8", "l");
          legend->Draw();

        c1->SaveAs(Form("exfit_ccang%dto%d.pdf", angmin, angmax));
        c1->SaveAs(Form("exfit_ccang%dto%d.png", angmin, angmax));
        c1->SaveAs(Form("ex_ccang%dto%d.root", angmin, angmax));

    // Clean up
    delete whole;
    file1->Close();
    file2->Close();
    file3->Close();
    file4->Close();
    file5->Close();
    file6->Close();
    file7->Close();
    file8->Close();
   // fcarbon->Close();
    datafile->Close();
}


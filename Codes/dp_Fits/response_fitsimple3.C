#include "response_fitsimple2.h"
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
#include "TF1.h"

TH1F* peak1g = nullptr;
TH1F* peak2g = nullptr;
TH1F* peak3g = nullptr;
TH1F* peak4g = nullptr;
TH1F* peak5g = nullptr;

void response_fitsimple3() 
{
    // Open response histograms
    TFile* file1 = TFile::Open("simulation9mm/hgs.root");
    TFile* file2 = TFile::Open("simulation9mm/h915.root");
    TFile* file3 = TFile::Open("simulation9mm/h1816.root");
    TFile* file4 = TFile::Open("simulation9mm/h2552.root");
    TFile* file5 = TFile::Open("simulation9mm/h2880.root");

    peak1g = (TH1F*)file1->Get("h0"); //peak1g->Rebin(2);
    peak2g = (TH1F*)file2->Get("h0"); //peak2g->Rebin(2);
    peak3g = (TH1F*)file3->Get("h0"); //peak3g->Rebin(2);
    peak4g = (TH1F*)file4->Get("h0"); //peak4g->Rebin(2);
    peak5g = (TH1F*)file5->Get("h0"); //peak5g->Rebin(2);

    TFile* datafile = TFile::Open("../hAllthetamore4keV.root");
    TH1F* h_data = (TH1F*)datafile->Get("ex");h_data->Rebin(2);

    TF1* whole = new TF1("f1", doubexp, -1, 10, 10);
    whole->SetParameters(50, 0, 50, 0, 50, 0, 50, 0, 50, 0);
    for (int i = 0; i < 10; i += 2) {
        whole->SetParLimits(i, 0, 500000);
        whole->SetParLimits(i + 1, -0.5, 0.5);
    }
    whole->SetLineColor(kRed);
    whole->SetLineWidth(2);
    whole->SetLineStyle(2);
    whole->SetNpx(5000);

    TCanvas* c1 = new TCanvas("c1", "Fit result", 800, 600);
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);
    h_data->Draw("E");
    h_data->Fit(whole, "R");

    // Extract integrals using histogram method
    double r1 = peak1g->Integral(peak1g->FindBin(-5 - whole->GetParameter(1)), peak1g->FindBin(20 - whole->GetParameter(1))) * whole->GetParameter(0) / 10000.0;
    double r2 = peak2g->Integral(peak2g->FindBin(-5 - whole->GetParameter(3)), peak2g->FindBin(20 - whole->GetParameter(3))) * whole->GetParameter(2) / 10000.0;
    double r3 = peak3g->Integral(peak3g->FindBin(-5 - whole->GetParameter(5)), peak3g->FindBin(20 - whole->GetParameter(5))) * whole->GetParameter(4) / 10000.0;
    double r4 = peak4g->Integral(peak4g->FindBin(-5 - whole->GetParameter(7)), peak4g->FindBin(20 - whole->GetParameter(7))) * whole->GetParameter(6) / 10000.0;
    double r5 = peak5g->Integral(peak5g->FindBin(-5 - whole->GetParameter(9)), peak5g->FindBin(20 - whole->GetParameter(9))) * whole->GetParameter(8) / 10000.0;

    // Output integrals
    printf("Integrals: R1=%.2f, R2=%.2f, R3=%.2f, R4=%.2f, R5=%.2f\n", r1, r2, r3, r4, r5);

    whole->Draw("same");
/*
    // Draw response components
    TF1* peak1f = new TF1("peak1f", resp1, -5, 20, 2);
    peak1f->SetParameters(whole->GetParameter(0), whole->GetParameter(1));
    peak1f->SetLineColor(kRed);
    peak1f->Draw("same");

    TF1* peak2f = new TF1("peak2f", resp2, -5, 20, 2);
    peak2f->SetParameters(whole->GetParameter(2), whole->GetParameter(3));
    peak2f->SetLineColor(kGreen + 2);
    peak2f->Draw("same");

    TF1* peak3f = new TF1("peak3f", resp3, -5, 20, 2);
    peak3f->SetParameters(whole->GetParameter(4), whole->GetParameter(5));
    peak3f->SetLineColor(kOrange + 7);
    peak3f->Draw("same");

    TF1* peak4f = new TF1("peak4f", resp4, -5, 20, 2);
    peak4f->SetParameters(whole->GetParameter(6), whole->GetParameter(7));
    peak4f->SetLineColor(kCyan + 1);
    peak4f->Draw("same");

    TF1* peak5f = new TF1("peak5f", resp5, -5, 20, 2);
    peak5f->SetParameters(whole->GetParameter(8), whole->GetParameter(9));
    peak5f->SetLineColor(kMagenta + 2);
    peak5f->Draw("same");
*/
c1->WaitPrimitive();
    c1->SaveAs("th155_result.pdf");
c1->SaveAs("th155_c1responsefit3.root");
    // Cleanup
    delete whole;
    file1->Close(); file2->Close(); file3->Close(); file4->Close(); file5->Close(); datafile->Close();
}


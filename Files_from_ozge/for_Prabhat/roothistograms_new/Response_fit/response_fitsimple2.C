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
TH1F* carbon = nullptr;
void response_fitsimple2() 
{

    // Open the response function ROOT files
    TFile* file1 = TFile::Open("simulation9mm/hgs.root");
    TFile* file2 = TFile::Open("simulation9mm/h915.root");
    TFile* file3 = TFile::Open("simulation9mm/h1816.root");
    TFile* file4 = TFile::Open("simulation9mm/h2552.root");
    TFile* file5 = TFile::Open("simulation9mm/h2880.root");
  //  TFile* fcarbon = TFile::Open("carbon/hcarbon.root");

    if (!file1 || file1->IsZombie() ||
        !file2 || file2->IsZombie() ||
        !file3 || file3->IsZombie() ||
        !file4 || file4->IsZombie() ||
       //!carbon || fcarbon->IsZombie() ||
        !file5 || file5->IsZombie()) {
        printf("Error: One or more response ROOT files could not be opened.\n");
        return;
    }

    // Get the response histograms
    peak1g = dynamic_cast<TH1F*>(file1->Get("h0"));
    peak2g = dynamic_cast<TH1F*>(file2->Get("h0"));
    peak3g = dynamic_cast<TH1F*>(file3->Get("h0"));
    peak4g = dynamic_cast<TH1F*>(file4->Get("h0"));
    peak5g = dynamic_cast<TH1F*>(file5->Get("h0"));
  //  carbon = dynamic_cast<TH1F*>(fcarbon->Get("h0"));
    peak1g->Rebin(2); peak2g->Rebin(2); peak3g->Rebin(2); peak4g->Rebin(2); peak5g->Rebin(2);
    if (!peak1g || !peak2g || !peak3g || !peak4g || !peak5g ){
        printf("Error: One or more peak histograms could not be found.\n");
        return;
    }


    TFile* datafile = TFile::Open("../hAllthetamore4kev.root");  
    if (!datafile || datafile->IsZombie()) {
        printf("Error: Data ROOT file could not be opened.\n");
        return;
    }

    TH1F* h_data = dynamic_cast<TH1F*>(datafile->Get("th155")); //h_data->Rebin(2); 
    if (!h_data) {
        printf("Error: Data histogram not found.\n");
        return;
    }

    

    TF1* whole = new TF1("f1", doubexp, -1, 5, 10);
    
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
// whole->SetParName(10,"bckg amplitude");
 // whole->SetParName(11,"MPV");
 // whole->SetParName(12,"sigma");
  
for (int i = 0; i < 10; i += 2) {
 	whole->SetParameter(i,500); 
 	whole->SetParameter(i+1,0); 
        whole->SetParLimits(i, 0, 50000);
        whole->SetParLimits(i + 1, -0.5, 0.5);
        }
// whole->FixParameter(10, 250);    // background amplitude
//whole->FixParameter(11, 8.5);      // MPV
//whole->FixParameter(12, 6.5);      // sigma
// whole->SetParameter(10, 250);    // background amplitude
//whole->SetParameter(11, 8.5);      // MPV
//whole->SetParameter(12, 6.5);      // sigma
    whole->SetLineColor( kBlue+2 );
    whole->SetLineWidth( 2 );
    whole->SetLineStyle(2);
  whole->SetNpx(5000);
         TCanvas* c1 = new TCanvas("c1", "Fit result", 800, 600); 
         gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);
       h_data->Fit(whole,"R");
          whole->Draw("same");
          whole->SetLineColor(kBlack);
// Draw response components gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);
    TF1* peak1f = new TF1("peak1f", resp1, -1, 10, 2);
    peak1f->SetParameters(whole->GetParameter(0), whole->GetParameter(1));
    peak1f->SetLineColor(kRed);
    peak1f->Draw("same");

    TF1* peak2f = new TF1("peak2f", resp2, -0, 10, 2);
    peak2f->SetParameters(whole->GetParameter(2), whole->GetParameter(3));
    peak2f->SetLineColor(kGreen + 2);
    peak2f->Draw("same");

    TF1* peak3f = new TF1("peak3f", resp3, -0, 10, 2);
    peak3f->SetParameters(whole->GetParameter(4), whole->GetParameter(5));
    peak3f->SetLineColor(kOrange + 7);
    peak3f->Draw("same");

    TF1* peak4f = new TF1("peak4f", resp4, -0, 10, 2);
    peak4f->SetParameters(whole->GetParameter(6), whole->GetParameter(7));
    peak4f->SetLineColor(kCyan + 1);
    peak4f->Draw("same");

    TF1* peak5f = new TF1("peak5f", resp5, -0, 10, 2);
    peak5f->SetParameters(whole->GetParameter(8), whole->GetParameter(9));
    peak5f->SetLineColor(kMagenta + 2);
    peak5f->Draw("same");

    
  // Extract integrals using histogram method
    double r1 = peak1g->Integral(peak1g->FindBin(-5 - whole->GetParameter(1)), peak1g->FindBin(20 - whole->GetParameter(1))) * whole->GetParameter(0) / 10000.0;
    double r2 = peak2g->Integral(peak2g->FindBin(-5 - whole->GetParameter(3)), peak2g->FindBin(20 - whole->GetParameter(3))) * whole->GetParameter(2) / 10000.0;
    double r3 = peak3g->Integral(peak3g->FindBin(-5 - whole->GetParameter(5)), peak3g->FindBin(20 - whole->GetParameter(5))) * whole->GetParameter(4) / 10000.0;
    double r4 = peak4g->Integral(peak4g->FindBin(-5 - whole->GetParameter(7)), peak4g->FindBin(20 - whole->GetParameter(7))) * whole->GetParameter(6) / 10000.0;
    double r5 = peak5g->Integral(peak5g->FindBin(-5 - whole->GetParameter(9)), peak5g->FindBin(20 - whole->GetParameter(9))) * whole->GetParameter(8) / 10000.0;

    // Output integrals
    printf("Integrals: R1=%.2f, R2=%.2f, R3=%.2f, R4=%.2f, R5=%.2f\n", r1, r2, r3, r4, r5);
    
  
    
    c1->WaitPrimitive();
    c1->SaveAs("exresponse_fit_result115.pdf");
c1->SaveAs("exc115.root");
    // Clean up
    delete whole;
    file1->Close();
    file2->Close();
    file3->Close();
    file4->Close();
    file5->Close();
    datafile->Close();
}


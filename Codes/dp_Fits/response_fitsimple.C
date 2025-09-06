#include "response_fitsimple.h"
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

void response_fitsimple() 
{

    // Open the response function ROOT files
    TFile* file1 = TFile::Open("1dhgs.root");
    TFile* file2 = TFile::Open("1dh2278.root");
    TFile* file3 = TFile::Open("1dh2555.root");
    TFile* file4 = TFile::Open("1dh2878.root");

    if (!file1 || file1->IsZombie() ||
        !file2 || file2->IsZombie() ||
        !file3 || file3->IsZombie() ||
        !file4 || file4->IsZombie()) {
        printf("Error: One or more response ROOT files could not be opened.\n");
        return;
    }

    // Get the response histograms
    peak1g = dynamic_cast<TH1F*>(file1->Get("h0"));
    peak2g = dynamic_cast<TH1F*>(file2->Get("h0"));
    peak3g = dynamic_cast<TH1F*>(file3->Get("h0"));
    peak4g = dynamic_cast<TH1F*>(file4->Get("h0"));

    if (!peak1g || !peak2g || !peak3g || !peak4g) {
        printf("Error: One or more peak histograms could not be found.\n");
        return;
    }

    // Open the data histogram (assuming you have another file)
    TFile* datafile = TFile::Open("../hAllthetamore.root");  // <-- update this if different
    if (!datafile || datafile->IsZombie()) {
        printf("Error: Data ROOT file could not be opened.\n");
        return;
    }

    TH1F* h_data = dynamic_cast<TH1F*>(datafile->Get("ex")); // <-- adjust name if different
    if (!h_data) {
        printf("Error: Data histogram not found.\n");
        return;
    }

    // Prepare a list of templates
    TObjArray* templates = new TObjArray(4);
    templates->Add(peak1g);
    templates->Add(peak2g);
    templates->Add(peak3g);
    templates->Add(peak4g);

    TF1* f1 = new TF1("f1", doubexp, -5, 20, 8);
    f1->SetParameters(100., 0., 100., 0., 100., 0., 100., 0.);
       h_data->Fit(f1,"R");
    // Fit the data with the templates
   // TFractionFitter* fit = new TFractionFitter(h_data, templates);

   // if (fit->Fit() == 0) {  // 0 means fit is OK
    //    printf("Fit successful.\n");
  //  } else {
  //      printf("Fit failed.\n");
  //  }

    // Draw result
    TCanvas* c1 = new TCanvas("c1", "Fit result", 800, 600);
    h_data->Draw("E");

   // TH1* result = (TH1*) f1->GetPlot();
   // if (result) {
    //    result->SetLineColor(kRed);
     //   result->Draw("SAME HIST");
    //}

    // Optional: print fit results
    for (int i = 0; i < 4; ++i) {
        double value, error;
       // f1->GetResult(i, value, error);
        printf("Component %d: fraction = %.4f ± %.4f\n", i+1, value, error);
    }

    // Save canvas
    c1->SaveAs("response_fit_result.pdf");

    // Clean up
    delete f1;
    file1->Close();
    file2->Close();
    file3->Close();
    file4->Close();
    datafile->Close();
}


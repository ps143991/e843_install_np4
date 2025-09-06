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
void response_fitPS_v2()
{
    const int angmin = 130;
    const int angmax = 160;
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
    // h_data ->Draw();

    //added 2d histograms



    peak1g = new TH1F(); hh1->ProjectionY("htemp1", projXmin, projXmax)->Copy(*peak1g);
    peak2g = new TH1F(); hh2->ProjectionY("htemp2", projXmin, projXmax)->Copy(*peak2g);
    peak3g = new TH1F(); hh3->ProjectionY("htemp3", projXmin, projXmax)->Copy(*peak3g);
    peak4g = new TH1F(); hh4->ProjectionY("htemp4", projXmin, projXmax)->Copy(*peak4g);
    peak5g = new TH1F(); hh5->ProjectionY("htemp5", projXmin, projXmax)->Copy(*peak5g);
    peak6g = new TH1F(); hh6->ProjectionY("htemp6", projXmin, projXmax)->Copy(*peak6g);

     TCanvas *cfit = new TCanvas("cfit", "Peak 1 Fit", 800, 600);
    /* G.S. */                peak1g->Fit("gaus", "Q", "",-1.0, 1.0);
    /* 915  */                peak2g->Fit("gaus", "Q", "", 0.0, 2.0);
    /* 1816 */                peak3g->Fit("gaus", "Q", "", 0.8, 2.8);
    /* 2552 */                peak4g->Fit("gaus", "Q", "", 1.5, 3.5); 
    /* 2880 */                peak5g->Fit("gaus", "Q", "", 1.8, 3.8); 
    /* 4000 */                peak6g->Fit("gaus", "Q", "", 3.0, 5.0);  

    TH1F* peaks[6] = {peak1g, peak2g, peak3g, peak4g, peak5g, peak6g};
    Double_t fit_parameters[21] = {0};
    for (int ii = 0; ii < 6; ++ii) {
    TF1* fitFunc = peaks[ii]->GetFunction("gaus");
    if (fitFunc){
        double amplitude = fitFunc->GetParameter(0); // amplitude
        double mean = fitFunc->GetParameter(1);      // mean
        double sigma = fitFunc->GetParameter(2);     // sigma
        
        // Store fit parameters
        fit_parameters[ii * 3] = amplitude;
        fit_parameters[ii * 3 + 1] = mean;
        fit_parameters[ii * 3 + 2] = sigma*1.5; // Adjusted sigma for better fit

        // Print or use as needed
        printf("Peak%d: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", ii+1, amplitude, mean, sigma);
    }}


    TCanvas* c2 = new TCanvas("c2", "carbon", 800, 600);

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


    TF1* whole = new TF1("whole", allgaus_plus_left_skewed_landau, -1, 5.5, 21);


/*     for(int ll = 0; ll<18; ll++){
    whole->FixParameter(ll,fit_parameters[ll]);} */


    whole->SetParameter(0, fit_parameters[0]);          // 1 amplitude
    whole->FixParameter(1, fit_parameters[1]);          //                  mean
    whole->FixParameter(2, fit_parameters[2]);          //                              sigma
    whole->SetParameter(3, fit_parameters[3]);          // 2 amplitude
    whole->FixParameter(4, fit_parameters[4]);          //                  mean
    whole->FixParameter(5, fit_parameters[5]);          //                              sigma
    whole->SetParameter(6, fit_parameters[6]);          // 3 amplitude
    whole->FixParameter(7, fit_parameters[7]);          //                  mean            
    whole->FixParameter(8, fit_parameters[8]);          //                              sigma
    whole->SetParameter(9, fit_parameters[9]);          // 4 amplitude
    whole->FixParameter(10, fit_parameters[10]);        //                  mean
    whole->FixParameter(11, fit_parameters[11]);        //                              sigma
    whole->SetParameter(12, fit_parameters[12]);        // 5 amplitude                      
    whole->FixParameter(13, fit_parameters[13]);        //                  mean
    whole->FixParameter(14, fit_parameters[14]);        //                              sigma
    whole->SetParameter(15, fit_parameters[15]);        // 6 amplitude
    whole->FixParameter(16, fit_parameters[16]);        //                  mean
    whole->FixParameter(17, fit_parameters[17]);        //                              sigma

    // whole->FixParameter(1, fit_parameters[x]);          // background amplitude
    whole->FixParameter(18, 110);          // background amplitude
    whole->FixParameter(19, 7.8);        // MPV
    whole->FixParameter(20, 0.95);        // sigma
    whole->SetLineColor(kBlack);
    whole->SetLineWidth(2);
    whole->SetNpx(5000);
    h_data->SetLineColor(kGray+1);
    TCanvas* c3 = new TCanvas("c3", "Fit result", 1300, 900);
    whole->Draw("same");
    h_data->Fit(whole,"R");             // fitting happens here

    TF1* bglandau = new TF1("bglandau", [](double* x, double* par) {
        return  (par[0] * TMath::Landau(2*par[1] - x[0], par[1], par[2]));}, -1,13, 3);
    bglandau->SetParameters(110,7.8,0.9);
    bglandau->SetLineColor(kRed+1);
    bglandau->SetLineWidth(2);
    bglandau->Draw("same");


    TF1* gaussian = new TF1("gaussian", [](double* x, double* par) {
        return par[0] * TMath::Gaus(x[0], par[1], par[2]);
    }, -1, 5.5, 3);
    gaussian->SetParameters(1, 0, 1);
    gaussian->SetLineColor(kBlue);
    gaussian->SetLineWidth(2);
    gaussian->Draw("same");


    TF1* peak1gfit = new TF1("peak1fit", gaus_plus_left_skewed_landau, -1, 1, 6);
    peak1gfit->SetParameters(whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20));
    peak1gfit->SetLineColor(kMagenta);
    peak1gfit->SetFillColor(kMagenta);
    peak1gfit->SetFillStyle(0); // No fill, just line
    peak1gfit->Draw("same");

    TF1* peak2gfit = new TF1("peak2fit", gaus_plus_left_skewed_landau, -1, 2, 6);
    peak2gfit->SetParameters(whole->GetParameter(3), whole->GetParameter(4), whole->GetParameter(5), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20));
    peak2gfit->SetLineColor(kGreen);
    peak2gfit->SetFillColor(kGreen);
    peak2gfit->SetFillStyle(0); // No fill, just line
    peak2gfit->Draw("same");

    TF1* peak3gfit = new TF1("peak3fit", gaus_plus_left_skewed_landau, 0.8, 2.8, 6);
    peak3gfit->SetParameters(whole->GetParameter(6), whole->GetParameter(7), whole->GetParameter(8), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20));
    peak3gfit->SetLineColor(kBlue);
    peak3gfit->SetFillColor(kBlue);
    peak3gfit->SetFillStyle(0); // No fill, just line
    peak3gfit->Draw("same");

/*     TF1* peak4gfit = new TF1("peak4fit", gaus_plus_left_skewed_landau, 1.5, 3.5, 6);
    // peak4gfit->SetParameters(whole->GetParameter(9), whole->GetParameter(10), whole->GetParameter(11), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20));
    peak4gfit->FixParameter(0, whole->GetParameter(9));
    peak4gfit->FixParameter(1, whole->GetParameter(10));
    peak4gfit->FixParameter(2, whole->GetParameter(11));
    peak4gfit->FixParameter(3, whole->GetParameter(18));
    peak4gfit->FixParameter(4, whole->GetParameter(19));
    peak4gfit->FixParameter(5, whole->GetParameter(20));
    peak4gfit->SetLineColor(kCyan); */
    /* peak4gfit->SetFillColor(kCyan);
    peak4gfit->SetFillStyle(0); // No fill, just line */
    // peak4gfit->Draw("same");

/*     TF1* peak5gfit = new TF1("peak5fit", gaus_plus_left_skewed_landau, 1.8, 3.8, 6);
    // peak5gfit->SetParameters(whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20));
    peak5gfit->FixParameter(0, whole->GetParameter(12));
    peak5gfit->FixParameter(1, whole->GetParameter(13));
    peak5gfit->FixParameter(2, whole->GetParameter(14));
    peak5gfit->FixParameter(3, whole->GetParameter(18));
    peak5gfit->FixParameter(4, whole->GetParameter(19));
    peak5gfit->FixParameter(5, whole->GetParameter(20)); */
    // peak5gfit->SetLineColor(kYellow+1);
/*     peak5gfit->SetFillColor(kYellow+1);
    peak5gfit->SetFillStyle(0); // No fill, just line */
/*     peak5gfit->Draw("same"); */
/* 
    TF1* peak6gfit = new TF1("peak6fit", gaus_plus_left_skewed_landau, 3.0, 5.0, 6);
    peak6gfit->SetParameters(whole->GetParameter(15), whole->GetParameter(16), whole->GetParameter(17), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20));
    peak6gfit->SetLineColor(kGreen); */
/*     peak6gfit->SetFillColor(kGreen);
    peak6gfit->SetFillStyle(0); // No fill, just line */
/*     peak6gfit->Draw("same");
 */


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


}


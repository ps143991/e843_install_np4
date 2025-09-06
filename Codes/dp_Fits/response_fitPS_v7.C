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

TCanvas* can1 = NULL;
TCanvas* can2 = NULL;
TCanvas* can3 = NULL;
TCanvas* can4 = NULL;
TCanvas* can5 = NULL;

TCanvas* can_reg[5];
TCanvas* call = NULL;

    void DrawFunctionsOnTop(TF1* whole){

    /***********************************************************************************************************/
    /***********************************************************************************************************/
    /********************************************Drawing functions on TOP***************************************/
    /***********************************************************************************************************/
    /***********************************************************************************************************/
    
/*     TF1* bglandau = new TF1("bglandau", [](double* x, double* par) {
        return  (par[0] * TMath::Landau(2*par[1] - x[0], par[1], par[2]));}, -1,15, 3);
    bglandau->SetParameters(whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20));
    bglandau->SetLineColor(kRed+1);
    bglandau->SetLineWidth(2);
    bglandau->Draw("same"); */


    // TF1* bcf = new TF1("bcf", left_skewed_landau, -2, 15.0, 3);

    // // bcf->SetParameter(0,whole->GetParameter(18));
    // bcf->FixParameter(0,275);
    // // bcf->SetParameter(1,whole->GetParameter(19));
    // bcf->FixParameter(1,8.5);
    // // bcf->SetParameter(2,whole->GetParameter(20));
    // bcf->FixParameter(2,0.9);
    
    // bcf->SetLineColor(kRed+ 1);
    // bcf->Draw("same");
    TF1* bc_pol4 = new TF1("bc_pol4", "pol4", -1, 5.2);
    bc_pol4->SetParameters(whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22));
    bc_pol4->SetLineColor(kRed+1);
    bc_pol4->SetLineWidth(2);
    bc_pol4->Draw("same");


        TF1* peak1gfit = new TF1("peak1fit", gaus_plus_pol4, -1, 1, 8);
    peak1gfit->SetParameters(whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22));
    peak1gfit->SetLineColor(kMagenta);
    peak1gfit->SetFillColor(kMagenta);
    peak1gfit->SetFillStyle(0); // No fill, just line
    peak1gfit->Draw("same");

        TF1* peak2gfit = new TF1("peak2fit", gaus_plus_pol4, -1, 2, 8);
    peak2gfit->SetParameters(whole->GetParameter(3), whole->GetParameter(4), whole->GetParameter(5), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22));
    peak2gfit->SetLineColor(kGreen);
    peak2gfit->SetFillColor(kGreen);
    peak2gfit->SetFillStyle(0); // No fill, just line
    peak2gfit->Draw("same");

        TF1* peak3gfit = new TF1("peak3fit", gaus_plus_pol4, 0.8, 2.8, 8);
    peak3gfit->SetParameters(whole->GetParameter(6), whole->GetParameter(7), whole->GetParameter(8), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22));
    peak3gfit->SetLineColor(kBlue);
    peak3gfit->SetFillColor(kBlue);
    peak3gfit->SetFillStyle(0); // No fill, just line
    peak3gfit->Draw("same");

        TF1* peak4gfit = new TF1("peak4fit", gaus_plus_pol4, 1.5, 3.5, 8);
    peak4gfit->SetParameters(whole->GetParameter(9), whole->GetParameter(10), whole->GetParameter(11), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22));
    peak4gfit->SetLineColor(kOrange);
    peak4gfit->SetFillColor(kOrange);
    peak4gfit->SetFillStyle(0); // No fill, just line
    peak4gfit->Draw("same");

        TF1* peak5gfit = new TF1("peak5fit", gaus_plus_pol4, 1.8, 3.8, 8);
    peak5gfit->SetParameters(whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22));
    peak5gfit->SetLineColor(kPink);
    peak5gfit->SetFillColor(kPink);
    peak5gfit->SetFillStyle(0); // No fill, just line
    peak5gfit->Draw("same");

        TF1* peak6gfit = new TF1("peak6fit", gaus_plus_pol4, 3.0, 5.0, 8);
    peak6gfit->SetParameters(whole->GetParameter(15), whole->GetParameter(16), whole->GetParameter(17), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22));
    peak6gfit->SetLineColor(kBrownCyan);
    peak6gfit->SetFillColor(kBrownCyan);
    peak6gfit->SetFillStyle(0); // No fill, just line
    peak6gfit->Draw("same");

          TF1* peak7gfit = new TF1("peak7fit", gaus_plus_pol4, 3.0, 5.0, 8);
    peak7gfit->SetParameters(whole->GetParameter(23), whole->GetParameter(24), whole->GetParameter(25), whole->GetParameter(18), whole->GetParameter(19), whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22));
    peak7gfit->SetLineColor(kYellow + 2);
    peak7gfit->SetFillColor(kYellow + 2);
    peak7gfit->SetFillStyle(0); // No fill, just line
    peak7gfit->Draw("same");

    	 TLegend* legend = new TLegend(0.75, 0.15, 0.90, 0.55);
          legend->SetHeader("Fit Components in keV", "C");
        //   legend->AddEntry((TObject*)nullptr, Form("ThetaLab: %d °", (angmin + angmax)/2), "");
          legend->SetTextSize(0.02);
          legend->SetBorderSize(1);
          legend->AddEntry(peak1gfit, "g.s", "l");
          legend->AddEntry(peak2gfit, "915", "l");
          legend->AddEntry(peak3gfit, "1816", "l");
          legend->AddEntry(peak4gfit, "2552", "l");
          legend->AddEntry(peak5gfit, "2886", "l");
          legend->AddEntry(peak7gfit, Form("new peak @ %.0f", whole->GetParameter(24)*1000), "l");
          legend->AddEntry(peak6gfit, "4000", "l");
        // legend->AddEntry(bcf, "landau", "l");
        //  legend->AddEntry(peak8f, "Response 8", "l");
          legend->Draw();         //Dont want to draw the legent right now:: PS
 /*        c3->SaveAs(Form("pdf/exfit_ang%dto%d.pdf", angmin, angmax));
        c3->SaveAs(Form("pngout/exfit_ang%dto%d.png", angmin, angmax));
        c3->SaveAs(Form("rootout/exfit_ang%dto%d.root", angmin, angmax)); */


}






using namespace std;
void response_fitPS_v7()
{
    const int angmin = 100;
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
    TFile* Breakupfile = TFile::Open("../breakup_deuteron/phase_BU.root");

    if (!file1 || file1->IsZombie() || !file2 || file2->IsZombie() ||!file3 || file3->IsZombie() || !file4 || file4->IsZombie() ||!file5 || file5->IsZombie() || !file6 || file6->IsZombie() ) {
        printf("Error: One or more response ROOT files could not be opened.\n");
        return;
    }
    if (!Breakupfile || Breakupfile->IsZombie()) {
        printf("Error: Breakup ROOT file could not be opened.\n");
        return;
    }
    if (!datafile || datafile->IsZombie()) {
        printf("Error: Data ROOT file could not be opened.\n");
        return;
    }
    if (!carbonfile || carbonfile->IsZombie()) {
        printf("Error: Carbon ROOT file could not be opened.\n");
        return;
    }
   
    TH2F* hhdata = (TH2F*)datafile->Get("extheta");
    TH2F* carbon_2d = (TH2F*)carbonfile->Get("extheta");
    TH1F* carbon = (TH1F*)carbonfile->Get("ex");
    TH1F* all_ex = (TH1F*)datafile->Get("ex");
    TH2F* hh1 = (TH2F*)file1->Get("hh0");
    TH2F* hh2 = (TH2F*)file2->Get("hh0");
    TH2F* hh3 = (TH2F*)file3->Get("hh0");
    TH2F* hh4 = (TH2F*)file4->Get("hh0");
    TH2F* hh5 = (TH2F*)file5->Get("hh0");
    TH2F* hh6 = (TH2F*)file6->Get("hh0");
                    
    call= new TCanvas("call", "Response Histograms", 800, 600);
    call->Divide(3, 2);
    
    TH1F* h_data_115_124 = new TH1F();
    TH1F* h_data_124_133 = new TH1F();
    TH1F* h_data_133_142 = new TH1F();
    TH1F* h_data_142_151 = new TH1F();
    TH1F* h_data_151_160 = new TH1F();

    TH1F* carbon_2d_projected_115_124 = new TH1F();
    TH1F* carbon_2d_projected_124_133 = new TH1F();
    TH1F* carbon_2d_projected_133_142 = new TH1F();
    TH1F* carbon_2d_projected_142_151 = new TH1F();
    TH1F* carbon_2d_projected_151_160 = new TH1F();
    

    hhdata->ProjectionY("hdatatemp_115_124", 15, 24)->Copy(*h_data_115_124);    //115 to 160... every angular cut is of 9 degrees
    hhdata->ProjectionY("hdatatemp_124_133", 24, 33)->Copy(*h_data_124_133);
    hhdata->ProjectionY("hdatatemp_133_142", 33, 42)->Copy(*h_data_133_142);
    hhdata->ProjectionY("hdatatemp_142_151", 42, 51)->Copy(*h_data_142_151);
    hhdata->ProjectionY("hdatatemp_151_160", 51, 60)->Copy(*h_data_151_160);

    carbon_2d->ProjectionY("carbon_temp_115_124", 15, 24)->Copy(*carbon_2d_projected_115_124);
    carbon_2d->ProjectionY("carbon_temp_124_133", 24, 33)->Copy(*carbon_2d_projected_124_133);
    carbon_2d->ProjectionY("carbon_temp_133_142", 33, 42)->Copy(*carbon_2d_projected_133_142);
    carbon_2d->ProjectionY("carbon_temp_142_151", 42, 51)->Copy(*carbon_2d_projected_142_151);
    carbon_2d->ProjectionY("carbon_temp_151_160", 51, 60)->Copy(*carbon_2d_projected_151_160);

    
    peak1g = new TH1F(); hh1->ProjectionY("htemp1", projXmin, projXmax)->Copy(*peak1g);
    peak2g = new TH1F(); hh2->ProjectionY("htemp2", projXmin, projXmax)->Copy(*peak2g);
    peak3g = new TH1F(); hh3->ProjectionY("htemp3", projXmin, projXmax)->Copy(*peak3g);
    peak4g = new TH1F(); hh4->ProjectionY("htemp4", projXmin, projXmax)->Copy(*peak4g);
    peak5g = new TH1F(); hh5->ProjectionY("htemp5", projXmin, projXmax)->Copy(*peak5g);
    peak6g = new TH1F(); hh6->ProjectionY("htemp6", projXmin, projXmax)->Copy(*peak6g);

// Arrays of pointers to your histograms
TH1F* h_data_arr[5] = {h_data_115_124, h_data_124_133, h_data_133_142, h_data_142_151, h_data_151_160};
TH1F* carbon_arr[5] = {carbon_2d_projected_115_124, carbon_2d_projected_124_133, carbon_2d_projected_133_142, carbon_2d_projected_142_151, carbon_2d_projected_151_160};

for (int i = 0; i < 5; ++i) {
    call->cd(i+1);
    // h_data_arr[i]->SetMaximum(110);
    // h_data_arr[i]->Rebin(4);
    h_data_arr[i]->Draw();
    TF1* left_skewed_landau_func = new TF1("left_skewed_landau", left_skewed_landau, -5, 15, 3);
    left_skewed_landau_func->FixParameter(0, 80);
    left_skewed_landau_func->FixParameter(1, 6.5);
    left_skewed_landau_func->FixParameter(2, 0.95);
    // h_data_arr[i]->Fit(left_skewed_landau_func, "R");
    left_skewed_landau_func->SetLineColor(kRed+1);
    left_skewed_landau_func->SetLineStyle(1); // Set line style to solid
    left_skewed_landau_func->SetLineWidth(1);
    left_skewed_landau_func->Draw("same");
    // carbon_arr[i]->Rebin(4); // Uncomment if you want to rebin carbon as well
    // carbon_arr[i]->SetLineColor(kRed);
    // carbon_arr[i]->Scale(3);
    // carbon_arr[i]->SetLineStyle(1); // Set line style to solid
    // carbon_arr[i]->Draw("same L");
}

    //  TCanvas *cfit = new TCanvas("cfit", "Peak 1 Fit", 800, 600);
    call->cd(6);
    
    /* G.S. */                peak1g->Fit("gaus", "Q", "",-1.0, 1.0);
    /* 915  */                peak2g->Fit("gaus", "Q", "", 0.0, 2.0);
    /* 1816 */                peak3g->Fit("gaus", "Q", "", 0.8, 2.8);
    /* 2552 */                peak4g->Fit("gaus", "Q", "", 1.5, 3.5); 
    /* 2880 */                peak5g->Fit("gaus", "Q", "", 1.8, 3.8); 
    /* 4000 */                peak6g->Fit("gaus", "Q", "", 3.0, 5.0);  



    TH1F* peaks[6] = {peak1g, peak2g, peak3g, peak4g, peak5g, peak6g};

    Double_t fit_parameters[26] = {0};
    cout<<"Simulation fit parameters:"<<endl;
    for (int ii = 0; ii < 6; ++ii) {
    TF1* fitFunc = peaks[ii]->GetFunction("gaus");
    if (fitFunc){
        double amplitude = fitFunc->GetParameter(0); // amplitude
        double mean = fitFunc->GetParameter(1);      // mean
        double sigma = fitFunc->GetParameter(2);     // sigma
        
        // Store fit parameters
        fit_parameters[ii * 3] = amplitude;
        fit_parameters[ii * 3 + 1] = mean;
        fit_parameters[ii * 3 + 2] = sigma; // Adjusted sigma for better fit

        // Print or use as needed
        printf("Peak %d: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", ii+1, amplitude, mean, sigma);
    }}
    call->Close();
    // call->Delete();


    can1 = new TCanvas("can1", "Peaks", 1900, 1050);

    all_ex->SetLineColor(kYellow+2);
    all_ex->SetLineWidth(1);
    all_ex->Rebin(4);
    all_ex->Draw("hist");

    // TCanvas* c2 = new TCanvas("c2", "carbon", 800, 600);

    // carbon_2d_projected->Draw();

    TF1* landau_carbon = new TF1("landau_carbon", left_skewed_landau, -5, 15, 3);
    landau_carbon->SetParameters(14, 8, 1.2);
    // carbon_2d_projected->Fit(landau_carbon, "R");
    landau_carbon->SetLineColor(kRed+1);
    // landau_carbon->Draw("same");
    
    TF1* carbon_func = new TF1("carbon_func", left_skewed_landau, -5, 15, 3);
    carbon_func->SetParameters(landau_carbon->GetParameter(0), landau_carbon->GetParameter(1), landau_carbon->GetParameter(2));
    carbon_func->SetLineColor(kGreen+ 1);
    // carbon_func->Draw("same");


    // TF1* whole = new TF1("whole", allgaus_plus_left_skewed_landau, -2, 5.2, 21);
    TF1* whole = new TF1("whole", allgaus_plus_pol4, -1.5, 5.2, 26);

    Double_t BG_Par[6][5];       // x index for 0-> total, 1-> region1, 2-> region2... 
                                        // y index for coefficients of pol4
    // BG_Par[0][0] = 4.6; BG_Par[0][1] = 4.9;  BG_Par[0][2] = 0.44; BG_Par[0][3] = 0.0; BG_Par[0][4] = 0.0; // for total with Rebin(2)
    BG_Par[0][0] = 8.0; BG_Par[0][1] = 6.3;  BG_Par[0][2] = 1.8; BG_Par[0][3] = 0.0; BG_Par[0][4] = 0.0; // for total with Rebin(4)
    BG_Par[1][0] = 1.7; BG_Par[1][1] = 0.84; BG_Par[1][2] = 0.27; BG_Par[1][3] = 0.0; BG_Par[1][4] = 0.0; // for region 1
    BG_Par[2][0] = 1.7; BG_Par[2][1] = 0.84; BG_Par[2][2] = 0.27; BG_Par[2][3] = 0.0; BG_Par[2][4] = 0.0; // for region 2
    BG_Par[3][0] = 1.7; BG_Par[3][1] = 0.84; BG_Par[3][2] = 0.27; BG_Par[3][3] = 0.0; BG_Par[3][4] = 0.0; // for region 3
    BG_Par[4][0] = 1.7; BG_Par[4][1] = 0.84; BG_Par[4][2] = 0.27; BG_Par[4][3] = 0.0; BG_Par[4][4] = 0.0; // for region 4
    BG_Par[5][0] = 1.7; BG_Par[5][1] = 0.84; BG_Par[5][2] = 0.27; BG_Par[5][3] = 0.0; BG_Par[5][4] = 0.0; // for region 5
    
    for(int li = 0; li < 18; ++li) {
        whole->SetParameter(li, fit_parameters[li]+0.01);}
        
        //MORE BREATHING ROOM FOR THE 2nd peak*************************
        whole->SetParLimits(0,  1,200);   whole->SetParLimits(1,  fit_parameters[1] -0.2, fit_parameters[1]  +0.2);  whole->SetParLimits(2, 0.2, 0.3);
        whole->SetParLimits(3,  1,200);   whole->SetParLimits(4,  fit_parameters[4] -0.4, fit_parameters[4]  +0.2);  whole->SetParLimits(5, 0.2, 0.3);
        whole->SetParLimits(6,  1,200);   whole->SetParLimits(7,  fit_parameters[7] -0.2, fit_parameters[7]  +0.2);  whole->SetParLimits(8, 0.2, 0.3);
        whole->SetParLimits(9,  1,200);   whole->SetParLimits(10, fit_parameters[10]-0.2, fit_parameters[10] +0.2);  whole->SetParLimits(11,0.2, 0.3);
        whole->SetParLimits(12, 1,200);  whole->SetParLimits(13, fit_parameters[13]-0.2, fit_parameters[13] +0.2);  whole->SetParLimits(14,0.2, 0.3);
        whole->SetParLimits(15, 1,200);  whole->SetParLimits(16, fit_parameters[16]-0.2, fit_parameters[16] +0.2);  whole->SetParLimits(17,0.2, 0.3);

        // whole->FixParameter(18, 145);        // background amplitude
        // whole->FixParameter(19, 7.96);        // MPV
        // whole->FixParameter(20, 1.63);           // sigma
        for(int jl = 0; jl < 5; ++jl) {
            whole->FixParameter(18 + jl, BG_Par[0][jl]);}        // pol4 background parameters

        whole->SetParameter(23, 6); whole->SetParameter(24, 3.6); whole->SetParameter(25, 0.25);
        whole->SetParLimits(23, 1,70); whole->SetParLimits(24, 3.6-0.2, 3.6+0.2); whole->SetParLimits(25, 0.2,0.3);


    all_ex->Fit(whole, "RQ"); // fitting happens here
    whole ->SetNpx(1000); // Increase number of points for smoother curve
    TText *text1 = new TText(10.0, 1.0, Form("RChi: %.2f", whole->GetChisquare()/whole->GetNDF()));  
    text1->Draw();

    cout<<"*******PS: CHI SQUARE RECIEVED: "<<whole->GetChisquare()<<endl;
    cout<<"*******PS: NDF RECIEVED: "<<whole->GetNDF()<<endl;
    cout<<"*******PS: REDUCED CHI SQUARE: "<<whole->GetChisquare()/whole->GetNDF()<<endl;

        for(int il = 0; il < 26; ++il) {
        fit_parameters[il] = whole->GetParameter(il);
    }
    whole->SetLineColor(kBlack);
    whole->SetLineWidth(2);
    whole->Draw("same");

    cout<< "Fit parameters from excitation function: " << endl;
    for (int iil = 0; iil < 5; ++iil) {
        printf("Peak %d: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", iil+1, fit_parameters[iil*3], fit_parameters[iil*3+1], fit_parameters[iil*3+2]);
    }
    printf("Peak 6: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", fit_parameters[23], fit_parameters[24], fit_parameters[25]);
    printf("Peak 7: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", fit_parameters[15], fit_parameters[16], fit_parameters[17]);
    printf("Background: %.2f + %.2f*x + %.2f*x^2 + %.2f*x^3 + %.2f*x^4\n\n",
           fit_parameters[18], fit_parameters[19], fit_parameters[20], fit_parameters[21], fit_parameters[22]);


    DrawFunctionsOnTop(whole);


    /***********************************************************************************************************/
    /***********************************************************************************************************/
    /********************************************Time for individual fits***************************************/
    /***********************************************************************************************************/
    /***********************************************************************************************************/
can3 = new TCanvas("can3", "divided pads for ang fit", 1900, 1050);
        can3->Divide(3,2);
    for(int ci = 0; ci < 5; ++ci) {

        // can_reg[ci] = new TCanvas(Form("canreg%d", ci+1), Form("Region %d", ci+1), 1200, 800);
        // can_reg[ci]->cd();
        can3->cd(ci+1);

        h_data_arr[ci]->SetLineColor(kRed);
        h_data_arr[ci]->Rebin(4);
        h_data_arr[ci]->Draw();
        TF1* whole_region = new TF1(Form("whole_region%d", ci+1), allgaus_plus_pol4, -1.5, 5.2, 26);
                for(int ill = 0; ill < 26; ++ill) {
                    whole_region->SetParameter(ill, fit_parameters[ill]);
                    whole_region->SetParLimits(0, 1, 200);   whole_region->FixParameter(1,  fit_parameters[1] );  whole_region->SetParLimits(2, 0.2, 0.3);
                    whole_region->SetParLimits(3, 1, 200);   whole_region->FixParameter(4,  fit_parameters[4] );  whole_region->SetParLimits(5, 0.2, 0.3);
                    whole_region->SetParLimits(6, 1, 200);   whole_region->FixParameter(7,  fit_parameters[7] );  whole_region->SetParLimits(8, 0.2, 0.3);
                    whole_region->SetParLimits(9, 1, 200);   whole_region->FixParameter(10, fit_parameters[10]);  whole_region->SetParLimits(11,0.2, 0.3);
                    whole_region->SetParLimits(12, 1,200);  whole_region->FixParameter(13, fit_parameters[13]);  whole_region->SetParLimits(14,0.2, 0.3);
                    whole_region->SetParLimits(15, 1,200);  whole_region->FixParameter(16, fit_parameters[16]);  whole_region->SetParLimits(17,0.2, 0.3);

                    // whole_region->FixParameter(18, 145);        // background amplitude
                    // whole_region->FixParameter(19, 7.96);        // MPV
                    // whole_region->FixParameter(20, 1.63);           // sigma

                    for(int jl = 0; jl < 5; ++jl) {
                        whole_region->FixParameter(18 + jl, BG_Par[ci+1][jl]);}        // pol4 background parameters

                    whole_region->SetParameter(23, 6); whole_region->SetParameter(24, 3.6); whole_region->SetParameter(25, 0.25);
                    whole_region->SetParLimits(23, 1,70); whole_region->FixParameter(24, fit_parameters[24]); whole_region->SetParLimits(25, 0.2,0.3);
                }
        whole_region->SetNpx(1000); // Increase number of points for smoother curve
        h_data_arr[ci]->Fit(whole_region, "R Q"); // fitting happens here
        whole_region->SetLineColor(kBlack);
        whole_region->SetLineWidth(2);
        whole_region->Draw("same");
        TText *text2 = new TText(10.0, 1.0, Form("RChi: %.2f", whole_region->GetChisquare()/whole_region->GetNDF()));  
        text2->Draw();
        cout<<"*******PS: REDUCED CHI SQUARE for region "<<ci+1<<": "<<whole_region->GetChisquare()/whole_region->GetNDF()<<endl;


        DrawFunctionsOnTop(whole_region);
            // TF1* bg_pol4 = new TF1("bg_pol4", "pol4", -1, 5.2);
            // bg_pol4->SetParameters(whole_region->GetParameter(18), whole_region->GetParameter(19), whole_region->GetParameter(20), whole_region->GetParameter(21), whole_region->GetParameter(22));
            // bg_pol4->SetLineColor(kBlack);
            // bg_pol4->SetLineWidth(2);
            // bg_pol4->Draw("same");
    }

    



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

}



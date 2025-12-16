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
    for(int fi = 0; fi < 5; ++fi){
        bc_pol4->SetParameter(fi, whole->GetParameter(fi));
    }
    bc_pol4->SetLineColor(kRed+1);
    bc_pol4->SetLineWidth(2);
    bc_pol4->Draw("same");


        TF1* peak1gfit = new TF1("peak1fit", gaus_plus_pol4, -1, 1, 8);
    peak1gfit->SetParameters(whole->GetParameter(5), whole->GetParameter(6), whole->GetParameter(7),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));
    peak1gfit->SetLineColor(kMagenta);
    peak1gfit->SetFillColor(kMagenta);
    peak1gfit->SetFillStyle(0); // No fill, just line
    peak1gfit->Draw("same");

        TF1* peak2gfit = new TF1("peak2fit", gaus_plus_pol4, -1, 2, 8);
    peak2gfit->SetParameters(whole->GetParameter(8), whole->GetParameter(9), whole->GetParameter(10),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));
    peak2gfit->SetLineColor(kGreen);
    peak2gfit->SetFillColor(kGreen);
    peak2gfit->SetFillStyle(0); // No fill, just line
    peak2gfit->Draw("same");

        TF1* peak3gfit = new TF1("peak3fit", gaus_plus_pol4, 0.8, 2.8, 8);
    peak3gfit->SetParameters(whole->GetParameter(11), whole->GetParameter(12), whole->GetParameter(13),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));
    peak3gfit->SetLineColor(kBlue);
    peak3gfit->SetFillColor(kBlue);
    peak3gfit->SetFillStyle(0); // No fill, just line
    peak3gfit->Draw("same");

        TF1* peak4gfit = new TF1("peak4fit", gaus_plus_pol4, 1.5, 3.5, 8);
    peak4gfit->SetParameters(whole->GetParameter(14), whole->GetParameter(15), whole->GetParameter(16),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));
    peak4gfit->SetLineColor(kOrange);
    peak4gfit->SetFillColor(kOrange);
    peak4gfit->SetFillStyle(0); // No fill, just line
    peak4gfit->Draw("same");

        TF1* peak5gfit = new TF1("peak5fit", gaus_plus_pol4, 1.8, 3.8, 8);
    peak5gfit->SetParameters(whole->GetParameter(17), whole->GetParameter(18), whole->GetParameter(19),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));
    peak5gfit->SetLineColor(kPink);
    peak5gfit->SetFillColor(kPink);
    peak5gfit->SetFillStyle(0); // No fill, just line
    peak5gfit->Draw("same");

        TF1* peak6gfit = new TF1("peak6fit", gaus_plus_pol4, 3.0, 5.0, 8);
    peak6gfit->SetParameters(whole->GetParameter(20), whole->GetParameter(21), whole->GetParameter(22),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));    
    peak6gfit->SetLineColor(kBrownCyan);
    peak6gfit->SetFillColor(kBrownCyan);
    peak6gfit->SetFillStyle(0); // No fill, just line
    peak6gfit->Draw("same");

          TF1* peak7gfit = new TF1("peak7fit", gaus_plus_pol4, 2.3, 4.3, 8);
    peak7gfit->SetParameters(whole->GetParameter(23), whole->GetParameter(24), whole->GetParameter(25),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));
    peak7gfit->SetLineColor(kYellow + 2);
    peak7gfit->SetFillColor(kYellow + 2);
    peak7gfit->SetFillStyle(0); // No fill, just line
    peak7gfit->Draw("same");


          TF1* peak8gfit = new TF1("peak8fit", gaus_plus_pol4, 0.5, 2.5, 8);
    peak8gfit->SetParameters(whole->GetParameter(26), whole->GetParameter(27), whole->GetParameter(28),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));
    peak8gfit->SetLineColor(kCyan + 2);
    peak8gfit->SetFillColor(kCyan + 2);
    peak8gfit->SetFillStyle(0); // No fill, just line
    peak8gfit->Draw("same");

            TF1* peak9gfit = new TF1("peak9fit", gaus_plus_pol4, 2.6, 4.6, 8);
    peak9gfit->SetParameters(whole->GetParameter(29), whole->GetParameter(30), whole->GetParameter(31),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));
    peak9gfit->SetLineColor(kGray + 2);
    peak9gfit->SetFillColor(kGray + 2);
    peak9gfit->SetFillStyle(0); // No fill, just line
    peak9gfit->Draw("same");

            TF1* peak10gfit = new TF1("peak10fit", gaus_plus_pol4, 3.8, 5.2, 8);
    peak10gfit->SetParameters(whole->GetParameter(32), whole->GetParameter(33), whole->GetParameter(34),
        whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(4));    
    peak10gfit->SetLineColor(kViolet + 2);
    peak10gfit->SetFillColor(kViolet + 2);
    peak10gfit->SetFillStyle(0); // No fill, just line
    peak10gfit->Draw("same");   

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
          legend->AddEntry(peak8gfit, Form("new peak @ %.0f", whole->GetParameter(27)*1000), "l");
        // legend->AddEntry(bcf, "landau", "l");
        //  legend->AddEntry(peak8f, "Response 8", "l");
        //   legend->Draw();         //Dont want to draw the legent right now:: PS
 /*        c3->SaveAs(Form("pdf/exfit_ang%dto%d.pdf", angmin, angmax));
        c3->SaveAs(Form("pngout/exfit_ang%dto%d.png", angmin, angmax));
        c3->SaveAs(Form("rootout/exfit_ang%dto%d.root", angmin, angmax)); */


}



using namespace std;
void response_fitPS_v9()
{
    const int angmin = 100;
    const int angmax = 160;
    const int projXmin = angmin - 100;
    const int projXmax = angmax - 100;

    TFile* file1 = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/simulation9mm/hhgs.root");
    TFile* file2 = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/simulation9mm/hh915.root");
    TFile* file3 = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/simulation9mm/hh1816.root");
    TFile* file4 = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/simulation9mm/hh2552.root");
    TFile* file5 = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/simulation9mm/hh2880.root");
    TFile* file6 = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/simulation9mm/hh4000.root");
    TFile* datafile = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/../hAllthetamore4kev.root");
    TFile* carbonfile = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/../hAllthetamore4kev_carbon2.root");
    TFile* Breakupfile = TFile::Open("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/roothistograms/Response_fit/../breakup_deuteron/phase_BU.root");

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
    Int_t totalparams = 5 + (3*6) + (3*4); // 5 for pol4, 6 simulated gaus, 2 new gaus
    Double_t fit_parameters[totalparams];
    cout<<"Simulation fit parameters:"<<endl;
    for (int ii = 0; ii < 6; ++ii) {
    TF1* fitFunc = peaks[ii]->GetFunction("gaus");
    if (fitFunc){
        double amplitude = fitFunc->GetParameter(0); // amplitude
        double mean = fitFunc->GetParameter(1);      // mean
        double sigma = fitFunc->GetParameter(2);     // sigma
        
        // Store fit parameters
        fit_parameters[5 + (ii * 3)] = amplitude;
        fit_parameters[5 + (ii * 3 + 1)] = mean;
        fit_parameters[5 + (ii * 3 + 2)] = sigma; // Adjusted sigma for better fit

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
    TF1* whole = new TF1("whole", allgaus_plus_pol4, -1.5, 5.2, totalparams);

    Double_t BG_Par[6][5];       // x index for 0-> total, 1-> region1, 2-> region2... 
                                        // y index for coefficients of pol4
    // BG_Par[0][0] = 4.6; BG_Par[0][1] = 4.9;  BG_Par[0][2] = 0.44; BG_Par[0][3] = 0.0; BG_Par[0][4] = 0.0; // for total with Rebin(2)
    BG_Par[0][0] = 8.0; BG_Par[0][1] = 6.3;  BG_Par[0][2] = 1.8; BG_Par[0][3] = 0.0; BG_Par[0][4] = 0.0; // for total with Rebin(4)
    BG_Par[1][0] = 1.7; BG_Par[1][1] = 0.84; BG_Par[1][2] = 0.27; BG_Par[1][3] = 0.0; BG_Par[1][4] = 0.0; // for region 1
    BG_Par[2][0] = 1.7; BG_Par[2][1] = 0.84; BG_Par[2][2] = 0.27; BG_Par[2][3] = 0.0; BG_Par[2][4] = 0.0; // for region 2
    BG_Par[3][0] = 1.7; BG_Par[3][1] = 0.84; BG_Par[3][2] = 0.27; BG_Par[3][3] = 0.0; BG_Par[3][4] = 0.0; // for region 3
    BG_Par[4][0] = 1.7; BG_Par[4][1] = 0.84; BG_Par[4][2] = 0.27; BG_Par[4][3] = 0.0; BG_Par[4][4] = 0.0; // for region 4
    BG_Par[5][0] = 1.7; BG_Par[5][1] = 0.84; BG_Par[5][2] = 0.27; BG_Par[5][3] = 0.0; BG_Par[5][4] = 0.0; // for region 5
    
    for(int li = 5; li < 23; ++li) {
        whole->SetParameter(li, fit_parameters[li]+0.01);}  
        
        //MORE BREATHING ROOM FOR THE 2nd peak*************************
        whole->SetParLimits(5,  1,200);   whole->SetParLimits(6,  fit_parameters[6] -0.01, fit_parameters[6]  +0.01);   whole->SetParLimits(7, 0.15, 0.35);
        whole->SetParLimits(8,  1,200);   whole->SetParLimits(9,  fit_parameters[9] -0.01, fit_parameters[9]  +0.01);   whole->SetParLimits(10,0.15, 0.35);
        whole->SetParLimits(11,  1,200);  whole->SetParLimits(12, fit_parameters[12]-0.01, fit_parameters[12] +0.01);  whole->SetParLimits(13,0.15, 0.35);
        whole->SetParLimits(14,  1,200);  whole->SetParLimits(15, 2.53, 2.57);  whole->SetParLimits(16,0.15, 0.35);
        whole->SetParLimits(17, 1,200);   whole->SetParLimits(18, 2.86, 2.9);  whole->SetParLimits(19,0.15, 0.35);
        whole->SetParLimits(20, 1,200);   whole->SetParLimits(21, fit_parameters[21]-0.01, fit_parameters[21] +0.01);  whole->SetParLimits(22,0.15, 0.35);

        // whole->FixParameter(18, 145);        // background amplitude
        // whole->FixParameter(19, 7.96);        // MPV
        // whole->FixParameter(20, 1.63);           // sigma
        for(int jl = 0; jl < 5; ++jl) {
            whole->FixParameter(jl, BG_Par[0][jl]);}        // pol4 background parameters

        whole->SetParameter(23, 6); whole->SetParameter(24, 3.3); whole->SetParameter(25, 0.25);
        whole->SetParLimits(23, 1,200); whole->SetParLimits(24, 3.3-0.2, 3.3+0.2); whole->SetParLimits(25, 0.15,0.3);

        whole->SetParameter(26, 20); whole->SetParameter(27, 1.5); whole->SetParameter(28, 0.25);
        whole->SetParLimits(26, 1,200); whole->SetParLimits(27, 1.5-0.2, 1.5+0.2); whole->SetParLimits(28, 0.15,0.3);

        whole->SetParameter(29, 6); whole->SetParameter(30, 3.7); whole->SetParameter(31, 0.25);
        whole->SetParLimits(29, 1,200); whole->SetParLimits(30, 3.7-0.2, 3.7+0.2); whole->SetParLimits(31, 0.15,0.3);

        whole->SetParameter(32, 20); whole->SetParameter(33, 4.8); whole->SetParameter(34, 0.25);
        whole->SetParLimits(32, 1,200); whole->SetParLimits(33, 4.8-0.2, 4.8+0.2); whole->SetParLimits(34, 0.15,0.3);
        


    all_ex->Fit(whole, "RQ"); // fitting happens here
    // whole ->SetNpx(1000); // Increase number of points for smoother curve
    TText *text1 = new TText(10.0, 1.0, Form("RChi: %.2f", whole->GetChisquare()/whole->GetNDF()));  
    text1->Draw();

    cout<<"*******PS: CHI SQUARE RECIEVED: "<<whole->GetChisquare()<<endl;
    cout<<"*******PS: NDF RECIEVED: "<<whole->GetNDF()<<endl;
    cout<<"*******PS: REDUCED CHI SQUARE: "<<whole->GetChisquare()/whole->GetNDF()<<endl;



    for(int il = 0; il < totalparams; ++il) {
        fit_parameters[il] = whole->GetParameter(il);
    }


    whole->SetLineColor(kBlack);
    whole->SetLineWidth(2);
    whole->Draw("same");

    cout<< "Fit parameters from excitation function: " << endl;
    for (int iil = 0; iil < 6; ++iil) {
        printf("Peak %d: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", iil+1, fit_parameters[5+(iil*3)], fit_parameters[5+(iil*3)+1], fit_parameters[5+(iil*3)+2]);
    }
    printf("Peak new 1: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", fit_parameters[23], fit_parameters[24], fit_parameters[25]);
    printf("Peak new 2: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", fit_parameters[26], fit_parameters[27], fit_parameters[28]);
    printf("Peak new 3: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", fit_parameters[29], fit_parameters[30], fit_parameters[31]);
    printf("Peak new 4: amplitude=%.2f, mean=%.2f, sigma=%.2f\n", fit_parameters[32], fit_parameters[33], fit_parameters[34]);
    printf("Background: %.2f + %.2f*x + %.2f*x^2 + %.2f*x^3 + %.2f*x^4\n\n",fit_parameters[0], fit_parameters[1], fit_parameters[2], fit_parameters[3], fit_parameters[4]);


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
    TF1* whole_region = new TF1(Form("whole_region%d", ci+1), allgaus_plus_pol4, -1.5, 5.2, totalparams);
    for(int ill = 0; ill < totalparams; ++ill) {
        whole_region->SetParameter(ill, fit_parameters[ill]);

        for(int jl = 0; jl < 5; ++jl) {
            whole_region->FixParameter(jl, BG_Par[ci+1][jl]);
        }        // pol4 background parameters
            
        whole_region->SetParLimits(5, 1, 200);   whole_region->SetParLimits(6,  fit_parameters[6]  -0.01,fit_parameters[6]  + 0.01);  whole_region->SetParLimits(7, fit_parameters[7] -0.02, fit_parameters[7] + 0.02);
        whole_region->SetParLimits(8, 1, 200);   whole_region->SetParLimits(9,  fit_parameters[9]  -0.01,fit_parameters[9]  + 0.01);  whole_region->SetParLimits(10,fit_parameters[10]-0.02, fit_parameters[10] + 0.02);
        whole_region->SetParLimits(11, 1, 200);  whole_region->SetParLimits(12, fit_parameters[12] -0.01,fit_parameters[12] + 0.01);  whole_region->SetParLimits(13,fit_parameters[13]-0.02, fit_parameters[13] + 0.02);
        whole_region->SetParLimits(14, 1, 200);  whole_region->SetParLimits(15, fit_parameters[15] -0.01,fit_parameters[15] + 0.01);  whole_region->SetParLimits(16,fit_parameters[16]-0.02, fit_parameters[16] + 0.02);
        whole_region->SetParLimits(17, 1,200);   whole_region->SetParLimits(18, fit_parameters[18] -0.01,fit_parameters[18] + 0.01);  whole_region->SetParLimits(19,fit_parameters[19]-0.02, fit_parameters[19] + 0.02);
        whole_region->SetParLimits(20, 1,200);   whole_region->SetParLimits(21, fit_parameters[21] -0.01,fit_parameters[21] + 0.01);  whole_region->SetParLimits(22,fit_parameters[22]-0.02, fit_parameters[22] + 0.02);
        whole_region->SetParLimits(23, 1,200);   whole_region->SetParLimits(24, fit_parameters[24] -0.01,fit_parameters[24] + 0.01);  whole_region->SetParLimits(25,fit_parameters[25]-0.02, fit_parameters[25] + 0.02);
        whole_region->SetParLimits(26, 1,200);   whole_region->SetParLimits(27, fit_parameters[27] -0.01,fit_parameters[27] + 0.01);  whole_region->SetParLimits(28,fit_parameters[28]-0.02, fit_parameters[28] + 0.02);   
        // whole_region->FixParameter(18, 145);        // background amplitude
        // whole_region->FixParameter(19, 7.96);        // MPV
        // whole_region->FixParameter(20, 1.63);           // sigma


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
    // h_data_arr[ci]->GetXaxis()->SetRangeUser(-1.5,1.5);
        // TF1* bg_pol4 = new TF1("bg_pol4", "pol4", -1, 5.2);
        // bg_pol4->SetParameters(whole_region->GetParameter(18), whole_region->GetParameter(19), whole_region->GetParameter(20), whole_region->GetParameter(21), whole_region->GetParameter(22));
        // bg_pol4->SetLineColor(kBlack);
        // bg_pol4->SetLineWidth(2);
        // bg_pol4->Draw("same");
    }

    



    double r1 = peak1g->Integral(peak1g->FindBin(-5 - whole->GetParameter(5+1)), peak1g->FindBin(20 - whole->GetParameter(5+1))) * whole->GetParameter(5+0) / 10000.0;
    double r2 = peak2g->Integral(peak2g->FindBin(-5 - whole->GetParameter(5+3)), peak2g->FindBin(20 - whole->GetParameter(5+3))) * whole->GetParameter(5+2) / 10000.0;
    double r3 = peak3g->Integral(peak3g->FindBin(-5 - whole->GetParameter(5+5)), peak3g->FindBin(20 - whole->GetParameter(5+5))) * whole->GetParameter(5+4) / 10000.0;
    double r4 = peak4g->Integral(peak4g->FindBin(-5 - whole->GetParameter(5+7)), peak4g->FindBin(20 - whole->GetParameter(5+7))) * whole->GetParameter(5+6) / 10000.0;
    double r5 = peak5g->Integral(peak5g->FindBin(-5 - whole->GetParameter(5+9)), peak5g->FindBin(20 - whole->GetParameter(5+9))) * whole->GetParameter(5+8) / 10000.0;
    double r6 = peak6g->Integral(peak6g->FindBin(-5 - whole->GetParameter(5+11)), peak6g->FindBin(20 - whole->GetParameter(5+11))) * whole->GetParameter(5+10) / 10000.0;
  
    // Output integrals
    printf("Integrals: R1=%.2f, R2=%.2f, R3=%.2f, R4=%.2f, R5=%.2f , R6=%.2f\n", r1, r2, r3, r4, r5, r6);

    // Save fit parameters and integrals to CSV
        FILE* csv = fopen(Form("csv/fit_results NC_ang%dto%d.csv", angmin, angmax), "w");
        if (csv) {
            fprintf(csv, "Parameter,Value");
            for (int i = 5; i < totalparams; ++i) {
                fprintf(csv, "par[%d],%f", i-5, whole->GetParameter(i));
            }
            fprintf(csv, "Integral,R1,R2,R3,R4,R5,R6");
            fprintf(csv, "Integral,%f,%f,%f,%f,%f,%f", r1, r2, r3, r4, r5, r6);
            fclose(csv);
        } else {
       printf("Error: Could not write CSV file.");
        }

}



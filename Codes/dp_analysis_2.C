

// #include "VDetector.h"
// #include "NPRootPlugin.h"
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2F.h>
#include"NPAnalysisFactory.h"
#include"NPDetectorManager.h"
#include"NPOptionManager.h"
#include"RootOutput.h"
#include"RootInput.h"
#include"TMust2Physics.h"
#include"TMugastPhysics.h"
#include"TCATSPhysics.h"
#include "TTACPhysics.h"
#include "TExogamPhysics.h"
#include "TZDDPhysics.h"
#include "NPReaction.h"
#include "NPEnergyLoss.h"
#include "TZDDPhysics.h"
#include <iostream>
#include <TVector.h>
#include <TVector3.h>
#include "TCutG.h"




using namespace std;
// fit_erf_line.C
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"

TChain* tree = NULL;

TCutG* cutg1 = NULL;
TCutG* cutg2 = NULL;
TCutG* cutgpl3 = NULL;
TCutG* cutgpl3_small = NULL;
TCutG* cutgpl4 = NULL;
TCutG* cut_dE_tof_cpl = NULL;
TCutG* cut_dE_tof_d4cats1 = NULL;

TCanvas* c1 = NULL;
TCanvas* c2 = NULL;
TCanvas* c3 = NULL;
TCanvas* c4 = NULL;
TCanvas* c5 = NULL;
TCanvas* c6 = NULL;
TCanvas* c7 = NULL;
TCanvas* c8 = NULL;
TCanvas* c9 = NULL;
TCanvas* c10 = NULL;
TCanvas* c11 = NULL;
TCanvas* c12 = NULL;
TCanvas* c13 = NULL;
TCanvas* c14 = NULL;
TCanvas* c15 = NULL;
TCanvas* c16 = NULL;


TH1F* h1 = NULL;
TH1F* h2 = NULL;
TH1F* h2_1 = NULL;
TH1F* h2_2 = NULL;

TH1F* h3 = NULL;
TH1F* h4 = NULL;
TH1F* h5 = NULL;
TH2F* h6 = NULL;
TH1F* h7 = NULL;
TH2F* h8 = NULL;
TH2F* h9 = NULL;
TH2F* h10 = NULL;
TH2F* h11 = NULL;
TH2F* h12 = NULL;
TH1F* h13 = NULL;
TH1F* h14 = NULL;
TH1F* h19 = NULL;
TH1F* h20 = NULL;
TH1F* h21 = NULL;
TH1F* h22 = NULL;
TH1F* h23 = NULL;
TH1F* h24 = NULL;
TH1F* h25 = NULL;
TH2F* h_ex_vs_thet = NULL;
TH2F* h_gamma_theta = NULL;
TH2F* h_gamma_gamma = NULL;

TH1F* h_exo = NULL;
TH2F* h_gamma_ex = NULL;
TH2F* hic_sum_V_tac_cpl = NULL;
TH2F* hic_sum_V_tac_d4_cats1 = NULL;
TH1F* hBeamE = NULL;

TH2F* h15 = NULL;

    TMust2Physics* M2;
    TMugastPhysics* MG;
    TCATSPhysics* CATS;
    TZDDPhysics* ZDD;
    //TZDDData* ZDDr;
    TExogamPhysics* Exogam;
    TTACPhysics* TAC;
    //TMugastPhysicsReader* MG_Reader;
    TMust2PhysicsReader* M2_Reader;

void loadCUTS(){
    TFile *cutfile = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGcats.root","READ");
    TFile *cutfile_drift = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGdrift.root","READ");
    TFile *cutPL3 = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGpl3.root","READ");
    TFile *cutPL3_small = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGpl3_small.root","READ");
    TFile *cutPl4 = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGpl4.root","READ");
    TFile *cut_cpl_dE_tof_file = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/cut_dE_TOF_cats_pl.root","READ");
    TFile *cut_d4cats1_dE_tof_file = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/cut_dE_TOF_d4_cats.root","READ");
    cutg1 = (TCutG*)cutfile->Get("CUTGcats");
    cutg2 = (TCutG*)cutfile_drift->Get("CUTG");
    cutgpl3 = (TCutG*)cutPL3->FindObjectAny("CUTG");
    cutgpl3_small = (TCutG*)cutPL3_small->FindObjectAny("CUTGpl3_small");
    cutgpl4 = (TCutG*)cutPl4->FindObjectAny("CUTGpl4");
    cut_dE_tof_cpl = (TCutG*)cut_cpl_dE_tof_file->FindObjectAny("tac_cats_pl");
    cut_dE_tof_d4cats1 = (TCutG*)cut_d4cats1_dE_tof_file->FindObjectAny("cut_d4_cats");
}


void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatA.txt.root");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatB.txt.root");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatC.txt.root");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatD.txt.root");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatE.txt.root");


}

    const double x1 = 579.0, x2 = 609.0;

    std::vector<std::pair<double,double>> holes = { {{579,609}} };

    Double_t bkgFunc(Double_t *x, Double_t *p){
    // Exclude bins in hole ranges
    for (auto &r : holes)
        if (x[0] >= r.first && x[0] <= r.second) {
        TF1::RejectPoint();
        return 0;
        }
    // Example: quadratic background
    return p[0] /* + p[1]*x[0] + p[2]*x[0]*x[0] */;
    }

        //cloning one projection on a separate canvas for checking

    void projections(){


    TCanvas *projections1 = new TCanvas("projections", "projections", 900,900);
    projections1->Divide(2,2);
    TCanvas *projections2 = new TCanvas("projections2", "projections2", 900,900);
    projections2->Divide(2,2);
    TCanvas *projections3 = new TCanvas("projections3", "projections3", 900,900);
    projections3->Divide(2,2);
    TCanvas *projections4 = new TCanvas("projections4", "projections4", 900,900);
    projections4->Divide(2,2);
    TCanvas *projections5 = new TCanvas("projections5", "projections5", 900,900);
    projections5->Divide(2,2);
    TCanvas *projections6 = new TCanvas("projections6", "projections6", 900,900);
    projections6->Divide(2,2);  
    TCanvas *projections7 = new TCanvas("projections7", "projections7", 900,900);
    projections7->Divide(2,2);
    TCanvas *projections8 = new TCanvas("projections8", "projections8", 900,900);
    projections8->Divide(2,2);
    TCanvas *projections9 = new TCanvas("projections9", "projections9", 900,900);
    projections9->Divide(2,2);
    TCanvas *projections10 = new TCanvas("projections10", "projections10", 900,900);
    projections10->Divide(2,2);

    for(int ii = 0; ii<8; ii++){
        for(int jj=0; jj<5; jj++){
            int index = ii*5 + jj;
            cout<<"Index: "<<index<<"... ";
            if(index < 4){
                projections1->cd(ii*5 + jj +1);}
            else if (index < 8){
                projections2->cd((index-4)+1);}
                else if (index < 12){
                    projections3->cd((index-8)+1);}
                else if (index <16){
                    projections4->cd((index-12)+1);}
                else if(index <20){
                    projections5->cd((index-16)+1);}
                else if(index <24){
                    projections6->cd((index-20)+1);}
                else if(index <28){
                    projections7->cd((index-24)+1);}
                else if(index <32){
                    projections8->cd((index-28)+1);}
                else if(index <36){
                    projections9->cd((index-32)+1);}
                else if(index <40){
                    projections10->cd((index-36)+1);}

            int bin_start = 91 + (ii*5 + jj);
            int bin_end = bin_start + 9;
            TString hist_name = Form("h_bin_%d_%d", bin_start, bin_end);
            cout<<"Creating projection for bins: "<<bin_start<<" to "<<bin_end<<" energy range: ";
            h_gamma_ex->ProjectionY(hist_name, bin_start, bin_end);
            TH1D *h_bin = (TH1D*)gDirectory->Get(hist_name);
            double ex_low = -1.0 + (ii*5 + jj)*0.1;
            double ex_high = ex_low + 1.0;
            cout<<"["<<ex_low<<", "<<ex_high<<"] MeV"<<endl;
            h_bin->SetTitle(Form("Projection Y for EXO Doppler for MG_Exnocor bin %.1f to %.1f MeV", ex_low, ex_high));
            h_bin->GetXaxis()->SetTitle("EXO Doppler Energy (keV)");
            h_bin->GetYaxis()->SetTitle("Counts");
            h_bin->Draw();
        }
    }
    }


    void fit_background_excluding_peaks() {
    TCanvas *projections_check1 = new TCanvas("projections_check1", "projections_check1", 800,600);
    projections_check1->Divide(2,2);
    projections_check1->cd(1);
    TH1D *hff = (TH1D*)gDirectory->Get("h_bin_91_100");
    hff->GetXaxis()->SetRangeUser(470,750);
    double Nobs = hff->Integral(145,152);
    double NobsErr = sqrt(Nobs);
    cout<<"Nobs: "<<Nobs<<endl;
    cout<<"NobsErr: "<<NobsErr<<endl;
    TF1 *fbkg = new TF1("fbkg", bkgFunc, 470, 750, 1);
    fbkg->SetParameters(0, 0, 0);  // seeds
    auto fr = hff->Fit(fbkg, "RL");             // "R" respects function range; RejectPoint excludes holes
    double B = fbkg->Integral(145,152);
    double Berr = fbkg->GetParError(0)*(152-145); // error propagation assuming linearity
    double S = Nobs - B;
    double Serr = sqrt(NobsErr*NobsErr + Berr*Berr);
    hff->Draw("E");
    fbkg->SetLineColor(kRed);
    fbkg->Draw("same");
    cout<<"Total counts: "<<S<<" +/- "<<Serr<<endl;

    projections_check1->cd(2);
    TH1D *hgg = (TH1D*)gDirectory->Get("h_bin_92_101");
    hgg->GetXaxis()->SetRangeUser(470,750);
    Nobs = hgg->Integral(145,152);
    NobsErr = sqrt(Nobs);
    cout<<"Nobs: "<<Nobs<<endl;
    cout<<"NobsErr: "<<NobsErr<<endl;
    TF1 *fbkg2 = new TF1("fbkg2", bkgFunc, 470, 750, 1);
    fbkg2->SetParameters(0, 0, 0);  // seeds
    auto fr2 = hgg->Fit(fbkg2, "RL");             // "R" respects function range; RejectPoint excludes holes
    B = fbkg2->Integral(145,152);
    Berr = fbkg2->GetParError(0)*(152-145); // error propagation assuming linearity
    S = Nobs - B;
    Serr = sqrt(NobsErr*NobsErr + Berr*Berr);
    hgg->Draw("E");
    fbkg2->SetLineColor(kRed);
    fbkg2->Draw("same");
    cout<<"Total counts: "<<S<<" +/- "<<Serr<<endl; 

    projections_check1->cd(3);
    TH1D *hhh = (TH1D*)gDirectory->Get("h_bin_93_102");
    hhh->GetXaxis()->SetRangeUser(470,750);
    Nobs = hhh->Integral(145,152);
    NobsErr = sqrt(Nobs);
    cout<<"Nobs: "<<Nobs<<endl;
    cout<<"NobsErr: "<<NobsErr<<endl;
    TF1 *fbkg3 = new TF1("fbkg3", bkgFunc, 470, 750, 1);
    fbkg3->SetParameters(0, 0, 0);  // seeds
    auto fr3 = hhh->Fit(fbkg3, "RL");             // "R" respects function range; RejectPoint excludes holes
    B = fbkg3->Integral(145,152);
    Berr = fbkg3->GetParError(0)*(152-145); // error propagation assuming linearity
    S = Nobs - B;   
    Serr = sqrt(NobsErr*NobsErr + Berr*Berr);
    hhh->Draw("E");
    fbkg3->SetLineColor(kRed);
    fbkg3->Draw("same");
    cout<<"Total counts: "<<S<<" +/- "<<Serr<<endl;

    projections_check1->cd(4);
    TH1D *hii = (TH1D*)gDirectory->Get("h_bin_94_103");
    hii->GetXaxis()->SetRangeUser(470,750);
    Nobs = hii->Integral(145,152);
    NobsErr = sqrt(Nobs);
    cout<<"Nobs: "<<Nobs<<endl;
    cout<<"NobsErr: "<<NobsErr<<endl;
    TF1 *fbkg4 = new TF1("fbkg4", bkgFunc, 470, 750, 1);
    fbkg4->SetParameters(0, 0, 0);  // seeds
    auto fr4 = hii->Fit(fbkg4, "RL");             // "R" respects function range; RejectPoint excludes holes    
    B = fbkg4->Integral(145,152);
    Berr = fbkg4->GetParError(0)*(152-145); // error propagation assuming linearity
    S = Nobs - B;
    Serr = sqrt(NobsErr*NobsErr + Berr*Berr);
    hii->Draw("E");
    fbkg4->SetLineColor(kRed);
    fbkg4->Draw("same");
    cout<<"Total counts: "<<S<<" +/- "<<Serr<<endl; 

    TCanvas *projections_check2 = new TCanvas("projections_check2", "projections_check2", 800,600);
    projections_check2->Divide(2,2);
    projections_check2->cd(1);
    TH1D *hjj = (TH1D*)gDirectory->Get("h_bin_95_104");
    hjj->GetXaxis()->SetRangeUser(470,750);
    Nobs = hjj->Integral(145,152);
    NobsErr = sqrt(Nobs);
    cout<<"Nobs: "<<Nobs<<endl;
    cout<<"NobsErr: "<<NobsErr<<endl;
    TF1 *fbkg5 = new TF1("fbkg5", bkgFunc, 470, 750, 1);
    fbkg5->SetParameters(0, 0, 0);  // seeds
    auto fr5 = hjj->Fit(fbkg5, "RL");             // "R" respects function range; RejectPoint excludes holes
    B = fbkg5->Integral(145,152);
    Berr = fbkg5->GetParError(0)*(152-145); // error propagation assuming linearity
    S = Nobs - B;
    Serr = sqrt(NobsErr*NobsErr + Berr*Berr);
    hjj->Draw("E");
    fbkg5->SetLineColor(kRed);
    fbkg5->Draw("same");
    cout<<"Total counts: "<<S<<" +/- "<<Serr<<endl; 

    projections_check2->cd(2);
    TH1D *hkk = (TH1D*)gDirectory->Get("h_bin_96_105");
    hkk->GetXaxis()->SetRangeUser(470,750);
    Nobs = hkk->Integral(145,152);
    NobsErr = sqrt(Nobs);
    cout<<"Nobs: "<<Nobs<<endl;
    cout<<"NobsErr: "<<NobsErr<<endl;   
    TF1 *fbkg6 = new TF1("fbkg6", bkgFunc, 470, 750, 1);
    fbkg6->SetParameters(0, 0, 0);  // seeds
    auto fr6 = hkk->Fit(fbkg6, "RL");             //
    B = fbkg6->Integral(145,152);
    Berr = fbkg6->GetParError(0)*(152-145); // error propagation assuming linearity
    S = Nobs - B;
    Serr = sqrt(NobsErr*NobsErr + Berr*Berr);
    hkk->Draw("E");
    fbkg6->SetLineColor(kRed);
    fbkg6->Draw("same");
    cout<<"Total counts: "<<S<<" +/- "<<Serr<<endl;

    projections_check2->cd(3);
    TH1D *hll = (TH1D*)gDirectory->Get("h_bin_97_106");
    hll->GetXaxis()->SetRangeUser(470,750);
    Nobs = hll->Integral(145,152);
    NobsErr = sqrt(Nobs);
    cout<<"Nobs: "<<Nobs<<endl;
    cout<<"NobsErr: "<<NobsErr<<endl;
    TF1 *fbkg7 = new TF1("fbkg7", bkgFunc, 470, 750, 1);
    fbkg7->SetParameters(0, 0, 0);  // seeds
    auto fr7 = hll->Fit(fbkg7, "RL");             //
    B = fbkg7->Integral(145,152);
    Berr = fbkg7->GetParError(0)*(152-145); // error propagation assuming linearity
    S = Nobs - B;
    Serr = sqrt(NobsErr*NobsErr + Berr*Berr);
    hll->Draw("E");
    fbkg7->SetLineColor(kRed);
    fbkg7->Draw("same");
    cout<<"Total counts: "<<S<<" +/- "<<Serr<<endl;

    projections_check2->cd(4);
    TH1D *hmm = (TH1D*)gDirectory->Get("h_bin_98_107");
    hmm->GetXaxis()->SetRangeUser(470,750   );
    Nobs = hmm->Integral(145,152);
    NobsErr = sqrt(Nobs);
    cout<<"Nobs: "<<Nobs<<endl;
    cout<<"NobsErr: "<<NobsErr<<endl;
    TF1 *fbkg8 = new TF1("fbkg8", bkgFunc, 470, 750, 1);
    fbkg8->SetParameters(0, 0, 0);  // seeds
    auto fr8 = hmm->Fit(fbkg8, "RL");             //
    B = fbkg8->Integral(145,152);
    Berr = fbkg8->GetParError(0)*(152-145); // error propagation assuming linearity
    S = Nobs - B;
    Serr = sqrt(NobsErr*NobsErr + Berr*Berr);
    hmm->Draw("E");
    fbkg8->SetLineColor(kRed);
    fbkg8->Draw("same");
    cout<<"Total counts: "<<S<<" +/- "<<Serr<<endl;

    }



void dp_analysis_2(){
    loadFILES();
    loadCUTS();
    cout<<"Total Entries: "<<tree->GetEntries()<<endl;


    CATS = new TCATSPhysics();
    MG = new TMugastPhysics();
    M2 = new TMust2Physics();
    ZDD = new TZDDPhysics();
    TAC = new TTACPhysics();
    Exogam = new TExogamPhysics();
    

    TTreeReader reader(tree);
    TTreeReaderArray<unsigned int> GATCONFMASTER_(reader, "GATCONF");
    TTreeReaderArray<unsigned long long> GATCONFMASTERTS_(reader, "GATCONFTS");
    TTreeReaderArray<double> mgexnocor(reader, "MG_Exnocor");
    TTreeReaderArray<double> mgex(reader, "MG_Ex");
    TTreeReaderArray<double> mg_lab(reader, "MG_ELab");
    TTreeReaderArray<double> mg_lab2(reader, "MG_ELab2");
    TTreeReaderValue<TVector3> BeamImpact(reader, "BeamImpact");
    // TTreeReaderArray<double> driftY(reader, "dy_C1_DC");
    TTreeReaderArray<unsigned int> Pl1(reader, "PL_E1");
    TTreeReaderArray<unsigned int> Pl2(reader, "PL_E2");
    TTreeReaderArray<unsigned int> Pl3(reader, "PL_E3");
    TTreeReaderArray<unsigned int> Pl4(reader, "PL_E4");
    TTreeReaderArray<unsigned int> Pl5(reader, "PL_E5");
    TTreeReaderArray<unsigned int> Pl6(reader, "PL_E6");
    TTreeReaderArray<unsigned int> Pl7(reader, "PL_E7");
    TTreeReaderArray<unsigned int> Pl8(reader, "PL_E8");
    TTreeReaderArray<unsigned int> Pl9(reader, "PL_E9");
    TTreeReaderArray<unsigned int> Pl10(reader, "PL_E10");
    TTreeReaderArray<double> tac_mmg_cats(reader, "TAC_MMG_CATS1");
    TTreeReaderArray<double> tac_cpl(reader, "TAC_CATS_PL");
    TTreeReaderArray<double> tac_mmg_exo(reader, "TAC_MMG_EXOGAM");
    TTreeReaderArray<double> tac_cats_exo(reader, "TAC_CATS_EXOGAM");
    TTreeReaderArray<double> tac_d4_cats1(reader, "TAC_D4_CATS1");


    TTreeReaderArray<unsigned int> IC1(reader, "IC_1");
    TTreeReaderArray<unsigned int> IC2(reader, "IC_2");
    TTreeReaderArray<unsigned int> IC3(reader, "IC_3");
    TTreeReaderArray<unsigned int> IC4(reader, "IC_4");
    TTreeReaderArray<unsigned int> IC5(reader, "IC_5");
    TTreeReaderArray<double> exo_doppler(reader, "EXO_Doppler_dp");
    TTreeReaderArray<double> theta(reader,"MG_ThetaLab");

    TTreeReaderArray<double> beam_energy(reader, "OriginalBeamEnergy");


    

    h1 = new TH1F("h1", "GATCONFTS", 1000, 0, 9000);
    h2_1 = new TH1F("h2_1", "MG_Exnocor", 1200, -10, 20);
    h2_2 = new TH1F("h2_2", "MG_Ex", 1200, -10, 20);
    h3 = new TH1F("h3", "MG_ELab", 1200, 0, 40);
    h4 = new TH1F("h4", "MG_ELab2", 1200, 0, 40);
    h5 = new TH1F("h5", "MG_conditioned with 1.0", 1200, -10, 20);
    h6 = new TH2F("h6", "BeamImpactY_vs_X", 200, -20, 20, 200, -20, 20);
    // h7 = new TH1F("h7", "driftY", 200, -10, 10);
    h8 = new TH2F("h8", "sqrt(PL_E1*PL_E6) vs log(PL_E1/PL_E6)", 1000, -1,1, 1500, 0, 15000);
    h9 = new TH2F("h9", "sqrt(PL_E2*PL_E7) vs log(PL_E2/PL_E7)", 1000, -1,1, 1500, 0, 15000);
    h10 = new TH2F("h10", "sqrt(PL_E3*PL_E8) vs log(PL_E3/PL_E8)", 1000, -1,1, 1500, 0, 15000);
    h11 = new TH2F("h11", "sqrt(PL_E4*PL_E9) vs log(PL_E4/PL_E9)", 1000, -1,1, 1500, 0, 15000);
    h12 = new TH2F("h12", "sqrt(PL_E5*PL_E10) vs log(PL_E5/PL_E10)", 1000, -1,1, 1500, 0, 15000);
    h13 = new TH1F("h13", "TAC_MMG_CATS1", 8000, 0, 2000);
    h14 = new TH1F("h14", "IC3_Energy", 5000, 0, 10000);
    h19 = new TH1F("h19", "MG_conditioned_with 1.2", 1200, -10, 20);
    h20 = new TH1F("h20", "MG_conditioned_with IC", 1200, -10, 20);
    h21 = new TH1F("h21", "MG_conditioned_with TAC_CATS_PL", 1200, -10, 20);
    h22 = new TH1F("h22", "MG_conditioned_with TAC_CATS_PL w/ and w/o IC cut", 1200, -10, 20);
    h23 = new TH1F("h23", "MG_conditioned_with Pl3 small", 1200, -10, 20);
    h24 = new TH1F("h24", "MG_conditioned_with Pl4", 1200, -10, 20);
    h25 = new TH1F("h25", "MG_conditioned_with TAC_CATS_PL w/o Beam and IC cut", 1200, -10, 20);

    h_exo = new TH1F("h_exo", "EXO_Doppler", 8000, 0, 4000);
    h_gamma_ex = new TH2F("h_gamma_ex", "EXO_Doppler_vs_MG_Exnocor", 1200, -10, 20, 8000, 0, 4000);
    hic_sum_V_tac_cpl = new TH2F("hic_sum_V_tac_cpl", "IC_Sum_vs_TAC_CATS_PL", 500,340,360, 1000, 5000, 10000);
    hic_sum_V_tac_d4_cats1 = new TH2F("hic_sum_V_tac_d4_cats1", "IC_Sum_vs_TAC_D4_CATS1", 5000,290,340, 1000, 5000, 10000);
    hBeamE = new TH1F("hBeamE", "Beam_Energy", 200, 800, 1200);
    h_ex_vs_thet = new TH2F("h_ex_vs_thet", "MG_Exnocor_vs_ThetaLab", 80, 100, 180, 1200, -10, 20);
    h_gamma_theta = new TH2F("h_gamma_theta", "EXO_Doppler_vs_ThetaLab", 80, 100, 180, 8000, 0, 4000);
    h_gamma_gamma = new TH2F("h_gamma_gamma", "EXO_Doppler1_vs_EXO_Doppler2", 1000, 0, 4000, 1000, 0, 4000);




    while (reader.Next()) {
        Long64_t t1;
        Double_t t2_1;
        Double_t t3;
        Double_t t5[10];
        Double_t tMC;
        Double_t t7[5];
        Double_t tCPl;
        Double_t gamma_energy;

        Double_t gamma_energy2;
        Double_t tME;
        Double_t tCE;
        Double_t thet;
        Double_t tdc;
        Double_t beamE;

        if(GATCONFMASTER_[0] == 1){
            // cout<<"GATCONFMASTER_: "<<GATCONFMASTER_[0]<<endl;
            // t1 = GATCONFMASTERTS_[0]/1E10;
            // h1->Fill(t1);
            if(mgexnocor.GetSize()>0){
                t2_1 = mgexnocor[0];
                h2_1->Fill(t2_1);
            }

            if ( mg_lab.GetSize()>0){
                t3 = mg_lab[0];
                h3->Fill(t3);
            }

            const TVector3& v = *BeamImpact;
            double beamX = v.X();
            double beamY = v.Y();
            double beamZ = v.Z();

            
            t5[0] = Pl1.GetSize()>0 ? Pl1[0] : 0;
            t5[1] = Pl2.GetSize()>0 ? Pl2[0] : 0;
            t5[2] = Pl3.GetSize()>0 ? Pl3[0] : 0;
            t5[3] = Pl4.GetSize()>0 ? Pl4[0] : 0;
            t5[4] = Pl5.GetSize()>0 ? Pl5[0] : 0;
            t5[5] = Pl6.GetSize()>0 ? Pl6[0] : 0;
            t5[6] = Pl7.GetSize()>0 ? Pl7[0] : 0;
            t5[7] = Pl8.GetSize()>0 ? Pl8[0] : 0;
            t5[8] = Pl9.GetSize()>0 ? Pl9[0] : 0;
            t5[9] = Pl10.GetSize()>0 ? Pl10[0] : 0; 

            h8->Fill(log(t5[0]/t5[5]), sqrt(t5[0]*t5[5]));
            h9->Fill(log(t5[1]/t5[6]), sqrt(t5[1]*t5[6]));
            h10->Fill(log(t5[2]/t5[7]), sqrt(t5[2]*t5[7]));
            h11->Fill(log(t5[3]/t5[8]), sqrt(t5[3]*t5[8]));
            h12->Fill(log(t5[4]/t5[9]), sqrt(t5[4]*t5[9]));

            tCPl = tac_cpl.GetSize()>0 ? tac_cpl[0] : 0;
            tMC = tac_mmg_cats.GetSize()>0 ? tac_mmg_cats[0] : 0;
            tCE = tac_cats_exo.GetSize()>0 ? tac_cats_exo[0] : 0;
            tME = tac_mmg_exo.GetSize()>0 ? tac_mmg_exo[0] : 0;
            thet = theta.GetSize()>0 ? theta[0] : 0;
            tdc = tac_d4_cats1.GetSize()>0 ? tac_d4_cats1[0] : 0;
            // h13->Fill(tMC);  

            t7[0] = IC1.GetSize()>0 ? IC1[0] : 0;
            t7[1] = IC2.GetSize()>0 ? IC2[0] : 0;
            t7[2] = IC3.GetSize()>0 ? IC3[0] : 0;
            t7[3] = IC4.GetSize()>0 ? IC4[0] : 0;
            t7[4] = IC5.GetSize()>0 ? IC5[0] : 0;

            gamma_energy = exo_doppler.GetSize()>0 ? exo_doppler[0] : 0;

            //more size gamma_energy for gamma-gamma coincidence
            gamma_energy2 = exo_doppler.GetSize()>1 ? exo_doppler[1] : 0;

            if(gamma_energy>0 && gamma_energy2>0){
                h_gamma_gamma->Fill(gamma_energy*1000, gamma_energy2*1000);
            }

            beamE = beam_energy.GetSize()>0 ? beam_energy[0] : 0;
            // if(cut_dE_tof_cpl->IsInside(tCPl, (t7[0]+t7[1]+t7[2]+t7[3]+t7[4]))){
            //     hic_sum_V_tac_cpl->Fill(tCPl,(t7[0]+t7[1]+t7[2]+t7[3]+t7[4]));
            // }
            // if(cut_dE_tof_d4cats1->IsInside(tdc, (t7[0]+t7[1]+t7[2]+t7[3]+t7[4]))){
            //     hic_sum_V_tac_d4_cats1->Fill(tdc,(t7[0]+t7[1]+t7[2]+t7[3]+t7[4]));
            // }


            h14->Fill(t7[2]);   //filling IC3 energy



            // if(beamE>900){
            //     hBeamE->Fill(beamE);
            // }



            // if(t3>1.0){                                                         //cut on MG_ELab is 1.0
            //     if(cutgpl3->IsInside(log(t5[2]/t5[7]),sqrt(t5[2]*t5[7]))){      //cut on PL3
            //         if(cutg1->IsInside(beamX, beamY)){                          //cut on Beam spot
            //             if(abs(tMC-387)<25){                                     //cut on TAC_MMG_CATS1
            //                 if(t7[2]>0){                                        //cut on IC3 energy
            //                     h5->Fill(t2_1);
            //                 }
            //             }
            //         }
            //     }
            // }

            // if(t3>1.0){                                                         //cut on MG_ELab
            //     if(tCPl>300 && tCPl<400){                                           //cut on TAC_CATS_PL                
            //         if(cutg1->IsInside(beamX, beamY)){                          //cut on Beam spot
            //             if(abs(tMC-387)<25){                                     //cut on TAC_MMG_CATS1
            //                 if(t7[2]>0){                                        //cut on IC3 energy
            //                     h21->Fill(t2_1);
            //                 }
            //             }
            //         }
            //     }
            // }

            if(t3>1.0){                                 //cut on MG_ELab                             
                // if(beamE>900){                              //cut on Beam Energy
                    if(tCPl>300 && tCPl<400){                     //cut on TAC_CATS_PL     
                        if(abs(tMC-387)<25){                            //cut on TAC_MMG_CATS1
                            // if(abs(tME-432)<30){                            //cut on TAC_MMG_EXOGAM                        
                                // if(abs(tCE - 440)<20){                         //cut on TAC_CATS_EXOGAM
                                    // if(t7[2]>0){                                     //cut on IC3 energy
                                        // if(thet>110 && thet<120){    
                                            // if(cutg1->IsInside(beamX, beamY)){                  //cut on Beam spot
                                                // if(cut_dE_tof_cpl->IsInside(tCPl, (t7[0]+t7[1]+t7[2]+t7[3]+t7[4]))){  //cut on TAC_CATS_PL vs IC sum
                                                //     if(cut_dE_tof_d4cats1->IsInside(tdc, (t7[0]+t7[1]+t7[2]+t7[3]+t7[4]))){ //cut on TAC_D4_CATS1 vs IC sum  
                                                        h25->Fill(t2_1);                                
                                                        h_ex_vs_thet->Fill(thet, t2_1);
                                                        if(gamma_energy>0){
                                                            h_exo->Fill(gamma_energy*1000);                 //FILLING EXO_Doppler
                                                            h_gamma_ex -> Fill(t2_1, gamma_energy*1000);
                                                            h_gamma_theta->Fill(thet, gamma_energy*1000);
                                                            // h_gamma_gamma->Fill(gamma_energy*1000, gamma_energy2*1000);
                                                        }
                                                    // }
                                                // }
                                            // }
                                        // }
                                    // }
                                // }
                            // }
                        }
                    }
                // }
            }

            // if(t3>1.0){                                                         //cut on MG_ELab
            //     if(tCPl>300 && tCPl<400){                                           //cut on TAC_CATS_PL                
            //         if(cutg1->IsInside(beamX, beamY)){                          //cut on Beam spot
            //             if(abs(tMC-387)<25){                                     //cut on TAC_MMG_CATS1
            //                 h22->Fill(t2_1);
            //             }
            //         }
            //     }
            // }

            // if(t3>1.0){                                                         //cut on MG_ELab
            //     if(tCPl>300 && tCPl<400){                                           //cut on TAC_CATS_PL                
            //         if(cutg1->IsInside(beamX, beamY)){                          //cut on Beam spot
            //             if(abs(tMC-387)<25){                                     //cut on TAC_MMG_CATS1
            //                 if(cutgpl3_small->IsInside(log(t5[2]/t5[7]),sqrt(t5[2]*t5[7]))){      //cut on PL3 small
            //                     h23->Fill(t2_1);
            //                 }
            //             }
            //         }
            //     }
            // }

            // if(t3>1.0){                                                         //cut on MG_ELab
            //     if(tCPl>300 && tCPl<400){                                           //cut on TAC_CATS_PL                
            //         if(cutg1->IsInside(beamX, beamY)){                          //cut on Beam spot
            //             if(abs(tMC-387)<25){                                     //cut on TAC_MMG_CATS1
            //                 if(cutgpl4->IsInside(log(t5[3]/t5[8]), sqrt(t5[3]*t5[8]))){      //cut on PL3 small
            //                     h24->Fill(t2_1);
            //                 }
            //             }
            //         }
            //     }
            // }

/* 
            if(t3>1.2){                                                         //cut on MG_ELab is 1.2
                if(cutgpl3->IsInside(log(t5[2]/t5[7]),sqrt(t5[2]*t5[7]))){      //cut on PL3
                    if(cutg1->IsInside(beamX, beamY)){                          //cut on Beam spot
                        if(abs(tMC-387)<25){                                     //cut on TAC_MMG_CATS1
                            if(t7[2]>0){                                        //cut on IC3 energy
                                h19->Fill(t2_1);
                            }
                        }
                    }
                }
            }

            if(t3>1.2){                                                         //cut on MG_ELab
                if(cutgpl3->IsInside(log(t5[2]/t5[7]),sqrt(t5[2]*t5[7]))){      //cut on PL3
                    if(cutg1->IsInside(beamX, beamY)){                          //cut on Beam spot
                        if(abs(tMC-387)<25){                                     //cut on TAC_MMG_CATS1
                                h20->Fill(t2_1);
                        }
                    }
                }
            } */



        }
    }
    // c1 = new TCanvas("c1", "c1", 800, 600);
    // c1->Divide(1,2);
    // c1->cd(1);
    // h2_1->Draw();
    // c1->cd(2);
    // h2_2->Draw();
    // c2 = new TCanvas("c2", "c2", 800, 600);
    // c2->Divide(1,2);
    // c2->cd(1);
    // h3->Draw();
    // c2->cd(2);
    // h4->Draw(); 
    // c3 = new TCanvas("c3", "ex with IC, TAC_CATS_PL, MMG_CATS, Beam_Impact, Elab>1.0", 800, 600);
    // h5->Draw();
    // // h19->Draw("same");

    // // c4 = new TCanvas("c4", "c4", 800, 600);
    // // h6->Draw("COLZ");
    // // cutg2->Draw("same");

    // // c5 = new TCanvas("c5", "c5", 800, 600);
    // // h7->Draw();

    // c6 = new TCanvas("c6", "beam impact", 800, 600);
    // c6->Divide(2,3);
    // c6->cd(1);
    // h8->Draw("COLZ");
    // c6->cd(2);
    // h9->Draw("COLZ");
    // c6->cd(3);
    // h10->Draw("COLZ");
    // cutgpl3->Draw("same");
    // cutgpl3_small->SetLineColor(kRed);
    // cutgpl3_small->Draw("same");
    // c6->cd(4);
    // h11->Draw("COLZ");
    // cutgpl4->SetLineColor(kRed);
    // cutgpl4->Draw("same");
    // c6->cd(5);
    // h12->Draw("COLZ");
    // c7 = new TCanvas("c7", "tac_mmg_cats", 800, 600);
    // h13->Draw();

    // c8 = new TCanvas("c8", "IC_3 energy", 800, 600);
    // h14->Draw();

  /*   c9 = new TCanvas("c9", "c9", 800, 600);
    // h19->Rebin(4);
    // h20->Rebin(4);
    h20->Draw();
    h19->Draw("same"); */

    // c10 = new TCanvas("c10", "comparison of Pl_energy cut vs tac_cats_pl cut", 800, 600);
    // h21->Draw();
    // h5->Draw("same");
    
    // c11 = new TCanvas("c11", "comparison of tac_cats_pl cut w/ and w/o IC cut", 800, 600);
    // h22->Draw();
    // h21->Draw("same");
    
    // c12 = new TCanvas("c12", "comparison of Pl3 cut small", 800, 600);
    // h24->Rebin(4);
    // h23->Rebin(4);
    // h24->Draw();
    // h24->SetLineColor(kRed);
    // h23->Draw("same");

    c13 = new TCanvas("c13", "without Beam and IC cut", 800, 600);
    // h25->Rebin(4);
    h25->Draw();
    // h21->SetLineColor(kRed);
    // h21->Rebin(4);
    // h21->Draw("same");

    c14 = new TCanvas("c14", "EXO Doppler", 800, 600);
    h_exo->Draw();


    c15 = new TCanvas("c15", "EXO Doppler vs excitation", 800, 600); 
    h_gamma_ex->Rebin2D(4,8);
    h_gamma_ex->Draw("COLZ");

    //scanning MG_Exnocor bins of 2MeV each starting from (-2.0 MeV - 0.0 MeV) to (1.8 MeV - 3.8 MeV) which translates to bin numbers [81-100] to [119-138]



    TCanvas *theta_ex = new TCanvas("theta_ex", "theta_ex", 800,600);
    theta_ex->Divide(2,1);
    theta_ex->cd(1);
    h_ex_vs_thet->Draw("COLZ");
    theta_ex->cd(2);
    h_gamma_theta->Draw("COLZ");
    TCanvas *gamma_gamma = new TCanvas("gamma_gamma", "gamma_gamma", 800,600);
    gamma_gamma->Divide(2,1);
    gamma_gamma->cd(1);

    h_gamma_gamma->Draw("COLZ");

    projections();
    fit_background_excluding_peaks();

    // projections->cd(1);
    // h_gamma_ex->ProjectionY("h_bin_100_119",100,119);
    // TH1D *h_bin_100_119 = (TH1D*)gDirectory->Get("h_bin_100_119");
    // h_bin_100_119->SetTitle("Projection Y for EXO Doppler for MG_Exnocor bin -0.1 to 1.9 MeV");
    // h_bin_100_119->GetXaxis()->SetTitle("EXO Doppler Energy (keV)");
    // h_bin_100_119->GetYaxis()->SetTitle("Counts");
    // h_bin_100_119->Draw();

    // TCanvas *cdetof = new TCanvas("cdetof", "cdetof", 800,600);
    // cdetof->Divide(2,1);
    // cdetof->cd(1);
    // hic_sum_V_tac_cpl->Draw("COLZ");
    // cdetof->cd(2);
    // hic_sum_V_tac_d4_cats1->Draw("COLZ");
    
    // TCanvas *cBeamE = new TCanvas("cBeamE", "cBeamE", 800,600);
    // hBeamE->Draw();
    
}
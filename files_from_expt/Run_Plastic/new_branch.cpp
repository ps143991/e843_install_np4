#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TBranch.h>
#include <iostream>

using namespace std;
int new_branch(){
TFile *file1 = TFile::Open("r0031_000a.root");
TTree *ad = (TTree*)file1->Get("AD");

TFile *file2 = TFile::Open("r0031_000_ps.root", "RECREATE");
TTree *ps = new TTree ("ps","all_pl_events");

UShort_t TAC_PL1_var, TAC_PL2_var, TAC_PL3_var, TAC_PL4_var, TAC_PL5_var;
ad->SetBranchAddress("TAC_PL_1", &TAC_PL1_var);
ad->SetBranchAddress("TAC_PL_2", &TAC_PL2_var);
ad->SetBranchAddress("TAC_PL_3", &TAC_PL3_var);
ad->SetBranchAddress("TAC_PL_4", &TAC_PL4_var);
ad->SetBranchAddress("TAC_PL_5", &TAC_PL5_var);

UShort_t TAC_PL1all_var, TAC_PL2all_var, TAC_PL3all_var, TAC_PL4all_var, TAC_PL5all_var;
ps->Branch("TAC_PL_1_all", &TAC_PL1all_var, "TAC_PL_1_all/s");
ps->Branch("TAC_PL_2_all", &TAC_PL2all_var, "TAC_PL_2_all/s");
ps->Branch("TAC_PL_3_all", &TAC_PL3all_var, "TAC_PL_3_all/s");
ps->Branch("TAC_PL_4_all", &TAC_PL4all_var, "TAC_PL_4_all/s");
ps->Branch("TAC_PL_5_all", &TAC_PL5all_var, "TAC_PL_5_all/s");

TH1F *htpl1all = new TH1F("htpl1all", "allEVENTpl1", 10000,17500,27500);
TH1F *htpl2all = new TH1F("htpl2all", "allEVENTpl2", 10000,17500,27500);
TH1F *htpl3all = new TH1F("htpl3all", "allEVENTpl3", 10000,17500,27500);
TH1F *htpl4all = new TH1F("htpl4all", "allEVENTpl4", 10000,17500,27500);
TH1F *htpl5all = new TH1F("htpl5all", "allEVENTpl5", 10000,17500,27500);

Long64_t Nevents= ad->GetEntries();
for (Long64_t i = 0; i<Nevents; i++){
    ad->GetEntry(i);
    if(TAC_PL1_var>1 && TAC_PL2_var>1 && TAC_PL3_var>1 && TAC_PL4_var>1 && TAC_PL5_var>1){
        TAC_PL1all_var=TAC_PL1_var;
        TAC_PL2all_var=TAC_PL2_var;
        TAC_PL3all_var=TAC_PL3_var;
        TAC_PL4all_var=TAC_PL4_var;
        TAC_PL5all_var=TAC_PL5_var;
        ps->Fill();
        htpl1all->Fill(TAC_PL1all_var);
        htpl2all->Fill(TAC_PL2all_var);
        htpl3all->Fill(TAC_PL3all_var);
        htpl4all->Fill(TAC_PL4all_var);
        htpl5all->Fill(TAC_PL5all_var);
    }   

}
ps->Write();
htpl1all->Write();
htpl2all->Write();
htpl3all->Write();
htpl4all->Write();
htpl5all->Write();
file1->Close();
file2->Close();
return 0;
}

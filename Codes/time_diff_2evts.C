#include <iostream>
#include <TFile.h>
#include "NPApplication.h"
#include <TChain.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>


using namespace std;
using namespace nptool;

TChain *chain = NULL; 
TH1F *hist = new TH1F("hist","hist",10000,0,1E6);
unsigned long long timestamp1;


void loadFILES(){
    chain = new TChain("PhysicsTree");
    chain -> Add("../output/analysis/run_518_0.root");
    cout<<"Total Entries: "<<chain->GetEntries()<<endl;
    chain->SetBranchAddress("GATCONFTS", &timestamp1);
}
void time_diff_2evts(){

    
    loadFILES();
    double time_difference[chain->GetEntries()];
    TCanvas *c1 = new TCanvas;
    //chain -> Draw("GATCONFTS/1E12>>hgcts(1E8,12.5,12.7)","GATCONF == 1", "");
    for (int i = 0; i<chain->GetEntries()-1; i++){
        chain ->GetEntry(i);
        unsigned long long prev_event = timestamp1;
        // cout<<"the timestamp is: "<<prev_event<<"  ";
        chain->GetEntry(i+1);
        unsigned long long next_event = timestamp1;
        // cout<<"the timestamp is: "<<next_event<<endl;

        time_difference[i] = static_cast<double>((next_event - prev_event)/100.0);
        // cout<<"time difference is: "<<time_difference[i]<<endl;

        
        hist->Fill(time_difference[i]);


        

    }
        hist->Draw();
        c1->Update();

}
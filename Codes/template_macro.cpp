#include "NPApplication.h"
using namespace nptool;
#include "NPDetectorManager.h"
#include "NPFunction.h"
#include "ZddPhysics.h"
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

#include <iostream>
using namespace zdd;
using namespace std;

TChain* tree = NULL;
TCanvas* c1 = NULL;

void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("../../data/analysed/558.root");
}

void template_macro(){
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    cout<<"Total Entries: "<<tree->GetEntries()<<endl;
    TTreeReader reader(tree);
    TTreeReaderValue<ZddPhysics> phy_zdd_r(reader, "zdd");

    TTreeReaderValue<unsigned int> GATCONF_r(reader, "GATCONF");
    TTreeReaderValue<unsigned long long> GATCONFTS_r(reader, "GATCONFTS");
    TTreeReaderArray<unsigned long long> DC_TS1_r(reader,"DC_TS1");
    c1 = new TCanvas("c1","c1",800,600);


    for(int nentry = 0; nentry < tree->GetEntries(); nentry++){
        tree->GetEntry(nentry);
        cout<<"Processing entry: "<<nentry<<endl;   

    }

    c1->Update();
}
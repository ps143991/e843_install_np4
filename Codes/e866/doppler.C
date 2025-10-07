#include "NPApplication.h"
// #include
// "/Users/valerian/Software/Analysis/e870/e870_analysis/plugins/exogam/src/Geometry_Clover_Exogam.h"
#include "ExogamDetector.h"
#include "ExogamPhysics.h"
#include "NPDetectorManager.h"
#include "NPFunction.h"
#include "NPReaction.h"
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TTreeReaderArray.h>

#include <iostream>

using namespace exogam;
TChain* tree= new TChain("AD");
TH1F* h=new TH1F("h","gamma",2000,0,2000);


void LoadFiles(){
    
    // Run 34Si
    // tree->Add("./output/analysis/run_0120_0.root");
    // tree->Add("./output/analysis/run_0121_0.root");
    
    // Run 36S
    tree->Add("/home/sharmap/Workplace/np4/e843/data/e866_roota/r0151_000a.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/e866_roota/r0152_000a.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/e866_roota/r0153_000a.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/e866_roota/r0154_000a.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/e866_roota/r0155_000a.root");


}

void doppler() {
  std::cout << "Init dummy application to use the detectors" << std::endl;
  auto app = nptool::Application::InitApplication("");
    // Initialize TTreeReader

    LoadFiles();
    cout<<"Total number of entries: "<<tree->GetEntries()<<endl;
  TTreeReader reader(tree);

  TTreeReaderArray<float> cle(reader, "ClE_AB");

  while (reader.Next()) {
    if (reader.GetCurrentEntry() % 100000 == 0) {
      cout << (double)reader.GetCurrentEntry() / tree->GetEntries() * 100.
           << endl;
    }
    if(cle.GetSize()>0){
        h->Fill(cle[0]);
    }
  }

  h->Draw();

}
// debug_cats_reco.C
#include "NPApplication.h"
using namespace nptool;
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <cmath>
#include <limits>
#include "CatsPhysics.h" // make sure the dictionary is loaded
#include "CatsDetector.h"
#include "TFile.h"
#include "TKey.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TChain.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "TLine.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
using namespace std;
using namespace cats;
TChain* tree = NULL;
TCanvas* c1 = NULL;
TCanvas *cmask = NULL;
TCanvas *ccats = NULL;

void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("../data/analysed/FIXING_ACTION_FILE/523_old_calib.root");
}

// double AnalyticHyperbolicSecant(int det)

void cats_charge_recon(){
    ///////////////////////////////////////////////////////main functions////////////////////////////////////////////////////////
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    Long64_t tot_entries= tree->GetEntries();
    cout<<"Total Entries: "<<tot_entries<<endl;

    TTreeReader reader(tree);
    CatsDetector* det_cats_r = new CatsDetector();
    TTreeReaderValue<CatsPhysics> phy_cats_r(reader, "cats");
    TTreeReaderValue<CatsData> raw_cats_r(reader, "cats");
    // TTreeReaderValue<CatsDetector> det_cats_r(reader, "cats");

    ///////////////////////////////////////////////////////output file def////////////////////////////////////////////////////////

    TFile* outputFile = new TFile("./../data/analysed/build_traj_macro_Output.root", "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        outputFile->Close();
        return;
    }
    TTree* outputTree = new TTree("t", "Tree");


    while(reader.Next()){
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }

        

        const auto& det  = phy_cats_r->DetNumber;
        const auto& posX = phy_cats_r->PositionX;
        const auto& cDetX= phy_cats_r->CalDetNumberX;
        const auto& cStrX= phy_cats_r->CalStripNumberX;   // same index as CalChargeX
        const auto& cQX  = phy_cats_r->CalChargeX;

        if(det.size() == 2){
        const auto& mapx = det_cats_r->MapX;
        const auto& maxQx = det_cats_r->MaxQX;
        // const auto& ASECH= det_cats_r->AnalyticHyperbolicSecant(maxQx[1], mapx, 0.0);
        

        //MaxQX is a map with key = det number, value = pair(strip,charge)
        //will plot in a TGraph the charge vs strip number
        TGraph* gQvsStrip[2];
        gQvsStrip[0] = new TGraph();
        gQvsStrip[1] = new TGraph();
        for(size_t i=0; i<cDetX.size(); i++){
            int d = cDetX[i];
            int s = cStrX[i];
            int q = cQX[i];
            gQvsStrip[d-1]->SetPoint(gQvsStrip[d-1]->GetN(), s, q);
        } 
        gQvsStrip[0]->SetMarkerStyle(20);
        gQvsStrip[1]->SetMarkerStyle(20);
        gQvsStrip[0]->SetTitle("CATS1 Calibrated Charge vs Inverted (if any) Strip");
        gQvsStrip[1]->SetTitle("CATS2 Calibrated Charge vs Inverted (if any) Strip");
        gQvsStrip[0]->GetXaxis()->SetTitle("Strip Number");
        gQvsStrip[0]->GetYaxis()->SetTitle("Charge");
        gQvsStrip[1]->GetXaxis()->SetTitle("Strip Number");
        gQvsStrip[1]->GetYaxis()->SetTitle("Charge"); 

        double_t PosX1;
        double_t PosX2;
        //also indicate the PositionX on the graph made by phy_cats_r->PositionX
        if (det[0] == 1 || det[1] == 1) {PosX1 = posX[0]; cout<<"PositionX CATS1: "<<PosX1<<endl;}
        if (det[0] == 2 || det[1] == 2) {PosX2 = posX[0]; cout<<"PositionX CATS2: "<<PosX2<<endl;}
        
        //remaining part
        
      
        // gQvsStrip[0]->GetListOfFunctions()->Add(line1);
        // gQvsStrip[1]->GetListOfFunctions()->Add(line2);

        c1 = new TCanvas("c1", "CATS Charge vs Strip", 800, 600);
        c1->Divide(2,1);
        c1->cd(1);
        gQvsStrip[0]->Draw("AP");
        TLine *line1 = new TLine(PosX1,0,PosX1,gPad->GetUymax());
        line1 -> SetLineColor(kRed);
        line1->Draw("same");

        c1->cd(2);
        gQvsStrip[1]->Draw("AP");
        TLine *line2 = new TLine(PosX2,0,PosX2,gPad->GetUymax());
        line2 -> SetLineColor(kRed);

        line2->Draw("same");

        c1->Update();
        c1->WaitPrimitive();
      }

    }
  }
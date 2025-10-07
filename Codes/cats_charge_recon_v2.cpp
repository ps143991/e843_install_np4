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
#include <vector>
#include <utility>
#include <cmath>
#include <limits>
#include <TSystem.h>




// TCanvas* c = nullptr;


// Build Map/MaxQ/QSum for one detector (X-plane) out of Cal* arrays
struct ClusterInputs {
  std::vector<std::pair<unsigned short, unsigned short>> Map; // (strip, charge)
  std::pair<unsigned short, unsigned short> MaxQ{0,0};        // (strip, charge)
  double QSum = 0.0;
};

static ClusterInputs build_cluster_inputs_X(int detWanted,
                                            const std::vector<int>&    cDetX,
                                            const std::vector<int>&    cStrX,
                                            const std::vector<double>& cQX,
                                            bool strips_one_based=false) {
  ClusterInputs ci;
  ci.Map.reserve(cDetX.size());
  for (size_t i=0;i<cDetX.size();++i){
    if (cDetX[i] != detWanted) continue;
    int s = cStrX[i];
    if (strips_one_based) s -= 1;          // normalize to 0..27 if needed
    if (s < 0 || s > 27) continue;
    double qd = cQX[i];
    if (qd < 0) qd = 0;
    unsigned short strip  = static_cast<unsigned short>(s);
    unsigned short charge = static_cast<unsigned short>(qd);   // NPTool uses UShort_t
    ci.Map.emplace_back(strip, charge);
    ci.QSum += qd;
    if (charge > ci.MaxQ.second) ci.MaxQ = {strip, charge};
  }
  return ci;
}

// Exact copy of your AnalyticHyperbolicSecant (made free function)
static double AnalyticHyperbolicSecant_pos(
    std::pair<unsigned short, unsigned short> MaxQ,
    const std::vector<std::pair<unsigned short, unsigned short>>& Map,
    double /*QSum*/) {

  double sech = -1000.0;

  if (MaxQ.second > 0 && MaxQ.first > 2 && MaxQ.first < 27) {
    double q2 = MaxQ.second;
    double q1 = 0, q3 = 0;
    for (auto& strip : Map) {
      if (strip.first == MaxQ.first - 1) q1 = strip.second;
      else if (strip.first == MaxQ.first + 1) q3 = strip.second;
    }

    if (q1 > 0 && q3 > 0) {
      double vs0 = std::sqrt(q2 / q3);
      double vs1 = std::sqrt(q2 / q1);
      double vs2 = 0.5 * (vs0 + vs1);
      double vs3 = std::log(vs2 + std::sqrt(vs2 * vs2 - 1.0));
      double vs4 = std::abs((vs0 - vs1) / (2.0 * std::sinh(vs3)));
      double vs5 = 0.5 * std::log((1.0 + vs4) / (1.0 - vs4));

      if (q3 > q1) sech = MaxQ.first + vs5 / vs3;
      else         sech = MaxQ.first - vs5 / vs3;
    }
  }
  return sech; // in strip units (0..27 + fraction)
}
void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("../data/analysed/558_with_strx5_disabled.root");
}

// double AnalyticHyperbolicSecant(int det)

void cats_charge_recon_v2(){
    ///////////////////////////////////////////////////////main functions////////////////////////////////////////////////////////
    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    Long64_t tot_entries= tree->GetEntries();
    cout<<"Total Entries: "<<tot_entries<<endl;

    TTreeReader reader(tree);
    CatsDetector* det_cats_r = new CatsDetector();
    TTreeReaderValue<CatsPhysics> phy_cats_r(reader, "cats");
    // TTreeReaderValue<CatsData> raw_cats_r(reader, "cats");
    // TTreeReaderValue<CatsDetector> det_cats_r(reader, "cats");

    ///////////////////////////////////////////////////////output file def////////////////////////////////////////////////////////

    TFile* outputFile = new TFile("./../data/analysed/charge_recon_v2.root", "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        outputFile->Close();
        return;
    }
    TTree* outputTree = new TTree("t", "Tree");
    auto c = new TCanvas("c", "Charge Reco", 1200, 600);


    while(reader.Next()){
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }
        
            // auto c = new TCanvas("c", "Charge Reco", 1200, 600);  
            // Pull branches
            const auto& det   = phy_cats_r->DetNumber;
            const auto& posX  = phy_cats_r->PositionX;
            const auto& cDetX = phy_cats_r->CalDetNumberX;
            const auto& cStrX = phy_cats_r->CalStripNumberX;
            const auto& cQX   = phy_cats_r->CalChargeX;

            // Skip if no CATS reconstructed
            if (det.empty()) continue;

            // map detector -> index in posX
            auto indexForDet = [&](int D)->int {
              for (size_t i=0;i<det.size();++i) if (det[i]==D) return (int)i;
              return -1;
            };
            int i1 = indexForDet(1);
            int i2 = indexForDet(2);

            double posX_tree_1 = (i1>=0)? posX[i1] : std::numeric_limits<double>::quiet_NaN();
            double posX_tree_2 = (i2>=0)? posX[i2] : std::numeric_limits<double>::quiet_NaN();

            // If your CalStripNumberX is 1..28, set this true:
            const bool strips_one_based = false;

            // Build inputs for ASECH from the calibrated arrays
            auto CI1 = build_cluster_inputs_X(1, cDetX, cStrX, cQX, strips_one_based);
            auto CI2 = build_cluster_inputs_X(2, cDetX, cStrX, cQX, strips_one_based);

            // Recompute ASECH
            double posX_asech_1 = (CI1.Map.empty()? std::numeric_limits<double>::quiet_NaN()
                                                : AnalyticHyperbolicSecant_pos(CI1.MaxQ, CI1.Map, CI1.QSum));
            double posX_asech_2 = (CI2.Map.empty()? std::numeric_limits<double>::quiet_NaN()
                                                : AnalyticHyperbolicSecant_pos(CI2.MaxQ, CI2.Map, CI2.QSum));

            // Skip dull events
            if (CI1.Map.empty() && CI2.Map.empty()) continue;

            // Build charge-vs-strip graphs from the same inputs (already inverted & calibrated)
            auto g1 = new TGraph(); g1->SetMarkerStyle(20);
            auto g2 = new TGraph(); g2->SetMarkerStyle(20);

            for (auto& p : CI1.Map) g1->SetPoint(g1->GetN(), p.first, p.second);
            for (auto& p : CI2.Map) g2->SetPoint(g2->GetN(), p.first, p.second);

            // Canvas
            static int canvas_id = 0;
            c->Divide(2,1);

            // Panel 1: CATS1
            c->cd(1);
            //set graph max y = 4000 mannually and x axis from 0 to 29
            g1->SetMaximum(4000);
            g1->SetMinimum(0);
            g1->GetXaxis()->SetLimits(-1, 28);
            

            g1->SetTitle("CATS1 X: Calibrated Charge vs Strip");
            g1->GetXaxis()->SetTitle("Strip index (0..27)");
            g1->GetYaxis()->SetTitle("Charge (arb.)");
            g1->Draw("AP");



            auto  line1 = new TLine(phy_cats_r->StripNumberX[0],0,phy_cats_r->StripNumberX[0],4000); line1->SetLineColor(kBlack); line1->SetLineWidth(2); line1->Draw("same");

            // Panel 2: CATS2
            c->cd(2);
            g2->SetMaximum(4000);
            g2->SetMinimum(0);
            g2->GetXaxis()->SetLimits(-1, 28);

            g2->SetTitle("CATS2 X: Calibrated Charge vs Strip");
            g2->GetXaxis()->SetTitle("Strip index (0..27)");
            g2->GetYaxis()->SetTitle("Charge (arb.)");
            g2->Draw("AP");


           

            auto  line2 = new TLine(phy_cats_r->StripNumberX[1],0,phy_cats_r->StripNumberX[1],4000); line2->SetLineColor(kBlack); line2->SetLineWidth(2); line2->Draw("same");

            c->Update();
            
            gSystem->Sleep(700); // sleep 0.1s to see the canvas

            c->WaitPrimitive();
            c->Clear();
      }   
    }
    
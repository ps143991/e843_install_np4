// debug_cats_reco.C
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <cmath>
#include <limits>
#include "CatsPhysics.h" // make sure the dictionary is loaded
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

using namespace cats;
/* static double centroidAround(int center, const std::vector<int>& strips,
                             const std::vector<unsigned int>& q, int det,
                             const std::vector<int>& dets, int window=2){
  double w=0, sw=0;
  for (size_t i=0;i<strips.size();++i){
    if (dets[i]!=det) continue;
    int s = strips[i];
    if (std::abs(s-center) <= window){ sw += s * q[i]; w += q[i]; }
  }
  return (w>0)? sw/w : center;
} */

template <class StripT, class ChargeT, class DetT>
static double centroidAround(int center,
                             const std::vector<StripT>& strips,
                             const std::vector<ChargeT>& q,
                             int det,
                             const std::vector<DetT>& dets,
                             int window = 2,
                             bool strips_one_based = false) {
  const size_t n = std::min({strips.size(), q.size(), dets.size()});
  double sw = 0.0, w = 0.0;
  for (size_t i = 0; i < n; ++i) {
    if ((int)dets[i] != det) continue;
    int s = (int)strips[i];
    if (strips_one_based) s -= 1;            // normalize to 0..27 if needed
    if (std::abs(s - center) <= window) {
      const double qi = (double)q[i];
      sw += (double)s * qi;
      w  += qi;
    }
  }
  return (w > 0.0) ? (sw / w) : (double)center;
}

void cats_charge_reconstruction(const char* fname="../data/analysed/558.root"){
  TFile f(fname); if (f.IsZombie()) return;
  TTree* T=nullptr;
  TIter next(f.GetListOfKeys());
  while (auto k=(TKey*)next()){
    if (strcmp(k->GetClassName(),"TTree")==0){
      auto t=(TTree*)k->ReadObj();
      if (t->GetBranch("cats")) { T=t; break; }
    }
  }
  if (!T){ printf("No tree with branch 'cats'\n"); return; }

  TTreeReader R(T);
  // TTreeReaderValue<CatsPhysics> cats(R,"cats");
    TTreeReaderValue<CatsPhysics> phy_cats_r(R, "cats");


  TH2F* hA_vs_frac_X1 = new TH2F("hA_vs_frac_X1","CATS1 X: asym vs frac; frac(PosX); A=(Q_{+1}-Q_{-1})/(sum)",100,0,1,120,-1.2,1.2);
  TH2F* hA_vs_frac_X2 = new TH2F("hA_vs_frac_X2","CATS2 X: asym vs frac; frac(PosX); A",100,0,1,120,-1.2,1.2);
  TH1F* hDeltaCOG_X1  = new TH1F("hDeltaCOG_X1","CATS1: (PosX - COG); strips",200,-1,1);
  TH1F* hDeltaCOG_X2  = new TH1F("hDeltaCOG_X2","CATS2: (PosX - COG); strips",200,-1,1);

  while (R.Next()){
    const auto& det  = phy_cats_r->DetNumber;
    const auto& posX = phy_cats_r->PositionX;
    const auto& cDetX= phy_cats_r->CalDetNumberX;
    const auto& cStrX= phy_cats_r->CalStripNumberX;   // same index as CalChargeX
    const auto& cQX  = phy_cats_r->CalChargeX;

    // For each detector present in this event
    for (size_t k=0;k<det.size();++k){
      int D = det[k]; if (D!=1 && D!=2) continue;
      double sx = posX[k];
      int sx0 = (int)std::floor(sx);
      int sx1 = sx0+1;
      if (sx0<0 || sx1>27) continue;

      // find Q at sx0 and sx1 from Cal arrays
      double q0=0, q1=0;
      for (size_t i=0;i<cStrX.size();++i){
        if (cDetX[i]!=D) continue;
        if (cStrX[i]==(unsigned)sx0) q0 += cQX[i];
        if (cStrX[i]==(unsigned)sx1) q1 += cQX[i];
      }
      double sum = q0+q1;
      if (sum>0){
        double frac = sx - sx0;                         // 0..1
        double asym = (q1 - q0) / sum;                  // -1..1
        if (D==1) hA_vs_frac_X1->Fill(frac,asym);
        else      hA_vs_frac_X2->Fill(frac,asym);
      }

      // Compare with centroid around the local cluster
      double cog = centroidAround(sx0, cStrX, cQX, D, cDetX, /*window=*/2);
      if (D==1) hDeltaCOG_X1->Fill(sx - cog);
      else      hDeltaCOG_X2->Fill(sx - cog);
    }
  }

  TCanvas* c1=new TCanvas("c1","Asymmetry vs Fraction",1100,500);
  c1->Divide(2,1);
  c1->cd(1); hA_vs_frac_X1->Draw("colz");
  c1->cd(2); hA_vs_frac_X2->Draw("colz");

  TCanvas* c2=new TCanvas("c2","Pos - COG",900,450);
  c2->Divide(2,1);
  c2->cd(1); hDeltaCOG_X1->Draw();
  c2->cd(2); hDeltaCOG_X2->Draw();
}


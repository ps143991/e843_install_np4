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
#include "TCutG.h"
#include "TString.h"

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


TH1F* all_ex = NULL;

TCanvas* can_reg[5]; 
TCanvas* call = NULL;

TFile* file1 = NULL;
TFile* file2 = NULL;
TFile* file3 = NULL;
TFile* datafile = NULL;
TFile* gatefile = NULL;

TCutG* gate594 = NULL;

TCutG *gate = NULL;


using namespace std;

void LoadFiles(){
    datafile = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/roothistograms/hAllthetalesscut.root");
    if (!datafile || datafile->IsZombie()) {
        printf("Error: Data ROOT file could not be opened.\n");
        return;
    }

}

void LoadGates(){
    gatefile = TFile::Open("gates/gates.root");
    if (!gatefile || gatefile->IsZombie()) {
        printf("Error: Gate ROOT file could not be opened.\n");
        return;
    }
    gate594 = (TCutG*)gatefile->Get("gate594");
    if(!gate594) {
        printf("Error: Gate 'gate594' not found in the gate file.\n");
        return;
    }
}

static TH2* ApplyGateTH2(const TH2* h2, const TCutG* gate, const char* newname="h2_gated"){
    if(!h2 || !gate) return nullptr;
    auto* out = (TH2*)h2->Clone(newname);
    out->Reset();
    const int nx = h2->GetNbinsX();
    const int ny = h2->GetNbinsY();
    for(int ix=1; ix<=nx; ix++) {
        const double x = h2->GetXaxis()->GetBinCenter(ix);
        for(int iy=1; iy<=ny; iy++) {
            const double y = h2->GetYaxis()->GetBinCenter(iy);
            if(gate->IsInside(x,y)) {
                out->SetBinContent(ix, iy, h2->GetBinContent(ix, iy));
                out->SetBinError(ix, iy, h2->GetBinError(ix, iy));
            }
        }
    }
    out->SetTitle(TString(h2->GetTitle()) + " (gated)");
    return out;
}

static TH1D* ProjectionX_Gated(const TH2* h2, const TCutG* gate, const char* newname="projX_gated"){
    auto* px = new TH1D(newname, TString(h2->GetTitle())+";X;Counts", h2->GetXaxis()->GetNbins(), 
                                    h2->GetXaxis()->GetXmin(), h2->GetXaxis()->GetXmax());
    const int nx = h2->GetNbinsX();
    const int ny = h2->GetNbinsY();
    for(int ix=1; ix<=nx; ix++) {
        const double x = h2->GetXaxis()->GetBinCenter(ix);
        double sum = 0, err2 = 0;
        for(int iy = 1; iy<=ny; iy++) {
            const double y = h2->GetYaxis()->GetBinCenter(iy);
            if(gate->IsInside(x,y)) {
                const double c = h2->GetBinContent(ix, iy);
                const double e = h2->GetBinError(ix, iy);
                sum += c;
                err2 += e*e;
            }
        }
        px->SetBinContent(ix, sum);
        px->SetBinError(ix, TMath::Sqrt(err2));
    }
    return px;
}

static TH1D* ProjectionY_Gated(const TH2* h2, const TCutG* gate, const char* name="py_gated"){
  auto* py = new TH1D(name, TString(h2->GetTitle())+";Y;Counts", h2->GetYaxis()->GetNbins(),
                      h2->GetYaxis()->GetXmin(), h2->GetYaxis()->GetXmax());
  const int nx = h2->GetNbinsX();
  const int ny = h2->GetNbinsY();
  for (int iy=1; iy<=ny; ++iy){
    const double y = h2->GetYaxis()->GetBinCenter(iy);
    double sum=0, err2=0;
    for (int ix=1; ix<=nx; ++ix){
      const double x = h2->GetXaxis()->GetBinCenter(ix);
      if (gate->IsInside(x,y)){
        const double c = h2->GetBinContent(ix,iy);
        const double e = h2->GetBinError(ix,iy);
        sum  += c;
        err2 += e*e;
      }
    }
    py->SetBinContent(iy, sum);
    py->SetBinError  (iy, std::sqrt(err2));
  }
  return py;
}


void correlation_gamma_PS_v1(){
    
    LoadFiles();
    // LoadGates();

    can1 = new TCanvas("can1", "Peaks", 1900, 1050);
    TH2F* hhdata = (TH2F*)datafile->Get("exogam2kpb_Ex");
    hhdata->Draw("colz");

    /*********************************************THE BEST THING EVER IS COMMENTED OUT BELOW******************************/
    // if(gatefile){
    //     gate = dynamic_cast<TCutG*>(gatefile.Get(gatename));
    //     if(gate) gate = (TCutG*)gate->Clone(TString(gatename)+"_clone");}
    // auto c0 = new TCanvas("c0", "Original 2D histogram", 1900, 1050);
    // hhdata->Draw("colz");
    TCanvas* c0 = new TCanvas("c0", "Original 2D histogram", 1900, 1050);
    if(!gate){
        printf("Draw a polygon with the mouse, then close the editor.\n");
        c0->cd(); hhdata->Draw("colz");
        c0->Update();
        gate = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
        if (!gate){ printf("No gate drawn.\n"); return; }
        gate->SetName("myGate");
    }
    gate->SetLineColor(kRed);
    gate->SetLineWidth(2);
    gate->Draw("same");
    c0->Update();
    
    auto h1x = ProjectionX_Gated(hhdata, gate, "hxgated");
    auto h1y = ProjectionY_Gated(hhdata, gate, "hygated");
    can1 = new TCanvas("can1", "Gated X projection", 1900, 1050);
    can1->Divide(2,1);
    can1->cd(1); h1x->Draw("hist"); 
    can1->cd(2); h1y->Draw("hist");
    can1->Update();
    auto h2g = ApplyGateTH2(hhdata, gate, "h2data_gated");

    // auto ccc = new TCanvas("ccc", "Gated 2D histogram", 1900, 1050);
    // if (gate->IsInside(hhdata)) {
    //     printf("Gate includes origin, which is suspicious.\n");
    // }

    auto c1 = new TCanvas("c1","Original vs Gated",1200,500);
    c1->Divide(2,1);
    c1->cd(1); hhdata->Draw("colz"); /* gate->Draw("same"); */
    c1->cd(2); h2g->Draw("colz");

    Double_t bin_Wx = hhdata->GetXaxis()->GetBinWidth(1);
    Double_t bin_Wy = hhdata->GetYaxis()->GetBinWidth(1);
    TH1F* projected_gamma_full = (TH1F*)hhdata->ProjectionY("projected_gamma_ex");
    can2 = new TCanvas("can2", "Projected gamma full", 1900, 1050);
    // projected_gamma_full->Rebin(4);
    projected_gamma_full->Draw("");

    TH1F* projected_ex_full = (TH1F*)hhdata->ProjectionX("projected_ex_full");
    can3 = new TCanvas("can3", "Projected ex full", 1900, 1050);
    // projected_ex_full->Rebin(2);
    projected_ex_full->Draw("");

    // TH1F* projected_ex_gate594 = (TH1F*)hhdata->ProjectionX("projected_ex_gate594", hhdata->GetYaxis()->FindBin(489), hhdata->GetYaxis()->FindBin(530));
    // auto newCAn = new TCanvas("newCAn", "limited 2d histogram", 1900, 1050);
    // //drawing the limited 2d histogram by cloning hhdata within a definite x and y range
    // TH2F* limited_2d = (TH2F*)hhdata->Clone("limited_2d");
    // //drawing using bin numbers
    // limited_2d->GetXaxis()->SetRange(hhdata->GetXaxis()->FindBin(-5), hhdata->GetXaxis()->FindBin(15));
    // limited_2d->GetYaxis()->SetRange(hhdata->GetYaxis()->FindBin(489), hhdata->GetYaxis()->FindBin(530));
    // limited_2d->Draw("colz");


    // can4 = new TCanvas("can4", "Projected ex gate 594", 1900, 1050);

    // projected_ex_gate594->Rebin(2);
    // projected_ex_gate594->Draw("");
    can1->Update();
    can2->Update();


    // if (!(gatefile) && gate){
    //     TFile gout("saved_gate.root","RECREATE");
    //     gate->Write("myGate");
    //     gout.Close();
    //     printf("Saved gate to saved_gate.root as myGate\n");
    // }
}
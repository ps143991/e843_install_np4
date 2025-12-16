// script to read the raw data of all strips of all telescopes and save them in a root file strip by strip



#include "NPApplication.h"
using namespace nptool;
#include "NPDetectorManager.h"
#include "NPFunction.h"
#include "MugastData.h"
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
#include <TProfile.h>
#include <TCutG.h>
#include <TGraph.h>

#include <iostream>
// fit_erf_line.C
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TRandom3.h"
#include "TLine.h"
#include "TStyle.h"
using namespace std;


TChain* tree = NULL;

const int N_STRIPS = 128;

vector<TH1D*> h_T5_X(N_STRIPS);
vector<TH1D*> h_T5_Y(N_STRIPS);

const int tele_under_insp =5; // telescope to be inspected

void loadFILES() {
    tree = new TChain("RawTree");
    // tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly0.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly1.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly2.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly3.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly4.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly5.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly6.root");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/converted/573_with_MMGonly7.root");
}

int Mugast_573(){

    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    cout<<"Total Entries in tree1: "<<tree->GetEntries()<<endl;

    TTreeReader reader(tree);
    TTreeReaderValue<mugast::MugastData> mugast_r(reader, "mugast");
    TTreeReaderArray<unsigned short> tele_X(reader, "fMG_DSSDXE_DetectorNbr");
    TTreeReaderArray<unsigned short> strip_X(reader, "fMG_DSSDXE_StripNbr");
    TTreeReaderArray<double> erg_X(reader, "fMG_DSSDXE_Energy");
    TTreeReaderArray<unsigned short> tele_Y(reader, "fMG_DSSDYE_DetectorNbr");
    TTreeReaderArray<unsigned short> strip_Y(reader, "fMG_DSSDYE_StripNbr");
    TTreeReaderArray<double> erg_Y(reader, "fMG_DSSDYE_Energy");


        // double coeff_strip_66_X_T4 = 0.00783446; //keV/channel
        // double offset_strip_66_X_T4 = -64.1597;  //keV

    
    for(int s = 0; s<N_STRIPS; s++){
        TString nameX = Form("h_T5_X_strip_%03d", s+1);
        TString titleX = Form("Telescope 5, X side, strip %d;Energy [keV];Counts", s+1);
        h_T5_X[s] = new TH1D(nameX, titleX, 1200, 7900, 9100);

        TString nameY = Form("h_T5_Y_strip_%03d", s+1);
        TString titleY = Form("Telescope 5, Y side, strip %d;Energy [keV];Counts", s+1);
        h_T5_Y[s] = new TH1D(nameY, titleY, 1200, 7100, 8300);
    }
    
    long long so_far = 0;
    while(reader.Next()/*  && so_far < 100000 */) {
        // so_far++;
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }

        const int nX = tele_X.GetSize();
        const int nY = tele_Y.GetSize();

        for(int i = 0; i<nX; ++i){
            if (tele_X[i] == tele_under_insp) { // telescope set at the top of the code

                unsigned short strip = strip_X[i];
                double energy_X = erg_X[i]; // apply calibration if needed

                int idx = static_cast<int>(strip) - 1;

                if (idx >= 0 && idx < N_STRIPS) {
                    h_T5_X[idx]->Fill(energy_X);
                }
            }
        }


        // ---------- Y side ----------
        for (int i = 0; i < nY; ++i) {
            if (tele_Y[i] == tele_under_insp) { // telescope set at the top of the code

                unsigned short strip = strip_Y[i];
                double energy_Y = erg_Y[i];


                // If strips numbered 1..128:
                int idx = static_cast<int>(strip) - 1;

                if (idx >= 0 && idx < N_STRIPS) {
                    h_T5_Y[idx]->Fill(energy_Y);
                }
            }
        }
    


    }

//write the canvas to a root file

    // TFile* outputFile = new TFile("/home/sharmap/Desktop/Telescope_5_X.root", "RECREATE");
    // if (!outputFile || outputFile->IsZombie()) {
    //     outputFile->Close();
    //     return 1;
    // }




//Canvas in 4x3 division to cover 128 X strips and 128Y strips
const int STRIPS_PER_CANVAS = 12;
const int N_CANVASES = 11; // ceil(128 / 12.0) = 11

std::vector<TCanvas*> c_T5_X;
c_T5_X.reserve(N_CANVASES);

// Optional: nicer style
gStyle->SetOptStat(0);

for (int c = 0; c < N_CANVASES; ++c) {
    int firstStrip = c * STRIPS_PER_CANVAS;          // 0-based index
    int lastStrip  = std::min(N_STRIPS, firstStrip + STRIPS_PER_CANVAS) - 1;

    TString cname  = Form("c_T5_X_%02d", c);
    TString ctitle = Form("Telescope 5, X side, strips %d to %d",
                          firstStrip + 1, lastStrip + 1); // +1 if strips are 1–128

    TCanvas* can = new TCanvas(cname, ctitle, 1200, 800);
    can->Divide(4, 3);
    c_T5_X.push_back(can);

    int pad = 1;
    for (int s = firstStrip; s <= lastStrip; ++s) {
        can->cd(pad);
        if (h_T5_X[s]) {
            h_T5_X[s]->Draw();
            // Optionally log scale:
            // gPad->SetLogy();
        }
        ++pad;
    }
    can->Write();
}

//write the canvas to a root file

    TFile* outputFile2 = new TFile("/home/sharmap/Desktop/Telescope_5_Y.root", "RECREATE");
    if (!outputFile2 || outputFile2->IsZombie()) {
        outputFile2->Close();
        return 1;
    }




//Canvas in 4x3 division to cover 128 X strips and 128Y strips
const int STRIPS_PER_CANVAS2 = 12;
const int N_CANVASES2 = 11; // ceil(128 / 12.0) = 11

std::vector<TCanvas*> c_T5_Y;
c_T5_Y.reserve(N_CANVASES2);
// Optional: nicer style
gStyle->SetOptStat(0);

for (int c = 0; c < N_CANVASES2; ++c) {
    int firstStrip = c * STRIPS_PER_CANVAS2;          // 0-based index
    int lastStrip  = std::min(N_STRIPS, firstStrip + STRIPS_PER_CANVAS2) - 1;

    TString cname  = Form("c_T5_Y_%02d", c);
    TString ctitle = Form("Telescope 5, Y side, strips %d to %d",
                          firstStrip + 1, lastStrip + 1); // +1 if strips are 1–128

    TCanvas* can = new TCanvas(cname, ctitle, 1200, 800);
    can->Divide(4, 3);
    c_T5_Y.push_back(can);

    int pad = 1;
    for (int s = firstStrip; s <= lastStrip; ++s) {
        can->cd(pad);
        if (h_T5_Y[s]) {
            h_T5_Y[s]->Draw();
            // Optionally log scale:
            // gPad->SetLogy();
        }
        ++pad;
    }
    // can->Write();
}

    return 0;
}
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

string stripX[128];
        double coeffX[128], offsetX[128];

        string stripY[128];
        double coeffY[128], offsetY[128];

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

void loadCalib(){
    // Load calibration coefficients from a file or define them here
    ifstream c_MG_T1_X("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_X_E_MG1.cal");
    ifstream c_MG_T2_X("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_X_E_MG2.cal");
    ifstream c_MG_T3_X("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_X_E_MG3.cal");
    ifstream c_MG_T4_X("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_X_E_MG4.cal");
    ifstream c_MG_T5_X("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_X_E_MG5.cal");
    ifstream c_MG_T1_Y("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_Y_E_MG1.cal");
    ifstream c_MG_T2_Y("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_Y_E_MG2.cal");
    ifstream c_MG_T3_Y("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_Y_E_MG3.cal");
    ifstream c_MG_T4_Y("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_Y_E_MG4.cal");
    ifstream c_MG_T5_Y("/home/sharmap/Workplace/np4/e843/calibration/Mugast_cal_run573/Cal_Str_Y_E_MG5.cal");

    // content of calib file is like:
    //Mugast_T1_DSSD_X1_E 0 0
    //Mugast_T1_DSSD_X2_E -65.5753 0.00801158    
    //first string is strip name, second is offset, third is coefficient
    string lineX;
    int countX = 0;
    while (getline(c_MG_T5_X, lineX)) {
        istringstream issX(lineX);
        string strip_nameX;
        double offX, coeffiX;
        if (!(issX >> strip_nameX >> offX >> coeffiX)) { break; } // error
        stripX[countX] = strip_nameX;
        offsetX[countX] = offX;
        coeffX[countX] = coeffiX;
        countX++;
    }
    c_MG_T5_X.close();

        string lineY;
    int countY = 0;
    while (getline(c_MG_T5_Y, lineY)) {
        istringstream issY(lineY);
        string strip_nameY;
        double offY, coeffiY;
        if (!(issY >> strip_nameY >> offY >> coeffiY)) { break; } // error
        // strip[countY] = strip_nameY;
        offsetY[countY] = offY;
        coeffY[countY] = coeffiY;
        countY++;
    }
    c_MG_T5_Y.close();
    

    cout<<"calibration coefficients loaded"<<endl;
    for(int i=0; i<128; i++){
        cout<<"Strip_X "<<i+1<<": Offset= "<<offsetX[i]<<" Coeff= "<<coeffX[i]<<endl;
    }

    cout<<"calibration coefficients loaded"<<endl;
    for(int i=0; i<128; i++){
        cout<<"Strip_Y "<<i+1<<": Offset= "<<offsetY[i]<<" Coeff= "<<coeffY[i]<<endl;
    }
}


int Mugast_573_calibrated(){

    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadCalib();
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




    
    for(int s = 0; s<N_STRIPS; s++){
        TString nameX = Form("h_T5_X_strip_%03d", s+1);
        TString titleX = Form("Telescope 5, X side, strip %d;Energy [keV];Counts", s+1);
        h_T5_X[s] = new TH1D(nameX, titleX, 600, 8400, 9000);

        TString nameY = Form("h_T5_Y_strip_%03d", s+1);
        TString titleY = Form("Telescope 5, Y side, strip %d;Energy [keV];Counts", s+1);
        h_T5_Y[s] = new TH1D(nameY, titleY, 600, 8400, 9000);
    }

    TH2F *h_2d_X = new TH2F("h_2d_X", "Telescope 5 X side: Strip vs Energy;Strip Number;Energy [keV]", 128, 0.5, 128.5, 1000, 0, 10);
    TH2F *h_2d_Y = new TH2F("h_2d_Y", "Telescope 5 Y side: Strip vs Energy;Strip Number;Energy [keV]", 128, 0.5, 128.5, 1000, 0, 10);
    
    long long so_far = 0;
    while(reader.Next()/*  && so_far < 100000 */) {
        // so_far++;
        if(reader.GetCurrentEntry() % 1000000 == 0) {
            cout << "Processing entry: " << (static_cast<double>(reader.GetCurrentEntry()) / tree->GetEntries()) * 100.0 << "%" << endl;
        }

        const int nX = tele_X.GetSize();
        const int nY = tele_Y.GetSize();

        for(int i = 0; i<nX; ++i){
            if (tele_X[i] == tele_under_insp) { // telescope number set at top of the code

                unsigned short strip = strip_X[i];
                double energy_X = erg_X[i]; // apply calibration if needed

                int idx = static_cast<int>(strip) - 1;

                if (idx >= 0 && idx < N_STRIPS) {
                    h_T5_X[idx]->Fill(energy_X);
                }
                // Calibrate energy
                double calibrated_energy_X = offsetX[idx] + coeffX[idx] * energy_X;
                h_2d_X->Fill(strip, calibrated_energy_X);
            }
        }


        // ---------- Y side ----------
        for (int i = 0; i < nY; ++i) {
            if (tele_Y[i] == tele_under_insp) { // telescope set at top of the code

                unsigned short strip = strip_Y[i];
                double energy_Y = erg_Y[i];


                // If strips numbered 1..128:
                int idy = static_cast<int>(strip) - 1;

                if (idy >= 0 && idy < N_STRIPS) {
                    h_T5_Y[idy]->Fill(energy_Y);
                }
                double calibrated_energy_Y = offsetY[idy] + coeffY[idy] * energy_Y;
                h_2d_Y->Fill(strip, calibrated_energy_Y);
            }
        }
    


    }

    TCanvas* c2d_X = new TCanvas("c2d_X", "Telescope 5 X side: Strip vs Energy", 800, 600);
    h_2d_X->Draw("COLZ");

    TCanvas* c2d_Y = new TCanvas("c2d_Y", "Telescope 5 Y side: Strip vs Energy", 800, 600);
    h_2d_Y->Draw("COLZ");



    TFile* outputFile = new TFile("/home/sharmap/Desktop/Mugast_calibrated.root", "UPDATE");
    if (!outputFile || outputFile->IsZombie()) {
        outputFile->Close();
        return 1;
    }
    TDirectory* dirX = outputFile->mkdir("Telescope_5_X");
    dirX->cd();
    h_2d_X->Write();
    TDirectory* dirY = outputFile->mkdir("Telescope_5_Y");
    dirY->cd();
    h_2d_Y->Write();

    outputFile->Close();


/* //Canvas in 4x3 division to cover 128 X strips and 128Y strips
const int STRIPS_PER_CANVAS = 12;
const int N_CANVASES = 11; // ceil(128 / 12.0) = 11

std::vector<TCanvas*> c_T4_X;
c_T4_X.reserve(N_CANVASES);

// Optional: nicer style
gStyle->SetOptStat(0);

for (int c = 0; c < N_CANVASES; ++c) {
    int firstStrip = c * STRIPS_PER_CANVAS;          // 0-based index
    int lastStrip  = std::min(N_STRIPS, firstStrip + STRIPS_PER_CANVAS) - 1;

    TString cname  = Form("c_T4_X_%02d", c);
    TString ctitle = Form("Telescope 4, X side, strips %d to %d",
                          firstStrip + 1, lastStrip + 1); // +1 if strips are 1–128

    TCanvas* can = new TCanvas(cname, ctitle, 1200, 800);
    can->Divide(4, 3);
    c_T4_X.push_back(can);

    int pad = 1;
    for (int s = firstStrip; s <= lastStrip; ++s) {
        can->cd(pad);
        if (h_T4_X[s]) {
            h_T4_X[s]->Draw();
            // Optionally log scale:
            // gPad->SetLogy();
        }
        ++pad;
    }
} */

    return 0;
}
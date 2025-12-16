//a script to calculate area under gamma peaks of 152 Eu spectrum after fitting with gaussian + linear background

#include "ExogamPhysics.h"
#include "NPApplication.h"
using namespace nptool;
#include "NPDetectorManager.h"
#include "NPFunction.h"

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
using namespace exogam;
using namespace std;
// fit_erf_line.C
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TRandom3.h"
#include "TROOT.h"
#include "TPolyMarker.h"
#include "TText.h"
#include "TSystem.h"

#include <string>
#include <string.h>
#include <TLegend.h>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <fstream>
#include <iomanip>
#include <TApplication.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TCut.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TString.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TFile.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TGraphErrors.h>
#include <TSpectrum.h>
#include <TPolyMarker.h>
#include <TText.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TButton.h>
#include <TComplex.h>
#include <TTimeStamp.h>
#include <TObjArray.h>
#include <TMinuit.h>
#include <TMath.h>
#include <TVirtualPad.h>

#define	DEBUG			1
#define	DPRINTF			if (DEBUG & 1) printf
#define	dprintf			if (DEBUG & 2) printf
#define	TDC_LLD			128
#define	TDC_ULD			4095
#define	TDC_PEAK		3500
#define	TDC_CALIB_WIDTH		500
#define	TDC_CALIB_LEFT		(TDC_PEAK-TDC_CALIB_WIDTH)
#define	TDC_CALIB_RIGHT		(TDC_PEAK+TDC_CALIB_WIDTH)
#define	SHOW_SPEC		1
#define	SUBTRACT_BGND		0
#define	SIGMA			4.0
#define	ProjSpecResol		0.01
#define	ProjSpecSigma		SIGMA
#define	BASE_LT			3
#define	BASE_RT			3
#define	NumBins			8192
#define	HistXmin		1
#define	HistXmax		8191
#define	ProjSpecName		"SPEC"
#define	ProjSpecLineWidth	2
#define	ProjSpecLineColor	kMagenta
#define	RECALIBRATE		0.5
#define	SkewL			0
#define	SkewR			0
#define	SkewedGauss		(SkewL+SkewR)
#define	QuadBack		0
#define	MIN_AREA		50
#define	NumPolyMax		5
#define	NumPoly			3
#define MAX_CALIB_LINES		300
#define MAX_SPEC_LENGTH		50
#define BUFFER_SIZE		1024 //Adjusted as per need (larger buffer size improves working but consumes memory)
using namespace std;

TChain *tree = NULL;
TH1F* gCurrentHistogram = nullptr;
Double_t X[100]; // the fitted peaks
std::vector<Double_t> Sigmas;


//	PROGRAM TO FIT A GAUSSIAN
Int_t xx1, xx2;
Int_t count1, count2, count3;
enum Parameters {
Peak, Centroid, Sigma, SkewLpar, SkewRpar, BackA, BackB, BackC, Area, BackArea, PeakY, NumPar };

void loadFiles(){
    tree = new TChain("PhysicsTree");
    tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/567.root");
}

Double_t background (Double_t *x, Double_t *par)
{
Double_t back;
	back = par[0]+ par[1]*x[0];
	return back;
}

Double_t gauss (Double_t*x, Double_t *par)
{
Double_t peak, arg;
	
	arg = (x[0] - par [1]) / par [2];
	
	peak = exp (-0.5*arg*arg);count3++;
	return par[0]*peak;
}

Double_t skewedGauss1 (Double_t*x, Double_t *par)
{
    return  (background (x, par) + gauss(x, &par[2]));
}


Double_t fitPeak (TH1F*hist, Double_t u, Double_t*par){
TF1		*f1, *f2, *f3;
Double_t	chi=1e10;
Double_t	x1, y1, x2, y2;
TFitResultPtr	fitres;
Int_t		result, ndf, pol1Used = 0;

    x1 = u - 1.5 * SIGMA;
    x2 = u + 1.5 * SIGMA;

    f1 = new TF1 ("f1", skewedGauss1, x1, x2, 5);
    f1->SetParameters(par);
    f1->SetParLimits(2, 1.0, 1e6); // Amplitude limits
    f1->SetParLimits(3, u-10, u+10); // Centroid limits
    f1->SetParLimits(4, 0.5, 20.0); // Sigma limits
    f1->SetLineColor (kGreen);
    f1->SetLineWidth (3);
    fitres = hist->Fit (f1, "QSR0");
    result = Int_t (fitres);
    f1->GetParameters (par);
    chi = f1->GetChisquare ();
    ndf = f1->GetNDF ();
    f1->Draw("same");

    f2 = new TF1 ("f2", background, x1, x2, 2);
    f2->SetParameters(par);
    f2->Draw("same");
    
    f3 = new TF1 ("f3", gauss, x1, x2, 3);
    f3->SetParameters(&par[2]);
    // f3->Draw("same");

    cout<<"area under peak is: "<<f1->Integral(x1, x2) - f2->Integral(x1,x2)<<endl;


    

    return chi / ndf;
}

// end of fitting funciton


// **************************************
// **************************************
// **************************************
// **************************************


Double_t IdentifyPeaks(TH1F *H, Double_t *X)

{


	Int_t	numP = H->ShowPeaks (2.0, "", 0.2);
	if (! numP) {cout<<"no Peaks found"<<endl;}
        cout<<"the number of peaks is: "<<numP<<endl;
    
    TList	*funcP = H->GetListOfFunctions ();
    if (! funcP) return 0;
        TPolyMarker	*mX = (TPolyMarker*)funcP->FindObject ("TPolyMarker");
    if (! mX) return 0;
        Double_t	tX, *peaks = mX->GetX ();
    if (! peaks) return 0;
	    cout<< "Peaks have been found and now are being stored in X[]"<<endl;

    for (Int_t p=0; p<numP; ++p) {
        for (Int_t q=p+1; q<numP; ++q) {
            if (peaks [p] > peaks [q]) {
                tX = peaks [p];
                peaks [p] = peaks [q];
                peaks [q] = tX;
            }}}

	
    Int_t numX = 0;
	
	Double_t par[5];
    for (Int_t i=0; i<numP; i++) {
        cout<<"peak "<< i <<" is at "<<peaks[i]<<"\n";
        par[3] = peaks[i]; // Initial guess for Centroid
        par[2] = 10000; // Initial guess for amplitude
        par[4] = SIGMA; // Initial guess for Sigma
        par[0] = 1.0;
        par[1] = 1.0;
        Double_t rChi = fitPeak (H, peaks [i], par);
        // cout<<"the value of rchi is "<<rChi<<endl;
        if (rChi <0) {cout<<"peak "<< i <<" fit failed, moving to next peak"<<endl;
            continue;
        }

        X [numX] = par[3];
        Sigmas.push_back(2.354820045 *par[4]);
        
        
        if (X [numX] > 0) {
            // printf ("the loop for X[numX]::%3d. %8.2lf %8.2lf %8.2lf %8.2lf\n", i+1, peaks [i], X [numX], par [Sigma], par [SkewLpar]);
            // cout<<"the value of numX is: "<<numX<<endl<<endl;
            numX++;
        }
        if (rChi<0){
            mX->Delete();
        }
        
	}
    cout<<endl;

        
        
   return numX;

}
void fit()
{   
    string padName (gPad->GetName());
    if (gCurrentHistogram != NULL)
    {	
        Int_t numX;
        numX = IdentifyPeaks(gCurrentHistogram, X);

    }
    
}


void Eu152_gamma_fit_2(){
    loadFiles();
    cout<<"Total Entries: "<<tree->GetEntries()<<endl;

    TTreeReader reader(tree);
    TTreeReaderValue<ExogamPhysics> phy_exogam_r(reader, "exogam");

    TFile* outputFile = new TFile("./152_Eu_gamma_fit_output.root", "RECREATE");

    if (!outputFile || outputFile->IsZombie()) {
        outputFile->Close();
        return;
    }

    TH1F *h1 = new TH1F("h1", "152 Eu gamma spectrum", 4000, 0, 4000);
    Long64_t entry =0;
    while (reader.Next() /* && entry < 100000 */) {
        entry++;
        for(int i = 0; i < phy_exogam_r->Exo_EAddBack.size(); i++){
            Double_t energy = phy_exogam_r->Exo_EAddBack[i];
            h1->Fill(energy);
        }
    }
    gCurrentHistogram=h1;

    h1->Draw();
    fit();

    TCanvas *canv_sigmas = new TCanvas("canv_sigmas", "Sigmas vs Peak positions", 800, 600);
    TGraph *sigmas = new TGraph(Sigmas.size());
    for (size_t i = 0; i < Sigmas.size(); i++) {
        sigmas->SetPoint(i, X[i], Sigmas[i]);}
    sigmas->SetTitle("Sigmas vs Peak positions;Peak Position (keV);Sigma (keV)");
    sigmas->SetMarkerStyle(20);
    sigmas->SetMarkerColor(kBlue);
    sigmas->GetXaxis()->SetLimits(0, 4000);
    sigmas ->GetXaxis()->SetRangeUser(0,4000);
    sigmas->GetYaxis()->SetLimits(2, 5);

    sigmas ->GetYaxis()->SetRangeUser(2,5);

    sigmas->Draw("AP");
    // FWHM(E) = sqrt( a^2 + b*E + c*E^2 ),  E in keV, FWHM in keV
    TF1 *fRes_keV = new TF1("fRes_keV","sqrt([0]*[0] + [1]*x + [2]*x*x)", 80., 4000.);   // set your range
    fRes_keV->SetParNames("a (keV)","b (keV)","c");
    fRes_keV->SetParameters(1.0, 2.5e-3, 1e-6);     // reasonable starts
    fRes_keV->SetParLimits(0, 0.0, 5.0);            // a >= 0
    fRes_keV->SetParLimits(1, 0.0, 1e-2);           // b >= 0
    fRes_keV->SetParLimits(2, 0.0, 1e-4);           // c >= 0

        sigmas->Fit(fRes_keV, "IER+");
        Double_t res_at_E = fRes_keV->Eval(594);
        cout<<"your FWHM at 594 keV is: "<<res_at_E<<" keV"<<endl;

TCanvas *canv_fwhm = new TCanvas("canv_fwhm", "FWHM vs Peak positions", 800, 600);

    // // %Res(E) = 100/E * sqrt( a^2 + b*E + c*E^2 ),  E in keV
    TGraph *resolutions = new TGraph(Sigmas.size());
    for (size_t i = 0; i < Sigmas.size(); i++) {
        Double_t res = 100.0 / X[i] * Sigmas[i];
        resolutions->SetPoint(i, X[i], res);}
    resolutions->SetTitle("Resolutions vs Peak positions;Peak Position (keV);Resolution (%)");
    resolutions->SetMarkerStyle(21);
    resolutions->SetMarkerColor(kRed);
    resolutions->GetXaxis()->SetLimits(0, 4000);
    resolutions ->GetXaxis()->SetRangeUser(0,4000);
    // resolutions->GetYaxis()->SetLimits(0, 10);
    // resolutions ->GetYaxis()->SetRangeUser(0,10);
    resolutions->Draw("AP");

    TF1 *fRes_pct = new TF1("fRes_pct",
                            "100.0/x*sqrt([0]*[0] + [1]*x + [2]*x*x)",
                            80., 4000.);
    fRes_pct->SetParNames("a (keV)","b (keV)","c");
    fRes_pct->SetParameters(1.0, 1.0, 1.0);
    fRes_pct->SetParLimits(0, 0.0, 5.0);
    fRes_pct->SetParLimits(1, 0.0, 1.0);
    fRes_pct->SetParLimits(2, 0.0, 1.0);
    resolutions->Fit(fRes_pct, "IER+");







}
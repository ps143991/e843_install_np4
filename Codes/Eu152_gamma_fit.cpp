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
#define	SIGMA			2.0
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
	if (QuadBack) back += par[2]*x[0]*x[0];
	return back;
}

Double_t gauss (Double_t*x, Double_t *par)
{
Double_t peak, arg;
	
	arg = (x[0] - par [1]) / par [2];
	if (SkewL && (arg <= -par[3]))
		{peak = exp (0.5*par[3]*par[3] + par[3]*arg); count1++;}
	else if (SkewR && (arg >= par[4]))
		{peak = exp (0.5*par[4]*par[4] + par[4]*arg); count2++;}
	else
		{peak = exp (-0.5*arg*arg);count3++;}
	return par[0]*peak;
}



Double_t skewNormal_L(Double_t* x, Double_t *par) {
    Double_t peak, arg;
    arg = (x[0] - par[1]) / par[2];
    peak = exp(-0.5 * arg * arg * (1 + par[3] * arg)); // Using a single skewness parameter par[3]
    return par[0] * peak;
}

Double_t skewNormal_R(Double_t* x, Double_t *par) {
    Double_t peak, arg;
    arg = (x[0] - par[1]) / par[2];
    peak = exp(-0.5 * arg * arg * (1 + par[4] * arg)); // Using a single skewness parameter par[3]
    return par[0] * peak;
}
Double_t skewedGauss2 (Double_t*x, Double_t *par)
{
    return  (background (x, par) + gauss(x, &par[2]) + gauss(x, &par[7]));
}
Double_t skewedGauss3 (Double_t*x, Double_t *par)
{
    return  (background (x, par) + gauss(x, &par[2]) + gauss(x, &par[7]) + gauss(x, &par[12]));
}


Double_t skewedGauss (Double_t *x, Double_t *par)
{
	return (gauss(x, par) + background (x, &par [3+SkewedGauss]));
}
/* Double_t skewedGauss2 (Double_t*x, Double_t *par)
{
    return  (background (x, par) + skewNormal_L(x, &par[2]) + skewNormal_R(x, &par[7]));
} */
Double_t skewedGauss1 (Double_t*x, Double_t *par)
{
    return  (background (x, par) + gauss(x, &par[2]));
}

Double_t fitPeak (TH1F*hist, Double_t u, Double_t*par)
{
TF1		*f1, *f2;
Double_t	chi=1e10;
Double_t	x1, y1, x2, y2;
TFitResultPtr	fitres;
Int_t		result, ndf, pol1Used = 0;

    x1 = u - 1.5 * SIGMA;
    x2 = u + 1.5 * SIGMA;

    f1 = new TF1 ("f1", "gaus", x1, x2);
    f1->SetLineColor (kGreen);
    f1->SetLineWidth (3);
    fitres = hist->Fit (f1, "QSR0");
    result = Int_t (fitres);
    f1->GetParameters (par);
    chi = f1->GetChisquare ();
    ndf = f1->GetNDF ();

    x1 = u - BASE_LT * par [2];
    x2 = u + BASE_RT * par [2];

    {
	y1 = hist->GetBinContent (x1);
	y2 = hist->GetBinContent (x2);
	par [3] = par [4] = 1.;
	par [4+SkewedGauss] = (y2 - y1) / (x2 - x1);
	par [3+SkewedGauss] = y1 - par [4+SkewedGauss] * x1;
	par [5+SkewedGauss] = par [4+SkewedGauss] * 1e-4;
//	par [3] = par [4] = par [5] = par [6] = par [7] = par [8] = 1.;
	f2 = new TF1 ("f2", skewedGauss, x1, x2, 5+SkewedGauss+QuadBack);
	f2->SetLineColor (kRed-4);
	f2->SetLineWidth (3);

	f2->SetParameters (par);
	fitres = hist->Fit (f2, "QR0");
	fitres = hist->Fit (f2, "QSR0");
	result = Int_t (fitres);
	f2->GetParameters (par);
	if (! result) {
	    chi = f2->GetChisquare ();
	    ndf = f2->GetNDF ();
	    pol1Used = 1;
		cout<<"the value of ChiSquare is: "<<chi<<endl;
	}
    }


    dprintf ("%8.2lf %8.2lf %8.2lf CHI=%.1lf\n",
		par [0], par [1], par [2], chi);
    if ((par [2] < 1) || (par [0] < MIN_AREA) || result
		|| (fabs (u-par [1]) > (BASE_RT*SIGMA)))
	return -1;

    for (Int_t p=0; p<5; ++p) dprintf ("\tpar[%d] = %.1lf\n", p, par [p]);
    dprintf ("RES=%d CHI=%.1lf NDF=%d RedChi=%.1lf\n",
		result, chi, ndf, chi / ndf);

    if (pol1Used) f2->Draw ("same"); else f1->Draw ("same");

    TF1*sig = new TF1 ("sig", gauss, x1, x2, SkewedGauss+3);
    sig->SetParameters (par);
    sig->SetLineColor (kTeal);
    sig->SetLineWidth (3);
    sig->Draw ("same");

    TF1*back = new TF1 ("back", background, x1, x2, 2+QuadBack);
    back->SetParameters (&par [3+SkewedGauss]);
    back->SetLineColor (kBlue);
    back->SetLineWidth (3);
    back->Draw ("same");

    par [BackArea] = 0.;
    if (pol1Used)
	par [BackArea] = back->Integral (x1, x2);
    xx1 = (par [1] - par [Sigma] * 2.35 - 0.5);
    xx2 = (par [1] + par [Sigma] * 2.35 + 0.5);
    par [Peak] = sig->Integral (x1, x2);

    dprintf ("***u=%8.1lf (%5.0lf-%5.0lf) A=%8.0lf Area=%8.0lf\n",
		u, x1, x2, par [Peak], par [Area]);

    return chi / ndf;
}
// end of fitting funciton


// **************************************
// **************************************
// **************************************
// **************************************


Double_t IdentifyPeaks(TH1F *H, Double_t *X)

{
	
	/*TSpectrum *spec = new TSpectrum();
	Int_t numP = spec -> Search(H, 3, "g", 0.05);
	H->Draw("same");
	*/
	//alternate option with ShowPeaks()	

	Int_t	numP = H->ShowPeaks (2.0, "", 0.01);
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
	
	Double_t par[10];
    for (Int_t i=0; i<numP; i++) {
        cout<<"peak "<< i <<" is at "<<peaks[i]<<"\n";
        Double_t rChi = fitPeak (H, peaks [i], par);
        cout<<"the value of rchi is "<<rChi<<endl;
        if (rChi <0) {cout<<"peak "<< i <<" fit failed, moving to next peak"<<endl;
            continue;
        }

        X [numX] = par[1];
        
        if (X [numX] > 0) {
            printf ("the loop for X[numX]::%3d. %8.2lf %8.2lf %8.2lf %8.2lf\n", i+1, peaks [i], X [numX], par [Sigma], par [SkewLpar]);
            cout<<"the value of numX is: "<<numX<<endl<<endl;
            numX++;
        }
        if (rChi<0){
            mX->Delete();
        }
        
	}
    cout<<endl;

        
        
        Double_t energy_at_marker;
        string energy_at_marker_string;
        const char *charstr;

	// Writing on the canvas, the number of peaks fitted to be used for calibration
	for (Int_t j = 0; j < numX; j++)
	{
        energy_at_marker = X[j];;

		Int_t jj = j+1;
        Double_t y_cord_marker = H -> GetBinContent(X[j]) + 150;
        Double_t x_cord_marker = X[j] - 2;
        TText *markerPrint = new TText(x_cord_marker, y_cord_marker, Form("(%d)  %.2f", jj, energy_at_marker));
        markerPrint -> SetTextAngle(90);
        markerPrint -> SetTextSize(0.02);
        markerPrint -> SetTextColor(kRed);
        markerPrint -> Draw("SAME");
	}
    for (Int_t i= 0; i<numX; i++)
        {
        energy_at_marker =X[i];
        printf("peak # %d: channel: %.2f energy: %.2f \n", i+1, X[i], energy_at_marker);
        // cout<<"peak # "<<i+1<<": \t"<<"channel: "<<X[i]<<" energy: "<<energy_at_marker<<"\n";
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


void Eu152_gamma_fit(){
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

    while (reader.Next()) {
        for(int i = 0; i < phy_exogam_r->Exo_EAddBack.size(); i++){
            Double_t energy = phy_exogam_r->Exo_EAddBack[i];
            h1->Fill(energy);
        }
    }
    gCurrentHistogram=h1;

    h1->Draw();
    fit();

    // // Fit peaks with Gaussian + linear background
    // TF1 *fitFunc = new TF1("fitFunc", "[0]*exp(-0.5*((x-[1])/[2])**2) + [3] + [4]*x", 0, 4000);
    // fitFunc->SetParameters(1000, 121.8, 2.0, 10, 0); // Initial parameters for the first peak
    // h1->Fit(fitFunc, "R");

    // outputFile->Write();
    // outputFile->Close();
}
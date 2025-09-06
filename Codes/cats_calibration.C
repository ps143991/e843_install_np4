#include <iostream>
#include <fstream>
#include <TFile.h>
// #include <NPApplication.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>
#include <TPolyMarker.h>
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
#include <TSystem.h>

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
#define	SIGMA			50.0                    // a very important initialization of sigma parameter for gaussian fit
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


using namespace nptool;
Double_t numX[28];
TChain *chain = NULL; 
TH1F *hist = new TH1F("hist","hist",10000,0,1E6);
unsigned long long timestamp1;
string name[28];
string hist_name[28];
Double_t X[28][10];
Double_t X_pedestal_sigma[28][1];
Double_t xx1, xx2;
    TCanvas *c1 = new TCanvas();

enum Parameters {
Peak, Centroid, Sigma, SkewLpar, SkewRpar, BackA, BackB, BackC, Area, BackArea, PeakY, NumPar };

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
		{peak = exp (0.5*par[3]*par[3] + par[3]*arg); }
	else if (SkewR && (arg >= par[4]))
		{peak = exp (0.5*par[4]*par[4] + par[4]*arg); }
	else
		{peak = exp (-0.5*arg*arg);}
	return par[0]*peak;
}


Double_t skewedGauss (Double_t *x, Double_t *par)
{
	return (gauss(x, par) + background (x, &par [3+SkewedGauss]));      
}

Double_t fitPeak (TH1F*hist, Double_t u, Double_t*par){
    TF1		*f1, *f2;
    Double_t	chi=1e10;
    Double_t	x1, y1, x2, y2;
    TFitResultPtr	fitres;
    Int_t		result, ndf, pol1Used = 0;

        x1 = u - 1.5 * SIGMA;           //mean value +- 3 sigma currently
        x2 = u + 1.5 * SIGMA;
        f1 = new TF1 ("f1", "gaus", x1, x2);    //gaussian fit
        f1->SetLineColor (kGreen);
        f1->SetLineWidth (3);
        fitres = hist->Fit (f1, "RQS0");        // fit the histogram with the gaussian function
        result = Int_t (fitres);
        f1->GetParameters (par);                // get the parameters of the fit
        chi = f1->GetChisquare ();
        ndf = f1->GetNDF ();
        x1 = u - BASE_LT * par [2];             // adjust the x1 and x2 values based on the fit parameters
        x2 = u + BASE_RT * par [2];
        {
        y1 = hist->GetBinContent (x1);
        y2 = hist->GetBinContent (x2);
        par [3] = par [4] = 0.;                 // initialize skewness parameters
        par [4+SkewedGauss] = (y2 - y1) / (x2 - x1);    // calculate the slope of the background
        par [3+SkewedGauss] = y1 - par [4+SkewedGauss] * x1;    // calculate the intercept of the background
        par [5+SkewedGauss] = par [4+SkewedGauss] * 1e-4;   // set a small value for the background slope
    //	par [3] = par [4] = par [5] = par [6] = par [7] = par [8] = 1.; 
        f2 = new TF1 ("f2", skewedGauss, x1, x2, 5+SkewedGauss+QuadBack);  // create a new function for the skewed gaussian fit
        f2->SetLineColor (kRed-4);
        f2->SetLineWidth (3);

        f2->SetParameters (par);    // set the parameters of the skewed gaussian function
        fitres = hist->Fit (f2, "QR0"); // fit the histogram with the skewed gaussian function
        fitres = hist->Fit (f2, "QSR0"); // fit the histogram with the skewed gaussian function again

        c1->Update(); // Update the canvas to reflect the changes
        result = Int_t (fitres); // get the result of the fit
        f2->GetParameters (par);    // get the parameters of the fit
        if (! result) {
            chi = f2->GetChisquare ();
            ndf = f2->GetNDF ();
            pol1Used = 1;
            // cout<<"the value of ChiSquare is: "<<chi<<endl;
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

        if (pol1Used) f2->Draw ("same"); else f1->Draw ("same"); // draw the fitted function on the histogram

        TF1*sig = new TF1 ("sig", gauss, x1, x2, SkewedGauss+3);  // create a new function for the gaussian fit
        sig->SetParameters (par);
        sig->SetLineColor (kTeal);
        sig->SetLineWidth (3);
        sig->Draw ("same");

        TF1*back = new TF1 ("back", background, x1, x2, 2+QuadBack); // create a new function for the background
        back->SetParameters (&par [3+SkewedGauss]);
        back->SetLineColor (kBlue);
        back->SetLineWidth (3);
        back->Draw ("same");

        par [BackArea] = 0.;        // initialize the background area to 0
        if (pol1Used)               // if a polynomial was used for the background
            par [BackArea] = back->Integral (x1, x2); // calculate the background area
        
        xx1 = (par [1] - par [Sigma] * 2.35 - 0.5);     // calculate the lower limit of the peak
        xx2 = (par [1] + par [Sigma] * 2.35 + 0.5);     // calculate the upper limit of the peak
        par [Peak] = sig->Integral (x1, x2);            // calculate the peak area

        dprintf ("***u=%8.1lf (%5.0lf-%5.0lf) A=%8.0lf Area=%8.0lf\n",
            u, x1, x2, par [Peak], par [Area]);

        return chi / ndf;           // return the reduced chi-square value
}

void loadFILES(){
    chain = new TChain("RawTree");
    chain -> Add("../data/converted/402.root");
    cout<<"Total Entries: "<<chain->GetEntries()<<endl;
    chain->SetBranchStatus("cats.fCATS_DetX",1);
    chain->SetBranchStatus("cats.fCATS_StripX",1);
    chain->SetBranchStatus("cats.fCATS_ChargeX",1);

    chain->SetBranchStatus("cats.fCATS_DetY",1);
    chain->SetBranchStatus("cats.fCATS_StripY",1);
    chain->SetBranchStatus("cats.fCATS_ChargeY",1);

}

Int_t IdentifyPeaks(TH1F* H, Double_t *X, Double_t *sigmaPedestal)
{
    Int_t	numP = H->ShowPeaks (3.0, "", 0.01);
        if (! numP) {cout<<"no Peaks found"<<endl;}
        
        TList	*funcP = H->GetListOfFunctions ();
        if (! funcP) return 0;
            TPolyMarker	*mX = (TPolyMarker*)funcP->FindObject ("TPolyMarker");
        if (! mX) return 0;
            Double_t	tX, *peaks = mX->GetX ();
        if (! peaks) return 0;
        for (Int_t p=0; p<numP; ++p) {
            for (Int_t q=p+1; q<numP; ++q) {
                if (peaks [p] > peaks [q]) {
                    tX = peaks [p];
                    peaks [p] = peaks [q];
                    peaks [q] = tX;
                }}}

        
        Int_t numX_l = 0;
        
        Double_t par[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Initialize parameters for the fit
        for (Int_t i=0; i<numP; i++) {
            // cout<<"peak "<< i <<" is at "<<peaks[i]<<"\n";
            Double_t rChi = fitPeak (H, peaks [i], par);        // ***********Fit function call***********
            // cout<<"the values of rchi for all the peaks are "<<rChi<<" ";
            if (rChi <0) {printf ("\t\tthe loop for rChi:: peak# %3d. peak cent. %8.2lf rchi %8.2lf %8.2lf\n",
                    i+1, peaks [i], rChi, par [1]);
                continue;
            }
            X [numX_l] = par[1];

            if(i==0) sigmaPedestal[0] = par[2]; // Store the pedestal sigma for the first peak

            
            if (X [numX_l] > 0) {
                // printf ("the loop for X[numX_l]::%3d. %8.2lf %8.2lf %8.2lf %8.2lf\n", i+1, peaks [i], X [numX_l], par [Sigma], par [SkewLpar]);
                // cout<<"the value of numX_l is: "<<numX_l<<endl<<endl;
                numX_l++;
            }
            if (rChi<0){
                mX->Delete();
            }
            
        }

    return numX_l;
}

class Model {
    public:
        static Double_t Function2Pt(Double_t *x, Double_t *par) {
        // model 2-Pt function: y = m*x + c
        return par[0] + par[1]*x[0];   
}
};

void calibrate(const Double_t *X_ref, const Double_t *X, Double_t *fitParameters) {

    Double_t X_ref_4pts[4]= {X_ref[1], X_ref[2], X_ref[3], X_ref[4]};
    Double_t X_4pts[4]= {X[1], X[2], X[3], X[4]};


    Int_t cPoints = 4;
    TCanvas *c3 = new TCanvas("c3", "Calibration Fit", 800, 600);
    TGraph *graph_1 = new TGraph(cPoints, X_4pts, X_ref_4pts);

	TF1 *model_1 = new TF1("model_2pt", Model::Function2Pt, 0, 15000, 2);
	model_1->SetParameter(0, 1);
    model_1->SetParameter(1, 1);


    // Fit the model to data
    graph_1->Fit("model_2pt","Q");
    graph_1->SetLineWidth(2);
    graph_1->SetMarkerStyle(20);
    graph_1->SetMarkerColor(kBlue);
    graph_1->Draw("APL");
    c3->Update();
    // c3->WaitPrimitive(); // Wait for user to close the canvas   
    gSystem->Sleep(500);
    c3->Update();
    
    
        // Retrieve fitted parameters
    fitParameters[0] = model_1->GetParameter(0);
    fitParameters[1] = model_1->GetParameter(1);
    cout << "Calibration parameters: c = " << fitParameters[0]<< " m = " << fitParameters[1] << endl;

}

void Write(const int& stripNumber, const int& CATS_nbr, const Double_t *fitParameters, const Double_t *pedestal_sigma, const Double_t *pedestal, const int& choice) {

    fstream calibFile("newcalib_cats1_PS_X.txt",ios::app);
    string det;
    if (choice == 1) {
        det = "CATS_D" + to_string(CATS_nbr) + "_X" + to_string(stripNumber);
    } else if (choice == 2){
        det = "CATS_D" + to_string(CATS_nbr) + "_Y" + to_string(stripNumber);
    }

    if(fitParameters[0] == 0 && fitParameters[1] == 0) {
        calibFile << det << " " << 0 << " " << 1 << endl;
        calibFile << det + "_Q " << 0 << " " << 1 << endl;
    }

    calibFile << det << " " << pedestal[0] << " " << pedestal_sigma[0] <<endl;
    calibFile << det + "_Q " << fitParameters[0] << " " << fitParameters[1] << endl;
    calibFile.close();
}


void cats_calibration(){

    loadFILES();
    // double time_difference[chain->GetEntries()];


    Int_t nPoints = 28;

    cout<<"CATS1 or CATS2? (1/2): "<<endl;
    Int_t choice[2];
    std::cin>>choice[0];
    cout<<"X or Y? (1/2): "<<endl;
    char XorYCHOICE;
    std::cin>>XorYCHOICE;
    if (XorYCHOICE == 'x' || XorYCHOICE == 'X') choice[1] = 1;
    else if (XorYCHOICE == 'y' || XorYCHOICE == 'Y') choice[1] = 2;
    else {
        cout  << "\033[1;31m" << "Invalid choice. Please enter 1 for X or 2 for Y." << "\033[0m" << endl;
    
        return;
    }

    /*******************************************Loop for CATS X******************************************/
    if (choice[1] == 1){
        for (int i = 1; i<=nPoints; i++){
            
            c1->Update();
            chain->Draw(Form("cats.fCATS_ChargeX>>hc%dX(3500,0,14000)",i),Form("cats.fCATS_StripX==%d",i),""); 

            numX[i-1]=IdentifyPeaks(dynamic_cast<TH1F*>(gPad->GetPrimitive(Form("hc%dX",i))), X[i-1], X_pedestal_sigma[i-1]);
            

            //numX[0] will have numb of peaks for strip 1, numX[1] for strip 2 and so on
            if (numX[i-1] < 6)
            cout<<"\033[1;31m" << "the number of peaks is: "<<numX[i-1]<<"\033[0m" << endl;
            else
            cout<<"\033[1;32m" << "the number of peaks is: "<<numX[i-1]<<"\033[0m" << endl;
            
        }
        Double_t pedestal[28] = {0};

        for (int ll = 1; ll <= nPoints; ll++) { // will run for ll = 1, 2
            pedestal[ll-1] = X[ll-1][0]; // store the pedestal for each strip
        }
        cout<<"\033[33m" <<"choose reference strip number"<<"\033[0m" <<endl;
        Int_t refStrip;
        cin>>refStrip;
        if (refStrip < 1 || refStrip > 28) {
            cout << "\033[1;31m" <<"Invalid strip number. Please enter a number between 1 and 28." <<"\033[0m" << endl;
            return; }

        cout<<"\033[33" <<"the reference strip is: "<<refStrip<<"\033[0m" <<endl;
        Double_t X_ref[6];
        for (int ii = 0; ii < numX[refStrip-1]; ii++){
            cout<<"\033[33m" <<"the reference strip peak # "<<ii+1<<" is at: "<<X[refStrip-1][ii]<<"\033[0m" <<endl;
            X_ref[ii] = X[refStrip-1][ii];
        }
        
    
        for(int i = 1; i<= nPoints; i++){
            Double_t fitParameters[2]={0, 0}; // Initialize fit parameters
            if(numX[i-1] > 4) {
                calibrate(X_ref, X[i-1], fitParameters); // Call the calibrate function with X_ref and X);
                cout<<"\033[1;32m" <<"the calibration parameters for strip "<<i<<" are: "<<fitParameters[0]<<" and "<<fitParameters[1]<<"\033[0m" <<endl;
                Write(i,choice[0], fitParameters, X_pedestal_sigma[i-1], &pedestal[i-1], choice[1]); // gotta write 4 parameters, pedestal, sigma, and the calibration parameters
            }
            else {
                cout<<"\033[1;31m" <<"Not enough peaks for calibration in strip "<<i<<". Writing default values (1.X + 0)."<<"\033[0m" <<endl;
                fitParameters[0] = 0.0; // Default calibration parameter
                fitParameters[1] = 0.0; // Default calibration parameter
                Write(i,choice[0], fitParameters, X_pedestal_sigma[i-1], &pedestal[i-1], choice[1]); // Writing default values for calibration
            }
        }
    }

        /*******************************************Loop for CATS Y******************************************/
    else if (choice[1] == 2){
        for (int i = 1; i<=nPoints; i++){
            
            c1->Update();
            chain->Draw(Form("cats.fCATS_ChargeY>>hc%dY(3500,0,14000)",i),Form("cats.fCATS_StripY==%d",i),"");

            numX[i-1]=IdentifyPeaks(dynamic_cast<TH1F*>(gPad->GetPrimitive(Form("hc%dY",i))), X[i-1], X_pedestal_sigma[i-1]);
            

            //numX[0] will have numb of peaks for strip 1, numX[1] for strip 2 and so on
            
            cout<<"\033[1;32m" << "the number of peaks is: "<<numX[i-1]<<"\033[0m" << endl;
            
        }
        Double_t pedestal[28] = {0};

        for (int ll = 1; ll <= nPoints; ll++) { // will run for ll = 1, 2
            pedestal[ll-1] = X[ll-1][0]; // store the pedestal for each strip
        }
        
        cout<<"\033[33m" <<"choose reference strip number"<<"\033[0m" <<endl;
        Int_t refStrip;
        cin>>refStrip;
        if (refStrip < 1 || refStrip > 28) {
            cout << "\033[1;31m" <<"Invalid strip number. Please enter a number between 1 and 28." <<"\033[0m" << endl;
            return; }

        cout<<"\033[33m" <<"the reference strip is: "<<refStrip<<"\033[0m" <<endl;
        Double_t X_ref[6];
        for (int ii = 0; ii < numX[refStrip-1]; ii++){
            cout<<"\033[33m" <<"the reference strip peak # "<<ii+1<<" is at: "<<X[refStrip-1][ii]<<"\033[0m" <<endl;
            X_ref[ii] = X[refStrip-1][ii];
        }
        for(int i = 1; i<= nPoints; i++){
            Double_t fitParameters[2]={0, 0}; // Initialize fit parameters
            if(numX[i-1] > 4) {
                calibrate(X_ref, X[i-1], fitParameters); // Call the calibrate function with X_ref and X);
                cout<<"\033[1;32m" <<"the calibration parameters for strip "<<i<<" are: "<<fitParameters[0]<<" and "<<fitParameters[1]<<"\033[0m" <<endl;
                Write(i,choice[0], fitParameters, X_pedestal_sigma[i-1], &pedestal[i-1], choice[1]); // gotta write 4 parameters, pedestal, sigma, and the calibration parameters
            }
            else {cout<<"\033[1;31m" <<"Not enough peaks for calibration in strip "<<i<<". Writing default values (1.X + 0)."<<"\033[0m" <<endl;
            fitParameters[0] = 0.0; // Default calibration parameter
            fitParameters[1] = 0.0; // Default calibration parameter
            Write(i,choice[0], fitParameters, X_pedestal_sigma[i-1], &pedestal[i-1], choice[1]); // Writing default values for calibration
            }
        }
    }
}   

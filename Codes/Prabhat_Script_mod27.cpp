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

////////////////////////////////////////THE GLOBALS/////////////////////////////////////////////////
TChain *t = new TChain ("RoseNIAS");
 //a global variable to store the current histogram	

Int_t gLineNumber;
TH1F* gCurrentHistogram = nullptr;
string currentHistString;
//array of colors
Int_t colors[] = {kRed, kBlue, kGreen, kOrange, kMagenta, kCyan, kYellow, kBlueGreenYellow, kRedBlue, kRed, kBlue, kGreen, kOrange, kMagenta, kCyan, kYellow, kBlueGreenYellow, kRedBlue, kRed, kBlue, kGreen, kOrange, kMagenta, kCyan, kYellow, kBlueGreenYellow, kRedBlue};

Double_t X[100]; // the fitted peaks
Int_t size_of_X = (sizeof(X[100]/sizeof(X[0])));
Double_t selected_Peaks[6]; // peaks used for calibration in calib();

char calibFileName[100]; // file of the original calib constants
char newFileName[100]; // file with the new 4- point calib constants
char calibSpec[MAX_CALIB_LINES][MAX_SPEC_LENGTH]; // Array to store calibration specifications
Double_t A[MAX_CALIB_LINES][6]; // Array to store calibration parameters
Int_t numCalib; // number of lines in the calib file

TCanvas *myC = new TCanvas ("myC", "Original", 1020, 710);


vector<pair<TPolyMarker*, Double_t>> markers;
vector <pair<TText*, Double_t>> energy_marker_values;

bool executeMyExec = true;
bool executeMyExec2 = true;
TCanvas *myCanv2, *myCanv3;

Double_t energiesEu152[15]= {121.78, 244.70, 344.28, 778.90, 964.8, 1408.01};
FILE *fp; // a global pointer to the opened calib file
FILE *newFp;
TTimeStamp timestamp;
TString dateAndTime;
Double_t total2_1_area;
Double_t total2_2_area;
Double_t total2_3_area;
Double_t peak2_1_area;
Double_t peak2_2_area;
Double_t peak2_3_area;
Double_t bkg0_area;

Int_t s_no{};
Int_t numSpectra;
string currentHistStringArr_Overlay[16];
string currentHistStringArr_Grid[16];

// Create the buttons
    TButton *btn1;
    TButton *btn2;
    TButton *btn3;
    TButton *btn4;
    TButton *btn5;
    TButton *btn6;
    TButton *btn7;
    TButton *btn8;
//////////////////////////////////////////////////////////////////////////////////////////////////////



void errorExit (const char*fmt, ...)
{
va_list ap;
	va_start (ap, fmt);
	vprintf (fmt, ap);
	va_end (ap);
	exit (0);
}

// globFiles: a library to get "(file.*)"
vector<string> globFiles(const string& pattern)
{
    vector<string> files;
    glob_t globResult;

    if (glob(pattern.c_str(), GLOB_TILDE, nullptr, &globResult) == 0) {
        for (size_t i = 0; i < globResult.gl_pathc; ++i) {
            files.push_back(globResult.gl_pathv[i]);
        }
        globfree(&globResult);
    }

    return files;
}
class Model {
public:
static Double_t Function(Double_t *x, Double_t *par) {
    // Model function: y = a + b*x + c*x^2 + d*sqrt(x)
    return par[0] + par[1]*x[0] + par[2]*x[0]*x[0] + par[3]*TMath::Sqrt(x[0]);
}

static Double_t Function2Pt(Double_t *x, Double_t *par) {
	// model 2-Pt function: y = m*x + c
	return par[0] + par[1]*x[0];   
}

static Double_t Function5Pt(Double_t *x, Double_t *par) {
	// model 2-Pt function: y = m*x + c
	return par[0] + par[1]*x[0]+ par[2]*x[0]*x[0]+ par[3]*x[0]*x[0]*x[0]+ par[4]*x[0]*x[0]*x[0]*x[0];
}
};

Int_t* values_setter(Int_t *dim) {
    static Int_t storedValues[2] = {0, 0}; // Initialize storedValues to keep track of previously set values

    if (dim[0] == 0 && dim[1] == 0) {
        return storedValues; // Return the stored values when both dimensions are 0
    } else {
        storedValues[0] = dim[0]; // Set the stored values
        storedValues[1] = dim[1];
        return nullptr; // Indicate that new values are set
    }
}
void myexec2()
{
    if (!executeMyExec) return;
    TH1F *H[numSpectra];
    // Get event information
    Int_t event = myC->GetEvent();
    Int_t px = myC->GetEventX();
    Int_t py = myC->GetEventY();
    Int_t pad_width = myC->GetWw();
    Int_t pad_height = myC-> GetWh();


    if(!currentHistString.empty()){
        H[0] = (TH1F*)myC->GetListOfPrimitives()->FindObject(currentHistString.c_str());
        }
    else if (!currentHistStringArr_Overlay[0].empty()){
        for (Int_t i = 0; i <numSpectra; i++){
        H[i] = (TH1F*)myC->GetListOfPrimitives()->FindObject(currentHistStringArr_Overlay[i].c_str());}
    }

    

    Int_t firstBin = H[0]->GetXaxis()->GetFirst()+128;
    Int_t lastBin = H[0]->GetXaxis()->GetLast()+128;
    Int_t totalBins = H[0]->GetNbinsX()+128;
    Int_t shift = (Int_t)(0.1 * (lastBin - firstBin + 1));

    if (event == kButton1Double){

        if (px< (0.05*pad_width)){
            if((firstBin-128)<shift){
                for(Int_t i=0; i<numSpectra; i++){
                H[i]->GetXaxis()->SetRangeUser(128, lastBin);}

            }
            else{
                for(Int_t i=0; i<numSpectra; i++){
                H[i]->GetXaxis()->SetRangeUser(firstBin - shift, lastBin - shift);
            }}

            myC->Modified();
            myC->Update();
            }
        else if(px>(0.92*pad_width)){
            if((lastBin+shift)>8192){
                for(Int_t i=0; i<numSpectra; i++){
                H[i]->GetXaxis()->SetRangeUser(firstBin, 8192);
            }}
            else{
                for(Int_t i=0; i<numSpectra; i++){
                H[i]->GetXaxis()->SetRangeUser(firstBin + shift, lastBin + shift);
            }}
            myC->Modified();
            myC->Update();

        }}
        return;
}
void myexec()
{   
    if (!executeMyExec) return;
    TH1F *H[numSpectra];
    
    // Get event information
    Int_t event = gPad->GetEvent();
    Int_t px = gPad->GetEventX();
    Int_t py = gPad->GetEventY();
    Int_t pad_width = gPad->GetWw();
    Int_t pad_height = gPad-> GetWh();


    if(!currentHistString.empty()){
        H[0] = (TH1F*)gPad->GetListOfPrimitives()->FindObject(currentHistString.c_str());
        }
    else if (!currentHistStringArr_Overlay[0].empty()){
        for (Int_t i = 0; i <numSpectra; i++){
        H[i] = (TH1F*)gPad->GetListOfPrimitives()->FindObject(currentHistStringArr_Overlay[i].c_str());}
    }

    

    Int_t firstBin = H[0]->GetXaxis()->GetFirst()+128;
    Int_t lastBin = H[0]->GetXaxis()->GetLast()+128;
    Int_t totalBins = H[0]->GetNbinsX()+128;
    Int_t shift = (Int_t)(0.1 * (lastBin - firstBin + 1));

    if (event == kButton1Down) { // Left mouse button click
        // Convert pixel coordinates to histogram coordinates
        Double_t xd = gPad->AbsPixeltoX(px);
        Double_t yd = gPad->AbsPixeltoY(py);
        Double_t x = gPad->PadtoX(xd);
        Double_t y = gPad->PadtoY(yd);
     
            // Convert histogram coordinates to bin number
            Double_t bin = H[0]->GetXaxis()->FindBin(x);
            Double_t binCenter [numSpectra];
            
            Double_t binContent[numSpectra];

            // Get bin center and content
            for (Int_t i = 0; i<numSpectra; i++){
                binCenter[i]  = H[i]->GetXaxis()->GetBinCenter(bin);
                binContent[i] = H[i]->GetBinContent(bin);
            }

            Double_t energy_at_marker;
            string energy_at_marker_string;
            const char *charstr;

            TText *marker_energy = new TText(); 
            
            bool markerFound = false;
            for (auto it = markers.begin(); it != markers.end(); ++it) {
                if (it->second == binCenter[0]) {
                    // Remove the existing marker
                    it->first->Delete();
                    markers.erase(it);
                    cout << "Existing marker at bin " << binCenter[0] << " deleted" << endl;
                    markerFound = true;
                    break; 
                }
            }


            bool textFound = false;
            for (auto itt = energy_marker_values.begin(); itt != energy_marker_values.end(); ++itt) {
                if (itt->second == binCenter[0]) {
                    // Remove the existing marker
                    itt->first->Delete();
                    energy_marker_values.erase(itt);
                    textFound = true;
                    break; 
                }
            }

            if (y>=binContent[0] && !markerFound && !textFound && x>=firstBin && x<=lastBin) {
            // Draw a marker at (bin center, bin content)

            TPolyMarker *marker[numSpectra];
            for(Int_t i = 0; i<numSpectra; i++){
            marker[i] = new TPolyMarker(1, &binCenter[i], &binContent[i]);
            marker[i]->SetMarkerStyle(23); // Set marker style as desired
            marker[i]->SetMarkerSize(1.5); // Set marker size as desired
            marker[i]->SetMarkerColor(colors[i]); // Set marker color as desired
            marker[i]->Draw();}
            markers.push_back(make_pair(marker[0],binCenter[0]));

            TF1 *model2;
            if (A[gLineNumber-1][5] != 0){
            model2 = new TF1 ("model2", Model::Function, 0, 8192, 4);
            model2 -> FixParameter(0, A[gLineNumber-1][0]);
            model2 -> FixParameter(1, A[gLineNumber-1][1]);
            model2 -> FixParameter(2, A[gLineNumber-1][2]);
            model2 -> FixParameter(3, A[gLineNumber-1][5]);}

            else if (A[gLineNumber-1][5] == 0){
            model2 = new TF1 ("model2", Model::Function5Pt, 0, 8192, 5);
            model2 -> FixParameter(0, A[gLineNumber-1][0]);
            model2 -> FixParameter(1, A[gLineNumber-1][1]);
            model2 -> FixParameter(2, A[gLineNumber-1][2]);
            model2 -> FixParameter(3, A[gLineNumber-1][3]);
            model2 -> FixParameter(4, A[gLineNumber-1][4]);        
            }
            
            
            energy_at_marker = model2 -> Eval(binCenter[0]);

            energy_at_marker_string = to_string(energy_at_marker);
            charstr = energy_at_marker_string.c_str();
            marker_energy -> SetText(binCenter[0], (binContent[0])*1.05, Form("%.2f", energy_at_marker));
            
            marker_energy -> SetTextSize(0.03);
            marker_energy -> SetTextAngle(90);
            marker_energy -> SetTextColor(kBlue);
            marker_energy -> Draw("same");
            energy_marker_values.push_back(make_pair(marker_energy, binCenter[0]));

            cout<<"marked channel: "<<binCenter[0]<<" "<<"with energy: "<<energy_at_marker<<endl;
        }
    }
    if (event == kButton1Double){

        if (px< (0.05*pad_width)){
            if((firstBin-128)<shift){
                for(Int_t i=0; i<numSpectra; i++){
                H[i]->GetXaxis()->SetRangeUser(128, lastBin);}

            }
            else{
                for(Int_t i=0; i<numSpectra; i++){
                H[i]->GetXaxis()->SetRangeUser(firstBin - shift, lastBin - shift);
            }}

            myCanv2->Modified();
            myCanv2->Update();
            }
        else if(px>(0.92*pad_width)){
            if((lastBin+shift)>8192){
                for(Int_t i=0; i<numSpectra; i++){
                H[i]->GetXaxis()->SetRangeUser(firstBin, 8192);
            }}
            else{
                for(Int_t i=0; i<numSpectra; i++){
                H[i]->GetXaxis()->SetRangeUser(firstBin + shift, lastBin + shift);
            }}
            myCanv2->Modified();
            myCanv2->Update();

        }}
        return;
    
}

vector<string> ListBranches(string alpha, string beta)
{		
    TObjArray *branches = t->GetListOfBranches();
    //create the search pattern (step1)
    string searchPattern;
    if (beta == "00") {
        searchPattern = "_" + alpha + "_E"; // For addback
    } else {
        searchPattern = "_" + alpha + "_E" + beta; // For regular crystal branches
    }
    
    // all matching clovers for the search (step1)
    vector<string> matchingBranches; 
    for (Int_t i = 0; i < branches->GetEntries(); ++i) {
        TBranch *branch = dynamic_cast<TBranch *>(branches->At(i));
        if (branch) {
            string branchName = branch->GetName();
            // Check if branchName contains the searchPattern
            if (beta == "00") {
                // For addback, match only if the branchName ends with "_E"
                if (branchName.substr(branchName.size() - 2) == "_E" && branchName.find(alpha) != string::npos) {
                    matchingBranches.push_back(branchName);
                    cout << "Branch :  " << branchName << endl;
                }
            } else {
                // For regular crystal branches, check for the search pattern
                if (branchName.find(searchPattern) != string::npos) {
                    matchingBranches.push_back(branchName);
                    cout << "Branch :  " << branchName << endl;
                }
            }
        }
    }
		if (!matchingBranches.empty())
		{
			return matchingBranches;
		}
		else
		{
			cout << "No matching branches found." << endl;
			return vector<string>(); // Return an empty string if no matching branches
		}
}		




//	PROGRAM TO FIT A GAUSSIAN
Int_t xx1, xx2;
Int_t count1, count2, count3;
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

void save()
{
    ofstream file1;
    file1.open("Areas.txt", ios::out);
    
    dateAndTime = timestamp.AsString("lc");
    
    file1<<fixed<<setprecision(2);
    if(peak2_1_area !=0){
        s_no++;
    file1 << s_no << "\t" << peak2_1_area << endl;
    }
    if(peak2_2_area !=0){
        s_no++;
    file1 << s_no << "\t" << peak2_2_area << endl;
    }
    if(peak2_3_area !=0){
        s_no++;
    file1 << s_no << "\t" << peak2_3_area << endl;
    }
    file1.close();
}
Double_t fit1PeakChosen (TH1F*hist, Double_t*par, Double_t* markersOf2ndCanvas, Int_t sizeDeMarkers)
{
    hist = dynamic_cast<TH1F*>(myC->GetPrimitive(currentHistString.c_str()));
        if(!hist)
        {cout<<"hist not found"<<endl; return 1;}
        hist->GetListOfFunctions()->Delete();
        hist->Draw();
        Int_t numPeaksMarked = sizeDeMarkers-2;
        cout<<"the total number of peaks marked are: "<<numPeaksMarked<<endl;
        TF1 *f3, *f4;
        Int_t		result, ndf, pol1Used = 0;
        gPad -> SetBorderMode(-1);
        Double_t	chi;
        TFitResultPtr	fitresMulti;
        Double_t x1, y1, x2, y2, xA, yA;
        xA = markersOf2ndCanvas[1];
        yA = hist->GetBinContent(xA-128+1); //"+1" done to count [0-127]: [127] = 128th bin number
        x1 = markersOf2ndCanvas[0];
        x2 = markersOf2ndCanvas[sizeDeMarkers-1];
        y1 = hist->GetBinContent(x1-128+1);
        y2 = hist->GetBinContent(x2-128+1);
        Double_t x1temp= xA - 2.5*SIGMA;
        Double_t x2temp= xA + 2.5*SIGMA;
        cout<<"x1 is: "<<x1<<" x2 is: "<<x2<<endl;
        cout<<"xA is: "<<xA<<endl;
        f3 = new TF1 ("f1", "gaus", x1temp, x2temp);
        f3->SetLineColor (kGreen);
        f3->SetLineWidth (3);
        f3->SetParameter(1,xA);
        f3->SetParLimits(2, 1.5, 3.5);
        hist->Fit(f3,"RQS0");
        f3->GetParameters(&par[2]);
        f4 = new TF1("f4", "skewedGauss1", x1, x2, 7);
        
        //***************** SETTING PARAMETERS ********************// 
        par [1] = (y2 - y1) / (x2 - x1);    
	    par [0] = y1 - par [1] * x1;

        f4->SetParameters (par);

        cout<<"Freeze background? (y/n)\t"<<endl;
        char freeze_bkg;
        cin>>freeze_bkg;
        if(freeze_bkg == 'y' || freeze_bkg == 'Y'){
            f4->FixParameter(0, par[0]);
            f4->FixParameter(1, par[1]);
        }

        cout <<"Freeze sigma? (y/n)\t"<<endl;
        char freeze_sigma;
        Double_t sigma;
        cin>>freeze_sigma;
        if(freeze_sigma == 'y' || freeze_sigma == 'Y'){
            cout<<"The approximated sigma is: \t"<<par[4]<<endl;
            cout<<"Assign/Re-insert the value of sigma: \t"<<endl;
            cin>>sigma;
            f4->FixParameter(4, sigma);
        }

        cout<<"Freeze Centroid? (y/n)\t"<<endl;
        char freeze_cent;
        cin>>freeze_cent;
        if(freeze_cent == 'y' || freeze_cent == 'Y'){
            f4 -> FixParameter(3, xA);
        }

        f4->SetLineColor (kRed+2);
        f4->SetLineWidth (3);
        fitresMulti = hist->Fit (f4, "R");

        result = Int_t (fitresMulti);
        f4 -> GetParameters (par);
        Double_t value_at_peak1;
        value_at_peak1= f4-> Eval(par[3]);

            chi = f4->GetChisquare();
            ndf = f4->GetNDF();
            cout<<"the value of chisquare is: "<<chi<<endl;
       
        TF1 *bkg1 = new TF1 ("sig1_bkg","[0]+[1]*(x)", x1, x2);
        bkg1->SetParameters(par);
        bkg1->SetLineColor (kBlack);
        bkg1->SetLineWidth (3);
        bkg1->Draw ("same");
        total2_1_area = f4 -> Integral(x1, x2);
        bkg0_area  = bkg1 -> Integral(x1, x2);
        peak2_1_area = total2_1_area - bkg0_area;

        cout<<"The AREA of the peak is: "<<peak2_1_area<<endl;
        



  /*       TF1 *sig1_bkg1 = new TF1 ("sig1_bkg","[0]+[1]*(x)+[2]*exp(-0.5*((x-[3])/[4])*((x-[3])/[4])*(1-([5]*(x-[3])/[4])))", x1, x2);
        sig1_bkg->SetParameters (par);
        sig1_bkg->SetLineColor (kTeal);
        sig1_bkg->SetLineWidth (3);
        sig1_bkg->Draw ("same");

        TF1 *sig2_bkg = new TF1 ("sig2_bkg","[0]+[1]*(x)+[2]*exp(-0.5*((x-[3])/[4])*((x-[3])/[4])*(1+([6]*(x-[3])/[4])))", x1, x2);
        for (Int_t i = 0; i <2; i++)
        {
            sig2_bkg->SetParameter(i, par[i]);
        }
        for (Int_t i = 2; i <7; i++)
        {
            sig2_bkg->SetParameter(i, par[i+5]);

        }
        sig2_bkg->SetLineColor (kTeal+2);
        sig2_bkg->SetLineWidth (3);
        sig2_bkg->Draw ("same");  
        TPolyMarker *newPMC1 = new TPolyMarker(1, &par[3], &value_at_peak1);
        TPolyMarker *newPMC2 = new TPolyMarker(1, &par[8], &value_at_peak2);
        newPMC1->SetMarkerStyle(23); // Set marker style as desired
        newPMC1->SetMarkerSize(1.5); // Set marker size as desired
        newPMC1->SetMarkerColor(kBlue); // Set marker color as desired
        newPMC2->SetMarkerStyle(23); // Set marker style as desired
        newPMC2->SetMarkerSize(1.5); // Set marker size as desired
        newPMC2->SetMarkerColor(kBlue); // Set marker color as desired
        newPMC1->Draw("same");
        newPMC2->Draw("same"); */

return chi/ndf;
}

Double_t fit2PeaksChosen (TH1F*hist, Double_t*par, Double_t* markersOf2ndCanvas, Int_t sizeDeMarkers)
{       
        hist = dynamic_cast<TH1F*>(myC->GetPrimitive(currentHistString.c_str()));
        if(!hist)
        {cout<<"hist not found"<<endl; return 1;}
        hist->GetListOfFunctions()->Delete();
        hist->Draw();
        Int_t numPeaksMarked = sizeDeMarkers-2;
        cout<<"the total number of peaks marked are: "<<numPeaksMarked<<endl;
        TF1		*f1, *f2;//  *f4, *f5, *f6, *f7;
        Int_t	result, ndf, pol1Used = 0;
        gPad -> SetBorderMode(-1);
        Double_t	chi;
        TFitResultPtr	fitresMulti;
        Double_t	x1, y1, x2, y2, xA, xB, yA, yB;
        xA = markersOf2ndCanvas[1];
        xB = markersOf2ndCanvas[2];
        yA = hist->GetBinContent(xA -128+1);
        yB = hist->GetBinContent(xB -128+1);
        x1 = markersOf2ndCanvas[0];
        x2 = markersOf2ndCanvas[sizeDeMarkers-1];
        y1 = hist->GetBinContent(x1-128+1);     //"+1" done to count [0-127]: [127] = 128th bin number
        y2 = hist->GetBinContent(x2-128+1);

        Double_t x1temp= xA - 2.5*SIGMA;
        Double_t x2temp= xB + 2.5*SIGMA;
        cout<<"x1 is: "<<x1<<" x2 is: "<<x2<<endl;
        cout<<"xA is: "<<xA<<" xB is: "<<xB<<endl;
        Double_t MiddlePoint = xA + ((xB - xA)/2);
        cout<<"MiddlePoint is: "<<MiddlePoint<<endl;
        f1 = new TF1 ("f1", "gaus", x1temp, MiddlePoint-0.5);
        f2 = new TF1 ("f2", "gaus", MiddlePoint+0.5, x2temp);

        f1->SetLineColor (kGreen);
        f1->SetLineWidth (3);
        f2->SetLineColor (kGreen+2);
        f2->SetLineWidth (3);
        
        f1->SetParameter(1,xA);
        f2->SetParameter(1,xB);
        //f1->SetParameter(0, yA);
        //f2->SetParameter(0, yB);
        f1->SetParLimits(2, 1.5, 3.5);
        f2->SetParLimits(2, 1.5, 3.5);
        
        
        hist->Fit(f1, "RQS0");
        hist->Fit(f2, "RQS0");

        f1->GetParameters(&par[2]);
        f2->GetParameters(&par[7]);

        //TF1 *total1 = new TF1("total", "gaus(0)+gaus(3)", x1, x2);
        TF1 *total1 = new TF1("total1", "gaus(0) + gaus(3)", x1temp, x2temp);
        total1 -> SetLineColor(kBlack);
        total1->SetLineWidth (2);

        for (Int_t i = 0; i < 3; ++i) {
            total1->SetParameter(i, par[i+2]);
        }
        for (Int_t i = 3; i < 6; ++i) {
            total1->SetParameter(i, par[i+4]);
        }
        
        fitresMulti = hist->Fit (total1, "RQS0");
        result = Int_t (fitresMulti);

        for(Int_t i = 0; i<3; i++)                            
        {                                                   
        par[i+2] = total1->GetParameter(i);                 
        }                                                   
        for(Int_t i = 3; i<6; i++)                            
        {                                                   
        par[i+4] = total1->GetParameter(i);                 
        }                                                   
                 
        chi = total1->GetChisquare ();
        ndf = total1->GetNDF ();
        /* x1 = xA - BASE_LT * par [4];
        x2 = xB + BASE_RT * par [9]; */

        cout<<"THE 2 SIGMAs "<<par[4]<<" "<<par[9]<<endl;
        cout<<"THE 2 PEAKS: "<<par[3]<<" "<<par[8]<<endl;
        { 
            TF1 *total2 = new TF1("total2", "skewedGauss2", x1, x2, 12);
            total2->SetLineColor (kRed+2);
	        total2->SetLineWidth (3);
            total2->SetParameters (par);

            //***************** SETTING PARAMETERS ********************// 

            par [1] = (y2 - y1) / (x2 - x1);
	        par [0] = y1 - par [1] * x1;
            par [5] = par [11] = 0;     // setting skewness parameters 
            par [6] = par [10] = 0;

            cout<<"Freeze background? (y/n)\t"<<endl;
            char freeze_bkg;
            cin>>freeze_bkg;
            if(freeze_bkg == 'y' || freeze_bkg == 'Y'){
            total2->FixParameter(0, par[0]);
            total2->FixParameter(1, par[1]);
            }
            
            cout <<"Freeze sigma? (y/n)\t"<<endl;
            char freeze_sigma;
            Double_t sigma;
            cin>>freeze_sigma;
            if(freeze_sigma == 'y' || freeze_sigma == 'Y'){
                if(yB>yA) {
                cout<<"The approximated sigma (largest peak) is: \t"<<par[9]<<endl;}
                else {
                cout<<"The approximated sigma (largest peak) is: \t"<<par[4]<<endl;}
                
                cout<<"Assign/Re-insert the value of sigma: \t"<<endl;
                cin>>sigma;
                total2->FixParameter(4, sigma);
                total2->FixParameter(9, sigma);
            }

            cout<<"Freeze Centroid? (y/n)\t"<<endl;
            char freeze_cent;
            cin>>freeze_cent;
            if(freeze_cent == 'y' || freeze_cent == 'Y'){
                cout<<"for 1st? (y/n)"<<endl;
                char freeze_cent1;
                cin>>freeze_cent1;
                if(freeze_cent1 == 'y' || freeze_cent1 == 'Y'){
                total2 -> FixParameter(3, xA);}
                cout<<"for 2nd? (y/n)"<<endl;
                char freeze_cent2;
                cin>>freeze_cent2;
                if(freeze_cent2 == 'y' || freeze_cent2 == 'Y'){
                total2 -> FixParameter(8, xB);}
            }

            fitresMulti = hist->Fit (total2, "R");

            result = Int_t (fitresMulti);
            total2 -> GetParameters (par);
            Double_t value_at_peak1, value_at_peak2;
            value_at_peak1= total2-> Eval(par[3]);
            value_at_peak2= total2-> Eval(par[8]);

            if (!result){
                chi = total2->GetChisquare();
                ndf = total2->GetNDF();
                cout<<"the value of chisquare is: "<<chi<<endl;
            }
            TF1 *bkg = new TF1 ("sig1_bkg","[0]+[1]*(x)", x1, x2);
            bkg->SetParameters(par);
            bkg->SetLineColor (kBlack);
            bkg->SetLineWidth (3);
            bkg->Draw ("same");
        

            TF1 *sig1_bkg = new TF1 ("sig1_bkg","[0]+[1]*(x)+gaus(2)", x1, x2);
            sig1_bkg->SetParameters (par);
            sig1_bkg->SetLineColor (kTeal);
            sig1_bkg->SetLineWidth (3);
            sig1_bkg->Draw ("same");

            TF1 *sig2_bkg = new TF1 ("sig2_bkg","[0]+[1]*(x)+ gaus(2)", x1, x2);
            for (Int_t i = 0; i <2; i++)
            {
                sig2_bkg->SetParameter(i, par[i]);

            }
            for (Int_t i = 2; i <5; i++)
            {
                sig2_bkg->SetParameter(i, par[i+5]);

            }
            sig2_bkg->SetLineColor (kTeal+2);
            sig2_bkg->SetLineWidth (3);
            sig2_bkg->Draw ("same");

            total2_1_area = sig1_bkg -> Integral(x1, x2);
            total2_2_area = sig2_bkg -> Integral(x1, x2);
            bkg0_area = bkg -> Integral(x1, x2);
            peak2_1_area = total2_1_area - bkg0_area;
            peak2_2_area = total2_2_area - bkg0_area;
            cout<<"The AREA of peak 1 is: "<<peak2_1_area<<endl;
            cout<<"The AREA of peak 2 is: "<<peak2_2_area<<endl;

 /*            TF1 *sig1_bkg= new TF1 ("sig1_bkg", [0]+[1]*(x) + gauss(2), x1, x2, );
            sig1_bkg->SetParameters (&par[2]);
            sig1_bkg->SetLineColor (kTeal);
            sig1_bkg->SetLineWidth (3);
            sig1_bkg->Draw ("same");

            TF1 *sig2_bkg= new TF1 ("sig1_bkg", gauss, x1, x2, 5);
            sig2_bkg->SetParameters (&par[7]);
            sig2_bkg->SetLineColor (kTeal+2);
            sig2_bkg->SetLineWidth (3);
            sig2_bkg->Draw ("same");
 */

            TPolyMarker *newPMC1 = new TPolyMarker(1, &par[3], &value_at_peak1);
            TPolyMarker *newPMC2 = new TPolyMarker(1, &par[8], &value_at_peak2);
            newPMC1->SetMarkerStyle(23); // Set marker style as desired
            newPMC1->SetMarkerSize(1.5); // Set marker size as desired
            newPMC1->SetMarkerColor(kBlue); // Set marker color as desired
            newPMC2->SetMarkerStyle(23); // Set marker style as desired
            newPMC2->SetMarkerSize(1.5); // Set marker size as desired
            newPMC2->SetMarkerColor(kBlue); // Set marker color as desired
            newPMC1->Draw("same");
            newPMC2->Draw("same");
            /* TList *list53 = hist -> GetListOfFunctions();
            if (list53) {
                TIter next(list53);
                TObject *obj;
                while ((obj = next())) {
                    // Check if the object is of type TF1
                    TF1 *func = dynamic_cast<TF1*>(obj);
                    if (func) {
                        // Print information about the TF1 object
                        cout << "Function name: " << func->GetName() << endl;
                        cout << "Function title: " << func->GetTitle() << endl;
                        // You can print more properties of the function if needed
                    } else {
                        // Handle other types of objects if necessary
                        cout << "Object is not a TF1 function." << endl;
                    }
                }
            } else {
                cout << "No list of functions found." << endl;
            } */
        } 
        

    return chi / ndf;
    
}
Double_t fit3PeaksChosen (TH1F*hist, Double_t*par, Double_t* markersOf2ndCanvas, Int_t sizeDeMarkers)
{
    hist = dynamic_cast<TH1F*>(myC->GetPrimitive(currentHistString.c_str()));
        if(!hist)
        {cout<<"hist not found"<<endl; return 1;}
        hist->GetListOfFunctions()->Delete();
        hist->Draw();
        Int_t numPeaksMarked = sizeDeMarkers-2;
        cout<<"the total number of peaks marked are: "<<numPeaksMarked<<endl;
        TF1 *f5, *f6, *f7;
        Int_t	result, ndf, pol1Used = 0;
        Double_t	chi;
        TFitResultPtr	fitresMulti;
        gPad -> SetBorderMode(-1);
        Double_t	x1, y1, x2, y2, xA, xB, xC, yA, yB, yC;
        xA = markersOf2ndCanvas[1];
        xB = markersOf2ndCanvas[2];
        xC = markersOf2ndCanvas[3];
        yA = hist->GetBinContent(xA-128+1);
        yB = hist->GetBinContent(xB-128+1);
        yC = hist->GetBinContent(xC-128+1);
        x1 = markersOf2ndCanvas[0];
        x2 = markersOf2ndCanvas[sizeDeMarkers-1];
        y1 = hist->GetBinContent(x1-128+1);
        y2 = hist->GetBinContent(x2-128+1);

        Double_t x1temp= xA - 2.5*SIGMA;
        Double_t x2temp= xB - 2.5*SIGMA;
        Double_t x3temp= xB + 2.5*SIGMA;
        Double_t x4temp= xC + 2.5*SIGMA;
        Double_t MiddlePoint1 = xA + ((xB - xA)/2);
        Double_t MiddlePoint2 = xB + ((xC - xB)/2);
        f5 = new TF1 ("f1", "gaus", x1temp, MiddlePoint1-0.5);
        f6 = new TF1 ("f2", "gaus", MiddlePoint1+0.5, MiddlePoint2-0.5);
        f7 = new TF1 ("f2", "gaus", MiddlePoint2+0.5, x4temp);
        
        f5->SetLineColor (kGreen);
        f5->SetLineWidth (3);
        f6->SetLineColor (kGreen+2);
        f6->SetLineWidth (3);
        f7->SetLineColor (kGreen+4);
        f7->SetLineWidth (3);

        f5->SetParameter(1,xA);
        f6->SetParameter(1,xB);
        f7->SetParameter(1,xC);
        f5->SetParLimits(2, 1.5, 3.5);
        f6->SetParLimits(2, 1.5, 3.5);
        f7->SetParLimits(2, 1.5, 3.5);

        hist -> Fit(f5, "QSR0");
        hist -> Fit(f6, "QSR0");
        hist -> Fit(f7, "QSR0");

        f5->GetParameters(&par[2]);
        f6->GetParameters(&par[7]);
        f7->GetParameters(&par[12]);

        TF1 *total1 = new TF1("total1", "gaus(0) + gaus(3) + gaus(6)", x1temp, x4temp);
        total1 -> SetLineColor(kBlack);
        total1->SetLineWidth (2);

        for (Int_t i = 0; i < 3; ++i) {
            total1->SetParameter(i, par[i+2]);
        }
        for (Int_t i = 3; i < 6; ++i) {
            total1->SetParameter(i, par[i+4]);
        }
        for (Int_t i = 6; i < 9; ++i) {
            total1->SetParameter(i, par[i+6]);
        }
        fitresMulti = hist->Fit (total1, "QSR0");
        result = Int_t (fitresMulti);

        for(Int_t i = 0; i<3; i++)                            
        {                                                   
        par[i+2] = total1->GetParameter(i);
        }             
        for(Int_t i = 3; i<6; i++)                            
        {                 
        par[i+4] = total1->GetParameter(i);
        }
        for(Int_t i = 6; i<9; i++)
        {                            
        par[i+6] = total1->GetParameter(i);
        }
        chi = total1->GetChisquare ();
        ndf = total1->GetNDF ();
        {   
            TF1 *total2 = new TF1("total2", "skewedGauss3", x1, x2, 17);
            total2 ->SetLineColor (kRed+2);
	        total2->SetLineWidth (3);
            total2 -> SetParameters(par);

            //***************** SETTING PARAMETERS ********************//

            par [1] = (y2 - y1) / (x2 - x1);
	        par [0] = y1 - par [1] * x1;
            par [5] = par [16] = 0;
            par [6] = par [10] = par [11] = par [15]= 0;
            
            cout<<"Freeze background? (y/n)\t"<<endl;
            char freeze_bkg;
            cin>>freeze_bkg;
            if(freeze_bkg == 'y' || freeze_bkg == 'Y'){
            total2->FixParameter(0, par[0]);
            total2->FixParameter(1, par[1]);
            }

            cout <<"Freeze sigma? (y/n)\t"<<endl;
            char freeze_sigma;
            Double_t sigma, which_sigma;
            cin>>freeze_sigma;
            if(freeze_sigma == 'y' || freeze_sigma == 'Y'){
                if(yC>yB){if(yC>yA) which_sigma=par[14]; else which_sigma=par[4];}
                else { if (2>1) which_sigma=par[9]; else which_sigma=par[4];}
                cout<<"The approximated sigma (largest peak)is: \t"<<which_sigma<<endl;
                cout<<"Assign/Re-insert the value of sigma: \t"<<endl;
                cin>>sigma;
                total2->FixParameter(4, sigma);
                total2->FixParameter(9, sigma);
                total2->FixParameter(14,sigma);
            }

            cout<<"Freeze Centroid? (y/n)\t"<<endl;
            char freeze_cent;
            cin>>freeze_cent;
            if(freeze_cent == 'y' || freeze_cent == 'Y'){
                cout<<"for 1st? (y/n)"<<endl;
                char freeze_cent1;
                cin>>freeze_cent1;
                if(freeze_cent1 == 'y' || freeze_cent1 == 'Y'){
                total2 -> FixParameter(3, xA);}
                cout<<"for 2nd? (y/n)"<<endl;
                char freeze_cent2;
                cin>>freeze_cent2;
                if(freeze_cent2 == 'y' || freeze_cent2 == 'Y'){
                total2 -> FixParameter(8, xB);}
                cout<<"for 3rd? (y/n)"<<endl;
                char freeze_cent3;
                cin>>freeze_cent3;
                if(freeze_cent3 == 'y' || freeze_cent3 == 'Y'){
                total2 -> FixParameter(14, xC);}
            }


            fitresMulti = hist->Fit (total2, "R");

            result = Int_t (fitresMulti);
            total2 -> GetParameters (par);
            Double_t value_at_peak1, value_at_peak2, value_at_peak3;
            value_at_peak1= total2-> Eval(par[3]);
            value_at_peak2= total2-> Eval(par[8]);
            value_at_peak3= total2-> Eval(par[13]);

            if (!result){
                chi = total2->GetChisquare();
                ndf = total2->GetNDF();
                cout<<"the value of chisquare is: "<<chi<<endl;
            }
            TF1 *bkg = new TF1 ("sig1_bkg","[0]+[1]*(x)", x1, x2);
            bkg->SetParameters(par);
            bkg->SetLineColor (kBlack);
            bkg->SetLineWidth (3);
            bkg->Draw ("same");
            Double_t area_bkg;

            TF1 *sig1_bkg = new TF1 ("sig1_bkg","[0]+[1]*(x)+gaus(2)", x1, x2);
            sig1_bkg->SetParameters (par);
            sig1_bkg->SetLineColor (kTeal);
            sig1_bkg->SetLineWidth (3);
            sig1_bkg->Draw ("same");

            TF1 *sig2_bkg = new TF1 ("sig2_bkg","[0]+[1]*(x)+ gaus(2)", x1, x2);
            for (Int_t i = 0; i <2; i++)
            {
                sig2_bkg->SetParameter(i, par[i]);

            }
            for (Int_t i = 2; i <5; i++)
            {
                sig2_bkg->SetParameter(i, par[i+5]);

            }
            sig2_bkg->SetLineColor (kTeal+2);
            sig2_bkg->SetLineWidth (3);
            sig2_bkg->Draw ("same");

            TF1 *sig3_bkg = new TF1 ("sig3_bkg","[0]+[1]*(x)+ gaus(2)", x1, x2);
            for (Int_t i = 0; i <2; i++)
            {
                sig3_bkg->SetParameter(i, par[i]);

            }
            for (Int_t i = 2; i <5; i++)
            {
                sig3_bkg->SetParameter(i, par[i+10]);

            }
            sig3_bkg->SetLineColor (kTeal+2);
            sig3_bkg->SetLineWidth (3);
            sig3_bkg->Draw ("same");

            total2_1_area = sig1_bkg -> Integral(x1, x2);
            total2_2_area = sig2_bkg -> Integral(x1, x2);
            total2_3_area = sig3_bkg -> Integral(x1, x2);

            bkg0_area = bkg -> Integral(x1, x2);
            peak2_1_area = total2_1_area - bkg0_area;
            peak2_2_area = total2_2_area - bkg0_area;
            peak2_3_area = total2_3_area - bkg0_area;
            cout<<"The AREA of peak 1 is: "<<peak2_1_area<<endl;
            cout<<"The AREA of peak 2 is: "<<peak2_2_area<<endl;
            cout<<"The AREA of peak 3 is: "<<peak2_3_area<<endl;

            TPolyMarker *newPMC1 = new TPolyMarker(1, &par[3], &value_at_peak1);
            TPolyMarker *newPMC2 = new TPolyMarker(1, &par[8], &value_at_peak2);
            TPolyMarker *newPMC3 = new TPolyMarker(1, &par[13], &value_at_peak3);
            newPMC1->SetMarkerStyle(23); // Set marker style as desired
            newPMC1->SetMarkerSize(1.5); // Set marker size as desired
            newPMC1->SetMarkerColor(kBlue); // Set marker color as desired
            newPMC2->SetMarkerStyle(23); // Set marker style as desired
            newPMC2->SetMarkerSize(1.5); // Set marker size as desired
            newPMC2->SetMarkerColor(kBlue); // Set marker color as desired
            newPMC3->SetMarkerStyle(23); // Set marker style as desired
            newPMC3->SetMarkerSize(1.5); // Set marker size as desired
            newPMC3->SetMarkerColor(kBlue); // Set marker color as desired
            newPMC1->Draw("same");
            newPMC2->Draw("same");
            newPMC3->Draw("same");


        }
        return chi/ndf;

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
        if (rChi <0) {dprintf ("\t\tthe loop for rChi::%3d. %8.2lf %8.2lf %8.2lf %8.2lf\n",
                i+1, peaks [i], rChi, par [1], par [SkewLpar]);
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
    TF1 *model2;
        if (A[gLineNumber-1][5] != 0){
        model2 = new TF1 ("model2", Model::Function, 0, 8192, 4);
        model2 -> FixParameter(0, A[gLineNumber-1][0]);
        model2 -> FixParameter(1, A[gLineNumber-1][1]);
        model2 -> FixParameter(2, A[gLineNumber-1][2]);
        model2 -> FixParameter(3, A[gLineNumber-1][5]);}

        else if (A[gLineNumber-1][5] == 0){
        model2 = new TF1 ("model2", Model::Function5Pt, 0, 8192, 5);
        model2 -> FixParameter(0, A[gLineNumber-1][0]);
        model2 -> FixParameter(1, A[gLineNumber-1][1]);
        model2 -> FixParameter(2, A[gLineNumber-1][2]);
        model2 -> FixParameter(3, A[gLineNumber-1][3]);
        model2 -> FixParameter(4, A[gLineNumber-1][4]);        
        }
        
        
        Double_t energy_at_marker;
        string energy_at_marker_string;
        const char *charstr;

	// Writing on the canvas, the number of peaks fitted to be used for calibration
	for (Int_t j = 0; j < numX; j++)
	{
        energy_at_marker = model2 -> Eval(X[j]);

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
        energy_at_marker = model2 -> Eval(X[i]);
        printf("peak # %d: channel: %.2f energy: %.2f \n", i+1, X[i], energy_at_marker);
        // cout<<"peak # "<<i+1<<": \t"<<"channel: "<<X[i]<<" energy: "<<energy_at_marker<<"\n";
        }
        cout<<endl;
   return numX;

}
void ShowSection(TH1F* histo, double x1, double x2) {
    histo->GetXaxis()->SetRangeUser(x1, x2);
    gPad->Modified();
    gPad->Update();
}

// Function to unzoom the spectrum
void UnZoom(TH1F *histo) {
    histo->GetXaxis()->UnZoom();
    gPad->Modified();
    gPad->Update();
}

void Expand(TH1F *histo) {
    TAxis* xAxis = histo->GetXaxis();
    double x1 = xAxis->GetBinLowEdge(xAxis->GetFirst());
    double x2 = xAxis->GetBinUpEdge(xAxis->GetLast());
    double center = (x1 + x2) / 2;
    double range = (x2 - x1) * 0.8 / 2;
    xAxis->SetRangeUser(center - range, center + range);
    gPad->Modified();
    gPad->Update();
}

void Compress(TH1F *histo) {
    TAxis* xAxis = histo->GetXaxis();
    double x1 = xAxis->GetBinLowEdge(xAxis->GetFirst());
    double x2 = xAxis->GetBinUpEdge(xAxis->GetLast());
    double center = (x1 + x2) / 2;
    double range = (x2 - x1) * 1.25 / 2; // Increase range by 25% (20% on each side)
    xAxis->SetRangeUser(center - range, center + range);
    gPad->Modified();
    gPad->Update();
}


TH1F* ShowSpectra (const string& branchName) {
    // Create the canvas

    myC->cd();

    // Create the histogram
    TH1F* histoo = new TH1F("histoo", "GlobalHistogram", 8064, 128, 8192);
    t->Draw((branchName + ">>histoo").c_str());
    histoo->SetName(branchName.c_str());
    
    if (histoo != NULL)
        return histoo;
    else
        return NULL;
}


void ShowSpectraGrid(const vector<vector<string>> &branchNames, Int_t rows, Int_t columns)
{
    Int_t dim[2];
    dim[0]=rows;
    dim[1]=columns;
    values_setter(dim);
    myC->Divide(rows, columns);

    // Create an array of histograms
    TH1F *histograms[numSpectra];

    for (Int_t i = 0; i < numSpectra; ++i)
    {
        myC->cd(i + 1);
        // Create a new histogram for each branch8192
        histograms[i] = new TH1F(("h" + to_string(i)).c_str(), branchNames[i][0].c_str(), 8064, 128, 8192);
        t->Draw((branchNames[i][0] + ">>h" + to_string(i)).c_str());
        histograms[i]->Draw();
        histograms[i]->SetName(branchNames[i][0].c_str());
        currentHistStringArr_Grid[i] = histograms[i]->GetName();
        cout<<"hist " <<currentHistStringArr_Grid[i]<<endl;
        myC->Update();
    }

    myC->Update();
    myC->Draw();
}

void ShowSpectraOverlay(const vector<vector<string>> &branchNames, Int_t numSpectra)
{
    // TCanvas *c2 = new TCanvas("CanvasOverlay", "Overlay");
    // c2->cd();
    // Create a legend to identify each spectrum
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);

    
	TH1F *hist[numSpectra];
    // Loop over the selected spectra
    for (Int_t i = 0; i < numSpectra; ++i)
    {
        // Draw the spectrum with a different color
        t->SetLineColor(colors[i]);
        hist[i] = new TH1F (("hist" + to_string(i)).c_str(), branchNames[i][0].c_str(), 8064, 128, 8192);
        if (i == 0)
        {
            t->Draw((branchNames[i][0] + ">>hist" + to_string(i)).c_str());
            hist[i] -> SetLineColor(colors[i]);
	    }
         else {
            t->Draw((branchNames[i][0] + ">>hist" + to_string(i)).c_str(), "", "same");
            hist[i] -> SetLineColor(colors[i]);
		}
        hist[i]->SetName(branchNames[i][0].c_str());
        currentHistStringArr_Overlay[i] = hist[i]->GetName();
        // Add the spectrum to the legend
	legend->AddEntry(hist[i], branchNames[i][0].c_str(), "l");
    }
    

    // Draw the legend
    legend->Draw();
    myC->Modified();
    myC->Update();
    myC->Draw();
}

void DrawButtons() {
    double btnWidth = 0.08, btnHeight = 0.03;
    double btnXStart = 0.1, btnYStart = 0.02;
    btn1 = new TButton("Zone 1", "ShowSection(gCurrentHistogram, 128, 1120)",  btnXStart, btnYStart, btnXStart + btnWidth, btnYStart + btnHeight);
    btn2 = new TButton("Zone 2", "ShowSection(gCurrentHistogram, 1120, 2112)", btnXStart + 0.1, btnYStart, btnXStart + 0.1 + btnWidth, btnYStart + btnHeight);
    btn3 = new TButton("Zone 3", "ShowSection(gCurrentHistogram, 2112, 3104)", btnXStart + 0.2, btnYStart, btnXStart + 0.2 + btnWidth, btnYStart + btnHeight);
    btn4 = new TButton("Zone 4", "ShowSection(gCurrentHistogram, 3104, 4096)", btnXStart + 0.3, btnYStart, btnXStart + 0.3 + btnWidth, btnYStart + btnHeight);
    btn5 = new TButton("UnZoom", "UnZoom(gCurrentHistogram)",                btnXStart + 0.4, btnYStart, btnXStart + 0.4 + btnWidth, btnYStart + btnHeight);
    btn6 = new TButton("Expand", "Expand(gCurrentHistogram)",                btnXStart + 0.5, btnYStart, btnXStart + 0.5 + btnWidth, btnYStart + btnHeight);
    btn7 = new TButton("Compress", "Compress(gCurrentHistogram)",             btnXStart + 0.6, btnYStart, btnXStart + 0.6 + btnWidth, btnYStart + btnHeight);
    //btn8 = new TButton("auto-fit", "fit()", btnXStart + 0.7, btnYStart, btnXStart + 0.7 + btnWidth, btnYStart + btnHeight);
    btn1->Draw();
    btn2->Draw();
    btn3->Draw();
    btn4->Draw();
    btn5->Draw();
    btn6->Draw();
    btn7->Draw();
    //btn8->Draw();
}
Int_t copy() {   
    // Create a new histogram
    TH1F *newH[numSpectra];

    TH1F *histog[numSpectra];
    if(myCanv2){myCanv2->Close();
    }

    myC->cd();
    if (!currentHistString.empty()) {
        histog[0] = dynamic_cast<TH1F*>(gPad->GetPrimitive(currentHistString.c_str()));
        if (!histog[0]) {
            cout << "Histogram " << currentHistString << " not found" << endl;
        }

    } else if (!currentHistStringArr_Overlay[0].empty()) {
        for (Int_t i = 0; i < numSpectra; i++) {
            histog[i] = dynamic_cast<TH1F*>(gPad->GetPrimitive(currentHistStringArr_Overlay[i].c_str()));
            if (!histog[i]) {
                cout << "Histogram " << currentHistStringArr_Overlay[i] << " not found" << endl;
            }
            
        }
    } else if (!currentHistStringArr_Grid[0].empty()) {

        for (Int_t i = 0; i < numSpectra; i++) {

            myC->cd(i+1);
            histog[i] = dynamic_cast<TH1F*>(gPad->GetPrimitive(currentHistStringArr_Grid[i].c_str()));
            if (!histog[i]) {
                cout << "Histogram " << currentHistStringArr_Grid[i] << " not found" << endl;
            }

        }    
    }
    else {cout<<"no hist whatsoever found"<<endl;}


    myCanv2 = new TCanvas("myCanv2", "Copy", 1020, 710);
    myCanv2->cd();

    


    for (Int_t i = 0; i < numSpectra; i++) {
        newH[i] = dynamic_cast<TH1F*>(histog[i]->Clone());
        if (!newH[i]) {
            cout << "Error cloning histogram " << i << endl;
            continue; // Skip to the next iteration if cloning fails
        }
        // Remove fitting functions from the histogram
        newH[i]->GetListOfFunctions()->Delete();

    }
        Int_t* dim;
    
        if(!currentHistStringArr_Grid[0].empty()){
            Int_t grid_dim[2]={0,0};
            dim=values_setter(grid_dim);
            Int_t rows = dim[0];
            Int_t columns = dim[1];
            cout<<"rows "<<rows<<" columns "<<columns<<endl;
            myCanv2->Divide(rows, columns);
            for(Int_t i = 0; i<numSpectra; i++){
                myCanv2->cd(i+1);
                newH[i]->Draw();
        }}
        else if (!currentHistStringArr_Overlay[0].empty()){
            for(Int_t i = 0; i<numSpectra; i++){
                newH[i]->Draw("same");
            }
        }
        else if (!currentHistString.empty()){
            newH[0]->Draw();
        }
    DrawButtons();
    if (!currentHistString.empty() || !currentHistStringArr_Overlay->empty()){
    executeMyExec = false;
    cout << "**** after you have finalized your markers ****" << endl;
    cout << "**** please close the canvas window and type ----> calmark() ****" << endl;
    gPad->AddExec("myexec", "myexec()");
    executeMyExec = true;}
    return 0; 
}


void view(char ask_copy = 'a'){
        // erasing a few things for recursive behaviour
        currentHistString.erase();
        currentHistStringArr_Grid->erase();
        currentHistStringArr_Overlay->erase();
        myC->Clear();
        if(myCanv2){
            cout<<"myCanv2 exists, will be closed: success"<<endl;
        myCanv2->Close();}

		cout<<"Do you wish to view OR overlay spectra? (v/o)"<<endl;
		char xx;
		cin>>xx;
		string searchNumber; 
		string searchCrystal;
		if(xx=='v' || xx == 'V')
		{	
			// view spectrum code
			cout<<"Do you want to see a single spectrum or different spectra in a grid? (g/s)"<<endl;
			char yy;
			cin>>yy;
			
			if(yy == 'g' || yy == 'G')
			{
                cout<<"Type ""0 0"" anytime to exit the loop"<<endl;
				// call the function to make a grid and show the spectra
				cout<<"Split the window into a grid of? (e.g. for 6, enter 2 3)"<<endl;
                
				Int_t rows, columns;
				cin>> rows >> columns;
				numSpectra=rows*columns;
				if(rows==0 && columns == 0){return; }
				vector<vector<string>> branches;

                    Int_t index = 0;
					while(index<(numSpectra))
					{   

                        cout<<"Enter the Clover number (e.g. 22 04 would mean CL_22_E04)"<<endl;
                        cin>>searchNumber>>searchCrystal;
                        if (searchNumber=="0" && searchCrystal == "0") break;
						vector<string> branchNames_for_grid = ListBranches(searchNumber.c_str(), searchCrystal.c_str());
                        while (branchNames_for_grid.empty()){
                            cout<<"please enter here again:"<<endl;
                            cin>>searchNumber>>searchCrystal;
                            branchNames_for_grid = ListBranches(searchNumber.c_str(), searchCrystal.c_str());
                        }
						branches.push_back(branchNames_for_grid);
                        
                        index++;
                        
					}
					if(index==numSpectra){cout<<"The signals you chose for the grid are: "<<endl;
					for (Int_t i = 0; i<(numSpectra); ++i)
					{
						cout<<branches[i][0]<<endl;
					}
					ShowSpectraGrid(branches, rows, columns);}
			}		
			else if (yy == 's' || yy == 'S')
			{
				//call the function to show a single spectrum
                numSpectra = 1;
                cout<<"Type ""0 0"" anytime to exit the loop"<<endl;
                cout<<"Enter the Clover number (e.g. 22 04 would mean CL_22_E04)"<<endl;
                cin>>searchNumber>>searchCrystal;
                if (searchNumber=="0" && searchCrystal == "0") {return;}
				vector<string> selectedBranch = ListBranches (searchNumber.c_str(), searchCrystal.c_str());
                while (selectedBranch.empty()){
                    cout<<"please enter here again:"<<endl;
                    cin>>searchNumber>>searchCrystal;
                    selectedBranch = ListBranches (searchNumber.c_str(), searchCrystal.c_str());
                    }
				gCurrentHistogram = ShowSpectra(selectedBranch[0]);

                currentHistString = gCurrentHistogram->GetName();
				
                DrawButtons();
                newFp = fopen(newFileName, "r");
                if (!newFp)
                {
                    printf("FAILED TO READ THE NEW CALIB FILE '%s'\n\n", newFileName);
		        }
                char line1[1024];
                Int_t lineNumber1 = 0;
                bool found = false;
                while (fgets(line1, sizeof(line1), newFp)){
                    lineNumber1++;
                    const char* searchString = currentHistString.c_str();
                    char *pos = strstr(line1, searchString);
                    if(pos != nullptr){
                        found = true;
                        gLineNumber = lineNumber1;
                    }}
                
                for (Int_t i = 0; i<6; i++)
                {
                    cout<<"par #"<<i<<" "<<A[gLineNumber-1][i]<<"\n";
                }
                cout<<endl;
                cout << "Do you wish to fit the peaks? Type   ---> fit();" << endl;
                executeMyExec2 = false;
                gPad->AddExec("myexec2", "myexec2()");
                executeMyExec2 = true;
			}
			else 
			{
				cout<<"Please choose a valid response"<<endl;
			}
			
		}
		
		else if (xx=='o' || xx =='O')
		{   
            cout<<"Type ""0 0"" anytime to exit the loop"<<endl;
			// overlay spectrum code
			cout << "Enter the number of spectra to overlay: ";
			
			cin >> numSpectra;

			// Get the branches for overlay
			vector<vector<string>> overlayBranches;
            Int_t index =0;
			while (index < numSpectra)
			{   
                cout<<"Enter the Clover number (e.g. 22 04 would mean CL_22_E04)"<<endl;
                cin>>searchNumber>>searchCrystal;
                if (searchNumber=="0" && searchCrystal == "0") break;
				vector<string> branchNames_for_overlay = ListBranches (searchNumber.c_str(), searchCrystal.c_str());
				while (branchNames_for_overlay.empty()){
                            cout<<"please enter here again:"<<endl;
                            cin>>searchNumber>>searchCrystal;
                            branchNames_for_overlay = ListBranches(searchNumber.c_str(), searchCrystal.c_str());
                        }
                overlayBranches.push_back(branchNames_for_overlay);
                index++;
			}
            
			// Call the function to show overlay spectra
			ShowSpectraOverlay(overlayBranches, numSpectra);
			executeMyExec2 = false;
            gPad->AddExec("myexec2", "myexec2()");
            executeMyExec2 = true;
		}
		
		else
		{
			cout<<"Please choose a valid response"<<endl;
			// some unknown functionality (to be added later)
		}
		cout<<"If you wish to view other spectra, type -->  view()"<<endl;
        cout<<"To create a copy of the current canvas, type --->  copy()"<<endl;
        if (ask_copy == 'c'){copy();}
	
}


void cal1()
{      
    if(myCanv2) myCanv2->Close();
    myC->cd();

	Double_t energies_2pt_Eu152[2];
	energies_2pt_Eu152[0] = 121.78;
	energies_2pt_Eu152[1] = 1408.01;
	cout<<"the pre-set energies of Eu152 are the following: (for other energies, change in script) \n";
	for (Int_t i=0; i<2; i++)
	{
		cout<<"energy #"<<i+1<<"\t"<<energies_2pt_Eu152[i]<<"\n"<<endl;
	}
	
	// doing the 2- point calib for reference
	cout<<"Do you wish to do the 2-point calibration for refernce with these values? (y/n)"<<endl;
	char q3;
	cin>>q3;
	if (q3 == 'n' || q3 == 'N')
	{	
		cout<<"Please enter the 2 gamma ray energies you'd like to calibrate with: "<<endl;
		cin>>energies_2pt_Eu152[0]>>energies_2pt_Eu152[1];

		cout<<"The energies chosen by you are: \n";
		for (Int_t i=0; i<2; i++)
	{
		cout<<"energy #"<<i+1<<"\t"<<energies_2pt_Eu152[i]<<"\n"<<endl;
	}
	}
		Double_t selected_2_point_peaks[2];
		Double_t select_peak_for_2_point[2];
        cout<<"calibrate from fitted peaks(a) or enter Channel # mannually(m)? (a/m)"<<endl;
        char q1;
        cin>>q1;
        if (q1 == 'a'|| q1 == 'A'){
            cout<<"Enter the peak numbers for 2-point calibration: (e.g. 2 16)"<<endl;
            for (Int_t jj =0; jj<2; ++jj)
            {
                cin>>select_peak_for_2_point[jj];
            }
            for (Int_t i=0; i<2; ++i)
            {
                Int_t jjj = select_peak_for_2_point[i]; //converted array to an integer
                selected_2_point_peaks[i] = X[jjj-1];
                cout<<"THE SELECTED ENERGY FOR PEAK #"<<i+1<<" IS: "<<selected_2_point_peaks[i]<<"\n";
            }
        }
        else if (q1 == 'm'|| q1 == 'M'){
            cout<<"Enter the two channel numbers: "<<endl;
            cin>>selected_2_point_peaks[0]>>selected_2_point_peaks[1];
            sort(selected_2_point_peaks, selected_2_point_peaks+2);
            
        }


	cout<<"do you wish to continue with these values? (y/n)"<<endl;
	char q4;
	cin>>q4;
	if(q4 == 'y'||q4 == 'Y')
	{
		//code for performing calibration


    const Int_t cPoints = 2;
	TGraph *graph_1 = new TGraph(cPoints, selected_2_point_peaks, energies_2pt_Eu152);
	TF1 *model_1 = new TF1("model_2pt", Model::Function2Pt, 0, 2, 2);

	model_1->SetParameter(0, 1);
    model_1->SetParameter(1, 1);

    // Fit the model to data
    graph_1->Fit("model_2pt");

    // Retrieve fitted parameters
    Double_t c1 = model_1->GetParameter(0);
    Double_t m1 = model_1->GetParameter(1);
    

    // Output fitted parameters
    cout << "Fitted parameters (y = m*x + c):\n";
    cout << "c = " << c1 << "\n";
    cout << "m = " << m1 << "\n";

        A[gLineNumber-1][0] = c1;
        A[gLineNumber-1][1] = m1;
        A[gLineNumber-1][2] = 0;
        A[gLineNumber-1][3] = 0;
        A[gLineNumber-1][4] = 0;
        A[gLineNumber-1][5] = 0;

	// Writing on the canvas, the energies from 2-pt calibration
	Int_t numX = IdentifyPeaks(gCurrentHistogram, X);
	for (Int_t j = 0; j < numX; j++){
        Double_t cord_value = gCurrentHistogram -> GetBinContent(X[j]);
        Double_t y_cord_marker = cord_value + 170;
        Double_t x_cord_marker = X[j] + 5;
        Double_t Y_value = m1*X[j] + c1;
        TText *markerPrint = new TText(x_cord_marker, y_cord_marker, Form("%9.1lf", Y_value));
        markerPrint -> SetTextAngle(90);
        markerPrint -> SetTextColor (kBlue);
        markerPrint -> SetTextSize(0.02);
        markerPrint -> Draw("SAME");
	}

	
	cout<<"To do the calibration, type  ---->  calAuto() OR calMark() OR calMan()"<<endl;

	}
	else if (q4 == 'n'||q4 == 'N')
	{
		cal1();
	}
	}

void cal_modelfit()
{
    const Int_t nPoints = 6;
    cout<<"Do you wish to calibrate with respect to the Eu152 energies?\n The energies are: "<<endl;
    for (Int_t i = 0; i<6; i++)
    {
        cout<<"Energy Peak #"<<i+1<<"  "<<energiesEu152[i]<<endl;
    }
    TGraph *graph;
    Double_t energies_for_calibration[6];
    cout<<"(y/n)? : "<<endl;
    char q5;
    cin>>q5;
    if(q5=='y'||q5=='Y')
    {graph = new TGraph(nPoints, selected_Peaks, energiesEu152);}
    
    else if (q5 == 'n'|| q5 == 'N')
    {
        cout<<"Enter the energies to be assigned separated by spaces"<<endl;
        for(Int_t i = 0; i<6; i++){cin>>energies_for_calibration[i];}
        graph = new TGraph(nPoints, selected_Peaks, energies_for_calibration);
    }
    
	TF1 *model = new TF1("model", Model::Function, 0, 6, 4);
    for (Int_t i = 0; i<4; i++)
	{model->SetParameter(i, 1);}

    // Fit the model to data
    graph->Fit("model");

    // Retrieve fitted parameters
    Double_t a = model->GetParameter(0);
    Double_t b = model->GetParameter(1);
    Double_t c = model->GetParameter(2);
    Double_t d = model->GetParameter(3);
	Double_t e =0, f = 0;

    // Output fitted parameters
    cout << "Fitted parameters (y = a + b*x + c*x^2 + d*sqrt(x)):\n";
    cout << "a = " << a << "\n";
    cout << "b = " << b << "\n";
    cout << "c = " << c << "\n";
    cout << "d = " << d << "\n";
	cout<<endl;
    cout<<"Do you wish to save these as parameters for 4-point calibration to the calib file? (y/n)";
	char q2;
	cin>>q2;

	


	if (q2 == 'y' || q2 == 'Y')
	
	{
		newFp = fopen(newFileName, "r");
		if (!newFp)
		{
			printf("FAILED TO CREATE NEW FILE '%s'\n\n", newFileName);
			fclose(fp);
		
		}
		FILE *tempFile = fopen("temp13131.txt", "w");
    	if (!tempFile) {
        cout << "FAILED TO CREATE TEMPORARY FILE" << endl;
        fclose(newFp);
        
  	 	}
		char line[1024];
    	Int_t lineNumber = 0;
    	bool found = false;
		
		while (fgets(line, sizeof(line), newFp)) 
		{
        	lineNumber++;
			const char* searchString = currentHistString.c_str();
			char *pos = strstr(line, searchString);
        	if (pos != nullptr) {
         	   // String found, replace it and write to temporary file
        	    found = true;
        	    fprintf(tempFile, "%-16s %+10.4lf %8.4lf%+9.3le%+9.3le%+9.3le%+9.3le", searchString, a, b, c, 0.0, 0.0, d);
        	    fputs("\n", tempFile);
      	      cout << "Found and replaced at line " << lineNumber << endl;
       		} else {
            	// String not found, write the line as is to the temporary file
            	fputs(line, tempFile);
       				}
    	}

		fclose(newFp);
		fclose(tempFile);

		remove (newFileName);
        // set the parameters in the global array A[gLineNumber-1][i] also

        

		rename("temp13131.txt", newFileName);
		if (!found)
		{
			cout<<"String not found in the file"<<endl;
		}
	}
    cout<<"do you wish to make these constants permanent for the current session? (y/n): ";
    char qn;
    cin>>qn;
    if (qn == 'y' || qn == 'Y')
    {
        A[gLineNumber-1][0] = a;
        A[gLineNumber-1][1] = b;
        A[gLineNumber-1][2] = c;
        A[gLineNumber-1][3] = 0;
        A[gLineNumber-1][4] = 0;
        A[gLineNumber-1][5] = d;
    }
    
}

void E()
{	

	TH1F* prevHist = dynamic_cast<TH1F*>(gPad -> GetPrimitive("newH"));
    if (prevHist) delete prevHist;
	markers.clear();
	copy();
}

Int_t calMark()      // to run calibration with markers drawn from copy() method
{
    Int_t cal2_sizeDeMarkers = markers.size();

    if (cal2_sizeDeMarkers>6){
        cout<<"more than 6 peaks selected for calibration, please change in script OR run <copy()> again"<<endl;
        markers.clear();

        return 1;}
    /* converting the vector created for markers to array */
    for (auto it = markers.begin(); it != markers.end(); ++it) 
    {
        TPolyMarker* marker = it->first;
        Double_t value = it->second;
        selected_Peaks[distance(markers.begin(), it)] = value;
    }
    
  
    //sorting the array w.r.t bins */
    sort(selected_Peaks, selected_Peaks+cal2_sizeDeMarkers); 

    for (Int_t i=0; i<cal2_sizeDeMarkers; ++i)
    {
    cout<<"the marker #"<<i<<" is at: "<<selected_Peaks[i]<<endl;
    }
    cout<<"do you wish to continue with these peaks or repeat? (y/n)";
	char c3;
	cin>>c3;
	if(c3 == 'y'||c3 == 'Y')
	{
		//code for performing calibration
        cal_modelfit();

	}
	else if (c3 == 'n'||c3 == 'N')
	{
	    copy();
	}
    for (Int_t i= 0; i<(sizeof(selected_Peaks)/sizeof(selected_Peaks[0])); i++)
    {selected_Peaks[i]=0;}          // reset the values of selected_peaks to 0 to avoid further confusion

    cout<<"calMan module ran successfully"<<endl;
	markers.clear();
    return 0;
}



Int_t calAuto()  //to run calibration with pre-fitted peaks
{
	cout<<"the pre-set energies of Eu152 are the following: (for other energies, change in script) \n";
	for (Int_t i=0; i<6; i++)
	{
		cout<<"energy #"<<i+1<<"\t"<<energiesEu152[i]<<"\n"<<endl;
	}

	//check if X has values

	for (Int_t l = 0; l<size_of_X; l++)
	{
	cout<<X[l]<<"\n";
	}
	cout<<endl;

	Double_t select_Peak_number[6];
	cout<<"Select the peaks for calib by peak number: (e.g. 1 4 6 7...)"<<endl;
	for (Int_t j =0; j<6; ++j)
	{
	cin>>select_Peak_number[j];
	}
	
	for (Int_t i=0; i<6; ++i)
	{
	Int_t jj = select_Peak_number[i]; //converted array to an integer
	selected_Peaks[i] = X[jj-1];
	cout<<"THE SELECTED ENERGY FOR PEAK #"<<i+1<<" IS: "<<selected_Peaks[i]<<"\n";
	}
	cout<<"do you wish to continue with these peaks or repeat? (y/n)";
	char c3;
	cin>>c3;
	if(c3 == 'y'||c3 == 'Y')
	{
		//code for performing calibration
        cal_modelfit();

	}
	else if (c3 == 'n'||c3 == 'N')
	{
	    calAuto();
	}
    for (Int_t i= 0; i<(sizeof(selected_Peaks)/sizeof(selected_Peaks[0])); i++)
    {selected_Peaks[i]=0;}          // reset the values of selected_peaks to 0 to avoid further confusion

cout<<"calAuto module ran successfully"<<endl;

return 0;
}

Int_t calMan(){

	cout<<"please type the channel numbers separated by \n ...spaces to be taken for 6 energies' calibration"<<endl;
	for(Int_t i= 0; i<6; i++){
	cin>>selected_Peaks[i];}

	//sorting the array w.r.t bins */
    sort(selected_Peaks, selected_Peaks+6); 

    for (Int_t i=0; i<6; ++i)
    {
    cout<<"the marker #"<<i<<" is at: "<<selected_Peaks[i]<<endl;
    }
    cout<<"do you wish to continue with these peaks or repeat? (y/n)";
	char c3;
	cin>>c3;
	if(c3 == 'y'||c3 == 'Y')
	{
		//code for performing calibration
        cal_modelfit();

	}
	else if (c3 == 'n'||c3 == 'N')
	{
	    calMan();
	}
    for (Int_t i= 0; i<(sizeof(selected_Peaks)/sizeof(selected_Peaks[0])); i++)
    {selected_Peaks[i]=0;}          // reset the values of selected_peaks to 0 to avoid further confusion

    cout<<"calMan module ran successfully"<<endl;
	
    return 0;
	
}

Int_t F()
{   
    
	TH1F* newH = dynamic_cast<TH1F*>(gPad -> GetPrimitive(currentHistString.c_str()));
    if (!newH)
    {
        cout<<"Histogram H not found"<<endl;
        return 1;
    }
    
    peak2_1_area=peak2_2_area=peak2_3_area=0.;
    Int_t sizeDeMarkers = markers.size();
    Double_t markersOf2ndcanvas [sizeDeMarkers];
    /* converting the vector created for markers to array */
    for (auto it = markers.begin(); it != markers.end(); ++it) 
    {
        TPolyMarker* marker = it->first;
        Double_t value = it->second;
        markersOf2ndcanvas[distance(markers.begin(), it)] = value;
    }
  
    //sorting the array w.r.t bins */
    sort(markersOf2ndcanvas, markersOf2ndcanvas+sizeDeMarkers); 
    for (Int_t i=0; i<sizeDeMarkers; ++i)
    {
    cout<<"the marker #"<<i<<" is at: "<<markersOf2ndcanvas[i]<<endl;
    }
    Double_t par[20];
    for (Int_t i = 0; i<20; i++)          //ensuring parameters are set to 0 when re-fitted
    {
        par[i]=0;
    }
    if(sizeDeMarkers == 4){
    Double_t rChi = fit2PeaksChosen (newH, par, markersOf2ndcanvas, sizeDeMarkers);
    cout<<"the rChi is: "<<rChi<<endl<<endl;}
    else if(sizeDeMarkers==3){
    Double_t rChi = fit1PeakChosen (newH, par, markersOf2ndcanvas, sizeDeMarkers);
    cout<<"the rChi is: "<<rChi<<endl<<endl;
    }
    else if(sizeDeMarkers==5){Double_t rChi = fit3PeaksChosen (newH, par, markersOf2ndcanvas, sizeDeMarkers);
    cout<<"the rChi is: "<<rChi<<endl<<endl;
    }
    cout<<"To fit another peak, call the function E() again"<<endl;
    return 0;
}
/* void ButtonDeleter(){btn1->Delete();btn2->Delete();btn3->Delete();btn4->Delete();
btn5->Delete();btn6->Delete();btn7->Delete();btn8->Delete();
btn1=nullptr;btn2=nullptr;btn3=nullptr;btn4=nullptr;
btn5=nullptr;btn6=nullptr;btn7=nullptr;btn8=nullptr;} */
void fit()
{   
    if(myCanv2){myCanv2->Close();}
    string padName (gPad->GetName());
    if (padName== "myC"){
    if (gCurrentHistogram != NULL)
    {	
        Int_t numX;
        numX = IdentifyPeaks(gCurrentHistogram, X);
        
        cout<<"OPTION 1: if you wish to do 2-pt calibration, type  -----> cal1()"<<endl;
        cout<<"OPTION 2: if you wish to calibrate with the marked peaks, type  ----> calAuto()"<<endl;
        cout<<"OPTION 3: if you wish to mark peaks yourself and calibrate, type  ----> copy()"<<endl;
        cout<<"...followed by  ----> calMark()"<<endl;
        DrawButtons();
    }
    else {cout<<"No histogram available."<<endl;}
    }
    else {cout<<"can only work in Original canvas, please select that one"<<endl; return;}
    
}


Double_t Fit1(Double_t *x, Double_t *par)
{
        Double_t arg1     = TMath::Log(0.01*x[0]);          // in keV
        Double_t arg2     = TMath::Log(0.001*x[0]);          // in keV

            Double_t fit_val = TMath::Exp(TMath::Power((TMath::Power((par[0]+par[1]*arg1+par[2]*arg1*arg1),(-1.0*par[3]))+TMath::Power((par[4]+par[5]*arg2+par[6]*arg2*arg2),(-1.0*par[3]))),(-1.0*1/par[3])))/1000.;
        return fit_val;
}
struct intensities_152Eu{
    Double_t Intensity[6]={28.58, 7.583, 26.5, 12.942, 14.605, 21.005};
}effic;
void eff()
{
    Double_t area_eff[6], energy_eff[6], eff_arr[6], norm_eff_arr[6];
    cout<<"the energies for efficiency plot are: "<<endl;
    for (Int_t i = 0; i<6; i++){cout<<"#"<<i+1<<"\t"<<energiesEu152[i]<<endl;}
    cout<<"please save the .dat file in the script directory"<<endl;
    fstream file;
    file.open("eff_file.dat", ios::in);
    Int_t j = 0;
    while (1)
    {
        file>>energy_eff[j]>>area_eff[j];
        j++;
        if(file.eof())
        break;
    }
    file.close();
    Double_t max_rel_eff{};
    for (Int_t jj = 0; jj<6; jj++){
        eff_arr [jj] = (area_eff[jj])/effic.Intensity[jj];
        if (eff_arr[jj]>max_rel_eff) max_rel_eff=eff_arr[jj];
            
    }
    for (Int_t j = 0; j<6; j++)
    {
        norm_eff_arr[j] = eff_arr[j]/max_rel_eff;
    }
    
    TGraph *gr_eff = new TGraph(6, energiesEu152, norm_eff_arr);
    gr_eff-> Draw("A*");

    TF1 *eff_curve = new TF1("eff_curve",Fit1,100.,1500.,7.);
    eff_curve->SetParameter(0,5.);
    eff_curve->SetParameter(1,3.);
    eff_curve->SetParameter(2,-1.);
    eff_curve->SetParameter(3,7.);
    eff_curve->SetParameter(4,6.);
    eff_curve->SetParameter(5,-0.5);
    eff_curve->SetParameter(6,1.);
    gr_eff->Fit("eff_curve","IER+");
    gr_eff->SetMarkerStyle(4);
    gr_eff->SetTitle("Efficiency Vs Energy(keV);Energy;Relative Efficiency");
    gStyle->SetOptFit(0111);  //showing the parameters on graph
    gr_eff->Draw("AP");
    Double_t eff_at_unknown_enrg; Double_t energy = 1112.0;
    eff_at_unknown_enrg= eff_curve->Eval(energy);
    cout<<"your eff. is: "<<eff_at_unknown_enrg<<endl;
}


Int_t Prabhat_Script_mod27()
{

    string fileNames;

    cout<<"Add Files from the Present Working Directory: ";
    cin>>fileNames;

    // use globFiles to get a vector of matching files

    vector<string> matchingFiles=globFiles(fileNames);

    for (const auto& file: matchingFiles){
    t -> Add (file.c_str());
    }

    t -> ls();
    //t -> Print();

            /* char zz;
            cout<<"Do you want to list all the branches individually?"<<endl;
            cin>>zz;

                if (zz == 'y' || zz == 'Y')
                {
                    TObjArray *branches = t -> GetListOfBranches();
                    for (Int_t i = 0; i<branches-> GetEntries(); ++i)
                    {   TBranch *branch = dynamic_cast<TBranch*>(branches->At(i)); 
                        if (branch){
                            cout<<"Branch "<<i+1<<": "<<branch->GetName()<<endl;
                        }
                    }
                    cout<<endl;

                } */


        // original calib file
        Int_t ch;
        printf("Enter the name of the Calib File:  ");
        scanf("%s", calibFileName);

        fp = fopen(calibFileName, "r");
        if (!fp) {
            printf("Failed to open calib file '%s'\n", calibFileName);
            return 1; // Exiting with error code
        }
        // new file
        printf("Enter the name of the new calib-file to be created for storing 4-point parameters (e.g. name.dat):  ");
        scanf("%s", newFileName);
        newFp = fopen(newFileName, "w");
        if (!newFp)
        {
            printf("Failed to create new file '%s'\n\n", newFileName);
            fclose(fp);
            return 1;
        }

        // copy the content of the old file to the new file

        while ((ch = fgetc(fp)) != EOF) {
            fputc(ch, newFp);
        }
    fclose(fp);
    fclose(newFp);
        
        printf("New calibration file created to store the new calib parameters\n");
    newFp = fopen(newFileName, "r");
        numCalib = 0;
    Int_t ret;
        while (numCalib < MAX_CALIB_LINES) {
            ret = fscanf(newFp, "%s %lf %lf %lf %lf %lf %lf\n",
                        calibSpec[numCalib], &A[numCalib][0], &A[numCalib][1],
                        &A[numCalib][2], &A[numCalib][3], &A[numCalib][4], &A[numCalib][5]);

            if (ret == EOF) { // Reached end of file
                break;}
            if (ret == 7) {
                // intentionally blank
            } else {
                A[numCalib][5] = 0;
            }
        
            numCalib++;
        }
        printf("Number of lines present in the calib file: %d\n", numCalib); // Print the number of lines read

        // Ask the user if they want to view the parameters stored in the calib file
        char q1;
        //printf("\nDo you wish to view the parameters stored in the calib file? (y/n)\t");
        //scanf(" %c", &q1); // Added space before %c to consume any previous newline character
        q1='y';
        if (q1 == 'y' || q1 == 'Y') {
            // Print the stored parameters
            for (Int_t i = 0; i < numCalib; i++) {
                printf("%3d. %-16s %+8.4lf %+8.4lf %+9.3le %+9.3le %+9.3le %+9.3le\n",
                    i + 1, calibSpec[i], A[i][0], A[i][1], A[i][2], A[i][3], A[i][4], A[i][5]);
            }
            cout<<endl;
    }
    fclose(newFp);
    ofstream newFp_file; 
        newFp_file.open (newFileName, ios::out);
        for (Int_t i = 0; i < numCalib; i++) {
            newFp_file << left << setw(18) << calibSpec[i]
                    << fixed << setprecision(4)
                    << right << setw(9) << showpos << A[i][0] << setw(12) << right << noshowpos << A[i][1]
                    << scientific << setprecision(3)
                    << setw(9) << showpos << A[i][2] << setw(9) << showpos << A[i][3]
                    << setw(9) << showpos << A[i][4] << setw(9) << showpos << A[i][5] << endl;
        }

        newFp_file.close();

    view();
    return 0;
}
////////////////////////////////////////////////////////////////////////////
// This macro takes a converted ROOT file from the GANIL e569 exp. and    //
// create an histogram for each strip (X and Y) of the MUST2 array filled //
// with the time. The histograms are dumped in an output ROOT file.     //
//                                                                        //
// This is especially usefull for calibration purposes when there is no   //
// need to work directly on the TChain                                     //
////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TBranch.h"
#include "TH1F.h"

#include "/home/mugast/analysis/nptool/NPLib/Detectors/MUST2/TMust2Data.h"
#include "/home/mugast/analysis/nptool/NPLib/Detectors/Mugast/TMugastData.h"
#include "/home/mugast/analysis/nptool/NPLib/include/RootInput.h"

#define NBTELESCOPE 5
#define	NBSTRIPS	128
#define NBSILI     0


void ExtractMust2Histos(int run_number=0, std::string fDet="MUST2")
{

  // RootInput* Input = RootInput::getInstance("RunToTreat.txt");
  // TChain* Chain = Input->GetChain();

  TChain* Chain = new TChain("AD");
  Chain->Add(Form("/data/mugastX/lise23/acquisition/converted_runs/RootA/r0%i_000a.root",run_number));
  const char* fname = Form("r0%i", run_number);
  cout << fname << endl;
  std::cout << Chain->GetFile()->GetName()<< std::endl;
  // exit(1);

  // connect the TfDet.c_str()Data branch  
  Chain->SetBranchStatus(fDet.c_str(),true);
  Chain->SetBranchStatus("fMM*",true);

  TMust2Data *rawMust2;
  TMugastData *rawMugast;
  if (fDet=="MUST2"){
    rawMust2 = new TMust2Data();
    Chain->SetBranchAddress(fDet.c_str(),&rawMust2);
  }
  else if (fDet=="Mugast"){
    rawMugast = new TMugastData();
    Chain->SetBranchAddress(fDet.c_str(),&rawMugast);
  }

  // open the output ROOT file
  TString outFileName = "./Histograms/";
  // const char* fname = Form("r_0%i", run_number);
  outFileName += fname;
  outFileName += "_Raw" + fDet + "Histos.root";
  cout<< outFileName<< endl;
  TFile *outFile = new TFile(outFileName, "recreate");

  // prepare output histograms for Must2
  TH1F* hStripXTime[NBTELESCOPE][NBSTRIPS];
  TH1F* hStripYTime[NBTELESCOPE][NBSTRIPS];
  //from i=4 because I wanted only the T5,T8
  for (Int_t i = 0; i < NBTELESCOPE; i++) {
    for (Int_t j = 0; j < NBSTRIPS; j++) {
      // strips XE
      TString hnameXE     = Form("hMM%d_STRX_T%d", i+1, j+1);
      TString htitleXE    = Form("MM%d_STRX_T%d", i+1, j+1);
      hStripXTime[i][j] = new TH1F(hnameXE, htitleXE, 16384, 0, 16384);
      // strips YE
      TString hnameYE     = Form("hMM%d_STRY_T%d", i+1, j+1);
      TString htitleYE    = Form("MM%d_STRY_T%d", i+1, j+1);
      hStripYTime[i][j] = new TH1F(hnameYE, htitleYE, 16384, 0, 16384);
    }
  }
  TH1F* hSiLiTime[NBTELESCOPE][NBSILI];
  for (Int_t i = 0; i < NBTELESCOPE; i++) {
    for (Int_t j = 0; j < NBSILI; j++) {
      TString hnameSiLiE     = Form("hMM%d_SILI_T%d", i+1, j+1);
      TString htitleSiLiE    = Form("MM%d_SILI_T%d", i+1, j+1);
      hSiLiTime[i][j] = new TH1F(hnameSiLiE, htitleSiLiE, 16384, 0, 16384);
    }
  }

  // read the data
  Int_t nentries = Chain->GetEntries();
  cout << "TChain contains " << nentries << " events" << endl;
  for (Int_t i = 0; i < nentries; i++) {
    if (i%1000 == 0) cout << "Entry " << i << endl;
    ///////////////////////
    // read Must2 branch //
    ///////////////////////
    //brMust2->GetEntry(i);
    Chain->GetEntry(i);
    // get StripXT multiplicity
    Int_t multXE = 0; 
    if (fDet=="MUST2"){
      multXE = rawMust2->GetMMStripXTMult();
    }else if (fDet=="Mugast"){
      multXE = rawMugast->GetDSSDXTMult();
    }
    //cout<< "multXE" << multXE<< endl;
    // loop on StripXT multiplicity and get information concerning the event
    // fill histograms
    //
    Int_t det = 0;   
    Int_t stripX = 0;
    Int_t time = 0;

    for (Int_t j = 0; j < multXE; j++) {
      if (fDet=="MUST2"){
        det    = rawMust2->GetMMStripXTDetectorNbr(j);
        stripX = rawMust2->GetMMStripXTStripNbr(j);
        time = rawMust2->GetMMStripXTTime(j);
      } else if (fDet=="Mugast"){
        det    = rawMugast->GetDSSDXEDetectorNbr(j);
        stripX = rawMugast->GetDSSDXEStripNbr(j);
        time = rawMugast->GetDSSDXTTime(j);
      }
      if ((det < NBTELESCOPE+1) && (stripX < NBSTRIPS+1)) {
        hStripXTime[det-1][stripX-1]->Fill(time);
      } else {
        cout << "Error filling histograms: X_T" << endl;
        if (fDet=="MUST2"){
          // rawMust2->Dump();
        } else if (fDet=="Mugast"){
        // rawMugast->Dump();
      }
    }
  }
  // get StripYT multiplicity
  Int_t multYE=0;
  if (fDet=="MUST2"){
    multYE = rawMust2->GetMMStripYTMult();
  } else if (fDet=="Mugast"){
    multYE = rawMugast->GetDSSDYTMult();
  }
  // loop on StripXT multiplicity and get information concerning the event
  // fill histograms

  det = 0;   
  Int_t stripY = 0;
  time = 0;
  for (Int_t j = 0; j < multYE; j++) {
    if (fDet=="MUST2"){
      det    = rawMust2->GetMMStripYTDetectorNbr(j);
      stripY = rawMust2->GetMMStripYTStripNbr(j);
      time = rawMust2->GetMMStripYTTime(j);
    }else if (fDet=="Mugast"){
      det    = rawMugast->GetDSSDYTDetectorNbr(j);
      stripY = rawMugast->GetDSSDYTStripNbr(j);
      time = rawMugast->GetDSSDYTTime(j);
    }

    if ((det < NBTELESCOPE+1) && (stripY < NBSTRIPS+1)) {
      hStripYTime[det-1][stripY-1]->Fill(time);
    }
    else {
      //cout << "Error filling histograms: Y_T" << endl;
      if (fDet=="MUST2"){
        // rawMust2->Dump();
      }else if (fDet=="Mugast"){
        // rawMugast->Dump();
      }
    }
  }
  // get SiLiE multiplicity

  //Int_t multSiLiE = rawMust2->GetMMSiLiTMult();
  // loop on StripXT multiplicity and get information concerning the event
  // fill histograms
  /* for (Int_t j = 0; j < multSiLiE; j++) {
     Int_t det    = rawMust2->GetMMSiLiEDetectorNbr(j);
     Int_t pad = rawMust2->GetMMSiLiEPadNbr(j);
     Int_t time = rawMust2->GetMMSiLiETime(j);
     if ((det < NBTELESCOPE+1) && (pad < NBSTRIPS+1)) {
       hSiLiTime[det-1][pad-1]->Fill(time);
     }
     else {
     cout << "Error filling histograms: Must2_SiLi_E" << endl;
     rawMust2->Dump();
     }
     }*/

}

// write on disk output file and close it
outFile->Write();
outFile->Close();
}

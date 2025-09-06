
#include <iostream>
#include <fstream>

#include "TCutG.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TF2.h"
#include "TStyle.h"
#include "TCATSData.h"
#include "TZDDData.h"
#include "TZDDPhysics.h"
#include "TCATSPhysics.h"
void drift_charge(){
    TZDDPhysics* ZDD = nullptr;  // Pointer to TAC data
    TCATSPhysics* CATS = nullptr;  // Pointer to TAC data
    TChain *chain = new TChain("PhysicsTree");
    chain->Add("Run508.root");

    TZDDData* tac = nullptr;  // Pointer to TAC data
    std::vector<unsigned long long>* gatconfts = nullptr;  // Pointer to GATCONFTS data
    std::vector<unsigned int >* gatconf = nullptr;
    TVector3 BeamImpact;
    TVector3 BeamDirection;
    // Enable the TAC and GATCONFTS branches
    chain->SetBranchStatus("ZDD", 1);
    chain->SetBranchAddress("ZDD", &ZDD);
    chain->SetBranchStatus("GATCONFTS", 1);
    chain->SetBranchAddress("GATCONFTS", &gatconfts);
    chain->SetBranchStatus("GATCONF", 1);
    chain->SetBranchAddress("GATCONF", &gatconf);
    chain->SetBranchStatus("CATS", 1);
    chain->SetBranchAddress("CATS", &CATS);
    //chain->SetBranchStatus("BeamImpact", 1);
    //chain->SetBranchAddress("BeamImpact", &BeamImpact);

    Float_t CATS1_X;
    Float_t CATS1_Y;
    Float_t CATS2_X;
    Float_t CATS2_Y;

    Float_t DCRaw[4];
    Int_t DCRawM;
    UShort_t DCRawNr[4];
    ULong64_t DCRawTS[4];

    ULong64_t DATATRIG_CATSTS;
    Double_t dx_off[2]={+53.055,-54.09 };
    Double_t dx_gain[2]={-0.33057,0.41992};
    Double_t dy_off[2]={-68.3353,76.538};
    Double_t dy_gain[2]={0.4735,-0.43558};
    Float_t Position[3];
    Position[0] = 6035.5;  //CATS1 z pos
    Position[1] = 6532.5;  //CATS2 z pos
    Position[2] = 8883.5;  //DC z pos

    Float_t Position_ZDD[4];
    // Position of first DC
    Position_ZDD[0] =  Position[2]-125;
    // Position of second DC
    Position_ZDD[1] = Position[2]+125;


    Double_t dx_C12 = 999.;
    Double_t dy_C12 = 999.;

    Double_t dx_C1_DC = 999.;
    Double_t dy_C1_DC = 999.;


    Int_t ix = -1;
    Int_t iy = -1;

    ULong64_t CATS_TS;

    Double_t dt_x = -1.;
    Double_t dt_y = -1.;
    Double_t dl_x = -1.;
    Double_t dl_y = -1.;
    Double_t dl_x1 = -1.;
    Double_t dl_y1 = -1.;
    Double_t dl_x2 = -1.;
    Double_t dl_y2 = -1.;
    Long64_t dt_x1 = -1.;


    Float_t cx[2];
    Float_t cy[2];

    Float_t ax[2];
    Float_t ay[2];




    //  Chain->SetBranchAddress("CATS1_X",&CATS1_X);
    //  Chain->SetBranchAddress("CATS1_Y",&CATS1_Y);
    // Chain->SetBranchAddress("CATS2_X",&CATS2_X);
    // Chain->SetBranchAddress("CATS2_Y",&CATS2_Y);

    // Chain->SetBranchAddress("DCRaw",&DCRaw);
    // Chain->SetBranchAddress("DCRawM",&DCRawM);
    // Chain->SetBranchAddress("DCRawNr",&DCRawNr);
    // Chain->SetBranchAddress("DCRawTS",&DCRawTS);

    // Chain->SetBranchAddress("DATATRIG_CATSTS",&DATATRIG_CATSTS);
    TH2D *dl_dc_0 = new TH2D ("dl_dc_0","TS DIff. vs dl_x",1000, -1000, 1000, 400, -200, 200);
    TH2D *dl_dc_1 = new TH2D ("dl_dc_1","TS DIff. vs dl_y",1000, -1000, 1000, 400, -200, 200);
    TH2D *dl_dc_2 = new TH2D ("dl_dc_2","TS DIff. vs dl_x",1000, -1000, 1000, 400, -200, 200);
    TH2D *dl_dc_3 = new TH2D ("dl_dc_3","TS DIff. vs dl_y",1000, -1000, 1000, 400, -200, 200);
    TH2D *hdcxy00 = new TH2D ("hdcxy00","hdcxy00",1000, -1000, 1000, 4000, -2000, 2000);
    TH2D *hdcxy01 = new TH2D ("hdcxy01","hdcxy01",1000, -1000, 1000, 4000, -2000, 2000);
    TH2D *hdcxy10 = new TH2D ("hdcxy10","hdcxy10",1000, -1000, 1000, 4000, -2000, 2000);
    TH2D *hdcxy11 = new TH2D ("hdcxy11","hdcxy11",1000, -1000, 1000, 4000, -2000, 2000);
    TH2D *hdcxy = new TH2D ("hdcxy","hdcxy",1000, -1000, 1000, 4000, -2000, 2000);
    TH2D *test = new TH2D ("test","Test",1000,-100,100,1000,-100,100);
    TH1D *hDC1 = new TH1D("dc1","dc1",4000,0,16000);
    TH1D *hDC2 = new TH1D("dc2","dc2",4000,0,16000);
    TH1D *hDC3 = new TH1D("dc3","dc3",4000,0,16000);
    TH1D *hDC0 = new TH1D("dc0","dc0",4000,0,16000);
    TH1D *hDCpos = new TH1D("hDCpos","hDCpos",400,-200,200);
    TH1D *hgatconts = new TH1D("hgatconts","hgatconts",300,0,3000);
    TH2F *Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 4000, 6000, 10000, 1000, -499, 500 );
    TH2F *Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 4000, 6000, 10000, 1000, -499, 500 );
    TH1D *hDC1ts = new TH1D("dc1ts","dc1ts",1000,-400,600);
    TH1D *hdcnum = new TH1D("hdcnum","hdcnum",6,0,6);
    TH1D *hdcnumx = new TH1D("hdcnumx","hdcnumx",6,0,6);
    TH1D *hdcnumy = new TH1D("hdcnumy","hdcnumy",6,0,6);

    Int_t nentries = chain->GetEntries();
    std::cout << "TChain contains " << nentries << " events" << std::endl;

    double pourcentage;

    //nentries = 100000;
    Int_t nentries_start = 1500000;

    for (Int_t i = 0; i < nentries; i++) {
        //for (Int_t i = nentries_start; i < nentries; i++) {
        pourcentage = 100.*i/nentries ;
        if (i%100000 == 0) std::cout << "\rEntry " << i << Form("---- Entry processed : %.2f%%",pourcentage) << std::flush;
        chain->GetEntry(i);
        BeamDirection = TVector3(CATS->PositionX[0] - CATS->PositionX[1],CATS->PositionY[0] - CATS->PositionY[1],-(CATS->PositionZ[0] - CATS->PositionZ[1]));



        for (size_t i = 0; i < gatconf->size(); i++) {
            // std::cout << "gatconfts " << gatconfts->size() << ": gatconf = " << gatconf->at(i) << std::endl;
            // std::cout << "i "<<i <<" gatconfts " << gatconfts->at(i) << ": gatconf = " << gatconf->at(i) << std::endl;



            // if(gatconf->at(i)==4){
            CATS_TS = gatconfts->at(i);
            hgatconts->Fill(CATS_TS/1e9);

            //}

            for (Int_t j =0; j<ZDD->DC_E.size();j++){
                if(CATS->PositionOnTargetX > -1000 && CATS->PositionOnTargetY > -1000 ){
                    // if(CATS1_X > -1500 && CATS2_X > -1500 && CATS1_Y > -1500 && CATS2_Y > -1500 && DCRawM == 2 && DATATRIG_CATSTS > 0 && DCRawTS[0] > 0 && DCRawTS[1] > 0){
                    CATS1_X =  CATS->PositionX[0];
                    CATS1_Y =  CATS->PositionY[0];
                    CATS2_X =  CATS->PositionX[1];
                    CATS2_Y = CATS->PositionY[1];


                    ay[0] = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
                    ay[1] = (Position[1] - Position[0])/(-BeamDirection.Y());
                    //  cout<< "cats " << ay[0] << "beamdirection " <<ay[1]<<endl;
                    ax[0] = (Position[1] - Position[0])/(CATS2_X - CATS1_X);
                    for(int k= Position[0]; k<Position[2]; k++){
                        Trajectory_XZ->Fill(k, CATS->PositionX[0] + (k - Position[0])/ax[0]);
                        Trajectory_YZ->Fill(k, CATS->PositionY[0] + (k - Position[0])/ay[0]);
                    }

                    dy_C1_DC = CATS1_Y + (Position_ZDD[0] - Position[0])/ay[0];
                    dx_C1_DC = CATS1_X + (Position_ZDD[0] - Position[0])/ax[0];
                    if(ZDD->DC_E[j] > 0 ){
                        hdcnum->Fill(ZDD->DC_Nbr[j]);
                        if(ZDD->DC_Nbr[j] == 2 ){
                            ix = 0;
                            //cout<<"ix1 " << ZDD->DC_Nbr[j] <<endl;
                            hDC2->Fill(ZDD->DC_E[j]);
                            dt_x = -(CATS_TS-ZDD->DC_TS[j]);
                            hDC1ts->Fill(dt_x);
                            hDCpos->Fill(dx_C1_DC);
                            dl_dc_2->Fill(dt_x,dx_C1_DC);
                            dl_x1 = dt_x*dx_gain[0] + dx_off[0];
                            //cout<< "dt_x1 " << dt_x <<"dl_x1 " << dl_x1 <<endl;
                        }
                        else if(ZDD->DC_Nbr[j] == 3){
                            ix = 1;
                            // cout<<"ix2 " << ZDD->DC_Nbr[j] <<endl;
                            hDC3->Fill(ZDD->DC_E[j]);
                            dt_x = -(CATS_TS-ZDD->DC_TS[j]);
                            dl_dc_3->Fill(dt_x,dx_C1_DC);
                            dl_x2 = dt_x*dx_gain[1] + dx_off[1];
                            //cout<< "dt_x2 " << dt_x <<"dl_x2 " << dl_x2 <<endl;

                        }
                        else if(ZDD->DC_Nbr[j] == 1 ){
                            iy = 0;
                            hDC1->Fill(ZDD->DC_E[j]);
                            dt_y = -(CATS_TS - ZDD->DC_TS[j]);
                            dl_dc_1->Fill(dt_y,dy_C1_DC);
                            dl_y1 = dt_y*dy_gain[0] + dy_off[0];
                            //cout<< "dt_y1 " << dt_y <<"dl_y1 " << dl_y1 <<endl;

                        }
                        else if(ZDD->DC_Nbr[j] == 0){
                            iy = 1;
                            hDC0->Fill(ZDD->DC_E[j]);
                            dt_y = -(CATS_TS - ZDD->DC_TS[j]);
                            dl_dc_0->Fill(dt_y,dy_C1_DC);
                            dl_y2 = dt_y*dy_gain[1] + dy_off[1];

                            //cout<< "dt_y2 " << dt_y <<"dl_y2 " << dl_y2 <<endl;


                        }

                        hdcnumx->Fill(ix);
                        hdcnumy->Fill(iy);


                    }
                    if(ix==0 && iy==0) hdcxy00->Fill(dt_x,dt_y);
                    else if(ix==0 && iy==1) hdcxy01->Fill(dt_x,dt_y);
                    else if(ix==1 && iy==0) hdcxy10->Fill(dt_x,dt_y);
                    else if(ix==1 && iy==1) hdcxy11->Fill(dt_x,dt_y);
                    if(ix >= 0 && iy >=0){
                        Double_t ran1 = rand() % 100000001;
                        Double_t ran2 = rand() % 100000001;
                        ran1 = ran1/100000000-0.5;
                        ran2 = ran2/100000000-0.5;
                        dt_x = dt_x + ran1;
                        dt_y = dt_y + ran2;

                        dl_x = dt_x*dx_gain[ix] + dx_off[ix];
                        dl_y = dt_y*dy_gain[iy] + dy_off[iy];
                        hdcxy->Fill(dl_x,dl_y);
                    }
                   // hdcxy->Fill(dl_x1+dl_x2,dl_y1+dl_y2);



                    // dt_y = -(CATS_TS - ZDD->DC_TS[iy]);
                    // dt_x = -(CATS_TS - ZDD->DC_TS[ix]);
                    BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.7,-(CATS->PositionOnTargetY)+2.3,0);
                    if(ZDD->DC_Nbr[j] == 2){
                        test->Fill(BeamImpact.X(),BeamImpact.Y());
                    }
                    if(ZDD->DC_Nbr[j] == 3){
                        //test->Fill(CATS1_X+30,CATS1_Y+30);
                    }
                    if(ZDD->DC_Nbr[j] == 1){
                        //  test->Fill(CATS1_X+30,CATS1_Y-30);
                    }
                    if(ZDD->DC_Nbr[j] == 0){
                        //test->Fill(CATS1_X-30,CATS1_Y-30);
                    }



                }
            }
        }
        // for(int j = 0; j< 4; j++){



    }
    TFile *fout = new TFile("./DC_XY_calc_tt2.root", "recreate");

    test->Write("");

    dl_dc_0->Write("");
    dl_dc_1->Write("");

    dl_dc_2->Write("");
    dl_dc_3->Write("");
    hdcxy00->Write("");
    hdcxy10->Write("");
    hdcxy01->Write("");
    hdcxy11->Write("");
hdcnum->Write("");
hdcnumx->Write("");

hdcnumy->Write("");

    hdcxy->Write("");
    hDC1->Write();
    hDC2->Write();
    hDC3->Write();
    hDC0->Write();
    Trajectory_XZ->Write();
    Trajectory_YZ->Write();
    hDC1ts->Write();
    hDCpos->Write();
    hgatconts->Write();
    fout->Write();

    fout->Close();


}





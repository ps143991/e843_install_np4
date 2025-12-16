

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
#include"NPAnalysisFactory.h"
#include"NPDetectorManager.h"
#include"NPOptionManager.h"
#include"RootOutput.h"
#include"RootInput.h"
#include"TMust2Physics.h"
#include"TMugastPhysics.h"
#include"TCATSPhysics.h"
#include "TTACPhysics.h"
#include "TExogamPhysics.h"
#include "TZDDPhysics.h"
#include "NPReaction.h"
#include "NPEnergyLoss.h"
#include "TZDDPhysics.h"
#include <iostream>
#include <TVector.h>
#include <TVector3.h>
#include "TCutG.h"




using namespace std;
// fit_erf_line.C
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"

TChain* tree = NULL;

TCutG* cutg1 = NULL;
TCutG* cutg2 = NULL;
TCutG* cutgpl3 = NULL;
TCutG* cutgpl3_small = NULL;
TCutG* cutgpl4 = NULL;
TCutG* cut_dE_tof_cpl = NULL;
TCutG* cut_dE_tof_d4cats1 = NULL;

TCanvas* c1 = NULL;
TCanvas* c2 = NULL;
TCanvas* c3 = NULL;
TCanvas* c4 = NULL;
TCanvas* c5 = NULL;
TCanvas* c6 = NULL;
TCanvas* c7 = NULL;
TCanvas* c8 = NULL;
TCanvas* c9 = NULL;
TCanvas* c10 = NULL;
TCanvas* c11 = NULL;
TCanvas* c12 = NULL;
TCanvas* c13 = NULL;
TCanvas* c14 = NULL;
TCanvas* c15 = NULL;
TCanvas* c16 = NULL;


TH1F* h1 = NULL;
TH1F* h2 = NULL;
TH1F* h2_1 = NULL;
TH1F* h2_2 = NULL;

TH1F* h3 = NULL;
TH1F* h4 = NULL;
TH1F* h5 = NULL;
TH2F* h6 = NULL;
TH1F* h7 = NULL;
TH2F* h8 = NULL;
TH2F* h9 = NULL;
TH2F* h10 = NULL;
TH2F* h11 = NULL;
TH2F* h12 = NULL;
TH1F* h13 = NULL;
TH1F* h14 = NULL;
TH1F* h19 = NULL;
TH1F* h20 = NULL;
TH1F* h21 = NULL;
TH1F* h22 = NULL;
TH1F* h23 = NULL;
TH1F* h24 = NULL;
TH1F* h25 = NULL;

TH1F* h_exo = NULL;
TH2F* h_gamma_ex = NULL;
TH2F* hic_sum_V_tac_cpl = NULL;
TH2F* hic_sum_V_tac_d4_cats1 = NULL;
TH1F* hBeamE = NULL;

TH2F* h15 = NULL;

    TMust2Physics* M2;
    TMugastPhysics* MG;
    TCATSPhysics* CATS;
    TZDDPhysics* ZDD;
    //TZDDData* ZDDr;
    TExogamPhysics* Exogam;
    TTACPhysics* TAC;
    //TMugastPhysicsReader* MG_Reader;
    TMust2PhysicsReader* M2_Reader;

void loadCUTS(){
    TFile *cutfile = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGcats.root","READ");
    TFile *cutfile_drift = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGdrift.root","READ");
    TFile *cutPL3 = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGpl3.root","READ");
    TFile *cutPL3_small = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGpl3_small.root","READ");
    TFile *cutPl4 = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/CUTGpl4.root","READ");
    TFile *cut_cpl_dE_tof_file = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/cut_dE_TOF_cats_pl.root","READ");
    TFile *cut_d4cats1_dE_tof_file = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/cut_dE_TOF_d4_cats.root","READ");
    cutg1 = (TCutG*)cutfile->Get("CUTGcats");
    cutg2 = (TCutG*)cutfile_drift->Get("CUTG");
    cutgpl3 = (TCutG*)cutPL3->FindObjectAny("CUTG");
    cutgpl3_small = (TCutG*)cutPL3_small->FindObjectAny("CUTGpl3_small");
    cutgpl4 = (TCutG*)cutPl4->FindObjectAny("CUTGpl4");
    cut_dE_tof_cpl = (TCutG*)cut_cpl_dE_tof_file->FindObjectAny("tac_cats_pl");
    cut_dE_tof_d4cats1 = (TCutG*)cut_d4cats1_dE_tof_file->FindObjectAny("cut_d4_cats");
}


void loadFILES() {
  tree = new TChain("PhysicsTree");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatA.txt.root");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatB.txt.root");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatC.txt.root");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatD.txt.root");
  tree->Add("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/RunToTreatE.txt.root");


}



void dp_analysis_2(){
    loadFILES();
    loadCUTS();
    cout<<"Total Entries: "<<tree->GetEntries()<<endl;


    CATS = new TCATSPhysics();
    MG = new TMugastPhysics();
    M2 = new TMust2Physics();
    ZDD = new TZDDPhysics();
    TAC = new TTACPhysics();
    Exogam = new TExogamPhysics();
    

    TTreeReader reader(tree);
    TTreeReaderArray<unsigned int> GATCONFMASTER_(reader, "GATCONF");
    TTreeReaderArray<unsigned long long> GATCONFMASTERTS_(reader, "GATCONFTS");
    TTreeReaderArray<double> mgexnocor(reader, "MG_Exnocor");
    TTreeReaderArray<double> mgex(reader, "MG_Ex");
    TTreeReaderArray<double> mg_lab(reader, "MG_ELab");
    TTreeReaderArray<double> mg_lab2(reader, "MG_ELab2");
    TTreeReaderValue<TVector3> BeamImpact(reader, "BeamImpact");
    // TTreeReaderArray<double> driftY(reader, "dy_C1_DC");
    TTreeReaderArray<unsigned int> Pl1(reader, "PL_E1");
    TTreeReaderArray<unsigned int> Pl2(reader, "PL_E2");
    TTreeReaderArray<unsigned int> Pl3(reader, "PL_E3");
    TTreeReaderArray<unsigned int> Pl4(reader, "PL_E4");
    TTreeReaderArray<unsigned int> Pl5(reader, "PL_E5");
    TTreeReaderArray<unsigned int> Pl6(reader, "PL_E6");
    TTreeReaderArray<unsigned int> Pl7(reader, "PL_E7");
    TTreeReaderArray<unsigned int> Pl8(reader, "PL_E8");
    TTreeReaderArray<unsigned int> Pl9(reader, "PL_E9");
    TTreeReaderArray<unsigned int> Pl10(reader, "PL_E10");
    TTreeReaderArray<double> tac_mmg_cats(reader, "TAC_MMG_CATS1");
    TTreeReaderArray<double> tac_cpl(reader, "TAC_CATS_PL");
    TTreeReaderArray<double> tac_mmg_exo(reader, "TAC_MMG_EXOGAM");
    TTreeReaderArray<double> tac_cats_exo(reader, "TAC_CATS_EXOGAM");
    TTreeReaderArray<double> tac_d4_cats1(reader, "TAC_D4_CATS1");


    TTreeReaderArray<unsigned int> IC1(reader, "IC_1");
    TTreeReaderArray<unsigned int> IC2(reader, "IC_2");
    TTreeReaderArray<unsigned int> IC3(reader, "IC_3");
    TTreeReaderArray<unsigned int> IC4(reader, "IC_4");
    TTreeReaderArray<unsigned int> IC5(reader, "IC_5");
    TTreeReaderArray<double> exo_doppler(reader, "EXO_Doppler_dp");
    TTreeReaderArray<double> theta(reader,"MG_ThetaLab");

    TTreeReaderArray<double> beam_energy(reader, "OriginalBeamEnergy");

    

    h1 = new TH1F("h1", "GATCONFTS", 1000, 0, 9000);
    h2_1 = new TH1F("h2_1", "MG_Exnocor", 1200, -10, 20);
    h2_2 = new TH1F("h2_2", "MG_Ex", 1200, -10, 20);
    h3 = new TH1F("h3", "MG_ELab", 1200, 0, 40);
    h4 = new TH1F("h4", "MG_ELab2", 1200, 0, 40);
    h5 = new TH1F("h5", "MG_conditioned with 1.0", 1200, -10, 20);
    h6 = new TH2F("h6", "BeamImpactY_vs_X", 200, -20, 20, 200, -20, 20);
    // h7 = new TH1F("h7", "driftY", 200, -10, 10);
    h8 = new TH2F("h8", "sqrt(PL_E1*PL_E6) vs log(PL_E1/PL_E6)", 1000, -1,1, 1500, 0, 15000);
    h9 = new TH2F("h9", "sqrt(PL_E2*PL_E7) vs log(PL_E2/PL_E7)", 1000, -1,1, 1500, 0, 15000);
    h10 = new TH2F("h10", "sqrt(PL_E3*PL_E8) vs log(PL_E3/PL_E8)", 1000, -1,1, 1500, 0, 15000);
    h11 = new TH2F("h11", "sqrt(PL_E4*PL_E9) vs log(PL_E4/PL_E9)", 1000, -1,1, 1500, 0, 15000);
    h12 = new TH2F("h12", "sqrt(PL_E5*PL_E10) vs log(PL_E5/PL_E10)", 1000, -1,1, 1500, 0, 15000);
    h13 = new TH1F("h13", "TAC_MMG_CATS1", 8000, 0, 2000);
    h14 = new TH1F("h14", "IC3_Energy", 5000, 0, 10000);
    h19 = new TH1F("h19", "MG_conditioned_with 1.2", 1200, -10, 20);
    h20 = new TH1F("h20", "MG_conditioned_with IC", 1200, -10, 20);
    h21 = new TH1F("h21", "MG_conditioned_with TAC_CATS_PL", 1200, -10, 20);
    h22 = new TH1F("h22", "MG_conditioned_with TAC_CATS_PL w/ and w/o IC cut", 1200, -10, 20);
    h23 = new TH1F("h23", "MG_conditioned_with Pl3 small", 1200, -10, 20);
    h24 = new TH1F("h24", "MG_conditioned_with Pl4", 1200, -10, 20);
    h25 = new TH1F("h25", "MG_conditioned_with TAC_CATS_PL w/o Beam and IC cut", 1200, -10, 20);

    h_exo = new TH1F("h_exo", "EXO_Doppler", 8000, 0, 4000);
    h_gamma_ex = new TH2F("h_gamma_ex", "EXO_Doppler_vs_MG_Exnocor", 1200, -10, 20, 8000, 0, 4000);
    hic_sum_V_tac_cpl = new TH2F("hic_sum_V_tac_cpl", "IC_Sum_vs_TAC_CATS_PL", 500,340,360, 1000, 5000, 10000);
    hic_sum_V_tac_d4_cats1 = new TH2F("hic_sum_V_tac_d4_cats1", "IC_Sum_vs_TAC_D4_CATS1", 5000,290,340, 1000, 5000, 10000);
    hBeamE = new TH1F("hBeamE", "Beam_Energy", 200, 800, 1200);





    while (reader.Next()) {
        Long64_t t1;
        Double_t t2_1;
        Double_t t3;
        Double_t t5[10];
        Double_t tMC;
        Double_t t7[5];
        Double_t tCPl;
        Double_t gamma_energy;
        Double_t tME;
        Double_t tCE;
        Double_t thet;
        Double_t tdc;
        Double_t beamE;

        if(GATCONFMASTER_[0] == 1){
            // cout<<"GATCONFMASTER_: "<<GATCONFMASTER_[0]<<endl;
            // t1 = GATCONFMASTERTS_[0]/1E10;
            // h1->Fill(t1);
            if(mgexnocor.GetSize()>0){
                t2_1 = mgexnocor[0];
                // h2_1->Fill(t2_1);
            }

            if ( mg_lab.GetSize()>0){
                t3 = mg_lab[0];
                // h3->Fill(t3);
            }




            const TVector3& v = *BeamImpact;
            double beamX = v.X();
            double beamY = v.Y();
            double beamZ = v.Z();

            
            t5[0] = Pl1.GetSize()>0 ? Pl1[0] : 0;
            t5[1] = Pl2.GetSize()>0 ? Pl2[0] : 0;
            t5[2] = Pl3.GetSize()>0 ? Pl3[0] : 0;
            t5[3] = Pl4.GetSize()>0 ? Pl4[0] : 0;
            t5[4] = Pl5.GetSize()>0 ? Pl5[0] : 0;
            t5[5] = Pl6.GetSize()>0 ? Pl6[0] : 0;
            t5[6] = Pl7.GetSize()>0 ? Pl7[0] : 0;
            t5[7] = Pl8.GetSize()>0 ? Pl8[0] : 0;
            t5[8] = Pl9.GetSize()>0 ? Pl9[0] : 0;
            t5[9] = Pl10.GetSize()>0 ? Pl10[0] : 0; 

            // h8->Fill(log(t5[0]/t5[5]), sqrt(t5[0]*t5[5]));
            // h9->Fill(log(t5[1]/t5[6]), sqrt(t5[1]*t5[6]));
            // h10->Fill(log(t5[2]/t5[7]), sqrt(t5[2]*t5[7]));
            // h11->Fill(log(t5[3]/t5[8]), sqrt(t5[3]*t5[8]));
            // h12->Fill(log(t5[4]/t5[9]), sqrt(t5[4]*t5[9]));

            tCPl = tac_cpl.GetSize()>0 ? tac_cpl[0] : 0;
            tMC = tac_mmg_cats.GetSize()>0 ? tac_mmg_cats[0] : 0;
            tCE = tac_cats_exo.GetSize()>0 ? tac_cats_exo[0] : 0;
            tME = tac_mmg_exo.GetSize()>0 ? tac_mmg_exo[0] : 0;
            thet = theta.GetSize()>0 ? theta[0] : 0;
            tdc = tac_d4_cats1.GetSize()>0 ? tac_d4_cats1[0] : 0;
            // h13->Fill(tMC);  

            t7[0] = IC1.GetSize()>0 ? IC1[0] : 0;
            t7[1] = IC2.GetSize()>0 ? IC2[0] : 0;
            t7[2] = IC3.GetSize()>0 ? IC3[0] : 0;
            t7[3] = IC4.GetSize()>0 ? IC4[0] : 0;
            t7[4] = IC5.GetSize()>0 ? IC5[0] : 0;

            gamma_energy = exo_doppler.GetSize()>0 ? exo_doppler[0] : 0;

            beamE = beam_energy.GetSize()>0 ? beam_energy[0] : 0;

            if(t3>1.0){                                 //cut on MG_ELab                             
                // if(beamE>900){                              //cut on Beam Energy
                    if(tCPl>300 && tCPl<400){                     //cut on TAC_CATS_PL     
                        if(abs(tMC-387)<25){                            //cut on TAC_MMG_CATS1
                            // if(abs(tME-432)<30){                            //cut on TAC_MMG_EXOGAM                        
                                // if(abs(tCE - 440)<20){                         //cut on TAC_CATS_EXOGAM
                                    // if(t7[2]>0){                                     //cut on IC3 energy
                                        // if(thet>110 && thet<120){    
                                            if(cutg1->IsInside(beamX, beamY)){                  //cut on Beam spot
                                                // if(cut_dE_tof_cpl->IsInside(tCPl, (t7[0]+t7[1]+t7[2]+t7[3]+t7[4]))){  //cut on TAC_CATS_PL vs IC sum
                                                //     if(cut_dE_tof_d4cats1->IsInside(tdc, (t7[0]+t7[1]+t7[2]+t7[3]+t7[4]))){ //cut on TAC_D4_CATS1 vs IC sum  
                                                        h25->Fill(t2_1);                                
                                                        if(gamma_energy>0){
                                                            h_exo->Fill(gamma_energy*1000);                 //FILLING EXO_Doppler
                                                            h_gamma_ex -> Fill(t2_1, gamma_energy*1000);
                                                        }
                                                    // }
                                                // }
                                            }
                                        // }
                                    // }
                                // }
                            // }
                        }
                    }
                // }
            }





        }
    }

    
}
#include "NPApplication.h"
#include "NPDetectorManager.h"
#include "NPFunction.h"
#include "ZddPhysics.h"
#include "CatsPhysics.h"
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
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TRandom3.h"
#include "TLine.h"
#include "TVector3.h"


using namespace std;
using namespace zdd;
using namespace cats;
using namespace nptool;
// fit_erf_line.C
#include "TF1.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraph.h"
#include "TAxis.h"

TChain* tree = NULL;



TH2F* h_ex_vs_thet = NULL;
TH2F* h_ex_vs_phi = NULL;
TH2F* h_gamma_theta = NULL;
TH2F* h_gamma_gamma = NULL;
TH1F* h_exo = NULL;
TH2F* h_gamma_ex = NULL;
TH2F* hic_sum_V_tac_cpl = NULL;
TH2F* hic_sum_V_tac_d4_cats1 = NULL;
TH1F* hBeamE = NULL;
// TH1F* h_phi_T1 = NULL;
// TH1F* h_phi_T2 = NULL;
// TH1F* h_phi_T3 = NULL;
// TH1F* h_phi_T4 = NULL;
// TH1F* h_phi_T5 = NULL;
// TH1F* h_ex_100_110 = NULL;
// TH1F* h_ex_110_120 = NULL;
// TH1F* h_ex_120_130 = NULL;
// TH1F* h_ex_130_140 = NULL;
// TH1F* h_ex_140_150 = NULL;
// TH1F* h_ex_150_160 = NULL;
TH1F* h1 = NULL;
TH1F* h2_1 = NULL;
TH1F* h3 = NULL;


void loadCUTS(){
    TFile *cut_d4cats1_dE_tof_file = new TFile("/home/sharmap/Workplace/np4/e843/Files_from_ozge/for_Prabhat/rootfiles/cut_dE_TOF_d4_cats.root","READ");
}


void loadFILES() {
  tree = new TChain("PhysicsTree");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp1_new_CATS.root");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp2_new_CATS.root");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp3_new_CATS.root");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp4_new_CATS.root");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp5_new_CATS.root");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp6_new_CATS.root");

//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp2_new_CATS_with_beam_spot_only_negativeX.root");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp3_new_CATS_with_beam_spot_only_negativeX.root");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp4_new_CATS_with_beam_spot_only_negativeX.root");
//   tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp5_new_CATS_with_beam_spot_only_negativeX.root");

    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp2_new_CATS_with_beam_spot_positiveXY.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp3_new_CATS_with_beam_spot_positiveXY.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp4_new_CATS_with_beam_spot_positiveXY.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/RTT_temp5_new_CATS_with_beam_spot_positiveXY.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_1.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_2.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_3.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_mP.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_mm1Pm1.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_m0P0.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_mP.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_mP2.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_mP3.root");

// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_sameSIGN_asSURVEYORGAVE_PP.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_mP_W_.root");

// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_mP_WO_heavyMYLAR.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_mP_WO_heavyMYLAR_m20CATSMYLAR.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_PP_WO_heavyMYLAR_m20CATSMYLAR.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_mP_WO_heavyMYLAR_m20CATSMYLAR_bDir_0m1.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_PP_WO_heavyMYLAR_m20CATSMYLAR_bDir1m0.root");

// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_mP_WO_heavyMYLAR_m20CATSMYLAR_bDir_1m0.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_PP_WO_heavyMYLAR_m20CATSMYLAR_bDir_1m0.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_both_inverted_PP_W_heavyMYLAR_bDir_1m0.root");
// tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_Ozges_CATS_m17m23.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_inverted_PP.root");
    // tree->Add("/home/sharmap/Workplace/nptool/Projects/e843_Prabhat/merged_data/Physics_converted/new_cord_mugast_and_cats_inverted_mP_with_bdirmPP.root");

// tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/545.root");
tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/537_0.root");
tree->Add("/home/sharmap/Workplace/np4/e843/data/analysed/537_1.root");



}


void dp_analysis_np4(){

    std::cout << "Init dummy application to use the detectors" << std::endl;
    auto app = nptool::Application::InitApplication("");
    loadFILES();
    loadCUTS();
    cout<<"Total Entries: "<<tree->GetEntries()<<endl;



/*     TTreeReaderArray<unsigned int> GATCONFMASTER_(reader, "GATCONF");
    TTreeReaderArray<unsigned long long> GATCONFMASTERTS_(reader, "GATCONFTS");
    TTreeReaderArray<double> mgexnocor(reader, "MG_Exnocor");
    TTreeReaderArray<double> mgex(reader, "MG_Ex");
    TTreeReaderArray<double> mg_lab(reader, "MG_ELab");
    TTreeReaderArray<double> mg_lab2(reader, "MG_ELab2");
    TTreeReaderValue<TVector3> BeamImpact(reader, "BeamImpact");

    TTreeReaderArray<double> tac_mmg_cats(reader, "TAC_MMG_CATS1");
    TTreeReaderArray<double> tac_cpl(reader, "TAC_CATS_PL");
    TTreeReaderArray<double> tac_mmg_exo(reader, "TAC_MMG_EXOGAM");
    TTreeReaderArray<double> tac_cats_exo(reader, "TAC_CATS_EXOGAM");
    TTreeReaderArray<double> tac_d4_cats1(reader, "TAC_D4_CATS1");


    TTreeReaderArray<double> theta(reader,"MG_ThetaLab");
    TTreeReaderArray<double> phi(reader,"MG_PhiLab");

    TTreeReaderArray<double> beam_energy(reader, "OriginalBeamEnergy"); */

    TTreeReader reader(tree);
    TTreeReaderValue<ZddPhysics> phy_zdd_r(reader, "zdd");
    TTreeReaderValue<unsigned int> GATCONF_r(reader, "GATCONF");
    TTreeReaderValue<unsigned long long> GATCONFTS_r(reader, "GATCONFTS");

    TTreeReaderValue<CatsPhysics> phy_cats_r(reader, "cats");
    TTreeReaderArray<double> mgex(reader, "MG_Ex");
    TTreeReaderArray<double> mg_lab(reader, "MG_ELab");
    TTreeReaderValue<TVector3> BeamImpact(reader, "BeamImpact");
    TTreeReaderArray<double> tac_cpl(reader, "TAC_CATS_PL");
    // TTreeReaderArray<double> tac_mmg_exo(reader, "TAC_MMG_EXOGAM");
    // TTreeReaderArray<double> tac_cats_exo(reader, "TAC_CATS_EXOGAM");
    // TTreeReaderArray<double> tac_d4_cats1(reader, "TAC_D4_CATS1");
    TTreeReaderArray<double> tac_mmg_cats(reader, "TAC_MMG_CATS1");
    TTreeReaderArray<double> theta(reader,"MG_ThetaLab");
    TTreeReaderArray<double> phi(reader,"MG_PhiLab");
    TTreeReaderArray<double> beam_energy(reader, "OriginalBeamEnergy"); 



    h1 = new TH1F("h25", "MG_conditioned_with TAC_CATS_PL w/o Beam and IC cut", 1200, -10, 20);

    h2_1 = new TH1F("h2_1", "MG_Exnocor", 1200, -10, 20);

    h3 = new TH1F("h3", "MG_ELab", 1200, 0, 40);
    

    h_ex_vs_thet = new TH2F("h_ex_vs_thet", "MG_Ex_vs_ThetaLab", 65, 100, 165, 1500, -3, 12);
    h_ex_vs_phi = new TH2F("h_ex_vs_phi", "MG_Ex_vs_PhiLab", 72, -180, 180, 1500, -3, 12);

    // h_ex_100_110 = new TH1F("h_ex_100_110", "MG_Ex_theta_100_110", 1500, -3, 12);
    // h_ex_110_120 = new TH1F("h_ex_110_120", "MG_Ex_theta_110_120", 1500, -3, 12);
    // h_ex_120_130 = new TH1F("h_ex_120_130", "MG_Ex_theta_120_130", 1500, -3, 12);
    // h_ex_130_140 = new TH1F("h_ex_130_140", "MG_Ex_theta_130_140", 1500, -3, 12);
    // h_ex_140_150 = new TH1F("h_ex_140_150", "MG_Ex_theta_140_150", 1500, -3, 12);
    // h_ex_150_160 = new TH1F("h_ex_150_160", "MG_Ex_theta_150_160", 1500, -3, 12);

    // h_phi_T1 = new TH1F("h_phi_T1", "MG_Ex_phi_T1_top", 1500, -3, 12);
    // h_phi_T2 = new TH1F("h_phi_T2", "MG_Ex_phi_T2_bottomRIGHT", 1500, -3, 12);
    // h_phi_T3 = new TH1F("h_phi_T3", "MG_Ex_phi_T3_bottomLEFT", 1500, -3, 12);
    // h_phi_T4 = new TH1F("h_phi_T4", "MG_Ex_phi_T4_topRIGHT", 1500, -3, 12);
    // h_phi_T5 = new TH1F("h_phi_T5", "MG_Ex_phi_T5_topLEFT", 1500, -3, 12);




    while (reader.Next()) {
        Long64_t t1;
        Double_t t2_1;
        Double_t t3;
        Double_t t5[10];
        Double_t tMC;
        Double_t tCPl;

        Double_t tME;
        Double_t tCE;
        Double_t thet;
        Double_t ph;
        Double_t tdc;
        Double_t beamE;

        if (/* *GATCONF_r == 1 || *GATCONF_r == 2 || */ *GATCONF_r == 1) {
            // cout<<"GATCONFMASTER_: "<<GATCONFMASTER_[0]<<endl;
            // t1 = GATCONFMASTERTS_[0]/1E10;
            // h1->Fill(t1);
            if(mgex.GetSize()>0){
                t2_1 = mgex[0];
                h2_1->Fill(t2_1);
            }

            if ( mg_lab.GetSize()>0){
                t3 = mg_lab[0];
                h3->Fill(t3);
            }

            const TVector3& v = *BeamImpact;
            double beamX = v.X();
            double beamY = v.Y();
            double beamZ = v.Z();

            tCPl = tac_cpl.GetSize()>0 ? tac_cpl[0] : 0;
            tMC = tac_mmg_cats.GetSize()>0 ? tac_mmg_cats[0] : 0;
            // tCE = tac_cats_exo.GetSize()>0 ? tac_cats_exo[0] : 0;
            // tME = tac_mmg_exo.GetSize()>0 ? tac_mmg_exo[0] : 0;
            thet = theta.GetSize()>0 ? theta[0] : 0;
            ph = phi.GetSize()>0 ? phi[0] : 0;
            // tdc = tac_d4_cats1.GetSize()>0 ? tac_d4_cats1[0] : 0;
            // h13->Fill(tMC);  


            //more size gamma_energy for gamma-gamma coincidence

            beamE = beam_energy.GetSize()>0 ? beam_energy[0] : 0;

            if(t3>1.0){                                 //cut on MG_ELab                             
                // if(beamE>900){                              //cut on Beam Energy
                    if(tCPl>300 && tCPl<400){                     //cut on TAC_CATS_PL     
                        if(abs(tMC-387)<25){                            //cut on TAC_MMG_CATS1
                            // if(abs(tME-432)<30){                            //cut on TAC_MMG_EXOGAM                        
                                // if(abs(tCE - 440)<20){                         //cut on TAC_CATS_EXOGAM
                                    // if(t7[2]>0){                                     //cut on IC3 energy
                                        // if(thet>110 && thet<120){    
                                            // if(cutg1->IsInside(beamX, beamY)){                  //cut on Beam spot
                                                // if(cut_dE_tof_cpl->IsInside(tCPl, (t7[0]+t7[1]+t7[2]+t7[3]+t7[4]))){  //cut on TAC_CATS_PL vs IC sum
                                                //     if(cut_dE_tof_d4cats1->IsInside(tdc, (t7[0]+t7[1]+t7[2]+t7[3]+t7[4]))){ //cut on TAC_D4_CATS1 vs IC sum  
                                                        h1->Fill(t2_1);                                
                                                        h_ex_vs_thet->Fill(thet, t2_1);
                                                        h_ex_vs_phi->Fill(ph, t2_1);

                                                        // if(thet>100 && thet<=110)     {h_ex_100_110->Fill(t2_1);}
                                                        // else if(thet>110 && thet<=120){h_ex_110_120->Fill(t2_1);}
                                                        // else if(thet>120 && thet<=130){h_ex_120_130->Fill(t2_1);}
                                                        // else if(thet>130 && thet<=140){h_ex_130_140->Fill(t2_1);}
                                                        // else if(thet>140 && thet<=150){h_ex_140_150->Fill(t2_1);}
                                                        // else if(thet>150 && thet<=160){h_ex_150_160->Fill(t2_1);}

                                                        // if(ph>60 && ph<=120)   {h_phi_T1->Fill(t2_1);}
                                                        // else if((ph>130 && ph<=180) || (ph>-180 && ph<=-170)){h_phi_T4->Fill(t2_1);}
                                                        // else if(ph>-80 && ph<=10){h_phi_T3->Fill(t2_1);}
                                                        // else if(ph>0 && ph<=60){h_phi_T5->Fill(t2_1);}
                                                        // else if(ph>-160 && ph<=-90){h_phi_T2->Fill(t2_1);}
                                                        
                                                    // }
                                                // }
                                            // }
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

    TCanvas *c2_1 = new TCanvas("c2_1", "unconditioned_ex", 800, 600);
    h2_1->Draw();

    TCanvas *c13 = new TCanvas("c13", "without Beam and IC cut", 800, 600);

    h1->Draw();



    TCanvas *theta_phi_ex = new TCanvas("theta_phi_ex", "theta_phi_ex", 1100, 800);

    // --- Left pad: takes 70% width, full height ---
    TPad *p1 = new TPad("p1", "left", 0.0, 0.0, 0.35, 1.0);
    p1->SetFillColor(0);
    //   p1->SetMargin(0.12, 0.05, 0.12, 0.05); // left, right, bottom, top margins
    p1->Draw();
    p1->cd();
    h_ex_vs_thet->RebinY(3);
    h_ex_vs_thet->Draw("COLZ");
        theta_phi_ex->cd();

    // --- Right-top pad: 30% width, top half ---
    TPad *p2 = new TPad("p2", "right-top", 0.37, 0.0, 1.0, 1.0);
    p2->SetFillColor(0);
    //   p2->SetMargin(0.15, 0.05, 0.02, 0.10);
    p2->Draw();
    p2->cd();
        h_ex_vs_phi->RebinY(3);
      h_ex_vs_phi->Draw("COLZ");  

    
}

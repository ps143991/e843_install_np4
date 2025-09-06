#include "NPApplication.h"
// #include
// "/Users/valerian/Software/Analysis/e870/e870_analysis/plugins/exogam/src/Geometry_Clover_Exogam.h"
#include "ExogamDetector.h"
#include "ExogamPhysics.h"
// #include "ExogamSpectra.h"
#include "Must2Physics.h"
#include "NPDetectorManager.h"
#include "NPFunction.h"
#include "NPReaction.h"
#include "ZddPhysics.h"
// #include "VDetector.h"
// #include "NPRootPlugin.h"
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TTreeReaderArray.h>

#include <iostream>

using namespace exogam;
using namespace must2;
using namespace zdd;
using namespace std;

void macroReader() {
  std::cout << "Init dummy application to use the detectors" << std::endl;
  auto app = nptool::Application::InitApplication("");
  nptool::Reaction r("34Si(p,d)33Si@1700MeV");

  double m1 = r.GetParticle1()->Mass();
  double m2 = r.GetParticle2()->Mass();
  double m3 = r.GetParticle3()->Mass();
  double m4 = r.GetParticle1()->Mass();

  // Commenting EXOGAM stuff for now since we don't need it to generate the Ex
  // spectrum

  // Must2Physics* phy_must2 = new Must2Physics();

  // TLorentzVector LV1, LV2, LV3, LV4;

  // Input
  // TFile* file = TFile::Open("../output/analysis/18O/run_0211_0.root");
  // TFile* file = TFile::Open(
  //     "~/Analysis/nptool_projects/e869_offline/output/analysis/"
  //     "run_0150_0.root");
  // if (!file || file->IsZombie()) {
  //   std::cerr << "Error opening file!" << std::endl;
  //   return;
  // }

  TChain* tree = new TChain("PhysicsTree");

  // Run 34Si
  // tree->Add("./output/analysis/run_0120_0.root");
  // tree->Add("./output/analysis/run_0121_0.root");
  // tree->Add("./output/analysis/run_0122_0.root");
  // tree->Add("./output/analysis/run_0123_0.root");
  // tree->Add("./output/analysis/run_0124_0.root");
  // tree->Add("./output/analysis/run_0125_0.root");
  // tree->Add("./output/analysis/run_0131_0.root");
  // tree->Add("./output/analysis/run_0131_1.root");
  // tree->Add("./output/analysis/run_0132_0.root");
  // tree->Add("./output/analysis/run_0134_0.root");
  // tree->Add("./output/analysis/run_0135_0.root");
  // tree->Add("./output/analysis/run_0136_0.root");
  // tree->Add("./output/analysis/run_0137_0.root");
  // tree->Add("./output/analysis/run_0138_0.root");
  // tree->Add("./output/analysis/run_0139_0.root");
  // tree->Add("./output/analysis/run_0140_0.root");
  // tree->Add("./output/analysis/run_0141_0.root");
  // tree->Add("./output/analysis/run_0142_0.root");
  // tree->Add("./output/analysis/run_0143_0.root");
  // tree->Add("./output/analysis/run_0144_0.root");
  // tree->Add("./output/analysis/run_0145_0.root");
  // tree->Add("./output/analysis/run_0147_0.root");
  // tree->Add("./output/analysis/run_0148_0.root");
  // tree->Add("./output/analysis/run_0149_0.root");
  // tree->Add("./output/analysis/run_0150_0.root");
  // tree->Add("./output/analysis/run_0150_1.root");
  // tree->Add("./output/analysis/run_0151_0.root");
  // tree->Add("./output/analysis/run_0152_0.root");
  // tree->Add("./output/analysis/run_0153_0.root");
  // // After 153 carbon
  // tree->Add("./output/analysis/run_0160_0.root");
  // tree->Add("./output/analysis/run_0161_0.root");
  // tree->Add("./output/analysis/run_0162_0.root");
  // tree->Add("./output/analysis/run_0163_0.root");
  // tree->Add("./output/analysis/run_0170_0.root");
  // tree->Add("./output/analysis/run_0170_1.root");
  // tree->Add("./output/analysis/run_0171_0.root");
  // tree->Add("./output/analysis/run_0171_1.root");
  // tree->Add("./output/analysis/run_0172_0.root");

  //Run 36S
  tree->Add("./output/analysis/run_0176_0.root");
  tree->Add("./output/analysis/run_0177_0.root");
  tree->Add("./output/analysis/run_0178_0.root");
  tree->Add("./output/analysis/run_0180_0.root");
  tree->Add("./output/analysis/run_0181_0.root");
  tree->Add("./output/analysis/run_0182_0.root");
  tree->Add("./output/analysis/run_0183_0.root");
  tree->Add("./output/analysis/run_0184_0.root");

  // Initialize TTreeReader
  TTreeReader reader(tree);

  ExogamDetector* det_exo = new ExogamDetector();
  ExogamPhysics* phy_exo = new ExogamPhysics();

  // Create TTreeReaderValues for branches
  TTreeReaderValue<ExogamPhysics> phy_exo_r(reader, "exogam");
  TTreeReaderValue<Must2Physics> phy_must2_r(reader, "must2");
  TTreeReaderValue<ZddPhysics> phy_zdd_r(reader, "zdd");

  TTreeReaderValue<unsigned int> GATCONF_r(reader, "GATCONF");
  TTreeReaderValue<unsigned long long> GATCONFTS_r(reader, "GATCONFTS");

  TTreeReaderValue<double> ELab_r(reader, "ELab");
  TTreeReaderValue<double> ThetaLab_r(reader, "ThetaLab");

  TTreeReaderArray<unsigned int> tac_cats_pl_r(reader, "TAC_CATS_PL");
  TTreeReaderArray<unsigned long long> tac_cats_pl_ts_r(reader,
                                                        "TAC_CATS_PL_TS");

  TTreeReaderArray<unsigned int> tac_mmg_exo_r(reader, "TAC_MMG_EXO");
  TTreeReaderArray<unsigned long long> tac_mmg_exo_ts_r(reader,
                                                        "TAC_MMG_EXO_TS");
  TTreeReaderArray<double> Ex_r(reader, "Ex");

  TFile* file_cutd = new TFile("macroVal/cutd.root");
  TCutG* cutd = (TCutG*)file_cutd->FindObjectAny("cutd");

  TFile* file_cutsi3 = new TFile("macroVal/cutSi3.root");
  TCutG* cutsi3 = (TCutG*)file_cutsi3->FindObjectAny("cutSi3");

  TFile* file_cutsi4 = new TFile("macroVal/cutSi4.root");
  TCutG* cutsi4 = (TCutG*)file_cutsi4->FindObjectAny("cutSi4");

  // Output
  TFile* outputFile = new TFile("macroVal/macroReaderOutput.root", "RECREATE");
  if (!outputFile || outputFile->IsZombie()) {
    outputFile->Close();
    return;
  }

  TH1F* h_ex = new TH1F("h_ex", "h_ex", 200, -10, 45);

  TTree* outputTree = new TTree("t", "Tree with Doppler corrected energies");

  double edc, sie, csie, tele, theta, phi, ex, e, ice, ple, icsum;
  double thetag, phig, egamma;
  double tac_mmg_exo, tac_cats_pl;
  int cryaddN;
  int pln;
  outputTree->Branch("edc", &edc, "edc/D");
  outputTree->Branch("egamma", &egamma, "egamma/D");
  outputTree->Branch("sie", &sie, "sie/D");
  outputTree->Branch("tele", &tele, "tele/D");
  outputTree->Branch("csie", &csie, "csie/D");
  // outputTree->Branch("beta", &beta, "beta/D");
  outputTree->Branch("theta", &theta, "theta/D");
  outputTree->Branch("phi", &phi, "phi/D");

  outputTree->Branch("thetag", &thetag, "thetag/D");
  outputTree->Branch("phig", &phig, "phig/D");

  outputTree->Branch("tac_mmg_exo", &tac_mmg_exo, "tac_mmg_exo/D");
  outputTree->Branch("tac_cats_pl", &tac_cats_pl, "tac_cats_pl/D");

  outputTree->Branch("cryaddN", &cryaddN, "cryaddN/I");

  outputTree->Branch("e", &e, "e/D");
  outputTree->Branch("ex", &ex, "ex/D");
  outputTree->Branch("ice", &ice, "ice/D");
  outputTree->Branch("ple", &ple, "ple/D");
  outputTree->Branch("pln", &pln, "pln/I");

  // beta 34Si 
  // double beta = 0.311287;
  // beta 36S
  double beta = 0.315;

  // Loop over the entries
  while (reader.Next()) {
    if (reader.GetCurrentEntry() % 100000 == 0) {
      cout << (double)reader.GetCurrentEntry() / tree->GetEntries() * 100.
           << endl;
    }

    if (*GATCONF_r == 8) {
      if (phy_must2_r->Si_E.size() == 1) {
        sie = phy_must2_r->Si_E[0];
        csie = phy_must2_r->CsI_E[0];
        if (cutd->IsInside(csie, sie)) {
          if (phy_zdd_r->ICSum.size() > 0) {
            if (phy_zdd_r->PL_E.size() > 0) {
              unsigned int plsize = phy_zdd_r->PL_TS.size();
              for (unsigned int i = 0; i < plsize; i++) {
                // if (phy_zdd_r->PL_E[i] > 27000 && phy_zdd_r->PL_E[i] < 32000)
                // {
                if ((abs(static_cast<long long>(phy_zdd_r->PL_TS[i]) -
                         static_cast<long long>(phy_must2_r->M2_TS[0]) - 17) <
                     10)) {
                  unsigned int icsumsize = phy_zdd_r->ICSum.size();
                  for (unsigned int j = 0; j < icsumsize; j++) {
                    if ((abs(static_cast<long long>(phy_zdd_r->ICSumTS[j]) -
                             static_cast<long long>(phy_must2_r->M2_TS[0]) -
                             35) < 10)) {
                      unsigned int tac_cats_pl_size = tac_cats_pl_r.GetSize();
                      for (unsigned int k = 0; k < tac_cats_pl_size; k++) {
                        if ((((phy_zdd_r->ICSumMult[j] == 3) &&
                              cutsi3->IsInside(tac_cats_pl_r[k],
                                               phy_zdd_r->ICSum[j])) ||
                             ((phy_zdd_r->ICSumMult[j] == 4) &&
                              cutsi4->IsInside(tac_cats_pl_r[k],
                                               phy_zdd_r->ICSum[j])))) {
                          if (Ex_r.GetSize() > 0) {
                            double Ex_aligned = 0;
                            if (phy_must2_r->TelescopeNumber[0] == 1) {
                              Ex_aligned = Ex_r[0] - 0.15;
                            } else if (phy_must2_r->TelescopeNumber[0] == 2) {
                              Ex_aligned = Ex_r[0] - 0.6;
                            } else if (phy_must2_r->TelescopeNumber[0] == 3) {
                              Ex_aligned = Ex_r[0] + 1.9;
                            } else if (phy_must2_r->TelescopeNumber[0] == 4) {
                              Ex_aligned = Ex_r[0] + 2.2;
                            }

                            e = *ELab_r;
                            theta = *ThetaLab_r;
                            ple = phy_zdd_r->PL_E[i];
                            pln = phy_zdd_r->PL_Nbr[i];
                            tac_cats_pl = tac_cats_pl_r[k];
                            ice = phy_zdd_r->ICSum[j];
                            ex = Ex_aligned;

                            if (phy_exo_r->Exo_EAddBack.size() == 1) {
                              if (tac_mmg_exo_r.GetSize())
                                tac_mmg_exo = tac_mmg_exo_r[0];
                              // Clover_struc Exogam_struc = Ask_For_Angles(
                              //     phy_exo_r->Exo_FlangeNAddBack[0],
                              //     det_exo->ComputeMeanFreePath(Egamma) + 20);

                              // double Theta_seg =
                              //     Exogam_struc
                              //         .Theta_Crystal_Seg[Crystal][Outer];
                              // double Phi_seg =
                              //     Exogam_struc.Phi_Crystal_Seg[Crystal][Outer];
                              // std::cout << phy_exo_r->Exo_FlangeNAddBack[0]
                              //           << " " << Theta_seg * 180. / M_PI <<
                              //           " "
                              //           << Phi_seg * 180. / M_PI <<
                              //           std::endl;

                              // edc = Doppler_Correction(Theta_seg, Phi_seg, 0,
                              // 0,
                              //                          beta, Egamma);

                              // std::cout <<"EXO: "<< phy_exo_r->Exo_Theta[0]
                              // << std::endl;

                              TVector3 TargetPos(0, 0, 0);
                              TVector3 BeamDir(0, 0, 1);
                              TVector3 OffsetTarget(0, 0, 100);

                              TVector3 ExoDetector(
                                  sin(phy_exo_r->Exo_Theta[0]) *
                                      cos(phy_exo_r->Exo_Phi[0]),
                                  sin(phy_exo_r->Exo_Theta[0]) *
                                      sin(phy_exo_r->Exo_Phi[0]),
                                  cos(phy_exo_r->Exo_Theta[0]));

                              // thetag = phy_exo_r->Exo_Theta[0] * 180. / M_PI;
                              // phig = phy_exo_r->Exo_Phi[0] * 180. / M_PI;

                              double Egamma = phy_exo_r->Exo_EAddBack[0];
                              unsigned int Crystal =
                                  phy_exo_r->Exo_CrystalNAddBack[0];
                              unsigned int Outer =
                                  phy_exo_r->Exo_OuterNAddBack[0];

                              Clover_struc Exogam_struc = Ask_For_Angles(
                                  phy_exo_r->Exo_FlangeNAddBack[0], 20);

                              double Theta_seg =
                                  Exogam_struc
                                      .Theta_Crystal_Seg[Crystal][Outer];
                              double Phi_seg =
                                  Exogam_struc.Phi_Crystal_Seg[Crystal][Outer];

                              thetag = Theta_seg * 180. / M_PI;
                              phig = Phi_seg * 180. / M_PI;

                              // std::cout << Theta_seg * 180. / M_PI << " "
                              //           << thetag << " "
                              //           << Phi_seg * 180. / M_PI << " " <<
                              //           phig
                              //           << std::endl;

                              cryaddN = phy_exo_r->Exo_FlangeNAddBack[0] * 4 +
                                        phy_exo_r->Exo_CrystalNAddBack[0];

                              // TVector3 ExoDetectorOffset =
                              //     ExoDetector - OffsetTarget;

                              egamma = Egamma;
                              edc = phy_exo_r->Exo_EAddBack[0] *
                                    ((1. - beta * cos(Theta_seg)) /
                                     (sqrt(1. - beta * beta)));

                              //                               Clover_struc
                              //                               Exogam_struc =
                              //                               Ask_For_Angles(
                              //                                   phy_exo_r->Exo_FlangeNAddBack[0],
                              //                                   det_exo->ComputeMeanFreePath(Egamma)
                              //                                   + 20);
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      } else {
        sie = -1000;
        csie = -1000;
        e = -1000;
        theta = -1000;
        phi = -1000;
        ex = -1000;
        edc = -1000;
        ice = -1000;
        ple = -1000;
      }
      outputTree->Fill();
    }
  }

  outputTree->Write();
  outputFile->Close();
}

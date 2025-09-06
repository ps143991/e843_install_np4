#include "Analysis.h"

#include "NPApplication.h"

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::Init() {
  auto app = nptool::Application::GetApplication();
  // mydetector =
  // std::dynamic_pointer_cast<my_detector::MyDetector>(app->GetDetector("mydetector"));

  cats =
      std::dynamic_pointer_cast<cats::CatsDetector>(app->GetDetector("cats"));

  must2 = std::dynamic_pointer_cast<must2::Must2Detector>(
      app->GetDetector("must2"));

  zdd = std::dynamic_pointer_cast<zdd::ZddDetector>(app->GetDetector("zdd"));

  exogam = std::dynamic_pointer_cast<exogam::ExogamDetector>(
      app->GetDetector("exogam"));
  gatconf =
      std::dynamic_pointer_cast<ebye::EbyEDetector>(app->GetDetector("ebye"));

  tac = std::dynamic_pointer_cast<tac::TacDetector>(app->GetDetector("tac"));

  // Declare directories
  TDirectory* anadir = gROOT->mkdir("analysis_online");
  anadir->cd();

  unsigned int nb_crystal = 38;
  unsigned int nb_crystal_zdd = 4;
  unsigned int nb_ic = 2;
  unsigned int nb_pl = 10;
  unsigned int nb_must2 = 4;
  unsigned int nb_cats = 2;
  unsigned int nb_tac = 2;
  unsigned int numexosize = 0;
  unsigned int np_ebye = 1;
  for (auto it : numexo->m_NumexoMap) {
    numexosize++;
  }

  auto c_ICTOF_CATS_PL = new TCanvas("ZDD_phy_TOF_PL");
  c_ICTOF_CATS_PL->cd();
  std::string hist_name = "ZDD_phy_TOF_PL";
  m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
                                      6500, 0, 65000, 1000, 0, 10000);
  m_phy_hist_2D[hist_name]->Draw("col");

  auto c_ICTOF_CATS_D4_CATS1 = new TCanvas("ZDD_phy_TOF_D4_CATS1");
  c_ICTOF_CATS_D4_CATS1->cd();
  hist_name = "ZDD_phy_TOF_D4_CATS1";
  m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
                                      6000, 0, 60000, 1000, 0, 10000);
  m_phy_hist_2D[hist_name]->Draw("col");

  auto c_ICTOF_CATS_D4_D6 = new TCanvas("ZDD_phy_TOF_D4_D6");
  c_ICTOF_CATS_D4_D6->cd();
  hist_name = "ZDD_phy_TOF_D4_D6";
  m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
                                      6000, 0, 60000, 1000, 0, 10000);
  m_phy_hist_2D[hist_name]->Draw("col");

  auto c_TSDiff = new TCanvas("TSDiff");
  unsigned int nb_tot =
      nb_crystal + nb_crystal_zdd + nb_ic + nb_must2 + nb_pl + numexosize;
  hist_name = "TS_diff";
  m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
                                      nb_tot, 0, nb_tot, 2400, -1200, 1200);
  c_TSDiff->cd();
  m_phy_hist_2D[hist_name]->Draw("col");

  unsigned int bin = 1;
  for (unsigned int i = 0; i < nb_crystal; i++) {
    std::string name = "EXO_" + nptool::itoa(i + 8);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin, name.c_str());
    m_bin_label[name] = bin;
    bin++;
  }
  for (unsigned int i = 0; i < nb_crystal_zdd; i++) {
    std::string name = "EXO_ZDD_" + nptool::itoa(i);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin, name.c_str());
    m_bin_label[name] = bin;
    bin++;
  }
  for (unsigned int i = 0; i < nb_ic; i++) {
    std::string name = "IC_ZDD_" + nptool::itoa(i);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin, name.c_str());
    m_bin_label[name] = bin;
    bin++;
  }
  for (unsigned int i = 0; i < nb_pl; i++) {
    std::string name = "PL_ZDD_" + nptool::itoa(i);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin, name.c_str());
    m_bin_label[name] = bin;
    bin++;
  }
  for (unsigned int i = 0; i < nb_must2; i++) {
    std::string name = "MM_" + nptool::itoa(i + 1);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin, name.c_str());
    m_bin_label[name] = bin;
    bin++;
  }
  // for (unsigned int i = 0; i < nb_cats; i++) {
  //   std::string name = "CATS_" + nptool::itoa(i + 1);
  //   m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin, name.c_str());
  //   m_bin_label[name] = bin;
  //   bin++;
  // }
  for (auto it : numexo->m_NumexoMap) {
    std::string name = it.second + "_TS";
    m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin, name.c_str());
    m_bin_label[name] = bin;
    bin++;
  }

  /*
     bin = 1;
  // hist_name = "Various_checks";
  // m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
  //        6, 0, 6, 2, 0, 2);
  std::string name = "PL_TS";
  m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin,name.c_str());
  m_bin_label[name] = bin;
  bin++;
  name = "TAC_TS";
  m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin,name.c_str());
  m_bin_label[name] = bin;
  bin++;
  name = "IC_TS";
  m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin,name.c_str());
  m_bin_label[name] = bin;
  bin++;
  name = "CONFDEC";
  m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin,name.c_str());
  m_bin_label[name] = bin;
  bin++;
  name = "DATATRIG1";
  m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin,name.c_str());
  m_bin_label[name] = bin;
  bin++;
  name = "DATATRIG2";
  m_phy_hist_2D[hist_name]->GetXaxis()->SetBinLabel(bin,name.c_str());
  m_bin_label[name] = bin;
  bin++;

  hist_name = "ICE_TOF";
  m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
  500,22500,25000,1000, 0,4000);

  // hist_name = "TS_diff_PL_EXOZDD";
  // TS_hist[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
  //        400, -400, 400);
  // hist_name = "TS_diff_PL_ebye";
  // TS_hist[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
  //        400, -400, 400);
  // hist_name = "TS_diff_PL_EXO";
  // TS_hist[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
  //        400, -400, 400);
  for (auto hist:m_phy_hist_2D) hist.second->Draw();
  // h_exopl_tsdiff->Draw("colz");
  */
  gROOT->cd();
}

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::TreatEvent() {
  // if( cats->m_PhysicsData->CalStripNumberX.size()>0)
  //   std::cout << cats->m_PhysicsData->CalStripNumberX.size() << " " <<
  //   must2->m_PhysicsData<< std::endl;
  unsigned int catssize = cats->m_RawData->fCATS_TimeStamp
                              .size();  // cats->m_PhysicsData->CATS_TS.size();
  // std::cout << cats->m_RawData->fCATS_TimeStamp.size() << std::endl;

  // if (cats->m_PhysicsData->DetNumber.size() > 0)
  //   std::cout << cats->m_PhysicsData->CATS_TS.size() << " "
  //             << cats->m_PhysicsData->DetNumber.size() << std::endl;
  unsigned int must2size = must2->m_RawData->fMM_TimeStamp.size();
  unsigned int icsize = zdd->m_RawData->fZDD_IC_TS.size();
  unsigned int plsize = zdd->m_RawData->fZDD_PL_TS.size();
  unsigned int exozddsize = zdd->m_RawData->fZDD_EXO_TS.size();
  unsigned int exosize = exogam->m_RawData->fExo_TS.size();

  //   unsigned int must2size = must2->m_PhysicsData->Si_E.size();
  //   unsigned int icsize = zdd->m_PhysicsData->IC_E.size();
  // unsigned int plsizephy = zdd->m_PhysicsData->PL_E.size();
  //   unsigned int exozddsize = zdd->m_PhysicsData->ZDDExo_E.size();
  //   unsigned int exosize = exogam->m_PhysicsData->Exo_E.size();
  // std::cout <<"PL: "<< plsize << std::endl;
  // std::cout <<"PLphy: "<< plsizephy << std::endl;

  // std::cout << ">>>>>>>>>> DEBUG : " << 0 << std::endl;
  if (catssize > 0) {
    // if (plsize > 0) {
    // if (icsize > 0) {
    std::string hist_name = "TS_diff";
    // unsigned long long TS_Pl_ref = *std::min_element(
    //     zdd->m_PhysicsData->PL_TS.begin(), zdd->m_PhysicsData->PL_TS.end());
    unsigned long long TS_ref =
        *std::min_element(cats->m_RawData->fCATS_TimeStamp.begin(),
                          cats->m_RawData->fCATS_TimeStamp.end());
    // unsigned long long TS_ref =
    //     *std::min_element(zdd->m_RawData->fZDD_PL_TS.begin(),
    //                       zdd->m_RawData->fZDD_PL_TS.end());
    // std::cout << TS_ref << std::endl;
    // unsigned long long TS_Pl_ref = *std::min_element(
    //     zdd->m_PhysicsData->IC_TS.begin(), zdd->m_PhysicsData->IC_TS.end());

    // std::cout << ">>>>>>>>>> DEBUG : " << 1 << std::endl;
    if (TS_ref > 0) {
      // std::cout << ">>>>>>>>>> DEBUG : " << 2 << std::endl;
      for (unsigned int i = 0; i < exosize; i++) {
        if (exogam->m_RawData->fExo_TS[i]) {
          std::string name =
              "EXO_" + nptool::itoa(exogam->m_RawData->fExo_Crystal[i]);
          m_phy_hist_2D[hist_name]->Fill(
              m_bin_label[name] - 1,
              int(exogam->m_RawData->fExo_TS[i] - TS_ref));
        }
      }
      // std::cout << ">>>>>>>>>> DEBUG : " << 3 << std::endl;

      for (unsigned int i = 0; i < exozddsize; i++) {
        if (zdd->m_RawData->fZDD_EXO_TS[i]) {
          std::string name =
              "EXO_ZDD_" + nptool::itoa(zdd->m_RawData->GetZDD_EXON(i));
          m_phy_hist_2D[hist_name]->Fill(
              m_bin_label[name] - 1,
              int(zdd->m_RawData->fZDD_EXO_TS[i] - TS_ref));
        }
      }
      // std::cout << ">>>>>>>>>> DEBUG : " << 4 << std::endl;

      for (unsigned int i = 0; i < icsize; i++) {
        if (zdd->m_RawData->GetZDD_ICMult() == icsize) {
          if (zdd->m_RawData->fZDD_IC_TS[i]) {
            std::string name =
                "IC_ZDD_" + nptool::itoa(zdd->m_RawData->GetZDD_ICN(i));
            m_phy_hist_2D[hist_name]->Fill(
                m_bin_label[name] - 1,
                int(zdd->m_RawData->fZDD_IC_TS[i] - TS_ref));
          }
        }
      }

      // std::cout << ">>>>>>>>>> DEBUG : " << 5 << std::endl;

      for (unsigned int i = 0; i < must2size; i++) {
        if (must2->m_RawData->fMM_TimeStamp[i]) {
          std::string name =
              "MM_" +
              nptool::itoa(must2->m_RawData->GetMMStripXEDetectorNbr(i));
          m_phy_hist_2D[hist_name]->Fill(
              m_bin_label[name] - 1,
              int(must2->m_RawData->fMM_TimeStamp[i] - TS_ref));
        }
      }
      // std::cout << ">>>>>>>>>> DEBUG : " << 6 << std::endl;

      // std::cout <<  zdd->m_RawData->GetZDD_PLMult() << " " << plsize <<
      // std::endl;
      for (unsigned int i = 0; i < plsize; i++) {
        if (zdd->m_RawData->GetZDD_PLMult() == plsize) {
          if (zdd->m_RawData->fZDD_PL_TS[i]) {
            std::string name =
                "PL_ZDD_" + nptool::itoa(zdd->m_RawData->GetZDD_PLN(i));
            m_phy_hist_2D[hist_name]->Fill(
                m_bin_label[name] - 1,
                int(zdd->m_RawData->fZDD_PL_TS[i] - TS_ref));
          }
        }
      }
      // std::cout << ">>>>>>>>>> DEBUG : " << 7 << std::endl;

      // for (unsigned int i = 0; i < catssize; i++) {
      //   if (cats->m_PhysicsData->CATS_TS.size() ==
      //       cats->m_PhysicsData->DetNumber.size()) {
      //     if (cats->m_PhysicsData->CATS_TS[i]) {
      //       std::string name =
      //           "CATS_" + nptool::itoa(cats->m_PhysicsData->DetNumber[i]);

      //       m_phy_hist_2D[hist_name]->Fill(
      //           m_bin_label[name] - 1,
      //           int(cats->m_PhysicsData->CATS_TS[i] - TS_Pl_ref));
      //     }
      //   }
      // }

      // std::cout << ">>>>>>>>>> DEBUG : " << 8 << std::endl;
      for (auto it : numexo->GenericRawBranchTS) {
        std::string name = it.first;
        for (auto value : *(it.second)) {
          if (value > 0) {
            m_phy_hist_2D[hist_name]->Fill(m_bin_label[name] - 1,
                                           int(value - TS_ref));
          }
        }
      }

      // for (auto it : gatconf->GenericRawBranchTS) {
      //   std::string name = it.first;
      //   for (auto value : *(it.second)) {
      //     if (value > 0) {
      //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name] - 1,
      //                                      int(value - TS_ref));
      //     }
      //   }
      // }

      //   for (auto it : ->GenericPhysicsBranch) {
      //     if (int(*(gatconf->GenericPhysicsBranchTS["GATCONFMASTERTS"])) > 0)
      //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name] - 1, 1);
      //     else
      //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name] - 1, 0);
      //   }

      // std::cout << ">>>>>>>>>> DEBUG : " << 9 << std::endl;
      // }
    }
    // std::cout << ">>>>>>>>>> DEBUG : " << 10 << std::endl;
    for (auto it : numexo->GenericPhysicsBranch) {
      std::string name = it.first;
      for (auto value : *(it.second)) {
        if (value > 0) {
          if (name == "TAC_CATS_PL") {
            std::string hist_name = "ZDD_phy_TOF_PL";
            unsigned int sizeICSum = zdd->m_PhysicsData->ICSum.size() > 0;
            if (sizeICSum > 0) {
              for (unsigned int i = 0; i < sizeICSum; i++) {
                if (zdd->m_PhysicsData->ICSumMult[i] == 4) {
                  m_phy_hist_2D[hist_name]->Fill(value,
                                                 zdd->m_PhysicsData->ICSum[i]);
                }
              }
            }
          }
          if (name == "TAC_D4_D6") {
            std::string hist_name = "ZDD_phy_TOF_D4_D6";
            unsigned int sizeICSum = zdd->m_PhysicsData->ICSum.size() > 0;
            if (sizeICSum > 0) {
              for (unsigned int i = 0; i < sizeICSum; i++) {
                if (zdd->m_PhysicsData->ICSumMult[i] == 4) {
                  m_phy_hist_2D[hist_name]->Fill(value,
                                                 zdd->m_PhysicsData->ICSum[i]);
                }
              }
            }
          }
          if (name == "TAC_D4_CATS1_L") {
            std::string hist_name = "ZDD_phy_TOF_D4_CATS1";
            unsigned int sizeICSum = zdd->m_PhysicsData->ICSum.size() > 0;
            if (sizeICSum > 0) {
              for (unsigned int i = 0; i < sizeICSum; i++) {
                if (zdd->m_PhysicsData->ICSumMult[i] == 4) {
                  m_phy_hist_2D[hist_name]->Fill(value,
                                                 zdd->m_PhysicsData->ICSum[i]);
                }
              }
            }
          }
        }
      }
    }
  }
  // std::string hist_name = "Various_checks";
  // if(plsize > 0){
  //   std::string name = "PL_TS";
  //   if(*std::min_element(zdd->m_PhysicsData->PL_TS.begin(),zdd->m_PhysicsData->PL_TS.end())
  //   > 0)
  //   {
  //     if(int(*(gatconf->GenericPhysicsBranchTS["GATCONFMASTERTS"]))>0)
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,1);
  //     else
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,0);
  //     }
  //   }
  // if(icsize > 0){
  //   std::string name = "IC_TS";
  //   if(*std::min_element(zdd->m_PhysicsData->IC_TS.begin(),zdd->m_PhysicsData->IC_TS.end())
  //   > 0)
  //   {
  //     if(int(*(gatconf->GenericPhysicsBranchTS["GATCONFMASTERTS"]))>0)
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,1);
  //     else
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,0);
  //     }
  //   }
  // if(tacsize > 0){
  //   std::string name = "TAC_TS";
  //   if(*std::min_element(tac->m_PhysicsData->TAC_TS.begin(),tac->m_PhysicsData->TAC_TS.end())
  //   > 0)
  //   {
  //     if(int(*(gatconf->GenericPhysicsBranchTS["GATCONFMASTERTS"]))>0)
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,1);
  //     else
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,0);
  //     }
  //   }
  // if(int(*(gatconf->GenericPhysicsBranch["GATCONFMASTER"])) > 0){
  //   std::string name = "CONFDEC";
  //   if(int(*(gatconf->GenericPhysicsBranch["CONFDEC"])) > 0)
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,1);
  //   else
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,0);
  //   name = "DATATRIG1";
  //   if(int(*(gatconf->GenericPhysicsBranch["DATATRIG1"])) > 0)
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,1);
  //   else
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,0);
  //   name = "DATATRIG2";
  //   if(int(*(gatconf->GenericPhysicsBranch["DATATRIG_CATS"])) > 0)
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,1);
  //   else
  //       m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,0);
  //       }

  // hist_name = "ICE_TOF";
  // for(unsigned int i=0; i < tacsize; i++){
  //   if(tac->m_PhysicsData->TAC_Name[i] == "TAC_CATS_PL" &&
  //   zdd->m_PhysicsData->ICSum > 0 && zdd->m_PhysicsData->IC_E.size() ==
  //   2){
  //       m_phy_hist_2D[hist_name]->Fill(tac->m_PhysicsData->TAC_Time[i],zdd->m_PhysicsData->ICSum);
  //   }
  // }

  // std::string name = "AGAVA_TS";
  // // std::cout << *(gatconf->GenericPhysicsBranch["GATCONFMASTER"]) << "
  // "
  // <<
  // *(gatconf->GenericPhysicsBranchTS["GATCONFMASTERTS"]) << std::endl;
  // // std::cout << *(gatconf->GenericRawBranch["GATCONFMASTER"]) << " " <<
  // *(gatconf->GenericRawBranchTS["GATCONFMASTERTS"]) << std::endl;
  // if(*(gatconf->GenericPhysicsBranch["GATCONFMASTER"]) > 0){
  // if(*(gatconf->GenericPhysicsBranchTS["GATCONFMASTERTS"]) > 0)
  //   m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,1);
  // else
  //   m_phy_hist_2D[hist_name]->Fill(m_bin_label[name]-1,0);
  // }
  // // if(plsize>0 && icsize>0){
  //   std::string hist_name = "TS_diff_PL_IC";
  //   TS_hist[hist_name]->Fill(zdd->m_PhysicsData->IC_TS[0]-zdd->m_PhysicsData->PL_TS[0]);
  // }
  // // if(plsize>0 && exosize>0){
  // //   std::string hist_name = "TS_diff_PL_EXOZDD";
  // //
  // TS_hist[hist_name]->Fill(zdd->m_PhysicsData->Exo_TS[0]-zdd->m_PhysicsData->PL_TS[0]);
  // // }
  // if(plsize>0 && *(gatconf->GenericPhysicsBranch["GATCONFMASTER"]) >0){
  //   std::string hist_name = "TS_diff_PL_ebye";
  //   TS_hist[hist_name]->Fill(*(gatconf->GenericPhysicsBranchTS["GATCONFMASTERTS"])-zdd->m_PhysicsData->PL_TS[0]);
  // }
  // // if(plsize>0 && exosize >0){
  // //
  // h_exopl_tsdiff->Fill(exogam->m_RawData->fExo_Crystal[0],exogam->m_RawData->fExo_TS[0]-zdd->m_PhysicsData->PL_TS[0]);
  // // }
}

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::End() {}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserAnalysis> ConstructUserAnalysis() {
  return std::make_shared<user_analysis::Analysis>();
}
}

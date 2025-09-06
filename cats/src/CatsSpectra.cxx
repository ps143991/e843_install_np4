/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girard-alcindor@ijclab.in2p3.fr                          *
 *                                                                           *
 * Creation Date  :    08/03/24                                              *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This class is heavily based on the nptool v3 CATS detector                *
 *                                                                           *
 *****************************************************************************/
#include "CatsSpectra.h"

#include "NPApplication.h"
#include "TDirectory.h"
#include "TROOT.h"

using namespace cats;
////////////////////////////////////////////////////////////////////////////////
CatsSpectra::CatsSpectra() {

  // Set Pointers:
  TDirectory *catsdir = gROOT->mkdir("cats");
  catsdir->cd();
  TDirectory *catsdir_raw = gROOT->mkdir("cats/Raw");
  TDirectory *catsdir_phy = gROOT->mkdir("cats/Phy");


  auto app = nptool::Application::GetApplication();
  // Set Pointers:
  m_detector =
      std::dynamic_pointer_cast<CatsDetector>(app->GetDetector("cats"));

  m_RawData = m_detector->m_RawData;
  m_RawData->Clear();

  m_CalData = m_detector->m_CalData;
  m_CalData->Clear();
  m_PhysicsData = m_detector->m_PhysicsData;
  m_PhysicsData->Clear();
  fStripsNumber = 28;


 /********************************DECLARING EMPTY HISTOGRAMS************************************/

  // Declare Raw Spectra
  if (app->HasFlag("--input-raw")) {
    for (int i = 0; i < 2; i++) {
      catsdir_raw->cd();
      std::string cats_prefix = "Cats" + nptool::itoa(i + 1);

      std::string histo_nameXQ = cats_prefix + "_raw_STRX_Q";
      m_raw_hist[histo_nameXQ] = new TH2F(
          histo_nameXQ.c_str(), histo_nameXQ.c_str(), 28, 1, 29, 512, 0, 16384);

      std::string histo_nameYQ = cats_prefix + "_raw_STRY_Q";
      m_raw_hist[histo_nameYQ] = new TH2F(
          histo_nameYQ.c_str(), histo_nameYQ.c_str(), 28, 1, 29, 512, 0, 16384);
    }
  }

  // Declare Phy Spectra
  if (app->HasFlag("--input-phy")) {
    for (int i = 0; i < 2; i++) {
      catsdir_phy->cd();
      std::string cats_prefix = "Cats" + nptool::itoa(i + 1);

      std::string histo_nameXQPhy = cats_prefix + "_phy_STRX_Q";
      m_phy_hist[histo_nameXQPhy] =
          new TH2F(histo_nameXQPhy.c_str(), histo_nameXQPhy.c_str(), 28, 1, 29,
                   512, 0, 16384);

      std::string histo_nameYQPhy = cats_prefix + "_phy_STRY_Q";
      m_phy_hist[histo_nameYQPhy] =
          new TH2F(histo_nameYQPhy.c_str(), histo_nameYQPhy.c_str(), 28, 1, 29,
                   512, 0, 16384);

      std::string histo_nameXQCal = cats_prefix + "_cal_STRX_Q";
      m_phy_hist[histo_nameXQCal] =
          new TH2F(histo_nameXQCal.c_str(), histo_nameXQCal.c_str(), 28, 1, 29,
                   512, 0, 16384);

      std::string histo_nameYQCal = cats_prefix + "_cal_STRY_Q";
      m_phy_hist[histo_nameYQCal] =
          new TH2F(histo_nameYQCal.c_str(), histo_nameYQCal.c_str(), 28, 1, 29,
                   512, 0, 16384);

      std::string histo_nameXMULTCal = cats_prefix + "_cal_STRX_MULT";
      m_phy_hist[histo_nameXMULTCal] = new TH1F(
          histo_nameXMULTCal.c_str(), histo_nameXMULTCal.c_str(), 28, 1, 29);

      std::string histo_nameYMULTCal = cats_prefix + "_cal_STRY_MULT";
      m_phy_hist[histo_nameYMULTCal] = new TH1F(
          histo_nameYMULTCal.c_str(), histo_nameYMULTCal.c_str(), 28, 1, 29);

      std::string histo_nameXMULTPhy = cats_prefix + "_phy_STRX_MULT";
      m_phy_hist[histo_nameXMULTPhy] = new TH1F(
          histo_nameXMULTPhy.c_str(), histo_nameXMULTPhy.c_str(), 28, 1, 29);

      std::string histo_nameYMULTPhy = cats_prefix + "_phy_STRY_MULT";
      m_phy_hist[histo_nameYMULTPhy] = new TH1F(
          histo_nameYMULTPhy.c_str(), histo_nameYMULTPhy.c_str(), 28, 1, 29);

      std::string histo_nameXYPhy = cats_prefix + "_phy_PosX_vs_PosY";
      m_phy_hist[histo_nameXYPhy] =
          new TH2F(histo_nameXYPhy.c_str(), histo_nameXYPhy.c_str(), 300, -30,
                   30, 300, -30, 30);
    }

    std::string histo_nameTPos = "Cats_phy_TARGET_POS";
    m_phy_hist[histo_nameTPos] =
        new TH2F(histo_nameTPos.c_str(), histo_nameTPos.c_str(), 320, -40, 40,
                 320, -40, 40);

    std::string histo_nameTPXZ = "Cats_phy_TRAJECTORY_XZ";
    m_phy_hist[histo_nameTPXZ] =
        new TH2F(histo_nameTPXZ.c_str(), histo_nameTPXZ.c_str(), 500, -2000,
                 1000, 2000, -200, 200);

    std::string histo_nameTPYZ = "Cats_phy_TRAJECTORY_YZ";
    m_phy_hist[histo_nameTPYZ] =
        new TH2F(histo_nameTPYZ.c_str(), histo_nameTPYZ.c_str(), 500, -2000,
                 1000, 2000, -200, 200);
  }




  /********************************DRAWING EMPTY HISTOGRAMS************************************/

  auto cEN_raw1 = new TCanvas("Cats raw Q");

  cEN_raw1->Divide(2, 2);

    if (app->HasFlag("--input-raw")) {
      // Loop through cats
      for (int j = 0; j < 2; ++j) {
        cEN_raw1->cd(j + 1);
        // Define histogram name based on canvas title and telescope index
        std::string histo_name = "Cats" + nptool::itoa(j + 1) + "_raw_";
        histo_name += "STRX_Q";
        m_raw_hist[histo_name]->Draw();

        cEN_raw1->cd(j + 3);
        histo_name = "Cats" + nptool::itoa(j + 1) + "_raw_";
        histo_name += "STRY_Q";
        m_raw_hist[histo_name]->Draw();
      }
    }

  // Loop through cats
  auto cEN_phy11 = new TCanvas("Cats cal Q");
  auto cEN_phy3 = new TCanvas("Cats cal MULT");
  auto cEN_phy1 = new TCanvas("Cats phy Q");
  // auto cEN_phy4 = new TCanvas("Cats phy MULT");
  auto cEN_phy5 = new TCanvas("Cats phy PosX vs PosY");
  auto cEN_phy6 = new TCanvas("Cats phy Target Pos");
  auto cEN_phy7 = new TCanvas("Cats phy Trajectory");
  // auto cEN_phy8 = new TCanvas("Cats phy Trajectory");

  cEN_phy11->Divide(2, 2);
  cEN_phy3->Divide(2, 2);
  // cEN_phy4->Divide(2, 2);
  cEN_phy5->Divide(2, 1);
  cEN_phy7->Divide(1, 2);


  
   if(app->HasFlag("--input-phy")) {
  // Loop through cats
  for (int j = 0; j < 2; ++j) {

    cEN_phy1->cd(j + 1);

    // Define histogram name based on canvas title and telescope index
    std::string histo_name = "Cats" + nptool::itoa(j + 1) + "_phy_";

  histo_name += "STRX_Q";

  m_phy_hist[histo_name]->Draw();                

    cEN_phy1->cd(j + 3);
    histo_name = "Cats" + nptool::itoa(j + 1) + "_phy_";
    histo_name += "STRY_Q";
    m_phy_hist[histo_name]->Draw();

    cEN_phy11->cd(j + 1);
    // Define histogram name based on canvas title and telescope index
    histo_name = "Cats" + nptool::itoa(j + 1) + "_cal_";
    histo_name += "STRX_Q";
    m_phy_hist[histo_name]->Draw();

    cEN_phy11->cd(j + 3);
    histo_name = "Cats" + nptool::itoa(j + 1) + "_cal_";
    histo_name += "STRY_Q";
    m_phy_hist[histo_name]->Draw();

    cEN_phy3->cd(j + 1);
    histo_name = "Cats" + nptool::itoa(j + 1) + "_cal_";
    histo_name += "STRX_MULT";
    m_phy_hist[histo_name]->Draw();

    cEN_phy3->cd(j + 3);
    histo_name = "Cats" + nptool::itoa(j + 1) + "_cal_";
    histo_name += "STRY_MULT";
    m_phy_hist[histo_name]->Draw();

    // cEN_phy4->cd(j + 1);
    // histo_name = "Cats" + nptool::itoa(j + 1) + "_phy_";
    // histo_name += "STRX_MULT";
    // m_phy_hist[histo_name]->Draw();

    // cEN_phy4->cd(j + 3);
    // histo_name = "Cats" + nptool::itoa(j + 1) + "_phy_";
    // histo_name += "STRY_MULT";
    // m_phy_hist[histo_name]->Draw();

    cEN_phy5->cd(j + 1);
    histo_name = "Cats" + nptool::itoa(j + 1) + "_phy_";
    histo_name += "PosX_vs_PosY";
    m_phy_hist[histo_name]->Draw();
  }

  cEN_phy6->cd();
  std::string histo_nameTP = "Cats_phy_TARGET_POS";
  m_phy_hist[histo_nameTP]->Draw();

  cEN_phy7->cd(1);
  std::string histo_nameTXZ = "Cats_phy_TRAJECTORY_XZ";
  m_phy_hist[histo_nameTXZ]->Draw("colz");

  cEN_phy7->cd(2);
  // cEN_phy8->cd();
  std::string histo_nameTYZ = "Cats_phy_TRAJECTORY_YZ";
  m_phy_hist[histo_nameTYZ]->Draw("colz");
  }}



  /********************************FILLING HISTOGRAMS************************************/

void CatsSpectra::FillRaw() {
  auto size = m_RawData->GetCATSMultX();
  if(size != 0 ){
    m_RawData->Dump();

  for (unsigned int i = 0; i < size; i++) {
    std::string cats_prefix = "Cats" + nptool::itoa(m_RawData->GetCATSDetX(i));
    auto hXQ = (TH2F *)(m_raw_hist[cats_prefix + "_raw_STRX_Q"]);
    hXQ->Fill(m_RawData->GetCATSStripX(i), m_RawData->GetCATSChargeX(i));
  }

  size = m_RawData->GetCATSMultY();
  for (unsigned int i = 0; i < size; i++) {
    std::string cats_prefix = "Cats" + nptool::itoa(m_RawData->GetCATSDetY(i));
    auto hYQ = (TH2F *)(m_raw_hist[cats_prefix + "_raw_STRY_Q"]);
    hYQ->Fill(m_RawData->GetCATSStripY(i), m_RawData->GetCATSChargeY(i));
  }
}}

/////////////////////////////////////////////////////////////////
void CatsSpectra::FillPhy() {
  auto sizex = m_PhysicsData->ChargeX.size();
  for (unsigned int i = 0; i < sizex; i++) {
    std::string cats_prefix =
        "Cats" + nptool::itoa(m_PhysicsData->DetNumber[i]);
    auto hXQPhy = (TH2F *)(m_phy_hist[cats_prefix + "_phy_STRX_Q"]);
    hXQPhy->Fill(m_PhysicsData->StripNumberX[i], m_PhysicsData->ChargeX[i]);
    auto hMULTX = (TH1F *)(m_phy_hist[cats_prefix + "_phy_STRX_MULT"]);
    if (m_PhysicsData->DetNumber[i] == 1) {
      hMULTX->Fill(m_PhysicsData->Mult1X[0]);
    }
    if (m_PhysicsData->DetNumber[i] == 2) {
      hMULTX->Fill(m_PhysicsData->Mult2X[0]);
    }
  }

  auto calsizex = m_PhysicsData->CalChargeX.size();
  std::map<unsigned short, std::vector<unsigned short>> CalStripX;
  unsigned int CalMultX[2] = {0, 0};
  for (unsigned int i = 0; i < calsizex; i++) {
    std::string cats_prefix =
        "Cats" + nptool::itoa(m_PhysicsData->CalDetNumberX[i]);
    auto hXQCal = (TH2F *)(m_phy_hist[cats_prefix + "_cal_STRX_Q"]);
    hXQCal->Fill(m_PhysicsData->CalStripNumberX[i],
                 m_PhysicsData->CalChargeX[i]);
    if (m_PhysicsData->CalDetNumberX[i] == 1) {
      CalStripX[0].push_back(m_PhysicsData->CalStripNumberX[i]);
      CalMultX[0]++;
    }
    if (m_PhysicsData->CalDetNumberX[i] == 2) {
      CalStripX[1].push_back(m_PhysicsData->CalStripNumberX[i]);
      CalMultX[1]++;
    }
  }
  for (unsigned int i = 0; i < 2; i++) {
    std::sort(CalStripX[i].begin(), CalStripX[i].end());
    bool is_consecutive =
        std::adjacent_find(CalStripX[i].begin(), CalStripX[i].end(),
                           [](int a, int b) { return b != a + 1; }) ==
        CalStripX[i].end();
    if (is_consecutive) {
      std::string cats_prefix = "Cats" + nptool::itoa(i + 1);
      auto hMULTXcal = (TH1F *)(m_phy_hist[cats_prefix + "_cal_STRX_MULT"]);
      hMULTXcal->Fill(CalMultX[i]);
    }
    // else {
    //   unsigned int size = CalStripX[i].size();
    //   std::cout << "///" << std::endl;
    //   for (unsigned int j = 0; j < size; j++) {
    //     std::cout << CalStripX[i][j] << std::endl;
    //   }
    // }
  }

  auto sizey = m_PhysicsData->ChargeY.size();
  for (unsigned int i = 0; i < sizey; i++) {
    std::string cats_prefix =
        "Cats" + nptool::itoa(m_PhysicsData->DetNumber[i]);
    auto hYQPhy = (TH2F *)(m_phy_hist[cats_prefix + "_phy_STRY_Q"]);
    hYQPhy->Fill(m_PhysicsData->StripNumberY[i], m_PhysicsData->ChargeY[i]);
    auto hMULTY = (TH1F *)(m_phy_hist[cats_prefix + "_phy_STRY_MULT"]);
    if (m_PhysicsData->DetNumber[i] == 1) {
      hMULTY->Fill(m_PhysicsData->Mult1Y[0]);
    }
    if (m_PhysicsData->DetNumber[i] == 2) {
      hMULTY->Fill(m_PhysicsData->Mult2Y[0]);
    }
  }

  auto calsizey = m_PhysicsData->CalChargeY.size();
  std::map<unsigned short, std::vector<unsigned short>> CalStripY;
  unsigned int CalMultY[2] = {0, 0};
  for (unsigned int i = 0; i < calsizey; i++) {
    std::string cats_prefix =
        "Cats" + nptool::itoa(m_PhysicsData->CalDetNumberY[i]);
    auto hYQCal = (TH2F *)(m_phy_hist[cats_prefix + "_cal_STRY_Q"]);
    hYQCal->Fill(m_PhysicsData->CalStripNumberY[i],
                 m_PhysicsData->CalChargeY[i]);
    if (m_PhysicsData->CalDetNumberY[i] == 1) {
      CalStripY[0].push_back(m_PhysicsData->CalStripNumberY[i]);
      CalMultY[0]++;
    }
    if (m_PhysicsData->CalDetNumberY[i] == 2) {
      CalStripY[1].push_back(m_PhysicsData->CalStripNumberY[i]);
      CalMultY[1]++;
    }
  }

  for (unsigned int i = 0; i < 2; i++) {
    std::sort(CalStripY[i].begin(), CalStripY[i].end());
    bool is_consecutive =
        std::adjacent_find(CalStripY[i].begin(), CalStripY[i].end(),
                           [](int a, int b) { return b != a + 1; }) ==
        CalStripY[i].end();
    if (is_consecutive) {
      std::string cats_prefix = "Cats" + nptool::itoa(i + 1);
      auto hMULTYcal = (TH1F *)(m_phy_hist[cats_prefix + "_cal_STRY_MULT"]);
      hMULTYcal->Fill(CalMultY[i]);
    }
  }

  for (unsigned int i = 0; i < sizex; i++) {
    std::string cats_prefix =
        "Cats" + nptool::itoa(m_PhysicsData->DetNumber[i]);
    auto hXYPhy = (TH2F *)(m_phy_hist[cats_prefix + "_phy_PosX_vs_PosY"]);
    hXYPhy->Fill(m_PhysicsData->PositionX[i], m_PhysicsData->PositionY[i]);
  }

  if (sizex > 0 && sizey > 0) {
    auto hTP = (TH2F *)(m_phy_hist["Cats_phy_TARGET_POS"]);
    hTP->Fill(m_PhysicsData->PositionOnTargetX,
              m_PhysicsData->PositionOnTargetY);

    auto hTXZ = (TH2F *)(m_phy_hist["Cats_phy_TRAJECTORY_XZ"]);
    auto hTYZ = (TH2F *)(m_phy_hist["Cats_phy_TRAJECTORY_YZ"]);

    for (unsigned int i = 0; i < hTXZ->GetNbinsX(); i++) {
      double z = hTXZ->GetXaxis()->GetBinCenter(i);
      double PositionOnTargetX;
      double PositionOnTargetY;
      double PositionZ0 = m_PhysicsData->PositionZ[0];
      double PositionZ1 = m_PhysicsData->PositionZ[1];

      if (m_PhysicsData->DetNumber[0] == 1) {
        double t = -(PositionZ1 - z) / (PositionZ1 - PositionZ0);
        PositionOnTargetX =
            m_PhysicsData->PositionX[1] +
            (m_PhysicsData->PositionX[1] - m_PhysicsData->PositionX[0]) * t;
        PositionOnTargetY =
            m_PhysicsData->PositionY[1] +
            (m_PhysicsData->PositionY[1] - m_PhysicsData->PositionY[0]) * t;
      } else if (m_PhysicsData->DetNumber[0] == 2) {
        double t = -(PositionZ0 - z) / (PositionZ0 - PositionZ1);
        PositionOnTargetX =
            m_PhysicsData->PositionX[0] +
            (m_PhysicsData->PositionX[0] - m_PhysicsData->PositionX[1]) * t;
        PositionOnTargetY =
            m_PhysicsData->PositionY[0] +
            (m_PhysicsData->PositionY[0] - m_PhysicsData->PositionY[1]) * t;
      } else {
        std::cout << "PROBLEM IN SPECTRA DETNUMBER IS WRONG!!! DetNumber[0] = "
                  << m_PhysicsData->DetNumber[0] << std::endl;
      }
      hTXZ->Fill(z, PositionOnTargetX);
      hTYZ->Fill(z, PositionOnTargetY);
    }
  }
  m_PhysicsData->Clear();
}
////////////////////////////////////////////////////////////////////////////////
void CatsSpectra::Clear() {
  for (auto h : m_raw_hist) h.second->Reset();
  for (auto h : m_phy_hist) h.second->Reset();
}

/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girard-alcindor@ijclab.in2p3.fr                          *
 *                                                                           *
 * Creation Date  : 08/03/24                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This class hold MUST2 Spectra definitions                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This class is heavily based on the nptool v3 MUST2 detector               *
 *                                                                           *
 *****************************************************************************/

#include "Must2Spectra.h"

#include "NPApplication.h"
#include "TDirectory.h"
#include "TROOT.h"

using namespace must2;
////////////////////////////////////////////////////////////////////////////////
Must2Spectra::Must2Spectra() {
  // Declare directories
  TDirectory* must2dir = gROOT->mkdir("must2");
  must2dir->cd();
  TDirectory* must2dir_raw = gROOT->mkdir("must2/Raw");
  TDirectory* must2dir_phy = gROOT->mkdir("must2/Phy");

  m_app = nptool::Application::GetApplication();
  // Set Pointers:
  m_detector =
      std::dynamic_pointer_cast<Must2Detector>(m_app->GetDetector("must2"));

  m_RawData = m_detector->m_RawData;
  m_RawData->Clear();
  m_PhysicsData = m_detector->m_PhysicsData;
  m_PhysicsData->Clear();

  unsigned int NTele =
      static_cast<unsigned int>(m_detector->GetNumberOfTelescopes());
  unsigned int NCol = NTele / 2 + NTele % 2;

  // Declare Raw Spectra
  if (m_app->HasFlag("--input-raw")) {
    must2dir_raw->cd();

    std::string cRawEStrip = "MM_raw_E_StripNbr";
    m_raw_canvas[cRawEStrip] =
        new TCanvas(cRawEStrip.c_str(), cRawEStrip.c_str());
    m_raw_canvas[cRawEStrip]->Divide(NTele, 2);

    std::string cRawTStrip = "MM_raw_T_StripNbr";
    m_raw_canvas[cRawTStrip] =
        new TCanvas(cRawTStrip.c_str(), cRawTStrip.c_str());
    m_raw_canvas[cRawTStrip]->Divide(NTele, 2);

    std::string cRawCsI = "MM_raw_E_CsINbr";
    m_raw_canvas[cRawCsI] = new TCanvas(cRawCsI.c_str(), cRawCsI.c_str());
    m_raw_canvas[cRawCsI]->Divide(NCol, 2);

    for (unsigned int i = 0; i < NTele; i++) {
      std::string telescope_prefix = "MM" + nptool::itoa(i + 1);

      // STRX_E_RAW
      std::string histo_nameXE = telescope_prefix + "_raw_XE_StripNbr";
      m_raw_hist[histo_nameXE] =
          new TH2F(histo_nameXE.c_str(), histo_nameXE.c_str(), 128, 1, 129,
                   1000, 8000, 16384);

      // STRY_E_RAW
      std::string histo_nameYE = telescope_prefix + "_raw_YE_StripNbr";
      m_raw_hist[histo_nameYE] =
          new TH2F(histo_nameYE.c_str(), histo_nameYE.c_str(), 128, 1, 129,
                   1000, 0, 9000);

      m_raw_canvas[cRawEStrip]->cd(i + 1);
      m_raw_hist[histo_nameXE]->Draw("col");
      m_raw_canvas[cRawEStrip]->cd(i + 1 + NTele);
      m_raw_hist[histo_nameYE]->Draw("col");

      // STRX_T_RAW
      std::string histo_nameXT = telescope_prefix + "_raw_XT_StripNbr";
      m_raw_hist[histo_nameXT] =
          new TH2F(histo_nameXT.c_str(), histo_nameXT.c_str(), 128, 1, 129,
                   1000, 0, 16384);

      // STRY_T_RAW
      std::string histo_nameYT = telescope_prefix + "_raw_YT_StripNbr";
      m_raw_hist[histo_nameYT] =
          new TH2F(histo_nameYT.c_str(), histo_nameYT.c_str(), 128, 1, 129,
                   1000, 0, 16384);

      m_raw_canvas[cRawTStrip]->cd(i + 1);
      m_raw_hist[histo_nameXT]->Draw("col");
      m_raw_canvas[cRawTStrip]->cd(i + 1 + NTele);
      m_raw_hist[histo_nameYT]->Draw("col");

      // CsI_E_RAW
      std::string histo_nameCsIE = telescope_prefix + "_raw_CsI_E";
      m_raw_hist[histo_nameCsIE] =
          new TH2F(histo_nameCsIE.c_str(), histo_nameCsIE.c_str(), 16, 1, 17,
                   1000, 0, 16384);
      m_raw_canvas[cRawCsI]->cd(i + 1);
      m_raw_hist[histo_nameCsIE]->Draw("col");
    }

    std::string cMMImpact = "MM_Impact_Matrix";
    m_phy_canvas[cMMImpact] = new TCanvas(cMMImpact.c_str(), cMMImpact.c_str());

    std::string cMMDEE = "MM_DE_E";
    m_phy_canvas[cMMDEE] = new TCanvas(cMMDEE.c_str(), cMMDEE.c_str());
    m_phy_canvas[cMMDEE]->Divide(NTele, 1);
    // Declare Phy Spectra
    //////////////////////////////////////////////////////////1pad
    if (m_app->HasFlag("--input-phy")) {
      must2dir_phy->cd();
      // X-Y Impact Matrix
      std::string histo_nameIM = "M2_phy_impact_matrix";
      m_phy_hist[histo_nameIM] =
          new TH2F(histo_nameIM.c_str(), histo_nameIM.c_str(), 500, -150, 150,
                   500, -150, 150);

      m_phy_canvas[cMMImpact]->cd();
      m_phy_hist[histo_nameIM]->Draw("col");

      std::string histo_nameM2SiCsI = "M2_phy_dE_E";
      m_phy_hist[histo_nameM2SiCsI] =
          new TH2F(histo_nameM2SiCsI.c_str(), histo_nameM2SiCsI.c_str(), 1000,
                   0, 500, 1000, 0, 100);

      // THETA-E
      std::string histo_nameTE = "M2_phy_kinematics";
      m_phy_hist[histo_nameTE] = new TH2F(
          histo_nameTE.c_str(), histo_nameTE.c_str(), 360, 0, 180, 500, 0, 50);

      //////////////////////////////////////////////////////////4pads
      for (int i = 0; i < NTele; i++) {
        std::string telescope_prefix = "MM" + nptool::itoa(i + 1);

        // STRX_E_PHY
        std::string histo_nameXEphy = telescope_prefix + "_phy_XE_StripNbr";
        m_phy_hist[histo_nameXEphy] =
            new TH2F(histo_nameXEphy.c_str(), histo_nameXEphy.c_str(), 128, 0,
                     128, 1000, 0, 20);

        // STRY_E_PHY
        std::string histo_nameYEphy = telescope_prefix + "_phy_YE_StripNbr";
        m_phy_hist[histo_nameYEphy] =
            new TH2F(histo_nameYEphy.c_str(), histo_nameYEphy.c_str(), 128, 0,
                     128, 1000, 0, 20);

        // SiE_SiT
        std::string histo_nameSiET = telescope_prefix + "_phy_dE_TOF";
        m_phy_hist[histo_nameSiET] =
            new TH2F(histo_nameSiET.c_str(), histo_nameSiET.c_str(), 1000, 0,
                     100, 1000, 300, 800);

        // X-Y Energy Correlation
        std::string histo_nameXYCOR = telescope_prefix + "_phy_XY_COR";
        m_phy_hist[histo_nameXYCOR] =
            new TH2F(histo_nameXYCOR.c_str(), histo_nameXYCOR.c_str(), 500, 0,
                     50, 500, 0, 50);

        // SiE_CsIE
        std::string histo_nameSiCsI = telescope_prefix + "_phy_dE_E";
        m_phy_hist[histo_nameSiCsI] =
            new TH2F(histo_nameSiCsI.c_str(), histo_nameSiCsI.c_str(), 1000, 0,
                     500, 1000, 0, 100);

        m_phy_canvas[cMMDEE]->cd(i + 1);
        m_phy_hist[histo_nameSiCsI]->Draw("col");
      }
    }
  }
  gROOT->cd();
}

////////////////////////////////////////////////////////////////////////////////
void Must2Spectra::FillRaw() {
  ////////////////////////////////////////////////////////////////////////////////
  // X Raw Spectra
  auto size = m_RawData->GetMMStripXEMult();
  for (unsigned int i = 0; i < size; i++) {
    std::string telescope_prefix =
        "MM" + nptool::itoa(m_RawData->GetMMStripXEDetectorNbr(i));
    auto hXE = (TH2F*)(m_raw_hist[telescope_prefix + "_raw_XE_StripNbr"]);
    hXE->Fill(m_RawData->GetMMStripXEStripNbr(i),
              m_RawData->GetMMStripXEEnergy(i));
  }
  size = m_RawData->GetMMStripXTMult();
  for (unsigned int i = 0; i < size; i++) {
    std::string telescope_prefix =
        "MM" + nptool::itoa(m_RawData->GetMMStripXTDetectorNbr(i));
    auto hXT = (TH2F*)(m_raw_hist[telescope_prefix + "_raw_XT_StripNbr"]);
    hXT->Fill(m_RawData->GetMMStripXTStripNbr(i),
              m_RawData->GetMMStripXTTime(i));
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Y Raw Spectra
  size = m_RawData->GetMMStripYEMult();
  for (unsigned int i = 0; i < size; i++) {
    std::string telescope_prefix =
        "MM" + nptool::itoa(m_RawData->GetMMStripYEDetectorNbr(i));
    auto hYE = (TH2F*)(m_raw_hist[telescope_prefix + "_raw_YE_StripNbr"]);
    hYE->Fill(m_RawData->GetMMStripYEStripNbr(i),
              m_RawData->GetMMStripYEEnergy(i));
  }
  size = m_RawData->GetMMStripYTMult();
  for (unsigned int i = 0; i < size; i++) {
    std::string telescope_prefix =
        "MM" + nptool::itoa(m_RawData->GetMMStripYTDetectorNbr(i));
    auto hYT = (TH2F*)(m_raw_hist[telescope_prefix + "_raw_YT_StripNbr"]);
    hYT->Fill(m_RawData->GetMMStripYTStripNbr(i),
              m_RawData->GetMMStripYTTime(i));
  }

  ////////////////////////////////////////////////////////////////////////////////
  // CsI Raw Spectra
  size = m_RawData->GetMMCsIEMult();
  for (unsigned int i = 0; i < size; i++) {
    std::string telescope_prefix =
        "MM" + nptool::itoa(m_RawData->GetMMCsIEDetectorNbr(i));
    auto hCsI = (TH2F*)(m_raw_hist[telescope_prefix + "_raw_CsI_E"]);
    hCsI->Fill(m_RawData->GetMMCsIECristalNbr(i),
               m_RawData->GetMMCsIEEnergy(i));
  }
}

//////////////////////////////////////////////////////////////////////////////////
void Must2Spectra::FillPhy() {
  auto size = m_PhysicsData->Si_E.size();
  for (unsigned int i = 0; i < size; i++) {
    std::string telescope_prefix =
        "MM" + nptool::itoa(m_PhysicsData->TelescopeNumber[i]);

    ////////////////////////////////////////////////////////////////////////////////
    // X Phy Spectra
    auto hXE = (TH2F*)(m_phy_hist[telescope_prefix + "_phy_XE_StripNbr"]);
    hXE->Fill(m_PhysicsData->Si_X[i], m_PhysicsData->Si_EX[i]);
    ////////////////////////////////////////////////////////////////////////////////
    // Y Phy Spectra
    auto hYE = (TH2F*)(m_phy_hist[telescope_prefix + "_phy_YE_StripNbr"]);
    hYE->Fill(m_PhysicsData->Si_Y[i], m_PhysicsData->Si_EY[i]);

    ////////////////////////////////////////////////////////////////////////////////
    // Si Phy Spectra
    auto hSiET = (TH2F*)(m_phy_hist[telescope_prefix + "_phy_dE_TOF"]);
    hSiET->Fill(m_PhysicsData->Si_E[i], m_PhysicsData->Si_T[i]);

    auto hSiCsI = (TH2F*)(m_phy_hist[telescope_prefix + "_phy_dE_E"]);
    hSiCsI->Fill(m_PhysicsData->CsI_E[i], m_PhysicsData->Si_E[i]);

    ////////////////////////////////////////////////////////////////////////////////
    // XY correlation
    auto hXYCOR = (TH2F*)(m_phy_hist[telescope_prefix + "_phy_XY_COR"]);
    hXYCOR->Fill(m_PhysicsData->Si_EX[i], m_PhysicsData->Si_EY[i]);

    ////////////////////////////////////////////////////////////////////////////////
    // Impact matrix
    auto hIM = (TH2F*)(m_phy_hist["M2_phy_impact_matrix"]);
    hIM->Fill(m_detector->GetPositionOfInteraction(i).x(),
              m_detector->GetPositionOfInteraction(i).y());

    auto hM2SiCsI = (TH2F*)(m_phy_hist["M2_phy_dE_E"]);
    hM2SiCsI->Fill(m_PhysicsData->CsI_E[i], m_PhysicsData->Si_E[i]);
    ////////////////////////////////////////////////////////////////////////////////
    // Interaction Position
    double Theta =
        m_detector->GetPositionOfInteraction(i).Angle(TVector3(0, 0, 1));
    Theta = Theta * (180 / 3.14);
    auto hTE = (TH2F*)(m_phy_hist["M2_phy_kinematics"]);
    hTE->Fill(Theta, m_PhysicsData->Si_E[i] + m_PhysicsData->CsI_E[i]);
  }
}

////////////////////////////////////////////////////////////////////////////////
void Must2Spectra::Clear() {
  for (auto h : m_raw_hist) h.second->Reset();
  for (auto h : m_phy_hist) h.second->Reset();
}

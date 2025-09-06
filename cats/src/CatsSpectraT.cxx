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

using namespace cats;
////////////////////////////////////////////////////////////////////////////////
CatsSpectra::CatsSpectra() {
// Set Pointers:
m_detector = std::dynamic_pointer_cast<CatsDetector>(
    nptool::Application::GetApplication()->GetDetector("cats"));

m_RawData     = m_detector->m_RawData;
m_RawData->Clear();
m_PhysicsData = m_detector->m_PhysicsData;
m_PhysicsData->Clear();
//   fStripsNumber = 28;



// Declare Raw Spectra
for (int i = 0; i < 2; i++) {
    std::string cats_prefix = "Cats" + nptool::itoa(i + 1);

    std::string histo_nameXQ = cats_prefix + "_raw_STRX_Q";
    m_raw_hist[histo_nameXQ] = new TH2F(histo_nameXQ.c_str(), histo_nameXQ.c_str(),
        28,1,29,512,0,16384);

    std::string histo_nameYQ = cats_prefix + "_raw_STRY_Q";
    m_raw_hist[histo_nameYQ] = new TH2F(histo_nameYQ.c_str(), histo_nameYQ.c_str(),
        28,1,29,512,0,16384);

    std::string histo_nameXMULT = cats_prefix + "_raw_STRX_MULT";
    m_raw_hist[histo_nameXMULT] = new TH1F(histo_nameXMULT.c_str(), histo_nameXMULT.c_str(),
        28,1,29);

    std::string histo_nameYMULT = cats_prefix + "_raw_STRY_MULT";
    m_raw_hist[histo_nameYMULT] = new TH1F(histo_nameYMULT.c_str(), histo_nameYMULT.c_str(),
        28,1,29);
}

    // Declare Phy Spectra
for (int i = 0; i < 2; i++) {
    std::string cats_prefix = "Cats" + nptool::itoa(i + 1);

    std::string histo_nameXQPhy = cats_prefix + "_phy_STRX_Q";
    m_phy_hist[histo_nameXQPhy] = new TH2F(histo_nameXQPhy.c_str(), histo_nameXQPhy.c_str(),
        28,1,29,512,0,16384);

    std::string histo_nameYQPhy = cats_prefix + "_phy_STRY_Q";
    m_phy_hist[histo_nameYQPhy] = new TH2F(histo_nameYQPhy.c_str(), histo_nameYQPhy.c_str(),
        28,1,29,512,0,16384);

    std::string histo_nameXMULTPhy = cats_prefix + "_phy_STRX_MULT";
    m_phy_hist[histo_nameXMULTPhy] = new TH1F(histo_nameXMULTPhy.c_str(), histo_nameXMULTPhy.c_str(),
        28,1,29);

    std::string histo_nameYMULTPhy = cats_prefix + "_phy_STRY_MULT";
    m_phy_hist[histo_nameYMULTPhy] = new TH1F(histo_nameYMULTPhy.c_str(), histo_nameYMULTPhy.c_str(),
        28,1,29);

    std::string histo_nameXYPhy = cats_prefix + "_phy_PosX_vs_PosY";
    m_phy_hist[histo_nameXYPhy] = new TH2F(histo_nameXYPhy.c_str(), histo_nameXYPhy.c_str(),
        28,1,29,100,0,1000);
}

// Build Raw Canvases
std::vector<TCanvas*> canvases;

// Define canvas titles
std::vector<std::string> canvasTitles = {
    "Cats raw STRX Q",
    "Cats raw STRY Q",
    "Cats raw STRX MULT",
    "Cats raw STRY MULT",
};

// Loop through canvas titles
for (size_t i = 0; i < canvasTitles.size(); ++i) {
    auto cEN_raw = new TCanvas(canvasTitles[i].c_str());
    canvases.push_back(cEN_raw);
    cEN_raw->Divide(2,1);

    // Loop through cats
    for (int j = 0; j < 2; ++j) {
    cEN_raw->cd(j + 1);

    // Define histogram name based on canvas title and telescope index
    std::string histo_name = "Cats" + nptool::itoa(j + 1) + "_raw_";
    if (i == 0)      histo_name += "STRX_Q";
    else if (i == 1) histo_name += "STRY_Q";
    else if (i == 2) histo_name += "STRX_MULT";
    else if (i == 3) histo_name += "STRY_MULT";

    m_raw_hist[histo_name]->Draw();
    }
} 

// Build Phy Canvases
std::vector<TCanvas*> canvases;

// Define canvas titles
std::vector<std::string> canvasTitlesPhy = {
    "Cats phy STRX Q",
    "Cats phy STRY Q",
    "Cats phy STRX MULT",
    "Cats phy STRY MULT",
    "Cats phy PosX vs PosY",
};

// Loop through canvas titles
for (size_t i = 0; i < canvasTitlesPhy.size(); ++i) {
    auto cEN_phy = new TCanvas(canvasTitlesPhy[i].c_str());
    canvases.push_back(cEN_phy);
    cEN_phy->Divide(2,1);

    // Loop through cats
    for (int j = 0; j < 2; ++j) {
    cEN_phy->cd(j + 1);

    // Define histogram name based on canvas title and telescope index
    std::string histo_name = "Cats" + nptool::itoa(j + 1) + "_phy_";
    if (i == 0)      histo_name += "STRX_Q";
    else if (i == 1) histo_name += "STRY_Q";
    else if (i == 2) histo_name += "STRX_MULT";
    else if (i == 3) histo_name += "STRY_MULT";
    else if (i == 4) histo_name += "PosX vs PosY";

    m_phy_hist[histo_name]->Draw();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void CatsSpectra::FillRaw() {
    auto size = m_RawData->GetCATSMultX();
    for (unsigned int i = 0; i < size; i++) {
        std::string cats_prefix = "Cats" + nptool::itoa(m_RawData->GetCATSDetX(i));
        auto hXQ = (TH2F*)(m_raw_hist[cats_prefix + "_raw_STRX_Q"]);
        hXQ->Fill(m_RawData->GetCATSStripX(i), m_RawData->GetCATSChargeX(i));
    }

    auto sizey = m_RawData->GetCATSMultY();
    for (unsigned int i = 0; i < sizey; i++) {
        std::string cats_prefix = "Cats" + nptool::itoa(m_RawData->GetCATSDetY(i));
        auto hYQ = (TH2F*)(m_raw_hist[cats_prefix + "_raw_STRY_Q"]);
        hYQ->Fill(m_RawData->GetCATSStripY(i), m_RawData->GetCATSChargeY(i));
    }

    // STRX_MULT
    int myMULT[fStripsNumber];
    for (unsigned int i = 0; i < m_detector->m_NumberOfCATS; i++) {
        myMULT[i] = 0;
    }
    for (unsigned int i = 0; i < m_RawData->GetCATSMultX(); i++) {
        myMULT[m_RawData->GetCATSDetX(i) - 1] += 1;
    }
    for (unsigned int i = 0; i < m_detector->m_NumberOfCATS; i++) {
        std::string cats_prefix = "Cats" + nptool::itoa(i+1);
        auto hXMULT = (TH1F*)(m_raw_hist[cats_prefix + "_raw_STRX_MULT"]);
        hXMULT->Fill(myMULT[i]);
    }

    // STRY_MULT
    for (unsigned int i = 0; i < m_detector->m_NumberOfCATS; i++) {
        myMULT[i] = 0;
    }
    for (unsigned int i = 0; i < m_RawData->GetCATSMultY(); i++) {
        myMULT[m_RawData->GetCATSDetY(i) - 1] += 1;
    }
    for (unsigned int i = 0; i < m_detector->m_NumberOfCATS; i++) {
        std::string cats_prefix = "Cats" + nptool::itoa(i+1);
        auto hYMULT = (TH1F*)(m_raw_hist[cats_prefix + "_raw_STRY_MULT"]);
        hYMULT->Fill(myMULT[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////
void CatsSpectra::FillPhy() {
    auto size = m_PhysicsData->ChargeX();
    for (unsigned int i = 0; i < size; i++) {
        std::string cats_prefix = "Cats" + nptool::itoa(m_PhysicsData->DetNumber[i]);
        auto hXQPhy = (TH2F*)(m_phy_hist[cats_prefix + "_phy_STRX_Q"]); 
        hXQPhy->Fill( m_PhysicsData->StripNumberX(i), m_PhysicsData->ChargeX(i));
    }

    auto sizey = m_PhysicsData->ChargeY();
    for (unsigned int i = 0; i < sizey; i++) {
        std::string cats_prefix = "Cats" + nptool::itoa(m_PhysicsData->DetNumber[i]);
        auto hYQPhy = (TH2F*)(m_phy_hist[cats_prefix + "_phy_STRY_Q"]);
        hYQPhy->Fill(m_PhysicsData->StripNumberY(i), m_PhysicsData->ChargeY(i));
    }
    
    auto sizexy = m_PhysicsData->PositionX();
    for (unsigned int i = 0; i < sizexy; i++) {
        std::string cats_prefix = "Cats" + nptool::itoa(m_PhysicsData->DetNumber[i]);
        auto hXYPhy = (TH2F*)(m_phy_hist[cats_prefix + "_phy_PosX_vs_PosY"]);
        hXYPhy->Fill(m_PhysicsData->PositionX(i), m_PhysicsData->PositionY(i));
    }

}

////////////////////////////////////////////////////////////////////////////////
void CatsSpectra::Clear() {
    for (auto h : m_raw_hist)
        h.second->Reset();
    for (auto h : m_phy_hist)
        h.second->Reset();
}

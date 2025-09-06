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
 *                                                                           *
 *****************************************************************************/
#include "TacSpectra.h"
#include "NPApplication.h"

using namespace tac;
////////////////////////////////////////////////////////////////////////////////
TacSpectra::TacSpectra() {
  auto app = nptool::Application::GetApplication();
  // Set Pointers:
  m_detector = std::dynamic_pointer_cast<TacDetector>(
      app->GetDetector("tac"));

  m_RawData     = m_detector->m_RawData;
  m_RawData->Clear();
  m_PhysicsData = m_detector->m_PhysicsData;
  m_PhysicsData->Clear();



  // Declare Raw Spectra
  if(app->HasFlag("--input-raw"))
  {

    for (auto map: m_detector->m_NumexoMap) {
      std::string hist_name = "TAC";
      m_raw_channels[hist_name].push_back(std::make_pair(map.first%1000,map.second));
      std::cout <<  map.first%1000 << " " << map.second <<  std::endl;
    }
    for (auto map: m_raw_channels) {
      std::string hist_name =  map.first;
      std::cout << "Hist name " << hist_name << std::endl;
      unsigned int channel_min = 0;
      unsigned int channel_max = map.second.size();
      m_raw_hist[map.first] = new TH2F(hist_name.c_str(), hist_name.c_str(),
         channel_max - channel_min +1, channel_min, channel_max+1, 65536, 0, 65536);
      unsigned int bin = 1;
      for (auto channel: map.second) {
      m_raw_hist[map.first]->GetXaxis()->SetBinLabel(bin,channel.second.c_str());
      m_bin_label[channel.second] = bin;
      std::cout << bin << " " << channel.second << std::endl;
      bin++;
      }  
    }
  }

/*
  // Build Raw Canvases
  std::vector<TCanvas*> canvases;

  auto cTAC_raw = new TCanvas("TACS");
  unsigned int c_Tac = 1;
  cTAC_raw->Divide(3,3);
  for (auto it: m_raw_hist) {

    cTAC_raw->cd(c_Tac);
    c_Tac++;

    it.second.second->Draw();
    }
    */
}

////////////////////////////////////////////////////////////////////////////////
void TacSpectra::FillPhy() {
}

////////////////////////////////////////////////////////////////////////////////
void TacSpectra::FillRaw() {
  auto size =  m_RawData->GetTAC_Mult();
  for (unsigned int i = 0; i < size; i++) {
    m_raw_hist["TAC"]->Fill(m_bin_label[m_RawData->GetTAC_Name(i)],m_RawData->GetTAC_Time(i));
  }
}

////////////////////////////////////////////////////////////////////////////////
void TacSpectra::Clear() {
  for (auto h : m_raw_hist)
    h.second->Reset();
  for (auto h : m_phy_hist)
    h.second.second->Reset();
}


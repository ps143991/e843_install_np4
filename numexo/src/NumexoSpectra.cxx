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
#include "NumexoSpectra.h"

#include "NPApplication.h"
#include "TDirectory.h"
#include "TROOT.h"

using namespace numexo;
////////////////////////////////////////////////////////////////////////////////
NumexoSpectra::NumexoSpectra() {
  // Declare directories
  TDirectory* numexodir = gROOT->mkdir("numexo");
  numexodir->cd();
  // TDirectory* numexodir_raw = gROOT->mkdir("numexo/Raw");
  // TDirectory* must2dir_phy = gROOT->mkdir("numexo/Phy");

  auto app = nptool::Application::GetApplication();
  // Set Pointers:
  m_detector =
      std::dynamic_pointer_cast<NumexoDetector>(app->GetDetector("numexo"));
  std::cout << "//////////////////////////////////////////////////////////"
            << std::endl;

  // Declare Raw Spectra
  if (app->HasFlag("--input-raw")) {
    for (auto map : m_detector->m_NumexoMap) {
      std::string hist_name = map.second;
      std::string cNumexo = hist_name;
      m_raw_canvas[cNumexo] = new TCanvas(cNumexo.c_str(), cNumexo.c_str());
      std::cout << "Hist name " << hist_name << std::endl;
      m_raw_hist[hist_name] =
          new TH1F(hist_name.c_str(), hist_name.c_str(), 16384, 0, 65536);
      m_raw_canvas[cNumexo]->cd();
      m_raw_hist[hist_name]->Draw();
    }
  }
  gROOT->cd();
}

////////////////////////////////////////////////////////////////////////////////
void NumexoSpectra::FillPhy() {}

////////////////////////////////////////////////////////////////////////////////
void NumexoSpectra::FillRaw() {
  for (auto map : m_detector->m_NumexoMap) {
    std::string hist_name = map.second;
    std::vector v_value = *(m_detector->GenericRawBranch[hist_name]);
    for (auto value : *(m_detector->GenericRawBranch[hist_name])) {
      if (value > 0) {
        m_raw_hist[hist_name]->Fill(value);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void NumexoSpectra::Clear() {
  for (auto h : m_raw_hist) h.second->Reset();
  for (auto h : m_phy_hist) h.second.second->Reset();
}

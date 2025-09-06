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
 * This class is heavily based on the nptool v3 EXOGAM detector              *
 *                                                                           *
 *****************************************************************************/
#include "ExogamSpectra.h"
#include "NPApplication.h"

using namespace exogam;
////////////////////////////////////////////////////////////////////////////////
ExogamSpectra::ExogamSpectra() {
  TDirectory* exodir = gROOT->mkdir("exogam");
  exodir->cd();
  auto app = nptool::Application::GetApplication();
  
  // Set Pointers --------------------------------------------------------------
  m_detector = std::dynamic_pointer_cast<ExogamDetector>(
      app->GetDetector("exogam"));

  m_RawData     = m_detector->m_RawData;
  m_RawData->Clear();

  m_PhysicsData = m_detector->m_PhysicsData;
  m_PhysicsData->Clear();

  // Initilise variables -------------------------------------------------------
  SetRefTS(0);  SetLastTS(0);
  unsigned int crystalmax =  0;
  unsigned int crystalmin = -1;
  std::string hist_name;
  std::string canv_name;

  // ---------------------------------------------------------------------------
  // Declare Raw Spectra -------------------------------------------------------

  for (auto it: m_detector->m_ExogamNumexoCrystalMap){
    if(it.second > crystalmax){ crystalmax = it.second; }
    if(it.second < crystalmin){ crystalmin = it.second; }
  }
  crystalmax += 1;

  int rawhist_range = 20000;//65536;
  int rawhist_binwd = 5;//65536;
  if(app->HasFlag("--input-raw"))
  {
    haveRaw=true;
    TDirectory* rawdir = gROOT->mkdir("exogam/raw");
    rawdir->cd();

    // Energy histograms -------------------------------------------------------
    hist_name = "Exo_2D_RawE";
    m_raw_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax, 
	rawhist_range/rawhist_binwd,0,rawhist_range);
    m_raw_hist_2D[hist_name]->GetXaxis()->SetTitle("Crystal");
    m_raw_hist_2D[hist_name]->GetYaxis()->SetTitle("Channel");

    hist_name = "Exo_2D_RawOuter";
    m_raw_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        (crystalmax-crystalmin)*10, crystalmin*10, crystalmax*10, 
	rawhist_range/rawhist_binwd,0,rawhist_range);
    m_raw_hist_2D[hist_name]->GetXaxis()->SetTitle("10*Crystal + Segment");
    m_raw_hist_2D[hist_name]->GetYaxis()->SetTitle("Channel");
    
    hist_name = "Exo_2D_RawE-CoreLG";
    m_raw_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax, 
	rawhist_range/rawhist_binwd,0,rawhist_range);
    m_raw_hist_2D[hist_name]->GetXaxis()->SetTitle("Crystal");
    m_raw_hist_2D[hist_name]->GetYaxis()->SetTitle("Channel");
    
    hist_name = "Exo_2D_RawE-CoreHG";
    m_raw_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax, 
	rawhist_range/rawhist_binwd,0,rawhist_range);
    m_raw_hist_2D[hist_name]->GetXaxis()->SetTitle("Crystal");
    m_raw_hist_2D[hist_name]->GetYaxis()->SetTitle("Channel");
    
    // Time histograms ---------------------------------------------------------
    hist_name = "Exo_2D_RawTDC";
    m_raw_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax, 
	//rawhist_range/rawhist_binwd,0,rawhist_range);
	16384,0,65536);

    hist_name = "Exo_1D_RawTDC";
    m_raw_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
	16384,0,65536);

    hist_name = "Exo_1D_TS";
    m_raw_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        1e4, 0, 1e12);
    m_raw_hist_1D[hist_name]->SetTitle("Event rate monitor (raw)");
    m_raw_hist_1D[hist_name]->GetXaxis()->SetTitle("Timestamp (reset every 1e12)");

    hist_name = "Exo_1D_TSDiff";
    m_raw_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        1.1e4, -1e4, 1e5);
    m_raw_hist_1D[hist_name]->SetTitle("Exo_1D_TSDiff (raw)");
    m_raw_hist_1D[hist_name]->GetXaxis()->SetTitle("TS(i) - TS(i-1)");

    hist_name = "Exo_1D_TSDiff_SameEvent";
    m_raw_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        1e4, -5e3, 5e3);
    m_raw_hist_1D[hist_name]->SetTitle("Exo_1D_TSDiff_SameEvent (raw)");
    m_raw_hist_1D[hist_name]->GetXaxis()->SetTitle("TS(i) - TS(i-1)");

    // Compton suppression histograms ------------------------------------------
    hist_name = "Exo_1D_RawBGO";
    m_raw_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        rawhist_range/rawhist_binwd,0,rawhist_range);
    m_raw_hist_1D[hist_name]->GetXaxis()->SetTitle("Channel");

    hist_name = "Exo_2D_RawBGO";
    m_raw_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax, 
	rawhist_range/rawhist_binwd,0,rawhist_range);
    m_raw_hist_2D[hist_name]->GetXaxis()->SetTitle("Associated crystal");
    m_raw_hist_2D[hist_name]->GetYaxis()->SetTitle("Channel");

    hist_name = "Exo_1D_RawCsI";
    m_raw_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        rawhist_range/rawhist_binwd,0,rawhist_range);
    m_raw_hist_1D[hist_name]->GetXaxis()->SetTitle("Channel");

    hist_name = "Exo_2D_RawCsI";
    m_raw_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax, 
	rawhist_range/rawhist_binwd,0,rawhist_range);
    m_raw_hist_2D[hist_name]->GetXaxis()->SetTitle("Associated crystal");
    m_raw_hist_2D[hist_name]->GetYaxis()->SetTitle("Channel");

    // Other -------------------------------------------------------------------
    hist_name = "Exo_1D_Pattern-Crystal";
    m_raw_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin+1, crystalmin, crystalmax+1);
    m_raw_hist_1D[hist_name]->SetTitle("Exo_1D_Pattern-Crystal (raw)");
    m_raw_hist_1D[hist_name]->GetXaxis()->SetTitle("Crystal");

    hist_name = "Exo_1D_Pattern-Segment";
    m_raw_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        (crystalmax-crystalmin)*10, crystalmin*10, crystalmax*10);
    m_raw_hist_1D[hist_name]->SetTitle("Exo_1D_Pattern-Segment (raw)");
    m_raw_hist_1D[hist_name]->GetXaxis()->SetTitle("10*Crystal + Segment");

    // 1D, Core E (LG) per crystal ---------------------------------------------
    int tmp_min = 0;
    int tmp_max = 20000;
    int tmp_bin = 10000;
    TDirectory* rawdirbin = gROOT->mkdir("exogam/raw/dump");
    rawdirbin->cd();
    hist_name = "Exo_V1D_E_";
    for(int f=4; f<64; f++){
      std::string id = std::to_string(f);
      if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
      std::string h = hist_name + id;
      m_raw_hist_V1D[h.c_str()] = new TH1F(h.c_str(), h.c_str(), tmp_bin,tmp_min,tmp_max);
    }
    // 1D, Core E (HG) per crystal ---------------------------------------------
    tmp_max = 6000;
    tmp_bin = 3000;
    hist_name = "Exo_V1D_EHG_";
    for(int f=4; f<64; f++){
      std::string id = std::to_string(f);
      if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
      std::string h = hist_name + id;
      m_raw_hist_V1D[h.c_str()] = new TH1F(h.c_str(), h.c_str(), tmp_bin,tmp_min,tmp_max);
    }
    // 1D, Outer per crystal ---------------------------------------------------
    tmp_max = 6000;
    tmp_bin = 3000;
    hist_name = "Exo_V1D_Outers_";
    for(int f=4; f<64; f++){
      std::string id = std::to_string(f);
      if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
      std::string hA = hist_name + id + "-1";
      std::string hB = hist_name + id + "-2";
      std::string hC = hist_name + id + "-3";
      std::string hD = hist_name + id + "-4";
      m_raw_hist_V1D[hA.c_str()] = new TH1F(hA.c_str(), hA.c_str(), tmp_bin,tmp_min,tmp_max);
      m_raw_hist_V1D[hB.c_str()] = new TH1F(hB.c_str(), hB.c_str(), tmp_bin,tmp_min,tmp_max);
      m_raw_hist_V1D[hC.c_str()] = new TH1F(hC.c_str(), hC.c_str(), tmp_bin,tmp_min,tmp_max);
      m_raw_hist_V1D[hD.c_str()] = new TH1F(hD.c_str(), hD.c_str(), tmp_bin,tmp_min,tmp_max);
    }
    rawdir->cd();

  }

  // ---------------------------------------------------------------------------
  // Declare Physics Spectra ---------------------------------------------------
  crystalmax=170;
  crystalmin=0;

  if(app->HasFlag("--input-phy"))
  {
    havePhy=true;
    TDirectory* phydir = gROOT->mkdir("exogam/phy");
    phydir->cd();

    // Energy histograms (no AddBack) ------------------------------------------
    hist_name = "Exo_2D_ECal";
    m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax, 2000,0,4000);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetTitle("10*Flange + Crystal");
    m_phy_hist_2D[hist_name]->GetYaxis()->SetTitle("Energy [keV]");

    hist_name = "Exo_1D_ECal";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        4000,0,8000);
    m_phy_hist_1D[hist_name]->GetXaxis()->SetTitle("Energy [keV]");

    // Energy histograms (with AddBack) ----------------------------------------
    hist_name = "Exo_2D_EAddB";
    m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax, 2000,0,4000);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetTitle("10*Flange + Crystal");
    m_phy_hist_2D[hist_name]->GetYaxis()->SetTitle("Energy [keV]");

    hist_name = "Exo_1D_EAddB";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        4000,0,8000);
    m_phy_hist_1D[hist_name]->GetXaxis()->SetTitle("Energy [keV]");

    // Time histograms ---------------------------------------------------------
    hist_name = "Exo_2D_TDC";
    m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax,// 2048,0,rawhist_range);
	16384,0,65536);

    hist_name = "Exo_1D_TDC";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
	16384,0,65536);

    hist_name = "Exo_1D_TS";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        1e4, 0, 1e12);
    m_phy_hist_1D[hist_name]->SetTitle("Event rate monitor (phy)");
    m_phy_hist_1D[hist_name]->GetXaxis()->SetTitle("Timestamp (reset every 1e12)");

    hist_name = "Exo_1D_TSDiff";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        1.1e4, -1e4, 1e5);
    m_phy_hist_1D[hist_name]->SetTitle("Exo_1D_TSDiff (phy)");
    m_phy_hist_1D[hist_name]->GetXaxis()->SetTitle("TS(i) - TS(i-1)");

    hist_name = "Exo_1D_TSDiff_SameEvent";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        1e4, -5e3, 5e3);
    m_phy_hist_1D[hist_name]->SetTitle("Exo_1D_TSDiff_SameEvent (phy)");
    m_phy_hist_1D[hist_name]->GetXaxis()->SetTitle("TS(i) - TS(i-1)");

    hist_name = "Exo_1D_TSDiff_VsClover";
    m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        20, -40, 40, 160, 0, 160);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetTitle("TS(i) - TS(i-1)");
    m_phy_hist_2D[hist_name]->GetYaxis()->SetTitle("Clover of first hit");

    // Gamma-gamma matrices ----------------------------------------------------
    hist_name = "Exo_2D_gg-ECal";
    m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        500, 50, 2050, 500, 50, 2050);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetTitle("Energy [keV]");
    m_phy_hist_2D[hist_name]->GetYaxis()->SetTitle("Energy [keV]");
     
    hist_name = "Exo_2D_gg-EAddB";
    m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        500, 50, 2050, 500, 50, 2050);
    m_phy_hist_2D[hist_name]->GetXaxis()->SetTitle("Energy [keV]");
    m_phy_hist_2D[hist_name]->GetYaxis()->SetTitle("Energy [keV]");

    // Multiplicity histograms -------------------------------------------------
    hist_name = "Exo_1D_Multiplicity";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        10, 0, 10);
    m_phy_hist_1D[hist_name]->GetXaxis()->SetTitle("Multiplicity");

    // Patterns ----------------------------------------------------------------
    hist_name = "Exo_1D_Pattern-Crystal";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        crystalmax-crystalmin, crystalmin, crystalmax);
    m_phy_hist_1D[hist_name]->SetTitle("Exo_1D_Pattern-Crystal (phy)");
    m_phy_hist_1D[hist_name]->GetXaxis()->SetTitle("10*Flange + Crystal");

    hist_name = "Exo_1D_Pattern-Segment";
    m_phy_hist_1D[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
        (crystalmax-crystalmin)*10, crystalmin*10, crystalmax*10);
    m_phy_hist_1D[hist_name]->SetTitle("Exo_1D_Pattern-Segment (phy)");
    m_phy_hist_1D[hist_name]->GetXaxis()->SetTitle("100*Flange + 10*Crystal + Segment");

    hist_name = "Exo_2D_Pattern-CryCry";
    m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        (crystalmax-crystalmin), crystalmin, crystalmax,
        (crystalmax-crystalmin), crystalmin, crystalmax);
    m_phy_hist_2D[hist_name]->SetTitle("Exo_1D_Pattern-CryCry (phy)");
    m_phy_hist_2D[hist_name]->GetXaxis()
	    ->SetTitle("10*Flange + Crystal"); //(first)");
    m_phy_hist_2D[hist_name]->GetYaxis()
	    ->SetTitle("10*Flange + Crystal"); //(second)");

    hist_name = "Exo_2D_Pattern-SegSeg";
    m_phy_hist_2D[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
        (crystalmax-crystalmin)*10, crystalmin*10, crystalmax*10,
        (crystalmax-crystalmin)*10, crystalmin*10, crystalmax*10);
    m_phy_hist_2D[hist_name]->SetTitle("Exo_1D_Pattern-SegSeg (phy)");
    m_phy_hist_2D[hist_name]->GetXaxis()
	    ->SetTitle("100*Flange + 10*Crystal + Segment"); //(first)");
    m_phy_hist_2D[hist_name]->GetYaxis()
	    ->SetTitle("100*Flange + 10*Crystal + Segment"); //(second)");
     
    // 1D, per crystal ---------------------------------------------------------
    int tmp_min = 0;
    int tmp_max = 4000;
    int tmp_bin = 2000;
    TDirectory* phydirbin = gROOT->mkdir("exogam/phy/dump");
    phydirbin->cd();
    hist_name = "Exo_V1D_ECal_";
    for(int f=2; f<17; f++){
      std::string id = std::to_string(f);
      if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
      std::string hA = hist_name + id + "A";
      std::string hB = hist_name + id + "B";
      std::string hC = hist_name + id + "C";
      std::string hD = hist_name + id + "D";
      m_phy_hist_V1D[hA.c_str()] = new TH1F(hA.c_str(), hA.c_str(), tmp_bin,tmp_min,tmp_max);
      m_phy_hist_V1D[hB.c_str()] = new TH1F(hB.c_str(), hB.c_str(), tmp_bin,tmp_min,tmp_max);
      m_phy_hist_V1D[hC.c_str()] = new TH1F(hC.c_str(), hC.c_str(), tmp_bin,tmp_min,tmp_max);
      m_phy_hist_V1D[hD.c_str()] = new TH1F(hD.c_str(), hD.c_str(), tmp_bin,tmp_min,tmp_max);
    }

    tmp_min = 0;
    tmp_max = 4000;
    tmp_bin = 2000;
    hist_name = "Exo_V1D_EAdd_";
    for(int f=2; f<17; f++){
      std::string id = std::to_string(f);
      if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
      std::string hA = hist_name + id + "A";
      std::string hB = hist_name + id + "B";
      std::string hC = hist_name + id + "C";
      std::string hD = hist_name + id + "D";
      m_phy_hist_V1D[hA.c_str()] = new TH1F(hA.c_str(), hA.c_str(), tmp_bin,tmp_min,tmp_max);
      m_phy_hist_V1D[hB.c_str()] = new TH1F(hB.c_str(), hB.c_str(), tmp_bin,tmp_min,tmp_max);
      m_phy_hist_V1D[hC.c_str()] = new TH1F(hC.c_str(), hC.c_str(), tmp_bin,tmp_min,tmp_max);
      m_phy_hist_V1D[hD.c_str()] = new TH1F(hD.c_str(), hD.c_str(), tmp_bin,tmp_min,tmp_max);
    }

    tmp_min = 0;
    tmp_max = 4000;
    tmp_bin = 2000;
    hist_name = "Exo_V1D_Seg_";
    for(int f=2; f<17; f++){
      std::string id = std::to_string(f);
      if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
      for(int seg=1; seg<5; seg++){
        std::string hA = hist_name + id + "A" + "-" + std::to_string(seg);
        std::string hB = hist_name + id + "B" + "-" + std::to_string(seg);
        std::string hC = hist_name + id + "C" + "-" + std::to_string(seg);
        std::string hD = hist_name + id + "D" + "-" + std::to_string(seg);
        m_phy_hist_V1D[hA.c_str()] = new TH1F(hA.c_str(), hA.c_str(), tmp_bin,tmp_min,tmp_max);
        m_phy_hist_V1D[hB.c_str()] = new TH1F(hB.c_str(), hB.c_str(), tmp_bin,tmp_min,tmp_max);
        m_phy_hist_V1D[hC.c_str()] = new TH1F(hC.c_str(), hC.c_str(), tmp_bin,tmp_min,tmp_max);
        m_phy_hist_V1D[hD.c_str()] = new TH1F(hD.c_str(), hD.c_str(), tmp_bin,tmp_min,tmp_max);
      }
    }
    phydir->cd();

  }

  // Canvases ----------------------------------------------------------------
  canv_name = "Exo_CompareAddback";
  m_canvas[canv_name] = new TCanvas(canv_name.c_str(), canv_name.c_str());
  m_canvas[canv_name]->Divide(2,2);
  m_canvas[canv_name]->cd(1);
    if(havePhy){m_phy_hist_1D["Exo_1D_ECal"]->Draw();}
  m_canvas[canv_name]->cd(2); m_canvas[canv_name]->cd(2)->SetLogz();
    if(havePhy){m_phy_hist_2D["Exo_2D_gg-ECal"]->Draw();}
  m_canvas[canv_name]->cd(3);
    if(havePhy){m_phy_hist_1D["Exo_1D_EAddB"]->Draw();}
  m_canvas[canv_name]->cd(4); m_canvas[canv_name]->cd(4)->SetLogz();
    if(havePhy){m_phy_hist_2D["Exo_2D_gg-EAddB"]->Draw();}

  canv_name = "Exo_RatesAndCoincidences";
  m_canvas[canv_name] = new TCanvas(canv_name.c_str(), canv_name.c_str());
  m_canvas[canv_name]->Divide(2,2);
  m_canvas[canv_name]->cd(1);
    if(haveRaw){m_raw_hist_1D["Exo_1D_TS"]->Draw();}
  m_canvas[canv_name]->cd(2); m_canvas[canv_name]->cd(2)->SetLogy();
    if(haveRaw){m_raw_hist_1D["Exo_1D_TSDiff"]->Draw();}
  m_canvas[canv_name]->cd(3);
    if(havePhy){m_phy_hist_1D["Exo_1D_Pattern-Segment"]->Draw();}
  m_canvas[canv_name]->cd(4); m_canvas[canv_name]->cd(4)->SetLogz();
    if(havePhy){m_phy_hist_2D["Exo_2D_Pattern-SegSeg"]->Draw("colz");}
  
  canv_name = "Exo_EnergyAndTDCs";
  m_canvas[canv_name] = new TCanvas(canv_name.c_str(), canv_name.c_str());
  m_canvas[canv_name]->Divide(2,2);
  m_canvas[canv_name]->cd(1); m_canvas[canv_name]->cd(1)->SetLogz();
    if(haveRaw){m_raw_hist_2D["Exo_2D_RawE"]->Draw();}
  m_canvas[canv_name]->cd(2); m_canvas[canv_name]->cd(2)->SetLogz();
    if(havePhy){m_phy_hist_2D["Exo_2D_ECal"]->Draw();}
  m_canvas[canv_name]->cd(3);
    if(haveRaw){m_raw_hist_2D["Exo_2D_RawTDC"]->Draw();}
  m_canvas[canv_name]->cd(4);
    if(havePhy){m_phy_hist_2D["Exo_2D_TDC"]->Draw();}

//  canv_name = "Exo_CoreEnergyCal1";
//  hist_name = "Exo_V1D_ECal_";
//  m_canvas[canv_name] = new TCanvas(canv_name.c_str(), canv_name.c_str());
//  m_canvas[canv_name]->Divide(4,5);
    int tmp_cnv = 1;
//  for(int f=2; f<7; f++){
//    std::string id = std::to_string(f);
//    if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
//    std::string hA = hist_name + id + "A";
//    std::string hB = hist_name + id + "B";
//    std::string hC = hist_name + id + "C";
//    std::string hD = hist_name + id + "D";
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hA.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hB.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hC.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hD.c_str()]->Draw();
//    tmp_cnv +=1;
//  }
//
//  canv_name = "Exo_CoreEnergyCal2";
//  hist_name = "Exo_V1D_ECal_";
//  m_canvas[canv_name] = new TCanvas(canv_name.c_str(), canv_name.c_str());
//  m_canvas[canv_name]->Divide(4,5);
//  tmp_cnv = 1;
//  for(int f=7; f<12; f++){
//    std::string id = std::to_string(f);
//    if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
//    std::string hA = hist_name + id + "A";
//    std::string hB = hist_name + id + "B";
//    std::string hC = hist_name + id + "C";
//    std::string hD = hist_name + id + "D";
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hA.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hB.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hC.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hD.c_str()]->Draw();
//    tmp_cnv +=1;
//  }
//
//  canv_name = "Exo_CoreEnergyCal3";
//  hist_name = "Exo_V1D_ECal_";
//  m_canvas[canv_name] = new TCanvas(canv_name.c_str(), canv_name.c_str());
//  m_canvas[canv_name]->Divide(4,5);
//  tmp_cnv = 1;
//  for(int f=12; f<17; f++){
//    std::string id = std::to_string(f);
//    if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
//    std::string hA = hist_name + id + "A";
//    std::string hB = hist_name + id + "B";
//    std::string hC = hist_name + id + "C";
//    std::string hD = hist_name + id + "D";
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hA.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hB.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hC.c_str()]->Draw();
//    tmp_cnv +=1;
//    m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hD.c_str()]->Draw();
//    tmp_cnv +=1;
//  }
//
  // Diagnostic, can remove for shifters
  if(haveRaw){
    for(int f=2; f<14; f++){
      canv_name = "Exo_Raw_Flange" + std::to_string(f);
      m_canvas[canv_name] = new TCanvas(canv_name.c_str(), canv_name.c_str());
      m_canvas[canv_name]->Divide(6,4);

      tmp_cnv = 1;
      for(int c=f*4; c<f*4+4; c++){
        std::string id = std::to_string(c);
        if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend

        hist_name = "Exo_V1D_E_" + id;
        m_canvas[canv_name]->cd(tmp_cnv); m_raw_hist_V1D[hist_name.c_str()]->Draw();
        tmp_cnv +=1;

        hist_name = "Exo_V1D_EHG_" + id;
        m_canvas[canv_name]->cd(tmp_cnv); m_raw_hist_V1D[hist_name.c_str()]->Draw();
        tmp_cnv +=1;

        for(int s=1; s<5; s++){
          hist_name = "Exo_V1D_Outers_" + id + "-" + std::to_string(s);
          m_canvas[canv_name]->cd(tmp_cnv); m_raw_hist_V1D[hist_name.c_str()]->Draw();
          tmp_cnv +=1;
        }
      }
    }
  }

  //// Diagnostic, can remove for shifters
  if(havePhy){
    for(int f=2; f<14; f++){
      std::string id = std::to_string(f);
      if(id.size() < 2) id = std::string(2-id.size(),'0')+id; // fast prepend
      canv_name = "Exo_Phy_Flange" + id;
      m_canvas[canv_name] = new TCanvas(canv_name.c_str(), canv_name.c_str());
      m_canvas[canv_name]->Divide(6,4);

      tmp_cnv = 1;
      static char cry[] = {"ABCD"};
      for(int c=0; c<4; c++){
        hist_name = "Exo_V1D_ECal_" + id + cry[c];
        m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hist_name.c_str()]->Draw();
        tmp_cnv +=1;

        hist_name = "Exo_V1D_EAdd_" + id + cry[c];
        m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hist_name.c_str()]->Draw();
        tmp_cnv +=1;

        for(int s=1; s<5; s++){
          hist_name = "Exo_V1D_Seg_" + id + cry[c] + "-" + std::to_string(s);
          m_canvas[canv_name]->cd(tmp_cnv); m_phy_hist_V1D[hist_name.c_str()]->Draw();
          tmp_cnv +=1;
        }
      }
    }
  }



  gROOT->cd();
}

////////////////////////////////////////////////////////////////////////////////
void ExogamSpectra::FillPhy() {
  auto size = m_PhysicsData->Exo_E.size();
  std::string hist_name;

  // Multiplicity histograms ---------------------------------------------------
  hist_name = "Exo_1D_Multiplicity";
  m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_E.size());

  // Singles -------------------------------------------------------------------
  for(unsigned int i = 0 ; i < size ; i++){

    // If timestamp beyond Exo_TS range, reset ---------------------------------
    if(m_PhysicsData->Exo_TS[i] > GetRefTS() + 1e12){
      SetRefTS(m_PhysicsData->Exo_TS[i]);
      m_phy_hist_1D["Exo_1D_TS"]->Reset();
    }

    // 2D, versus crystal ------------------------------------------------------
    hist_name = "Exo_2D_ECal";
    m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*10
        	                                  +m_PhysicsData->Exo_CrystalN[i],
        			   m_PhysicsData->Exo_E[i]);

    hist_name = "Exo_2D_EAddB";
    m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*10
        	                                  +m_PhysicsData->Exo_CrystalN[i],
        	                   m_PhysicsData->Exo_EAddBack[i]);

    // 1D ----------------------------------------------------------------------
    hist_name = "Exo_1D_ECal";
    m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_E[i]);

    hist_name = "Exo_1D_EAddB";
    m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_EAddBack[i]);

    // 1D, per crystal ---------------------------------------------------------
    std::string id = std::to_string(m_PhysicsData->Exo_FlangeN[i]);
    if(id.size() < 2) id = std::string(2 - id.size(), '0') + id; // fast prepend
    if(m_PhysicsData->Exo_CrystalN[i]==0) id = id + "A";
    if(m_PhysicsData->Exo_CrystalN[i]==1) id = id + "B";
    if(m_PhysicsData->Exo_CrystalN[i]==2) id = id + "C";
    if(m_PhysicsData->Exo_CrystalN[i]==3) id = id + "D";
    hist_name = "Exo_V1D_ECal_" + id;
    m_phy_hist_V1D[hist_name]->Fill(m_PhysicsData->Exo_E[i]);
   
    hist_name = "Exo_V1D_EAdd_" + id;
    m_phy_hist_V1D[hist_name]->Fill(m_PhysicsData->Exo_EAddBack[i]);
   
    if(m_PhysicsData->Exo_Outer1[i]>0){
      hist_name = "Exo_V1D_Seg_" + id + "-1";
      m_phy_hist_V1D[hist_name]->Fill(m_PhysicsData->Exo_Outer1[i]);
    }
    if(m_PhysicsData->Exo_Outer2[i]>0){
      hist_name = "Exo_V1D_Seg_" + id + "-2";
      m_phy_hist_V1D[hist_name]->Fill(m_PhysicsData->Exo_Outer2[i]);
    }
    if(m_PhysicsData->Exo_Outer3[i]>0){
      hist_name = "Exo_V1D_Seg_" + id + "-3";
      m_phy_hist_V1D[hist_name]->Fill(m_PhysicsData->Exo_Outer3[i]);
    }
    if(m_PhysicsData->Exo_Outer4[i]>0){
      hist_name = "Exo_V1D_Seg_" + id + "-4";
      m_phy_hist_V1D[hist_name]->Fill(m_PhysicsData->Exo_Outer4[i]);
    }

    // Time histograms ---------------------------------------------------------
    hist_name = "Exo_2D_TDC";
    m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*10
        	                                  +m_PhysicsData->Exo_CrystalN[i],
        	                   m_PhysicsData->Exo_TDC[i]);
    hist_name = "Exo_1D_TDC";
    m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_TDC[i]);

    hist_name = "Exo_1D_TS";
    m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_TS[i]-GetRefTS());

    hist_name = "Exo_1D_TSDiff";
    m_phy_hist_1D[hist_name]->Fill(abs((long long) m_PhysicsData->Exo_TS[i]
        	                    -(long long) GetLastTS()));

    // Patterns ----------------------------------------------------------------
    hist_name = "Exo_1D_Pattern-Crystal";
    m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*10
        	                                  +m_PhysicsData->Exo_CrystalN[i]);

    hist_name = "Exo_1D_Pattern-Segment";
    if(m_PhysicsData->Exo_Outer1[i]>0)
      m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*100
        	                                +m_PhysicsData->Exo_CrystalN[i]*10
        	                                +1);
    if(m_PhysicsData->Exo_Outer2[i]>0)
      m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*100
        	                                +m_PhysicsData->Exo_CrystalN[i]*10
        	                                +2);
    if(m_PhysicsData->Exo_Outer3[i]>0)
      m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*100
        	                                +m_PhysicsData->Exo_CrystalN[i]*10
        	                                +3);
    if(m_PhysicsData->Exo_Outer4[i]>0)
      m_phy_hist_1D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*100
        	                                +m_PhysicsData->Exo_CrystalN[i]*10
        	                                +4);

    // Finally, retain timestamp for next loop
    SetLastTS(m_PhysicsData->Exo_TS[i]);
  }

  // Simple coincidences -------------------------------------------------------
  if(size>1){
    for(unsigned int i = 0 ; i < size-1 ; i++){
      //if(m_PhysicsData->Exo_E[i]>0){
      if(m_PhysicsData->Exo_E[i]>0 && m_PhysicsData->Exo_E[i+1]>0){
        // idea is that, if two exos in same merge frame, ts diff should be zero
        hist_name = "Exo_1D_TSDiff_SameEvent";
        m_phy_hist_1D[hist_name]->Fill((long long) m_PhysicsData->Exo_TS[i] 
          	                      -(long long) m_PhysicsData->Exo_TS[i+1]);


        hist_name = "Exo_1D_TSDiff_VsClover";
        m_phy_hist_2D[hist_name]->Fill((long long) m_PhysicsData->Exo_TS[i] 
          	                      -(long long) m_PhysicsData->Exo_TS[i+1],
				      m_PhysicsData->Exo_FlangeN[i]*10
                                                   +m_PhysicsData->Exo_CrystalN[i]
				      );

	// if in same flange
	//if(m_PhysicsData->Exo_FlangeN[i]==m_PhysicsData->Exo_FlangeN[i+1]){
	
	// if in coincidence peak
	if(abs((long long) m_PhysicsData->Exo_TS[i]
	     - (long long) m_PhysicsData->Exo_TS[i+1])<5){
	
	  
	  unsigned int segfirst, segsecond;
          if(m_PhysicsData->Exo_Outer1[i]>0){segfirst = 1;}
	  if(m_PhysicsData->Exo_Outer2[i]>0){segfirst = 2;}
          if(m_PhysicsData->Exo_Outer3[i]>0){segfirst = 3;}
          if(m_PhysicsData->Exo_Outer4[i]>0){segfirst = 4;}
          if(m_PhysicsData->Exo_Outer1[i+1]>0){segsecond = 1;}
	  if(m_PhysicsData->Exo_Outer2[i+1]>0){segsecond = 2;}
          if(m_PhysicsData->Exo_Outer3[i+1]>0){segsecond = 3;}
          if(m_PhysicsData->Exo_Outer4[i+1]>0){segsecond = 4;}

          hist_name = "Exo_2D_Pattern-SegSeg";
          m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*100
                                              +m_PhysicsData->Exo_CrystalN[i]*10
	  				      +segfirst,
                                         m_PhysicsData->Exo_FlangeN[i+1]*100
                                              +m_PhysicsData->Exo_CrystalN[i+1]*10
					      +segsecond);
	  // mirror on axis because frame in 'seg number' order, not 'time' order
          m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i+1]*100
                                              +m_PhysicsData->Exo_CrystalN[i+1]*10
	  				      +segsecond,
                                         m_PhysicsData->Exo_FlangeN[i]*100
                                              +m_PhysicsData->Exo_CrystalN[i]*10
					      +segfirst);



          hist_name = "Exo_2D_Pattern-CryCry";
          m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i]*10
                                              +m_PhysicsData->Exo_CrystalN[i],
                                         m_PhysicsData->Exo_FlangeN[i+1]*10
                                              +m_PhysicsData->Exo_CrystalN[i+1]);
	  // mirror on axis because frame
          m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_FlangeN[i+1]*10
                                              +m_PhysicsData->Exo_CrystalN[i+1],
                                         m_PhysicsData->Exo_FlangeN[i]*10
                                              +m_PhysicsData->Exo_CrystalN[i]);

          hist_name = "Exo_2D_gg-ECal";
          m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_E[i],
            	                         m_PhysicsData->Exo_E[i+1]);
          
          hist_name = "Exo_2D_gg-EAddB";
          m_phy_hist_2D[hist_name]->Fill(m_PhysicsData->Exo_EAddBack[i],
            	                         m_PhysicsData->Exo_EAddBack[i+1]);


	}
	//}
      }
    }
  }

}

////////////////////////////////////////////////////////////////////////////////
void ExogamSpectra::FillRaw() {
  auto size =  m_RawData->GetExoMult();
  std::string hist_name;

  // Singles -------------------------------------------------------------------
  for (unsigned int i = 0; i < size; i++) {

    // If timestamp beyond Exo_TS range, reset ---------------------------------
    if(m_RawData->GetExoTS(i) > GetRefTS() + 1e12){
      SetRefTS(m_RawData->GetExoTS(i));
      m_raw_hist_1D["Exo_1D_TS"]->Reset();
    }

    // Energy histograms -------------------------------------------------------
    hist_name = "Exo_2D_RawE";
    m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i),
		                   m_RawData->GetExoE(i));

    hist_name = "Exo_2D_RawOuter";
    if(m_RawData->GetExoOuter1(i)>0)
      m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i)*10+1,
		                     m_RawData->GetExoOuter1(i));
    if(m_RawData->GetExoOuter2(i)>0)
      m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i)*10+2,
		                     m_RawData->GetExoOuter2(i));
    if(m_RawData->GetExoOuter3(i)>0)
      m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i)*10+3,
		                     m_RawData->GetExoOuter3(i));
    if(m_RawData->GetExoOuter4(i)>0)
      m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i)*10+4,
		                     m_RawData->GetExoOuter4(i));

    hist_name = "Exo_2D_RawE-CoreLG";
    m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i),
                                   m_RawData->GetExoE(i));

    hist_name = "Exo_2D_RawE-CoreHG";
    m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i),
                                   m_RawData->GetExoEHG(i));

    // 1D, per crystal ---------------------------------------------------------
    std::string id = std::to_string(m_RawData->GetExoCrystal(i));
    if(id.size() < 2) id = std::string(2 - id.size(), '0') + id; // fast prepend

    hist_name = "Exo_V1D_E_" + id;
    m_raw_hist_V1D[hist_name]->Fill(m_RawData->GetExoE(i));

    hist_name = "Exo_V1D_EHG_" + id;
    m_raw_hist_V1D[hist_name]->Fill(m_RawData->GetExoEHG(i));

    if(m_RawData->GetExoOuter1(i)>0){
      hist_name = "Exo_V1D_Outers_" + id + "-1";
      m_raw_hist_V1D[hist_name]->Fill(m_RawData->GetExoOuter1(i));
    }
    if(m_RawData->GetExoOuter2(i)>0){
      hist_name = "Exo_V1D_Outers_" + id + "-2";
      m_raw_hist_V1D[hist_name]->Fill(m_RawData->GetExoOuter2(i));
    }
    if(m_RawData->GetExoOuter3(i)>0){
      hist_name = "Exo_V1D_Outers_" + id + "-3";
      m_raw_hist_V1D[hist_name]->Fill(m_RawData->GetExoOuter3(i));
    }
    if(m_RawData->GetExoOuter4(i)>0){
      hist_name = "Exo_V1D_Outers_" + id + "-4";
      m_raw_hist_V1D[hist_name]->Fill(m_RawData->GetExoOuter4(i));
    }

    // Time histograms ---------------------------------------------------------
    hist_name = "Exo_2D_RawTDC";
    m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i),
		                   m_RawData->GetExoTDC(i));

    hist_name = "Exo_1D_RawTDC";
    m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoTDC(i));

    hist_name = "Exo_1D_TS";
    m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoTS(i)-GetRefTS());

    hist_name = "Exo_1D_TSDiff";
    m_raw_hist_1D[hist_name]->Fill(abs((long long) m_RawData->GetExoTS(i)
		                    -(long long) GetLastTS()));

    // Compton suppression histograms ------------------------------------------
    hist_name = "Exo_1D_RawBGO";
    m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoBGO(i));

    hist_name = "Exo_2D_RawBGO";
    m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i),
		                   m_RawData->GetExoBGO(i));

    hist_name = "Exo_1D_RawCsI";
    m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoCsI(i));
    
    hist_name = "Exo_2D_RawCsI";
    m_raw_hist_2D[hist_name]->Fill(m_RawData->GetExoCrystal(i),
		                   m_RawData->GetExoCsI(i));

    // Other -------------------------------------------------------------------
    hist_name = "Exo_1D_Pattern-Crystal";
    m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoCrystal(i));

    hist_name = "Exo_1D_Pattern-Segment";
    if(m_RawData->GetExoOuter1(i)>0)
      m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoCrystal(i)*10+1);
    if(m_RawData->GetExoOuter2(i)>0)
      m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoCrystal(i)*10+2);
    if(m_RawData->GetExoOuter3(i)>0)
      m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoCrystal(i)*10+3);
    if(m_RawData->GetExoOuter4(i)>0)
      m_raw_hist_1D[hist_name]->Fill(m_RawData->GetExoCrystal(i)*10+4);

    // Finally, retain timestamp for next loop
    SetLastTS(m_RawData->GetExoTS(i));
  }

  // Simple coincidences -------------------------------------------------------
  if(size>1){
    for(unsigned int i = 0 ; i < size-1 ; i++){
      hist_name = "Exo_1D_TSDiff_SameEvent";
      m_raw_hist_1D[hist_name]->Fill((long long) m_RawData->GetExoTS(i) 
		                      -(long long) m_RawData->GetExoTS(i+1));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamSpectra::Clear() {
  for (auto h : m_raw_hist_1D)
    h.second->Reset();
  for (auto h : m_raw_hist_2D)
    h.second->Reset();
  for (auto h : m_raw_hist_V1D)
    h.second->Reset();
  for (auto h : m_phy_hist_1D)
    h.second->Reset();
  for (auto h : m_phy_hist_2D)
    h.second->Reset();
  for (auto h : m_phy_hist_V1D)
    h.second->Reset();
}


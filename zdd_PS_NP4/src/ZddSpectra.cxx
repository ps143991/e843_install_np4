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
 * This class is heavily based on the nptool v3 ZDD detector                 *
 *                                                                           *
 *****************************************************************************/
#include "ZddSpectra.h"
#include "NPApplication.h"

#include "TROOT.h"
#include "TDirectory.h"

using namespace zdd;

////////////////////////////////////////////////////////////////////////////////
ZddSpectra::ZddSpectra() {
  // Declare directories
  TDirectory* zdddir = gROOT->mkdir("zdd");
  zdddir->cd();
  TDirectory* zdddir_raw = gROOT->mkdir("zdd/Raw");
  // TDirectory* zdddir_phy = gROOT->mkdir("zdd/Phy");

  auto app = nptool::Application::GetApplication();
  // Set Pointers:
  m_detector = std::dynamic_pointer_cast<ZddDetector>(
      app->GetDetector("zdd"));

  m_RawData = m_detector->m_RawData;
  m_RawData->Clear();

  m_PhysicsData = m_detector->m_PhysicsData;
  m_PhysicsData->Clear();

  // Declare Raw Spectra
  if(app->HasFlag("--input-raw"))
  {
    zdddir_raw->cd();
    for (auto map: m_detector->m_NumexoMap) {
      std::string hist_name = "ZDD_" + map.second;
      m_raw_channels[hist_name].push_back(map.first%1000);
    }
    for (auto map: m_raw_channels) {
      std::string hist_name =  map.first;
      std::cout << "Hist name " << hist_name << std::endl;
      unsigned int channel_min = *std::min_element(map.second.begin(),map.second.end());
      unsigned int channel_max = *std::max_element(map.second.begin(),map.second.end());
      m_raw_hist[map.first] = new TH2F(hist_name.c_str(), hist_name.c_str(),
          channel_max - channel_min +1, channel_min, channel_max+1, 65536/4, 0, 65536);
    }
    /*std::string hist_name = "ZDD_PL_LvsR";
      m_raw_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(), 1024, 0, 65536, 1024, 0, 65536);
      std::cout << "Hist name " << hist_name << std::endl;*/


    for (int i = 1; i <= 5; ++i) {
      std::string hist_name = "ZDD_PL_" + std::to_string(i) + "_LvsR";
      m_raw_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(), 1024, 0, 65536, 1024, 0, 65536);
      std::cout << "Hist name " << hist_name << std::endl;
    }
    m_raw_hist["drift_time"] = new TH2F("drift_time", "PL2_TS_subtracted_DCTS",
        4, 0, 4, 2000, -1000, 1000);
    m_raw_hist["drift_dist"] = new TH2F("drift_dist", "drift_length",
        4, 0, 4, 2000, -1000, 1000);


    std::string hist_name = "ZDD_EXO_T";
    m_raw_hist[hist_name] = new TH2F(
          hist_name.c_str(), hist_name.c_str(), 4,
          0, 4, 65536 / 4, 0, 65536);

  }
  /*
  // Build Raw Canvases
  std::vector<TCanvas*> canvases;

  auto cIC_raw = new TCanvas("ZDD raw IC");
  cIC_raw->Divide(3,2);
  auto cPL_raw = new TCanvas("ZDD raw PL");
  cPL_raw->Divide(5,2);
  auto cDC_raw = new TCanvas("ZDD raw DC");
  cDC_raw->Divide(2,2);
  auto cEXO_raw = new TCanvas("ZDD raw EXO");
  cEXO_raw->Divide(2,2);
  unsigned int c_IC = 1;
  unsigned int c_DC = 1;
  unsigned int c_PL = 1;
  unsigned int c_EXO = 1;
  // Loop through cristals
  for (auto det: m_raw_hist) {
  if(det.first.find("IC") != std::string::npos){
  for (auto hist: det.second) {
  cIC_raw->cd(c_IC);
  c_IC++;
  hist.second->Draw();
  }
  }
  else if(det.first.find("DC") != std::string::npos){
  for (auto hist: det.second) {
  cDC_raw->cd(c_DC);
  c_DC++;
  hist.second->Draw();
  }
  }
  else if(det.first.find("PL") != std::string::npos){
  for (auto hist: det.second) {
  cPL_raw->cd(c_PL);
  c_PL++;
  hist.second->Draw();
  }
  }
  else if(det.first.find("EXO") != std::string::npos){
  for (auto hist: det.second) {
  cEXO_raw->cd(c_EXO);
  c_EXO++;
  hist.second->Draw();
  }
  }
  else{
  std::cout << "Initialized something that is not a ZDD : " << det.first << std::endl;
  }
  }
  */

  /*
  ////////////////////////////////////////////////////////////////////////////////
  // phy histo
  if(app->HasFlag("--input-phy"))
  {
  zdddir_phy->cd();
  unsigned int ExoSize = 4;
  std::string hist_name = "ZDD_EXO_Cal"; 
  m_phy_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
  ExoSize, 0, ExoSize, 5000, 0, 10000);

    // auto cZDD_IC_PL_phy = new TCanvas("ZDD ID IC PL");
    // hist_name = "ZDD_phy_IC_PL";
    // m_phy_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
    // 2000,
    //                                  0, 200, 4000, 0, 8000);
    // m_phy_hist[hist_name]->Draw("colz");

    // auto cZDD_IC_PL_phy = new TCanvas("ZDD ID IC PL");
    // hist_name = "ZDD_phy_IC_PL";
    // m_phy_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
    // 2000,
    //                                  0, 200, 4000, 0, 8000);
    // m_phy_hist[hist_name]->Draw("colz");

    auto cZDD_IC_PL_phy = new TCanvas("ZDD ID IC PL");
    hist_name = "ZDD_phy_IC_PL";
    m_phy_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(), 2000,
                                     0, 60000, 1000, 0, 8000);
    m_phy_hist[hist_name]->Draw("colz");


  hist_name = "ZDD_EXO_All";
  m_phy_hist[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
  5000, 0, 10000);

  hist_name = "ZDD_ICSum_PLc";
  m_phy_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
  4000, 0, 60000, 2000,0,10000);

  unsigned int PlSize = 5;
  hist_name = "ZDD_PL_Phy"; 
  m_phy_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
  PlSize, 0, PlSize, 5000, 0, 20000);

  hist_name = "IC_dE_dE";
  m_phy_hist[hist_name] = new TH2F(hist_name.c_str(), hist_name.c_str(),
  2000,0,2000,2000, 0, 2000);

  hist_name = "IC_PL_TSdiff";
  m_phy_hist[hist_name] = new TH1F(hist_name.c_str(), hist_name.c_str(),
  2400,-1200,1200);
  c_EXO = 1;
  auto cEXO_phy = new TCanvas("ZDD phy EXO");
  cEXO_phy->Divide(2,2);

  c_PL = 1;
  auto cPL_phy = new TCanvas("ZDD phy PL");
  cPL_phy->Divide(3,2);

  for (auto hist: m_phy_hist){
  if(hist.first.find("ZDD_EXO_phy_") != std::string::npos){
  cEXO_phy->cd(c_EXO);
  c_EXO++;
  hist.second->Draw();
  }
  if(hist.first.find("ZDD_PL_phy_") != std::string::npos){
  cPL_phy->cd(c_PL);
  c_PL++;
  hist.second->Draw();
  }
  if(hist.first.find("IC_phy_sum") != std::string::npos){
  cPL_phy->cd(c_PL);
  c_PL++;
  hist.second->Draw();
  }
  }
  auto cZDD_ID_phy = new TCanvas("ZDD ID IC_E PL_E");
  hist_name = "ZDD_IC_E_PL_E_phy";
  m_phy_hist[hist_name]->Draw("colz");
  */
  // }
  gROOT->cd();
}

////////////////////////////////////////////////////////////////////////////////
void ZddSpectra::FillPhy() {
  /*
     auto size_EXO = m_PhysicsData->Exo_E.size();

  // unsigned int sizePL = m_PhysicsData->PL_E.size();
  // for (unsigned int i = 0; i < sizePL; i++) {
  //   if (m_PhysicsData->ICSum > 0) {
  //     // std::cout << i << " PLN " << m_PhysicsData->PL_Nbr[i] << " PLE " <<
  //     // m_PhysicsData->PL_E[i] << m_PhysicsData->ICSum << std::endl; if
  //     // (std::all_of(m_PhysicsData->IC_E.cbegin(),
  //     m_PhysicsData->IC_E.cend(),
  //     //                 [](auto i) { return i > 0; }) &&
  //     //     tmp_IC_Nbr.size() == 5) {
  //     std::string hist_name = "ZDD_phy_IC_PL";
  //     if (m_PhysicsData->PL_Nbr[i] == 2) {
  //       // m_phy_hist[hist_name]->Fill(sqrt(m_PhysicsData->PL_E[i]),
  //       //                             m_PhysicsData->ICSum);
  //       m_phy_hist[hist_name]->Fill(m_PhysicsData->PL_E[i],
  //                                   m_PhysicsData->ICSum);
  //     }
  //   }
  // }

  unsigned int sizePL = m_PhysicsData->PL_E.size();
  unsigned int sizeICSum = m_PhysicsData->ICSum.size();
  for (unsigned int i = 0; i < sizePL; i++) {
<<<<<<< HEAD
    std::string hist_name = "ZDD_phy_IC_PL";
    if (m_PhysicsData->PL_Nbr[i] == 2) {
      if (sizeICSum > 0) {
        for (unsigned int j = 0; j < sizeICSum; j++) {
          if (m_PhysicsData->ICSumMult[j] == 4) {
            m_phy_hist[hist_name]->Fill(m_PhysicsData->PL_E[j],
                                        m_PhysicsData->ICSum[j]);
          }
        }
=======
    if (m_PhysicsData->ICSum > 0) {
      // std::cout << i << " PLN " << m_PhysicsData->PL_Nbr[i] << " PLE " <<
      // m_PhysicsData->PL_E[i] << m_PhysicsData->ICSum << std::endl; if
      // (std::all_of(m_PhysicsData->IC_E.cbegin(), m_PhysicsData->IC_E.cend(),
      //                 [](auto i) { return i > 0; }) &&
      //     tmp_IC_Nbr.size() == 5) {
      std::string hist_name = "ZDD_phy_IC_PL";
      if (m_PhysicsData->PL_Nbr[i] == 2) {
        // m_phy_hist[hist_name]->Fill(sqrt(m_PhysicsData->PL_E[i]),
        //                             m_PhysicsData->ICSum);
        m_phy_hist[hist_name]->Fill(m_PhysicsData->PL_E[i],
                                    m_PhysicsData->ICSum);
>>>>>>> 6e69f6a98e37e99e4733477953aa6dc100809dd2
      }
    }
  }
  // unsigned int size_PL = m_PhysicsData->PL_E.size();
  // std::set<int> tmp_IC_Nbr(m_PhysicsData->IC_Nbr.begin(), m_PhysicsData->IC_Nbr.end());
  // for(unsigned int i = 0 ;i< size_PL ; i++){
  //   if(m_PhysicsData->PL_E[i]>0){
  //     std::string hist_name = "ZDD_PL_phy_" +  nptool::itoa(m_PhysicsData->PL_Nbr[i]); 
  //     m_phy_hist[hist_name]->Fill(m_PhysicsData->PL_E[i]);
  //     if(m_PhysicsData->ICSum>0&&m_PhysicsData->PL_Nbr[i] == 2){
  //       if(std::all_of(m_PhysicsData->IC_E.cbegin(), m_PhysicsData->IC_E.cend(), [](auto i){ return i > 0; }) && tmp_IC_Nbr.size() == 5){
  //         std::string hist_name = "ZDD_IC_E_PL_E_phy";
  //         m_phy_hist[hist_name]->Fill(m_PhysicsData->PL_E[i],m_PhysicsData->ICSum);
  //       }
  //     }
  //   }
  // }
  // if(m_PhysicsData->ICSum>0){
  //   if(std::all_of(m_PhysicsData->IC_E.cbegin(), m_PhysicsData->IC_E.cend(), [](auto i){ return i > 0 ;}) && tmp_IC_Nbr.size() == 5){
  //     std::string hist_name = "IC_phy_Sum"; 
  //     m_phy_hist[hist_name]->Fill(m_PhysicsData->ICSum);
  //   }
  // }
  */
}

////////////////////////////////////////////////////////////////////////////////
void ZddSpectra::FillRaw() {
  unsigned int size_IC = m_RawData->GetZDD_ICMult();
  for (unsigned int i = 0; i < size_IC; i++) {
    if (m_RawData->GetZDD_ICE(i) > 0) {
      m_raw_hist["ZDD_IC"]->Fill(m_RawData->GetZDD_ICN(i),
                                 m_RawData->GetZDD_ICE(i));
    }
  }
  unsigned int size_PL = m_RawData->GetZDD_PLMult();
  for (unsigned int i = 0; i < size_PL; i++) {
    if (m_RawData->GetZDD_PLE(i) > 0) {
      // std::cout << m_RawData->GetZDD_PLN(i) << " " << m_RawData->GetZDD_PLE(i)
      //           << std::endl;
      m_raw_hist["ZDD_PL"]->Fill(m_RawData->GetZDD_PLN(i),
                                 m_RawData->GetZDD_PLE(i));
    }
  }
  unsigned int size_DC = m_RawData->GetZDD_DCMult();
  for (unsigned int i = 0; i < size_DC; i++) {
    if (m_RawData->GetZDD_DCE(i) > 0) {
      m_raw_hist["ZDD_DC"]->Fill(m_RawData->GetZDD_DCN(i),
                                 m_RawData->GetZDD_DCE(i));
    }
  }
  unsigned int size_EXO = m_RawData->GetZDD_EXOMult();
  for (unsigned int i = 0; i < size_EXO; i++) {
    if (m_RawData->GetZDD_EXOE(i) > 0) {
      m_raw_hist["ZDD_EXO"]->Fill(m_RawData->GetZDD_EXON(i),
                                  m_RawData->GetZDD_EXOE(i));
    }
  //auto size_IC =  m_RawData->GetZDD_ICMult();
  //for (unsigned int i = 0; i < size_IC; i++) {
  //  if(m_RawData->GetZDD_ICE(i)>0)
  //    m_raw_hist["ZDD_IC"]->Fill(m_RawData->GetZDD_ICN(i), m_RawData->GetZDD_ICE(i));
  //}

  //auto size_PL =  m_RawData->GetZDD_PLMult();
  //int PS = 0;
  //for (unsigned int i = 0; i < size_PL; i++) {
  //  if(m_RawData->GetZDD_PLE(i)>0)
  //  {m_raw_hist["ZDD_PL"]->Fill(m_RawData->GetZDD_PLN(i), m_RawData->GetZDD_PLE(i));}
  //}
  
  //// std::cout << ">>>>>>>>>> DEBUG : " << 0 << std::endl;
  //if (size_PL%2 == 0){      // should have even number of events

  //  // std::cout << ">>>>>>>>>> DEBUG : " << 1 << std::endl;
  //  for (unsigned int i = 0; i<size_PL/2; i++){   // looping with #events + 2
  //    // std::cout << ">>>>>>>>>> DEBUG : " << 2 << std::endl;

  //    // std::cout << ">>>>>>>>>> DEBUG : " << 3 <<" " << size_PL << " "<< m_RawData->GetZDD_PLTS(2*i) << " "<<  m_RawData->GetZDD_PLTS(2*i + 1)<< " "
  //      // <<abs(static_cast<long long>(m_RawData->GetZDD_PLTS(2*i)) - static_cast<long long>(m_RawData->GetZDD_PLTS(2*i + 1)))<< std::endl;
  //    if (abs(static_cast<long long>(m_RawData->GetZDD_PLTS(2*i)) - static_cast<long long>(m_RawData->GetZDD_PLTS(2*i + 1)) <= 10)){   //if the events came within 30 ns window of each other 
  //      // loop for PL 1
  //      //if the first event was right and 2nd in left
  //      if (m_RawData->GetZDD_PLN(2*i) == 0  && m_RawData->GetZDD_PLN(2*i + 1) == 5){
  //        m_raw_hist["ZDD_PL_1_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i + 1), m_RawData->GetZDD_PLE(2*i));                // draw 2, 1 as L,R (x,y)
  //      }
  //      else if (m_RawData->GetZDD_PLN(2*i) == 5 && m_RawData->GetZDD_PLN(2*i + 1) == 0){    // if the first event was left and second in right
  //        m_raw_hist["ZDD_PL_1_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i), m_RawData->GetZDD_PLE(2*i + 1));              // draw 1, 2 as L,R (x,y)
  //        // std::cout << ">>>>>>>>>> DEBUG : " << 4 << std::endl;
  //      }
  //      //loop for PL 2******
  //      else if (m_RawData->GetZDD_PLN(2*i) == 1  && m_RawData->GetZDD_PLN(2*i + 1) == 6){ //if the first event was right and 2nd in left
  //        m_raw_hist["ZDD_PL_2_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i + 1), m_RawData->GetZDD_PLE(2*i));                // draw 2, 1 as L,R (x,y)
  //      }
  //      else if (m_RawData->GetZDD_PLN(2*i) == 6 && m_RawData->GetZDD_PLN(2*i + 1) == 1){    // if the first event was left and second in right
  //        m_raw_hist["ZDD_PL_2_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i), m_RawData->GetZDD_PLE(2*i + 1));              // draw 1, 2 as L,R (x,y)
  //        // std::cout << ">>>>>>>>>> DEBUG : " << 5 << std::endl;
  //      }
  //      //loop for PL 3******
  //      else if (m_RawData->GetZDD_PLN(2*i) == 2  && m_RawData->GetZDD_PLN(2*i + 1) == 7){ //if the first event was right and 2nd in left
  //        m_raw_hist["ZDD_PL_3_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i + 1), m_RawData->GetZDD_PLE(2*i));                // draw 2, 1 as L,R (x,y)
  //      }
  //      else if (m_RawData->GetZDD_PLN(2*i) == 7 && m_RawData->GetZDD_PLN(2*i + 1) == 2){    // if the first event was left and second in right
  //        m_raw_hist["ZDD_PL_3_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i), m_RawData->GetZDD_PLE(2*i + 1));              // draw 1, 2 as L,R (x,y)
  //        // std::cout << ">>>>>>>>>> DEBUG : " << 6 << std::endl;
  //      }
  //      //loop for PL 4
  //      else if (m_RawData->GetZDD_PLN(2*i) == 3  && m_RawData->GetZDD_PLN(2*i + 1) == 8){ //if the first event was right and 2nd in left
  //        m_raw_hist["ZDD_PL_4_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i + 1), m_RawData->GetZDD_PLE(2*i));                // draw 2, 1 as L,R (x,y)
  //      }
  //      else if (m_RawData->GetZDD_PLN(2*i) == 8 && m_RawData->GetZDD_PLN(2*i + 1) == 3){    // if the first event was left and second in right
  //        m_raw_hist["ZDD_PL_4_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i), m_RawData->GetZDD_PLE(2*i + 1));              // draw 1, 2 as L,R (x,y)
  //        // std::cout << ">>>>>>>>>> DEBUG : " << 7 << std::endl;
  //      }
  //      //loop for PL 5
  //      else if (m_RawData->GetZDD_PLN(2*i) == 4  && m_RawData->GetZDD_PLN(2*i + 1) == 9){ //if the first event was right and 2nd in left
  //        m_raw_hist["ZDD_PL_5_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i + 1), m_RawData->GetZDD_PLE(2*i));                // draw 2, 1 as L,R (x,y)
  //      }
  //      else if (m_RawData->GetZDD_PLN(2*i) == 9 && m_RawData->GetZDD_PLN(2*i + 1) == 4){    // if the first event was left and second in right
  //        m_raw_hist["ZDD_PL_5_LvsR"]->Fill(m_RawData->GetZDD_PLE(2*i), m_RawData->GetZDD_PLE(2*i + 1));              // draw 1, 2 as L,R (x,y)
  //        // std::cout << ">>>>>>>>>> DEBUG : " << 8 << std::endl;
  //      }
  //    }
  //  }
  //}

  ///*************************************/

  //auto size_DC =  m_RawData->GetZDD_DCMult();
  //for (unsigned int i = 0; i < size_DC; i++) {
  //  dt_x=0;
  //  dt_y=0;


  //  dl_x1=0.0;
  //  dl_x2=0.0;
  //  dl_y1=0.0;
  //  dl_y2=0.0;

  //  if(m_RawData->GetZDD_DCE(i)>0){
  //    m_raw_hist["ZDD_DC"]->Fill(m_RawData->GetZDD_DCN(i), m_RawData->GetZDD_DCE(i));

  //    if (m_RawData->Get_Central_Right_PL_TS(0)){
  //      Central_Right_PL_TS = m_RawData->Get_Central_Right_PL_TS(0);         // DEFINE reference time stamp for DC :PS
  //      if (Central_Right_PL_TS != -1){

  //        if (m_RawData->GetZDD_DCN(i)==2){
  //          dt_x = static_cast<long long>(Central_Right_PL_TS) - static_cast<long long>(m_RawData->GetZDD_DCTS(i));
  //          if(dt_x != 0){
  //            dl_x1 = (dt_x*dx_gain[0] + dx_off[0]);
  //            m_raw_hist["drift_time"]->Fill(2.0,dt_x);
  //            if(dl_x1 != 0 && abs(dl_x1)<10000)
  //              m_raw_hist["drift_dist"]->Fill(2.0,dl_x1);
  //          }
  //        }

  //        else if (m_RawData->GetZDD_DCN(i)==3){
  //          dt_x = static_cast<long long>(Central_Right_PL_TS) - static_cast<long long>(m_RawData->GetZDD_DCTS(i));
  //          if(dt_x != 0){
  //            dl_x2 = (dt_x*dx_gain[1] + dx_off[1]);
  //            m_raw_hist["drift_time"]->Fill(3.0,dt_x);
  //            if(dl_x2 != 0 && abs(dl_x2)<10000)
  //              m_raw_hist["drift_dist"]->Fill(3.0,dl_x2);
  //          }
  //        }

  //        else if (m_RawData->GetZDD_DCN(i)==1){
  //          dt_y = static_cast<long long>(Central_Right_PL_TS) - static_cast<long long>(m_RawData->GetZDD_DCTS(i));
  //          if(dt_y != 0){
  //            dl_y1 = (dt_y*dy_gain[0] + dy_off[0]);
  //            m_raw_hist["drift_time"]->Fill(1.0,dt_y);
  //            if(dl_y1 != 0 && abs(dl_y1)<10000)
  //              m_raw_hist["drift_dist"]->Fill(1.0,dl_y1);
  //          }
  //        }

  //        else if (m_RawData->GetZDD_DCN(i)==0){
  //          dt_y = static_cast<long long>(Central_Right_PL_TS) - static_cast<long long>(m_RawData->GetZDD_DCTS(i));
  //          if(dt_y != 0){
  //            dl_y2 = (dt_y*dy_gain[1] + dy_off[1]);
  //            m_raw_hist["drift_time"]->Fill(0.0,dt_y);
  //            if(dl_y2 != 0 && abs(dl_y2)<10000)
  //              m_raw_hist["drift_dist"]->Fill(0.0,dl_y2);
  //          }
  //        }
  //        // m_raw_hist["beam_spot_DC"]->Fill()
  //      }
  //    }
  //  }

  }


  //auto size_EXO =  m_RawData->GetZDD_EXOMult();

  //// cout<<"the multip of exo is: "<<size_EXO<<endl;
  //for (unsigned int i = 0; i < size_EXO; i++) {
  //  if (m_RawData->GetZDD_EXOE(i) > 0){
  //    m_raw_hist["ZDD_EXO"]->Fill(m_RawData->GetZDD_EXON(i),
  //                                m_RawData->GetZDD_EXOE(i));

  //    std::cout << " AIM I ALIVE??" << std::endl;
////      //std::cout <<  m_RawData->GetZDD_EXOE(i) << std::endl;
////      std::cout << typeid(m_RawData->GetZDD_EXOTime(i)).name() << std::endl;
  //    std::cout << m_RawData->fZDD_EXO_Time.size() << std::endl;
  //    m_raw_hist["ZDD_EXO_T"]->Fill(m_RawData->GetZDD_EXON(i),
  //                                //m_RawData->GetZDD_EXOE(i));
  //                                m_RawData->GetZDD_EXOTime(i));
  //  }
  //}
}

////////////////////////////////////////////////////////////////////////////////
void ZddSpectra::Clear() {
  for (auto det : m_raw_hist)
    det.second->Reset();
  for (auto hist : m_phy_hist)
    hist.second->Reset();
}


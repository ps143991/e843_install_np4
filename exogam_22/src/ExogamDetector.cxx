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
#include "ExogamDetector.h"

#include <iostream>

#include "ExogamSpectra.h"
#include "NPFunction.h"
#include "NPRootPlugin.h"
using namespace exogam;
using namespace std;
using namespace ROOT::Math;

////////////////////////////////////////////////////////////////////////////////
ExogamDetector::ExogamDetector() {
  m_RawData = new exogam::ExogamData();
  m_CalData = new exogam::ExogamData();
  m_PhysicsData = new exogam::ExogamPhysics();
  m_Cal.InitCalibration();

  // Thresholds, raw
  m_EXO_E_RAW_Threshold = 0;
  m_EXO_EHG_RAW_Threshold = 0;
  m_EXO_TDC_RAW_Threshold = 0;
  m_EXO_OuterUp_RAW_Threshold = 4294966296;
  m_EXO_OuterLw_RAW_Threshold = 0;
  m_EXO_BGO_RAW_Threshold = 0;
  m_EXO_CsI_RAW_Threshold = 0;

  // Thresholds, calibrated
  m_EXO_E_Threshold = 0;

  // Counters
  count_CoreNoSeg = 0;
  count_SegNoCore = 0;
  count_GammaHit = 0;
  count_AntiCompton = 0;

  // Warning limiters
  limit_CoreNoSeg = 100.0;    //%
  limit_SegNoCore = 100.0;    //%
  limit_AntiCompton = 100.0;  //%

  // Record of last timestamp
  lastTS = 0;
  m_beta = 12;
}

////////////////////////////////////////////////////////////////////////////////
ExogamDetector::~ExogamDetector() {}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::ReadConfiguration(nptool::InputParser parser) {
  auto blocks = parser.GetAllBlocksWithToken("exogam");

  std::vector<std::string> token = {"Board", "Flange", "NUMEXOChannel",
                                    "NUMEXOChannelGroup"};

  for (unsigned int i = 0; i < blocks.size(); i++) {
    if (blocks[i]->HasTokenList(token)) {
      unsigned int Board = blocks[i]->GetInt("Board");
      unsigned int NUMEXOChannelGroup = blocks[i]->GetInt("NUMEXOChannelGroup");
      unsigned int Flange = blocks[i]->GetInt("Flange");

      std::vector<int> NUMEXOChannel = blocks[i]->GetVectorInt("NUMEXOChannel");

      for (unsigned int j = 0; j < 2; j++) {
        SetExogamNumexoMap(Board, Flange, (unsigned int)NUMEXOChannel[j],
                           NUMEXOChannelGroup);
      }
    }
  }
  ReadAnalysisConfig();
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::SetExogamNumexoMap(unsigned int Board, unsigned int Flange,
                                        unsigned int NUMEXOChannel,
                                        unsigned int NUMEXOChannelGroup) {
  m_ExogamNumexoGroupMap[Board] = NUMEXOChannelGroup;
  unsigned int index = Board * 1e3 + NUMEXOChannel;
  unsigned int Crystal = Flange * 4 + NUMEXOChannel;
  m_ExogamNumexoCrystalMap[index] = Crystal;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int ExogamDetector::GetCrystalFromNumexoMap(unsigned int Board,
                                                     unsigned int Channel) {
  unsigned int NUMEXOChannel;
  unsigned int NUMEXOChannelGroup = m_ExogamNumexoGroupMap[Board];
  if (NUMEXOChannelGroup == 0)
    NUMEXOChannel = Channel;
  else
    NUMEXOChannel = Channel + 2;

  unsigned int index = Board * 1e3 + NUMEXOChannel;
  unsigned int Crystal = m_ExogamNumexoCrystalMap[index];
  return Crystal;
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigExogam.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    std::cout << " No ConfigExogam.dat found: Default parameters loaded for "
                 "Analysis "
              << FileName << std::endl;
    return;
  }

  string DataBuffer, whatToDo;
  string LineBuffer;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 12, "ConfigExogam") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus) {
      whatToDo = "";
      AnalysisConfigFile >> whatToDo;
      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n');

      } else if (whatToDo == "EXO_THRESH_E_RAW_CORE_LG") {
        // Low-gain raw core signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        m_EXO_E_RAW_Threshold = atoi(DataBuffer.c_str());
        std::cout << whatToDo << " " << m_EXO_E_RAW_Threshold << std::endl;

      } else if (whatToDo == "EXO_THRESH_E_RAW_CORE_HG") {
        // High-gain raw core signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        m_EXO_EHG_RAW_Threshold = atoi(DataBuffer.c_str());
        std::cout << whatToDo << " " << m_EXO_EHG_RAW_Threshold << std::endl;

      } else if (whatToDo == "EXO_THRESH_E_RAW_OUTER_UP") {
        // Segment raw signal (upper threshold)
        AnalysisConfigFile >> DataBuffer;
        m_EXO_OuterUp_RAW_Threshold = atoi(DataBuffer.c_str());
        std::cout << whatToDo << " " << m_EXO_OuterUp_RAW_Threshold
                  << std::endl;

      } else if (whatToDo == "EXO_THRESH_E_RAW_OUTER_LOW") {
        // Segment raw signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        m_EXO_OuterLw_RAW_Threshold = atoi(DataBuffer.c_str());
        std::cout << whatToDo << " " << m_EXO_OuterLw_RAW_Threshold
                  << std::endl;

      } else if (whatToDo == "EXO_THRESH_E") {
        // Core processed signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        m_EXO_E_Threshold = atoi(DataBuffer.c_str());
        std::cout << whatToDo << " " << m_EXO_E_Threshold << std::endl;

      } else if (whatToDo == "EXO_THRESH_BGO_RAW") {
        // Core processed signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        m_EXO_BGO_RAW_Threshold = atoi(DataBuffer.c_str());
        std::cout << whatToDo << " " << m_EXO_BGO_RAW_Threshold << std::endl;

      } else if (whatToDo == "EXO_THRESH_CSI_RAW") {
        // Core processed signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        m_EXO_CsI_RAW_Threshold = atoi(DataBuffer.c_str());
        std::cout << whatToDo << " " << m_EXO_CsI_RAW_Threshold << std::endl;

      } else if (whatToDo == "EXO_THRESH_TDC_RAW") {
        // Time signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        m_EXO_TDC_RAW_Threshold = atoi(DataBuffer.c_str());
        std::cout << whatToDo << " " << m_EXO_TDC_RAW_Threshold << std::endl;

      } else if (whatToDo == "EXO_ALERTLIMIT_CORENOSEG") {
        // Time signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        limit_CoreNoSeg = atof(DataBuffer.c_str());
        std::cout << whatToDo << " " << limit_CoreNoSeg << std::endl;

      } else if (whatToDo == "EXO_ALERTLIMIT_SEGNOCORE") {
        // Time signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        limit_SegNoCore = atof(DataBuffer.c_str());
        std::cout << whatToDo << " " << limit_SegNoCore << std::endl;

      } else if (whatToDo == "EXO_ALERTLIMIT_ANTICOMPTON") {
        // Time signal (lower threshold)
        AnalysisConfigFile >> DataBuffer;
        limit_AntiCompton = atof(DataBuffer.c_str());
        std::cout << whatToDo << " " << limit_AntiCompton << std::endl;

      } else if (whatToDo == "PhotonCS") {
        // Photon cross-section path
        AnalysisConfigFile >> DataBuffer;
        string CSFilename = DataBuffer;

        ifstream CSFile;
        CSFile.open(CSFilename.c_str());
        if (!CSFile.is_open()) {
          std::cout << " No CS file found " << CSFilename << std::endl;
          std::cout << " Loading CoherentGe.xcom from exogam plugin "
                    << std::endl;
          CSFilename = "./CoherentGe.xcom";
          return;
        }

        while (CSFile.good()) {
          double gammaE, CrossSection;
          getline(CSFile, LineBuffer);
          istringstream ss(LineBuffer);
          ss >> gammaE >> CrossSection;  // E in MeV, converted to keV,
                                         // CrossSection in cm2/g
          gammaE *= 1000.;               // Convertion to keV
          CrossSection *= 100.;
          Map_PhotonCS[gammaE] = CrossSection;
        }

      } else if (whatToDo == "MAP_EXO") {
        // Mapping raw crystal onto <flange, crystal in flange>
        unsigned int CrystalNb;
        unsigned int Flange;
        unsigned int CrystalNb2;

        AnalysisConfigFile >> DataBuffer;
        CrystalNb = stoi(DataBuffer);

        AnalysisConfigFile >> DataBuffer;
        Flange = stoi(DataBuffer);

        AnalysisConfigFile >> DataBuffer;
        CrystalNb2 = stoi(DataBuffer);
        MapCrystalFlangeClover[CrystalNb] = std::make_pair(Flange, CrystalNb2);

        if (!IsCloverActive[Flange]) {
          SetCloverActive(Flange);
        }
        if (!IsCrystalActive[CrystalNb]) {
          SetCrystalActive(CrystalNb);
        }

      } else if (whatToDo == "DISABLE_EXO_CRYSTAL") {
        // Disabling a crystal (by raw number 0-63, not flange-mapped)
        AnalysisConfigFile >> DataBuffer;
        SetCrystalInactive(stoi(DataBuffer));

      } else if (whatToDo == "DISABLE_EXO_SEGMENT") {
        // Disabling a segment (by raw number 0-63, not flange-mapped)
        unsigned int CrystalNb;
        unsigned int Segment;
        AnalysisConfigFile >> DataBuffer;
        CrystalNb = stoi(DataBuffer);
        AnalysisConfigFile >> DataBuffer;
        Segment = stoi(DataBuffer);

        SetSegmentInactive(CrystalNb, Segment);

      } else {
        ReadingStatus = false;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::BuildRawEvent(const std::string& daq_name,
                                   const std::string& st_type_key,
                                   void* commonframe) {
#ifdef MFM_FOUND
  int type_key = ((MFMCommonFrame*)commonframe)->GetFrameType();
  if (type_key == MFM_EXO2_FRAME_TYPE) {
    TreatFrame((MFMCommonFrame*)commonframe);
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::TreatFrame(void* commonframe) {
#ifdef MFM_FOUND

  std::shared_ptr<MFMExogamFrame> ExogamFrame =
      std::make_shared<MFMExogamFrame>();
  ExogamFrame->SetAttributs(((MFMCommonFrame*)commonframe)->GetPointHeader());

  // Data straight from frame
  // ----------------------------------------------------
  int Board = ExogamFrame->ExoGetBoardId();
  int Channel = (ExogamFrame->ExoGetTGCristalId()) / 8;
  unsigned int E = ExogamFrame->ExoGetInnerM(0);    // E = Inner6MV
  unsigned int EHG = ExogamFrame->ExoGetInnerM(1);  // EHG = Inner20MV
  unsigned int BGO = ExogamFrame->ExoGetBGO();
  unsigned int CsI = ExogamFrame->ExoGetCsi();
  unsigned int TDC = ExogamFrame->ExoGetDeltaT();
  unsigned long long TS = ExogamFrame->GetTimeStamp();

  // Get crystal number from map
  // -------------------------------------------------
  unsigned int Crystal = GetCrystalFromNumexoMap(Board, Channel);

  if (Board == 0) {
    nptool::message("red", "exogam", "exogam::ExogamDetector::TreatFrame()",
                    "Receiving event with board ID = 0!");
  }

  if (Crystal == 0) {
    std::string msg1 = "Mapping failed, does detector.yaml match the boards? ";
    std::string msg2 =
        "See board " + to_string(Board) + ", Channel " + to_string(Channel);
    nptool::message("red", "exogam", "exogam::ExogamDetector::TreatFrame()",
                    msg1 + msg2);
  }

  if (!IsCrystalActive[Crystal]) {
    // Deactivated crystal, ignore!
    return;
  }

  // Treat outers
  // ----------------------------------------------------------------
  unsigned short ExoSegStatus;
  ExoSegStatus = ExogamFrame->ExoGetStatus(2);
  bool ExoSegCharge[4];
  unsigned int Outer[4];

  for (unsigned int seg = 0; seg < 4; seg++) {
    ExoSegCharge[seg] = !(((ExoSegStatus >> (8 * Channel)) >> (seg + 2)) & 1);
  }

  for (int seg = 0; seg < 4; seg++) {
    if (ExogamFrame->ExoGetOuter(seg) > 0 && ExoSegCharge[seg] == 1)
      Outer[seg] = ExogamFrame->ExoGetOuter(seg);
    else
      Outer[seg] = -1000;
  }

  unsigned int Outer1 = Outer[0];
  unsigned int Outer2 = Outer[1];
  unsigned int Outer3 = Outer[2];
  unsigned int Outer4 = Outer[3];

  // Exception checks
  // ------------------------------------------------------------
  bool cL = E > m_EXO_E_RAW_Threshold;
  bool cH = EHG > m_EXO_EHG_RAW_Threshold;
  bool o1 = (m_EXO_OuterUp_RAW_Threshold > Outer1) &&
            (Outer1 > m_EXO_OuterLw_RAW_Threshold);
  bool o2 = (m_EXO_OuterUp_RAW_Threshold > Outer2) &&
            (Outer2 > m_EXO_OuterLw_RAW_Threshold);
  bool o3 = (m_EXO_OuterUp_RAW_Threshold > Outer3) &&
            (Outer3 > m_EXO_OuterLw_RAW_Threshold);
  bool o4 = (m_EXO_OuterUp_RAW_Threshold > Outer4) &&
            (Outer4 > m_EXO_OuterLw_RAW_Threshold);

  std::string msgThresh =
      +"\n\t\tThreshE    = " + to_string((int)m_EXO_E_RAW_Threshold) +
      "\n\t\tThreshEHG  = " + to_string((int)m_EXO_EHG_RAW_Threshold) +
      "\n\t\tThreshOutL = " + to_string((int)m_EXO_OuterLw_RAW_Threshold) +
      "\n\t\tThreshOutH = " + to_string((int)m_EXO_OuterUp_RAW_Threshold);

  std::string msgValues =
      +"\n\t\tE   = " + to_string(E) + "\n\t\tEHG = " + to_string(EHG) +
      "\n\t\tO1  = " + to_string(Outer1) + "\n\t\tO2  = " + to_string(Outer2) +
      "\n\t\tO3  = " + to_string(Outer3) + "\n\t\tO4  = " + to_string(Outer4);

  if ((cL || cH) && !(o1 || o2 || o3 || o4)) {
    count_CoreNoSeg++;
  } else if (!(cL || cH) && (o1 || o2 || o3 || o4)) {
    count_SegNoCore++;
  } else {
    count_GammaHit++;
  }

  if (BGO > m_EXO_BGO_RAW_Threshold || CsI > m_EXO_CsI_RAW_Threshold) {
    count_AntiCompton++;
  }

  // Exception check: is timestamp == 0?
  // -----------------------------------------
  if (TS == 0) {
    std::string msg = "Timestamp == 0!  Brd " + to_string(Board) + ", Chan " +
                      to_string(Channel) + ", TS " + to_string(TS);
    throw(nptool::Error("exogam::ExogamDetector::TreatFrame()", msg));
  }

  // Exception check: charge in core/segments
  // ------------------------------------
  if (100. * ((double)count_CoreNoSeg / (double)count_GammaHit) >
      limit_CoreNoSeg) {
    if (count_CoreNoSeg > 10 &&
        count_GammaHit > 10) {  // ignore if at start of data
      std::string msg = "Core has charge, but segments do not!";
      nptool::message("red", "exogam", "ExogamDetector::TreatFrame()",
                      "!WARNING! " + msg + " Fraction = " +
                          to_string(100. * ((double)count_CoreNoSeg /
                                            (double)count_GammaHit)) +
                          "%");
    }
  }

  if (100. * ((double)count_SegNoCore / (double)count_GammaHit) >
      limit_SegNoCore) {
    if (count_SegNoCore > 10 &&
        count_GammaHit > 10) {  // ignore if at start of data
      std::string msg = "Segment has charge, but core does not!";
      nptool::message("red", "exogam", "ExogamDetector::TreatFrame()",
                      "!WARNING! " + msg + " Fraction = " +
                          to_string(100. * ((double)count_SegNoCore /
                                            (double)count_GammaHit)) +
                          "%");
    }
  }

  // Exception check: Compton supression fraction
  // --------------------------------
  if (100. * ((double)count_AntiCompton / (double)count_GammaHit) >
      limit_AntiCompton) {
    if (count_AntiCompton > 10 &&
        count_GammaHit > 10) {  // ignore if at start of data
      std::string msg = "Number of anti-Compton vs good hits is high! ";
      nptool::message("red", "exogam", "ExogamDetector::TreatFrame()",
                      "!WARNING! " + msg + " Fraction = " +
                          to_string(100. * ((double)count_AntiCompton /
                                            (double)count_GammaHit)) +
                          "%");
    }
  }

  m_RawData->SetExo(Crystal, E, EHG, TS, TDC, BGO, CsI, Outer1, Outer2, Outer3,
                    Outer4);

#endif
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::BuildPhysicalEvent() {
  // PreTreat (applies thresh to m_RawData, put in m_CalData)
  // --------------------
  PreTreat();

  // -----------------------------------------------------------------------------
  // This maps stores ID of events sorted by flange number. Map key is flange
  // nbr, vector should contain ID of events
  std::map<unsigned int, std::vector<unsigned int>> HitsID;

  for (unsigned int i = 0; i < m_CalData->GetExoMult(); i++) {
    // Doing flange and crystal matching
    flange_nbr = MapCrystalFlangeClover[m_CalData->GetExoCrystal(i)].first;
    crystal_nbr = MapCrystalFlangeClover[m_CalData->GetExoCrystal(i)].second;

    if (!IsFlangeCrystalActive[flange_nbr][crystal_nbr]) {
      // Deactivated crystal, ignore!
      return;
    }

    if (flange_nbr > 17) {
      // SOME WEIRD ERROR!
      return;
    }

    // Fill physics data structure
    m_PhysicsData->Exo_E.push_back(m_CalData->GetExoE(i));
    m_PhysicsData->Exo_EHG.push_back(m_CalData->GetExoEHG(i));
    m_PhysicsData->Exo_Outer1.push_back(m_CalData->GetExoOuter1(i));
    m_PhysicsData->Exo_Outer2.push_back(m_CalData->GetExoOuter2(i));
    m_PhysicsData->Exo_Outer3.push_back(m_CalData->GetExoOuter3(i));
    m_PhysicsData->Exo_Outer4.push_back(m_CalData->GetExoOuter4(i));
    m_PhysicsData->Exo_FlangeN.push_back(flange_nbr);
    m_PhysicsData->Exo_CrystalN.push_back(crystal_nbr);
    m_PhysicsData->Exo_TDC.push_back(m_CalData->GetExoTDC(i));
    m_PhysicsData->Exo_TS.push_back(m_CalData->GetExoTS(i));

    // Broken segment?
    // -------------------------------------------------------------
    unsigned int seg_count = 0;
    unsigned int seg_flag = 99;
    for (unsigned int seg = 0; seg < 4; seg++) {
      if (IsSegmentActive[(flange_nbr * 4) + crystal_nbr][seg] == true) {
        seg_count++;
      } else {
        seg_flag = seg;
      }
    }

    // If only one segment broken, recover energy as seg = (core - other segs)
    // -----
    if (seg_count == 3) {  // if ONE segment missing
      switch (seg_flag) {
        case 0:
          m_PhysicsData->Exo_Outer1.pop_back();  // remove last element
          m_PhysicsData->Exo_Outer1.push_back(m_PhysicsData->Exo_E.back() -
                                              m_PhysicsData->Exo_Outer2.back() -
                                              m_PhysicsData->Exo_Outer3.back() -
                                              m_PhysicsData->Exo_Outer4.back());
          break;
        case 1:
          m_PhysicsData->Exo_Outer2.pop_back();  // remove last element
          m_PhysicsData->Exo_Outer1.push_back(m_PhysicsData->Exo_E.back() -
                                              m_PhysicsData->Exo_Outer1.back() -
                                              m_PhysicsData->Exo_Outer3.back() -
                                              m_PhysicsData->Exo_Outer4.back());
          break;
        case 2:
          m_PhysicsData->Exo_Outer3.pop_back();  // remove last element
          m_PhysicsData->Exo_Outer1.push_back(m_PhysicsData->Exo_E.back() -
                                              m_PhysicsData->Exo_Outer1.back() -
                                              m_PhysicsData->Exo_Outer2.back() -
                                              m_PhysicsData->Exo_Outer4.back());
          break;
        case 3:
          m_PhysicsData->Exo_Outer4.pop_back();  // remove last element
          m_PhysicsData->Exo_Outer1.push_back(m_PhysicsData->Exo_E.back() -
                                              m_PhysicsData->Exo_Outer1.back() -
                                              m_PhysicsData->Exo_Outer2.back() -
                                              m_PhysicsData->Exo_Outer3.back());
          break;
      }
    }

    // Filling HitsID
    HitsID[flange_nbr].push_back(i);
  }

  // -----------------------------------------------------------------------------
  // Now that HitsID is full, we use it to process simple AddBack of events in
  // the same flange Basically looping on all flanges, then on al events ID in
  // each flange
  for (auto it = HitsID.begin(); it != HitsID.end(); it++) {
    double E_AddBack = 0;
    double E_Max = 0;
    unsigned int Id_Max = 0;
    for (auto itvec = (*it).second.begin(); itvec != (*it).second.end();
         itvec++) {
      E_AddBack += m_CalData->GetExoE(*itvec);
      cout<<"E_AddBack: "<<E_AddBack<<endl;
      if (E_Max < m_CalData->GetExoE(*itvec)) {
        E_Max = m_CalData->GetExoE(*itvec);
        Id_Max = *itvec;
      }
    }
    // Doing it again for this loop, it's a bit unhappy but didnt find a better
    // way to do it yet
    flange_nbr = (*it).first;
    crystal_nbr =
        MapCrystalFlangeClover[m_CalData->GetExoCrystal(Id_Max)].second;

    // Adding all AddBack (AB) related stuff
    m_PhysicsData->Exo_EAddBack.push_back(E_AddBack);
    m_PhysicsData->Exo_FlangeNAddBack.push_back(flange_nbr);
    m_PhysicsData->Exo_SizePreAddBack.push_back((*it).second.size());

    // Adding these parameters for Doppler correction purposes (D)
    m_PhysicsData->Exo_CrystalNAddBack.push_back(crystal_nbr);
    int MaxOuterId = GetMaxOuter(Id_Max);
    m_PhysicsData->Exo_OuterNAddBack.push_back(GetMaxOuter(Id_Max));

    if (flange_nbr > 0 && flange_nbr < 17) {
      if (MaxOuterId > -1) {
        Exogam_struc = Ask_For_Angles((int)flange_nbr,
                                      (double)ComputeMeanFreePath(E_AddBack));
        double Theta_seg =
            Exogam_struc.Theta_Crystal_Seg[crystal_nbr][MaxOuterId];
        double Phi_seg = Exogam_struc.Phi_Crystal_Seg[crystal_nbr][MaxOuterId];
        m_PhysicsData->Exo_Theta.push_back(
            Theta_seg);  // * (180./TMath::Pi()) ); //ADDED CONVERSION TO
                         // RADIANS
        m_PhysicsData->Exo_Phi.push_back(
            Phi_seg);  // * (180./TMath::Pi()) ); //ADDED CONVERSION TO RADIANS
      } else {
        m_PhysicsData->Exo_Theta.push_back(-1000);
        m_PhysicsData->Exo_Phi.push_back(-1000);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::PreTreat() {
  // Clearing PreTreat TExogamData
  // -----------------------------------------------
  ClearPreTreatedData();

  // Fill m_CalData from m_RawData over thresholds
  // -------------------------------
  m_EXO_Mult = m_RawData->GetExoMult();

  for (unsigned int i = 0; i < m_EXO_Mult; ++i) {
    ResetPreTreatVariable();

    if (m_RawData->GetExoE(i) > m_EXO_E_RAW_Threshold) EXO_E = fEXO_E(i);

    if (m_RawData->GetExoEHG(i) > m_EXO_EHG_RAW_Threshold)
      EXO_EHG = fEXO_EHG(i);

    if (m_RawData->GetExoTDC(i) > m_EXO_TDC_RAW_Threshold) EXO_TDC = fEXO_T(i);

    if (m_RawData->GetExoOuter1(i) < m_EXO_OuterUp_RAW_Threshold)
      EXO_Outer1 = fEXO_Outer(i, 0);
    else
      EXO_Outer1 = 0;

    if (m_RawData->GetExoOuter2(i) < m_EXO_OuterUp_RAW_Threshold)
      EXO_Outer2 = fEXO_Outer(i, 1);
    else
      EXO_Outer2 = 0;

    if (m_RawData->GetExoOuter3(i) < m_EXO_OuterUp_RAW_Threshold)
      EXO_Outer3 = fEXO_Outer(i, 2);
    else
      EXO_Outer3 = 0;

    if (m_RawData->GetExoOuter4(i) < m_EXO_OuterUp_RAW_Threshold)
      EXO_Outer4 = fEXO_Outer(i, 3);
    else
      EXO_Outer4 = 0;

    if (EXO_E > m_EXO_E_Threshold) {
      m_CalData->SetExo(m_RawData->GetExoCrystal(i), EXO_E, EXO_EHG,
                        m_RawData->GetExoTS(i), EXO_TDC,
                        m_RawData->GetExoBGO(i), m_RawData->GetExoCsI(i),
                        EXO_Outer1, EXO_Outer2, EXO_Outer3, EXO_Outer4);
    }
  }
  // -----------------------------------------------------------------------------
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::ResetPreTreatVariable() {
  EXO_E = -1000;
  EXO_EHG = -1000;
  EXO_TDC = -1000;
  EXO_Outer1 = -1000;
  EXO_Outer2 = -1000;
  EXO_Outer3 = -1000;
  EXO_Outer4 = -1000;
}

//////////////////////////////////////////////////////////////////////////////
double ExogamDetector::GetDoppler(double Energy, unsigned int Flange,
                                  unsigned int Crystal, unsigned int Outer) {
  // MAKE SURE THAT ANY LOOPS ARE USING THE SIZE OF ADDBACK!
  if (ComputeMeanFreePath(Energy) != -1000) {
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "
              << ComputeMeanFreePath(Energy) << std::endl;
  }

  // Exogam_struc     = Ask_For_Angles((int) Flange, 20.);
  Exogam_struc =
      Ask_For_Angles((int)Flange, (double)ComputeMeanFreePath(Energy));
  double Theta_seg = Exogam_struc.Theta_Crystal_Seg[Crystal][Outer];
  double Phi_seg = Exogam_struc.Phi_Crystal_Seg[Crystal][Outer];
  return Doppler_Correction(Theta_seg, Phi_seg, 0, 0, GetBeta(), Energy);
}

////////////////////////////////////////////////////////////////////////////////
double ExogamDetector::ComputeMeanFreePath(double Energy) {
  auto b = Map_PhotonCS.lower_bound(Energy);
  auto a = b;

  if (b != Map_PhotonCS.begin())
    a = prev(b);
  else
    return -1000;

  if (b == Map_PhotonCS.begin()) {
    a = b;
    b++;
  } else if (b == Map_PhotonCS.end()) {
    b--;
    a = prev(b);
  }

  double coeff = (Energy - a->first) / (b->first - a->first);

  double PhotonCrossSection =
      a->second + coeff * (b->second - a->second);  // mm2/g
  return 1. / (GeDensity * PhotonCrossSection);
}

////////////////////////////////////////////////////////////////////////////////
double ExogamDetector::GetDopplerSimple(double E, double Angle) {
  if(GetBeta()<0) {
    std::cout << "BETA LESS THAN ZERO! Is GetBeta called?" << std::endl;
    return 0;
  } else {

    // E of gamma Doppler shifted, Angle is the angle between gamma and part, Beta
    // is speed of part
    double EDC = 0;
    //  double Gamma = 1./sqrt(1.-Beta*Beta);
    //  EDC = Gamma*E*(1.-Beta*std::cos(Angle));

    EDC = E * ((1. - GetBeta() * TMath::Cos(Angle)) /
               (TMath::Sqrt(1. - GetBeta() * GetBeta())));

    // std::cout << " In simple = " << E << " " << Angle << " " << Beta << " " <<
    // Gamma << " " << std::cos(Angle) << " " << EDC << std::endl; std::cout << "
    // Gamma value  = " << -Beta*Beta << " " << 1.-Beta*Beta << " " <<
    // sqrt(1.-Beta*Beta) << " " << 1./sqrt(1.-Beta*Beta) << std::endl;

    return EDC;
  }
}

////////////////////////////////////////////////////////////////////////////////
//	tranform an integer to a string
double ExogamDetector::fEXO_E(const unsigned int& i) {
  static string name;
  name = "EXO_E";
  name += nptool::itoa(m_RawData->GetExoCrystal(i));
  return m_Cal.ApplyCalibration(name, m_RawData->GetExoE(i), 1);
  // NOTE! This results in no data if the calibration has failed!
}

////////////////////////////////////////////////////////////////////////////////
double ExogamDetector::fEXO_EHG(const unsigned int& i) {
  static string name;
  name = "EXO_EHG";
  name += nptool::itoa(m_RawData->GetExoCrystal(i));
  return m_Cal.ApplyCalibration(name, m_RawData->GetExoEHG(i), 1);
  // NOTE! This results in no data if the calibration has failed!
}

////////////////////////////////////////////////////////////////////////////////
double ExogamDetector::fEXO_T(const unsigned int& i) {
  static string name;
  name = "EXOGAM_Cr_";
  name += nptool::itoa(m_RawData->GetExoCrystal(i));
  name += "_TDC";
  return m_Cal.ApplyCalibration(name, m_RawData->GetExoTDC(i), 1);
  // NOTE! This results in no data if the calibration has failed!
}

////////////////////////////////////////////////////////////////////////////////
double ExogamDetector::fEXO_Outer(const unsigned int& i,
                                  const unsigned int OuterNumber) {
  static string name;
  name = "EXO_Outer";
  name += nptool::itoa(m_RawData->GetExoCrystal(i));
  name += "_";
  name += nptool::itoa(OuterNumber);
  if (OuterNumber == 0)
    return m_Cal.ApplyCalibration(name, m_RawData->GetExoOuter1(i), 1);
  else if (OuterNumber == 1)
    return m_Cal.ApplyCalibration(name, m_RawData->GetExoOuter2(i), 1);
  else if (OuterNumber == 2)
    return m_Cal.ApplyCalibration(name, m_RawData->GetExoOuter3(i), 1);
  else if (OuterNumber == 3)
    return m_Cal.ApplyCalibration(name, m_RawData->GetExoOuter4(i), 1);
  else {
    std::cout << "WARNING: Outer number != 0-3, something is wrong\n";
    return 0;
  };
  // NOTE! This results in no data if the calibration has failed!
}

////////////////////////////////////////////////////////////////////////////////
int ExogamDetector::GetMaxOuter(unsigned int EventId) {
  double OuterMax = 0;
  int OuterId = -1;
  if (m_RawData->GetExoOuter1(EventId) > OuterMax) {
    OuterMax = m_RawData->GetExoOuter1(EventId);
    OuterId = 0;
  }
  if (m_RawData->GetExoOuter2(EventId) > OuterMax) {
    OuterMax = m_RawData->GetExoOuter2(EventId);
    OuterId = 1;
  }
  if (m_RawData->GetExoOuter3(EventId) > OuterMax) {
    OuterMax = m_RawData->GetExoOuter3(EventId);
    OuterId = 2;
  }
  if (m_RawData->GetExoOuter4(EventId) > OuterMax) {
    OuterMax = m_RawData->GetExoOuter4(EventId);
    OuterId = 3;
  }
  return OuterId;
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::InitializeDataInputRaw(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("exogam", "exogam::ExogamData", &m_RawData);
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::InitializeDataInputPhysics(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("exogam", "exogam::ExogamPhysics", &m_PhysicsData);
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::InitializeDataOutputRaw(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("exogam", "exogam::ExogamData", &m_RawData);
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::InitializeDataOutputPhysics(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("exogam", "exogam::ExogamPhysics", &m_PhysicsData);
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::InitSpectra() {
  m_Spectra = std::make_shared<exogam::ExogamSpectra>();
};

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::FillSpectra() {
  auto app = nptool::Application::GetApplication();
  if (app->HasFlag("--input-raw")) m_Spectra->FillRaw();
  if (app->HasFlag("--input-phy")) m_Spectra->FillPhy();
};

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::SetCloverActive(int flange) {
  cout << "Activating clover in flange " << flange << endl;
  if (!IsCloverActive[flange]) {
    IsCloverActive[flange] = true;
    IsFlangeCrystalActive[flange][0] = true;
    IsFlangeCrystalActive[flange][1] = true;
    IsFlangeCrystalActive[flange][2] = true;
    IsFlangeCrystalActive[flange][3] = true;
  } else {
    nptool::message("yellow", "exogam", "ExogamDetector::SetCloverActive()",
                    "Tried to activate clover " + to_string(flange) +
                        " but it is already activated");
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::SetCloverInactive(int flange) {
  cout << "Dectivating clover in flange " << flange << endl;
  if (IsCloverActive[flange]) {
    IsCloverActive[flange] = false;
    IsFlangeCrystalActive[flange][0] = false;
    IsFlangeCrystalActive[flange][1] = false;
    IsFlangeCrystalActive[flange][2] = false;
    IsFlangeCrystalActive[flange][3] = false;
  } else {
    nptool::message("yellow", "exogam", "ExogamDetector::SetCloverInactive()",
                    "Tried to deactivate clover " + to_string(flange) +
                        " but it is already deactivated");
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::SetCrystalActive(int rawcrystal) {
  cout << "Activating crystal " << rawcrystal << " (flange "
       << MapCrystalFlangeClover[rawcrystal].first << ", crys "
       << MapCrystalFlangeClover[rawcrystal].second << ")" << endl;

  if (!IsCrystalActive[rawcrystal]) {
    IsCrystalActive[rawcrystal] = true;
    IsFlangeCrystalActive[MapCrystalFlangeClover[rawcrystal].first]
                         [MapCrystalFlangeClover[rawcrystal].second] = true;
    IsSegmentActive[rawcrystal][0] = true;
    IsSegmentActive[rawcrystal][1] = true;
    IsSegmentActive[rawcrystal][2] = true;
    IsSegmentActive[rawcrystal][3] = true;

  } else {
    nptool::message(
        "yellow", "exogam", "ExogamDetector::SetCrystalActive()",
        "Tried to activate crystal " + to_string(rawcrystal) + " (flange " +
            to_string(MapCrystalFlangeClover[rawcrystal].first) + " crystal " +
            to_string(MapCrystalFlangeClover[rawcrystal].second) +
            ") but it is already activated");
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::SetCrystalInactive(int rawcrystal) {
  cout << "Deactivating crystal " << rawcrystal << " (flange "
       << MapCrystalFlangeClover[rawcrystal].first << ", crys "
       << MapCrystalFlangeClover[rawcrystal].second << ")" << endl;

  if (IsCrystalActive[rawcrystal]) {
    IsCrystalActive[rawcrystal] = false;
    IsFlangeCrystalActive[MapCrystalFlangeClover[rawcrystal].first]
                         [MapCrystalFlangeClover[rawcrystal].second] = false;
    IsSegmentActive[rawcrystal][0] = false;
    IsSegmentActive[rawcrystal][1] = false;
    IsSegmentActive[rawcrystal][2] = false;
    IsSegmentActive[rawcrystal][3] = false;
  } else {
    nptool::message(
        "yellow", "exogam", "ExogamDetector::SetCrystalInactive()",
        "Tried to deactivate crystal " + to_string(rawcrystal) + " (flange " +
            to_string(MapCrystalFlangeClover[rawcrystal].first) + " crystal " +
            to_string(MapCrystalFlangeClover[rawcrystal].second) +
            ") but it is already deactivated");
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::SetSegmentActive(int rawcrystal, int seg) {
  cout << "Activating segment " << rawcrystal << "-" << seg << " (flange "
       << MapCrystalFlangeClover[rawcrystal].first << ", crys "
       << MapCrystalFlangeClover[rawcrystal].second << ", seg " << seg << ")"
       << endl;

  if (!IsSegmentActive[rawcrystal][seg - 1]) {
    IsSegmentActive[rawcrystal][seg - 1] = true;
  } else {
    nptool::message("yellow", "exogam", "ExogamDetector::SetSegmentActive()",
                    "Tried to activate segment " + to_string(rawcrystal) + "-" +
                        to_string(seg) + "but it is already activated");
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::SetSegmentInactive(int rawcrystal, int seg) {
  cout << "Deactivating segment " << rawcrystal << "-" << seg << " (flange "
       << MapCrystalFlangeClover[rawcrystal].first << ", crys "
       << MapCrystalFlangeClover[rawcrystal].second << ", seg " << seg << ")"
       << endl;

  if (IsSegmentActive[rawcrystal][seg - 1]) {
    IsSegmentActive[rawcrystal][seg - 1] = false;
  } else {
    nptool::message("yellow", "exogam", "ExogamDetector::SetSegmentInactive()",
                    "Tried to deactivate segment " + to_string(rawcrystal) +
                        "-" + to_string(seg) + "but it is already deactivated");
  }
}

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::WriteSpectra(){};

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::CheckSpectra(){};

////////////////////////////////////////////////////////////////////////////////
void ExogamDetector::ClearSpectra() { m_Spectra->Clear(); };

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
extern "C" {
shared_ptr<nptool::VDetector> ConstructDetector() {
  return make_shared<exogam::ExogamDetector>();
};
}

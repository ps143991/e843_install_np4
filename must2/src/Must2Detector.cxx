#include "Must2Detector.h"
#include "Must2Spectra.h"

#include "NPFunction.h"
#include "NPRootPlugin.h"
#include <iostream>

// For Geant4 Simulation
#ifdef Geant4_FOUND
#include "Must2Geant4.h"
#endif

// std
#include <dlfcn.h>

using namespace must2;
using namespace std;
using namespace ROOT::Math;

////////////////////////////////////////////////////////////////////////////////
Must2Detector::Must2Detector() {
  m_RawData     = new must2::Must2Data();
  m_CalData     = new must2::Must2Data();
  m_PhysicsData = new must2::Must2Physics();
  m_Cal.InitCalibration();
  m_NumberOfTelescope                = 0;
  m_MaximumStripMultiplicityAllowed  = 10;
  m_StripEnergyMatchingSigma         = 0.020;
  m_StripEnergyMatchingNumberOfSigma = 3;
  // Raw Threshold
  m_Si_X_E_RAW_Threshold = 8192;
  m_Si_Y_E_RAW_Threshold = 8192;
  m_SiLi_E_RAW_Threshold = 8192;
  m_CsI_E_RAW_Threshold  = 8210;
  // m_CsI_E_RAW_Threshold = 0;
  // Calibrated Threshold
  m_Si_X_E_Threshold = 0;
  m_Si_Y_E_Threshold = 0;
  m_SiLi_E_Threshold = 0;
  m_CsI_E_Threshold  = 6;

  m_Ignore_not_matching_SiLi = false;
  m_Ignore_not_matching_CsI  = false;

  m_Take_E_Y = false;
  m_Take_T_Y = true;

  //////////////////
  // SiLi matching //
  //////////////////

  m_SiLi_Size = 32;
  m_SiLi_MatchingX.resize(16, 0);
  m_SiLi_MatchingY.resize(16, 0);

  m_SiLi_MatchingX[0] = 112;
  m_SiLi_MatchingY[0] = 112;

  m_SiLi_MatchingX[1] = 112;
  m_SiLi_MatchingY[1] = 80;

  m_SiLi_MatchingX[2] = 80;
  m_SiLi_MatchingY[2] = 112;

  m_SiLi_MatchingX[3] = 80;
  m_SiLi_MatchingY[3] = 80;
  //
  m_SiLi_MatchingX[4] = 48;
  m_SiLi_MatchingY[4] = 80;

  m_SiLi_MatchingX[5] = 48;
  m_SiLi_MatchingY[5] = 112;

  m_SiLi_MatchingX[6] = 16;
  m_SiLi_MatchingY[6] = 80;

  m_SiLi_MatchingX[7] = 16;
  m_SiLi_MatchingY[7] = 112;
  //
  m_SiLi_MatchingX[8] = 112;
  m_SiLi_MatchingY[8] = 16;

  m_SiLi_MatchingX[9] = 112;
  m_SiLi_MatchingY[9] = 48;

  m_SiLi_MatchingX[10] = 80;
  m_SiLi_MatchingY[10] = 16;

  m_SiLi_MatchingX[11] = 80;
  m_SiLi_MatchingY[11] = 48;
  //
  m_SiLi_MatchingX[12] = 48;
  m_SiLi_MatchingY[12] = 48;

  m_SiLi_MatchingX[13] = 48;
  m_SiLi_MatchingY[13] = 16;

  m_SiLi_MatchingX[14] = 16;
  m_SiLi_MatchingY[14] = 48;

  m_SiLi_MatchingX[15] = 16;
  m_SiLi_MatchingY[15] = 16;

  //////////////////
  // CsI matching //
  //////////////////

  m_CsI_Size = 32;
  m_CsI_MatchingX.resize(16, 0);
  m_CsI_MatchingY.resize(16, 0);

  m_CsI_MatchingX[0] = 112;
  m_CsI_MatchingY[0] = 112;

  m_CsI_MatchingX[1] = 112;
  m_CsI_MatchingY[1] = 80;

  m_CsI_MatchingX[2] = 112;
  m_CsI_MatchingY[2] = 48;

  m_CsI_MatchingX[3] = 112;
  m_CsI_MatchingY[3] = 16;
  //
  m_CsI_MatchingX[4] = 80;
  m_CsI_MatchingY[4] = 16;

  m_CsI_MatchingX[5] = 80;
  m_CsI_MatchingY[5] = 48;

  m_CsI_MatchingX[6] = 80;
  m_CsI_MatchingY[6] = 80;

  m_CsI_MatchingX[7] = 80;
  m_CsI_MatchingY[7] = 112;
  //
  m_CsI_MatchingX[8] = 48;
  m_CsI_MatchingY[8] = 16;

  m_CsI_MatchingX[9] = 48;
  m_CsI_MatchingY[9] = 48;

  m_CsI_MatchingX[10] = 48;
  m_CsI_MatchingY[10] = 80;

  m_CsI_MatchingX[11] = 48;
  m_CsI_MatchingY[11] = 112;
  //
  m_CsI_MatchingX[12] = 16;
  m_CsI_MatchingY[12] = 16;

  m_CsI_MatchingX[13] = 16;
  m_CsI_MatchingY[13] = 48;

  m_CsI_MatchingX[14] = 16;
  m_CsI_MatchingY[14] = 80;

  m_CsI_MatchingX[15] = 16;
  m_CsI_MatchingY[15] = 112;

  m_CsI_MatchingX[0] = 112;
  m_CsI_MatchingY[0] = 112;

  m_CsI_MatchingX[1] = 112;
  m_CsI_MatchingY[1] = 80;

  m_CsI_MatchingX[2] = 112;
  m_CsI_MatchingY[2] = 48;

  m_CsI_MatchingX[3] = 112;
  m_CsI_MatchingY[3] = 16;
  //
  m_CsI_MatchingX[4] = 80;
  m_CsI_MatchingY[4] = 16;

  m_CsI_MatchingX[5] = 80;
  m_CsI_MatchingY[5] = 48;

  m_CsI_MatchingX[6] = 80;
  m_CsI_MatchingY[6] = 80;

  m_CsI_MatchingX[7] = 80;
  m_CsI_MatchingY[7] = 112;
  //
  m_CsI_MatchingX[8] = 48;
  m_CsI_MatchingY[8] = 16;

  m_CsI_MatchingX[9] = 48;
  m_CsI_MatchingY[9] = 48;

  m_CsI_MatchingX[10] = 48;
  m_CsI_MatchingY[10] = 80;

  m_CsI_MatchingX[11] = 48;
  m_CsI_MatchingY[11] = 112;
  //
  m_CsI_MatchingX[12] = 16;
  m_CsI_MatchingY[12] = 16;

  m_CsI_MatchingX[13] = 16;
  m_CsI_MatchingY[13] = 48;

  m_CsI_MatchingX[14]      = 16;
  m_CsI_MatchingY[14]      = 80;
  faction_file_initialized = false;
}

Must2Detector::~Must2Detector() {}

////////////////////////////////////////////////////////////////////////////////
void Must2Detector::ReadConfiguration(nptool::InputParser parser) {

  faction_file_initialized = false;
  auto blocks              = parser.GetAllBlocksWithToken("must2");
  // Cartesian Case
  vector<string> cart = {"X001_Y001", "X001_Y128", "X128_Y001", "X128_Y128",
                         "SI",        "SILI",      "CSI"};
  // Spherical Case
  vector<string> sphe = {"R", "THETA", "PHI", "BETA", "SI", "SILI", "CSI"};

  int number = 0;
  for (auto block : blocks) {
    if (block->HasTokenList(cart)) {
      nptool::message("green", "", "", "Must2 Telescope");
      if (block->HasToken("number"))
        number = block->GetInt("number");
      else
        number++;
      auto A = nptool::ConvertXYZVector(block->GetVector3("X001_Y001", "mm"));
      auto B = nptool::ConvertXYZVector(block->GetVector3("X128_Y001", "mm"));
      auto C = nptool::ConvertXYZVector(block->GetVector3("X001_Y128", "mm"));
      auto D = nptool::ConvertXYZVector(block->GetVector3("X128_Y128", "mm"));
      AddTelescope(A, B, C, D);
      m_SiPresent[number]   = block->GetInt("SI");
      m_CsIPresent[number]  = block->GetInt("CSI");
      m_SiLiPresent[number] = block->GetInt("SILI");

      // if (block->HasToken("ActionFile") && !faction_file_initialized) {
      //   ReadActionFile(block->GetString("ActionFile"));
      // }
    } else {
      cout << "ERROR: Missing token for M2Telescope blocks, check your "
              "input "
              "file"
           << endl;
      exit(1);
    }
  }

  // get_action_file
  auto app = nptool::Application::GetApplication();
  if (app->HasFlag("--action")) {
    auto arg = app->GetVectorArg("--action");
    std::cout << "MUST2 ACTION FILE :"<< arg[0] << std::endl;
    if (arg.size()) {
      ReadActionFile(arg[0]);
    }
  }

  InitializeStandardParameter();
  ReadAnalysisConfig();
}

///////////////////////////////////////////////////////////////////////////

void Must2Detector::InitializeStandardParameter() {
  //   Enable all channel
  vector<bool> ChannelStatus;
  m_XChannelStatus.clear();
  m_YChannelStatus.clear();
  m_SiLiChannelStatus.clear();
  m_CsIChannelStatus.clear();

  ChannelStatus.resize(128, true);
  for (int i = 0; i < m_NumberOfTelescope; ++i) {
    m_XChannelStatus[i] = ChannelStatus;
    m_YChannelStatus[i] = ChannelStatus;
  }

  ChannelStatus.resize(16, true);
  for (int i = 0; i < m_NumberOfTelescope; ++i) {
    m_SiLiChannelStatus[i] = ChannelStatus;
    m_CsIChannelStatus[i]  = ChannelStatus;
  }

  m_MaximumStripMultiplicityAllowed = m_NumberOfTelescope;

  return;
}

///////////////////////////////////////////////////////////////////////////
void Must2Detector::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigMust2.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigMust2.dat found: Default parameters loaded for "
            "Analysis "
         << FileName << endl;
    return;
  }
  cout << " Loading user parameters for Analysis from ConfigMust2.dat " << endl;

  // // Save it in a TAsciiFile
  // TAsciiFile* asciiConfig =
  // RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  // asciiConfig->AppendLine("%%% ConfigMust2.dat %%%");
  // asciiConfig->Append(FileName.c_str());
  // asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer, DataBuffer, whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 11, "ConfigMust2") == 0)
      ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus) {

      whatToDo = "";
      AnalysisConfigFile >> whatToDo;
      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n');
      }

      else if (whatToDo == "MAX_STRIP_MULTIPLICITY") {
        AnalysisConfigFile >> DataBuffer;
        m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str());
        cout << "MAXIMUN STRIP MULTIPLICITY "
             << m_MaximumStripMultiplicityAllowed << endl;
      }

      else if (whatToDo == "STRIP_ENERGY_MATCHING_SIGMA") {
        AnalysisConfigFile >> DataBuffer;
        m_StripEnergyMatchingSigma = atof(DataBuffer.c_str());
        cout << "STRIP ENERGY MATCHING SIGMA " << m_StripEnergyMatchingSigma
             << endl;
      }

      else if (whatToDo == "STRIP_ENERGY_MATCHING_NUMBER_OF_SIGMA") {
        AnalysisConfigFile >> DataBuffer;
        m_StripEnergyMatchingNumberOfSigma = atoi(DataBuffer.c_str());
        cout << "STRIP ENERGY MATCHING NUMBER OF SIGMA "
             << m_StripEnergyMatchingNumberOfSigma << endl;
      }

      else if (whatToDo == "DISABLE_ALL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int          telescope = atoi(DataBuffer.substr(2, 1).c_str());
        vector<bool> ChannelStatus;
        ChannelStatus.resize(128, false);
        m_XChannelStatus[telescope - 1] = ChannelStatus;
        m_YChannelStatus[telescope - 1] = ChannelStatus;
        ChannelStatus.resize(16, false);
        m_SiLiChannelStatus[telescope - 1] = ChannelStatus;
        m_CsIChannelStatus[telescope - 1]  = ChannelStatus;
      }

      else if (whatToDo == "DISABLE_CHANNEL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int telescope = atoi(DataBuffer.substr(2, 1).c_str());
        int channel   = -1;
        if (DataBuffer.compare(3, 4, "STRX") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_XChannelStatus[telescope - 1].begin() + channel - 1) = false;
        }

        else if (DataBuffer.compare(3, 4, "STRY") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_YChannelStatus[telescope - 1].begin() + channel - 1) = false;
        }

        else if (DataBuffer.compare(3, 4, "SILI") == 0) {
          channel = atoi(DataBuffer.substr(7).c_str());
          *(m_SiLiChannelStatus[telescope - 1].begin() + channel - 1) = false;
        }

        else if (DataBuffer.compare(3, 3, "CSI") == 0) {
          channel = atoi(DataBuffer.substr(6).c_str());
          *(m_CsIChannelStatus[telescope - 1].begin() + channel - 1) = false;
        }

        else
          cout << "Warning: detector type for Must2 unknown!" << endl;
      }

      else if (whatToDo == "TAKE_E_Y") {
        m_Take_E_Y = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo == "TAKE_T_Y") {
        m_Take_T_Y = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo == "TAKE_E_X") {
        m_Take_E_Y = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo == "TAKE_T_X") {
        m_Take_T_Y = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo == "IGNORE_NOT_MATCHING_CSI") {
        m_Ignore_not_matching_CsI = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo == "CSI_SIZE") {
        AnalysisConfigFile >> DataBuffer;
        m_CsI_Size = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_CsI_Size << endl;
      }

      else if (whatToDo == "IGNORE_NOT_MATCHING_SILI") {
        m_Ignore_not_matching_SiLi = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo == "SILI_SIZE") {
        AnalysisConfigFile >> DataBuffer;
        m_SiLi_Size = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_SiLi_Size << endl;
      }

      else if (whatToDo == "SI_X_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Si_X_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Si_X_E_RAW_Threshold << endl;
      }

      else if (whatToDo == "SI_Y_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Si_Y_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Si_Y_E_RAW_Threshold << endl;
      }

      else if (whatToDo == "SILI_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_SiLi_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_SiLi_E_RAW_Threshold << endl;
      }

      else if (whatToDo == "CSI_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_CsI_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_CsI_E_Threshold << endl;
      }

      else if (whatToDo == "SI_X_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Si_X_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Si_X_E_Threshold << endl;
      }

      else if (whatToDo == "SI_Y_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Si_Y_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Si_Y_E_Threshold << endl;
      }

      else if (whatToDo == "SILI_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_SiLi_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_SiLi_E_Threshold << endl;
      }

      else if (whatToDo == "CSI_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_CsI_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_CsI_E_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void Must2Detector::ReadActionFile(std::string ActionFile) {
  faction_file_initialized = true;
  std::string LabelName;
  ifstream    file(ActionFile, std::ifstream::in);
  int         Label, c, d;
  if (!file)
    exit(1);
  while (file >> LabelName >> Label >> c >> d) {
    fLabelMap[Label] = LabelName;
  }
}

////////////////////////////////////////////////////////////////////////////////
void Must2Detector::AddTelescope(XYZVector C_X1_Y1, XYZVector C_X128_Y1,
                                 XYZVector C_X1_Y128, XYZVector C_X128_Y128) {

  m_X1_Y1.push_back(C_X1_Y1);
  m_X128_Y1.push_back(C_X128_Y1);
  m_X1_Y128.push_back(C_X1_Y128);
  m_X128_Y128.push_back(C_X128_Y128);

  // To avoid warning
  C_X128_Y128 *= 1;

  m_NumberOfTelescope++;

  //   Vector U on Telescope Face (paralelle to Y Strip) (NB: remember that
  //   Y strip are allong X axis)
  XYZVector U      = C_X128_Y1 - C_X1_Y1;
  double    Ushift = (U.r() - 98) / 2.;
  U                = U.Unit();
  //   Vector V on Telescope Face (parallele to X Strip)
  XYZVector V      = C_X1_Y128 - C_X1_Y1;
  double    Vshift = (V.r() - 98) / 2.;
  V                = V.Unit();

  //   Position Vector of Strip Center
  XYZVector StripCenter = XYZVector(0, 0, 0);
  //   Position Vector of X=1 Y=1 Strip
  XYZVector Strip_1_1;

  //   Geometry Parameter
  double Face          = 98; // mm
  double NumberOfStrip = 128;
  double StripPitch    = Face / NumberOfStrip; // mm
  //   Buffer object to fill Position Array
  vector<double> lineX;
  vector<double> lineY;
  vector<double> lineZ;

  vector<vector<double>> OneTelescopeStripPositionX;
  vector<vector<double>> OneTelescopeStripPositionY;
  vector<vector<double>> OneTelescopeStripPositionZ;

  //   Moving StripCenter to 1.1 corner:
  Strip_1_1 = C_X1_Y1 + (U + V) * (StripPitch / 2.);
  Strip_1_1 += U * Ushift + V * Vshift;

  for (int i = 0; i < 128; ++i) {
    lineX.clear();
    lineY.clear();
    lineZ.clear();

    for (int j = 0; j < 128; ++j) {
      StripCenter = Strip_1_1 + StripPitch * (i * U + j * V);
      lineX.push_back(StripCenter.X());
      lineY.push_back(StripCenter.Y());
      lineZ.push_back(StripCenter.Z());
    }

    OneTelescopeStripPositionX.push_back(lineX);
    OneTelescopeStripPositionY.push_back(lineY);
    OneTelescopeStripPositionZ.push_back(lineZ);
  }
  m_StripPositionX.push_back(OneTelescopeStripPositionX);
  m_StripPositionY.push_back(OneTelescopeStripPositionY);
  m_StripPositionZ.push_back(OneTelescopeStripPositionZ);
}

void Must2Detector::BuildRawEvent(const std::string& daq_name,
                                  const std::string& st_type_key,
                                  void*              commonframe) {
#ifdef MFM_FOUND
  int type_key = ((MFMCommonFrame*)commonframe)->GetFrameType();
  if (type_key == MFM_EBY_EN_TS_FRAME_TYPE) {
    TreatFrame((MFMCommonFrame*)commonframe);
  }
#endif
}

void Must2Detector::TreatFrame(void* commonframe) {
#ifdef MFM_FOUND
  unsigned short     NItems = 0;
  unsigned short     value, label_id;
  std::string        label;
  unsigned short     LblData[2];
  unsigned long long TS;

  std::shared_ptr<MFMEbyedatFrame> EbyEframe
      = std::make_shared<MFMEbyedatFrame>();
  EbyEframe->SetAttributs(((MFMCommonFrame*)commonframe)->GetPointHeader());

  NItems = EbyEframe->GetNbItems();
  TS     = EbyEframe->GetTimeStamp();
  if (TS == 0) {
    std::cout << "/////////////////////////////" << std::endl;
    std::cout << "   MUST2 WARNING TS IS 0     " << std::endl;
    std::cout << "/////////////////////////////" << std::endl;
  }

  for (unsigned short i = 0; i < NItems; i++) {
    EbyEframe->EbyedatGetParameters(i, &label_id, &value);
    LblData[0] = label_id;
    LblData[1] = value;

    label = fLabelMap[label_id];

    if (label.compare(0, 2, "MM") == 0) {
      double det = atoi(label.substr(2, 1).c_str());
      double channel;
      if (label.compare(4, 6, "STRX_E") == 0) {
        channel = atoi(label.substr(10).c_str());
        m_RawData->SetStripXE(det, channel, value);
      }

      else if (label.compare(4, 6, "STRX_T") == 0) {
        channel = atoi(label.substr(10).c_str());
        m_RawData->SetStripXT(det, channel, value);
      }

      else if (label.compare(4, 6, "STRY_E") == 0) {
        channel = atoi(label.substr(10).c_str());
        m_RawData->SetStripYE(det, channel, value);
      }

      else if (label.compare(4, 6, "STRY_T") == 0) {
        channel = atoi(label.substr(10).c_str());
        m_RawData->SetStripYT(det, channel, value);
      }

      else if (label.compare(4, 6, "SILI_E") == 0) {
        channel = atoi(label.substr(10).c_str());
        m_RawData->SetSiLiE(det, channel, value);
      }

      else if (label.compare(4, 6, "SILI_T") == 0) {
        channel = atoi(label.substr(10).c_str());
        m_RawData->SetSiLiT(det, channel, value);
      }

      else if (label.compare(4, 5, "CSI_E") == 0) {
        channel = atoi(label.substr(9).c_str());
        m_RawData->SetCsIE(det, channel, value);
      }

      else if (label.compare(4, 5, "CSI_T") == 0) {
        channel = atoi(label.substr(9).c_str());
        m_RawData->SetCsIT(det, channel, value);
      }
      m_RawData->SetMMTS(TS);
    }
  }
#endif
}

void Must2Detector::PreTreat() {
  ClearPreTreatedData();
  m_StripXEMult = m_RawData->GetMMStripXEMult();
  m_StripYEMult = m_RawData->GetMMStripYEMult();
  m_StripXTMult = m_RawData->GetMMStripXTMult();
  m_StripYTMult = m_RawData->GetMMStripYTMult();
  m_SiLiEMult   = m_RawData->GetMMSiLiEMult();
  m_SiLiTMult   = m_RawData->GetMMSiLiTMult();
  m_CsIEMult    = m_RawData->GetMMCsIEMult();
  m_CsITMult    = m_RawData->GetMMCsITMult();
  map<int, int> hitX;
  map<int, int> hitY;

  //   X
  //   E
  for (unsigned int i = 0; i < m_StripXEMult; ++i) {
    if (m_RawData->GetMMStripXEEnergy(i) > m_Si_X_E_RAW_Threshold
        && IsValidChannel(0, m_RawData->GetMMStripXEDetectorNbr(i),
                          m_RawData->GetMMStripXEStripNbr(i))) {
      double EX = fSi_X_E(i);
      if (EX > m_Si_X_E_Threshold)
        m_CalData->SetStripXE(m_RawData->GetMMStripXEDetectorNbr(i),
                              m_RawData->GetMMStripXEStripNbr(i), EX);
    }
  }

  //   T
  for (unsigned int i = 0; i < m_StripXTMult; ++i) {
    if (IsValidChannel(0, m_RawData->GetMMStripXTDetectorNbr(i),
                       m_RawData->GetMMStripXTStripNbr(i)))
      m_CalData->SetStripXT(m_RawData->GetMMStripXTDetectorNbr(i),
                            m_RawData->GetMMStripXTStripNbr(i), fSi_X_T(i));
  }

  //   Y
  //   E
  for (unsigned int i = 0; i < m_StripYEMult; ++i) {
    if (m_RawData->GetMMStripYEEnergy(i) < m_Si_Y_E_RAW_Threshold
        && IsValidChannel(1, m_RawData->GetMMStripYEDetectorNbr(i),
                          m_RawData->GetMMStripYEStripNbr(i))) {
      double EY = fSi_Y_E(i);
      if (EY > m_Si_Y_E_Threshold)
        m_CalData->SetStripYE(m_RawData->GetMMStripYEDetectorNbr(i),
                              m_RawData->GetMMStripYEStripNbr(i), EY);
    }
  }
  //   T
  for (unsigned int i = 0; i < m_StripYTMult; ++i) {
    if (IsValidChannel(1, m_RawData->GetMMStripYTDetectorNbr(i),
                       m_RawData->GetMMStripYTStripNbr(i)))
      m_CalData->SetStripYT(m_RawData->GetMMStripYTDetectorNbr(i),
                            m_RawData->GetMMStripYTStripNbr(i), fSi_Y_T(i));
  }

  //   CsI
  //   E
  for (unsigned int i = 0; i < m_CsIEMult; ++i) {
    if (m_RawData->GetMMCsIEEnergy(i) > m_CsI_E_RAW_Threshold
        && IsValidChannel(3, m_RawData->GetMMCsIEDetectorNbr(i),
                          m_RawData->GetMMCsIECristalNbr(i))) {
      // Implementing special CSI E treatment: to get a calibrated and non
      // calibrated branch in the analysis, the calibration is applied later,
      // but the threshold is still checked here
      double ECsI = fCsI_E(i);
      if (ECsI > m_CsI_E_Threshold) {
        m_CalData->SetCsIE(m_RawData->GetMMCsIEDetectorNbr(i),
                           m_RawData->GetMMCsIECristalNbr(i),
                           m_RawData->GetMMCsIEEnergy(i));
        // std::cout << m_RawData->GetMMCsIEMult() << " " <<
        // m_CalData->GetMMCsIEMult() << std::endl;
      }
    }
  }

  //   T
  for (unsigned int i = 0; i < m_CsITMult; ++i) {
    if (IsValidChannel(3, m_RawData->GetMMCsITDetectorNbr(i),
                       m_RawData->GetMMCsITCristalNbr(i)))
      m_CalData->SetCsIT(m_RawData->GetMMCsITDetectorNbr(i),
                         m_RawData->GetMMCsITCristalNbr(i), fCsI_T(i));
  }

  //   SiLi
  //   E
  for (unsigned int i = 0; i < m_SiLiEMult; ++i) {
    if (m_RawData->GetMMSiLiEEnergy(i) > m_SiLi_E_RAW_Threshold
        && IsValidChannel(2, m_RawData->GetMMSiLiEDetectorNbr(i),
                          m_RawData->GetMMSiLiEPadNbr(i))) {
      double ESiLi = fSiLi_E(i);
      if (ESiLi > m_SiLi_E_Threshold)
        m_CalData->SetSiLiE(m_RawData->GetMMSiLiEDetectorNbr(i),
                            m_RawData->GetMMSiLiEPadNbr(i), ESiLi);
    }
  }

  //   T
  for (unsigned int i = 0; i < m_SiLiTMult; ++i) {
    if (IsValidChannel(2, m_RawData->GetMMSiLiTDetectorNbr(i),
                       m_RawData->GetMMSiLiTPadNbr(i)))
      m_CalData->SetSiLiT(m_RawData->GetMMSiLiTDetectorNbr(i),
                          m_RawData->GetMMSiLiTPadNbr(i), fSiLi_T(i));
  }

  return;
}

TVector3 Must2Detector::GetPositionOfInteraction(const int i) const {
  TVector3 Position = TVector3(
      GetStripPositionX(m_PhysicsData->TelescopeNumber[i],
                        m_PhysicsData->Si_X[i], m_PhysicsData->Si_Y[i]),
      GetStripPositionY(m_PhysicsData->TelescopeNumber[i],
                        m_PhysicsData->Si_X[i], m_PhysicsData->Si_Y[i]),
      GetStripPositionZ(m_PhysicsData->TelescopeNumber[i],
                        m_PhysicsData->Si_X[i], m_PhysicsData->Si_Y[i]));

  return (Position);
}

TVector3 Must2Detector::GetTelescopeNormal(const int i) const {
  TVector3 U
      = TVector3(GetStripPositionX(m_PhysicsData->TelescopeNumber[i], 128, 1),
                 GetStripPositionY(m_PhysicsData->TelescopeNumber[i], 128, 1),
                 GetStripPositionZ(m_PhysicsData->TelescopeNumber[i], 128, 1))
        - TVector3(GetStripPositionX(m_PhysicsData->TelescopeNumber[i], 1, 1),
                   GetStripPositionY(m_PhysicsData->TelescopeNumber[i], 1, 1),
                   GetStripPositionZ(m_PhysicsData->TelescopeNumber[i], 1, 1));

  TVector3 V
      = TVector3(GetStripPositionX(m_PhysicsData->TelescopeNumber[i], 128, 128),
                 GetStripPositionY(m_PhysicsData->TelescopeNumber[i], 128, 128),
                 GetStripPositionZ(m_PhysicsData->TelescopeNumber[i], 128, 128))
        - TVector3(
            GetStripPositionX(m_PhysicsData->TelescopeNumber[i], 128, 1),
            GetStripPositionY(m_PhysicsData->TelescopeNumber[i], 128, 1),
            GetStripPositionZ(m_PhysicsData->TelescopeNumber[i], 128, 1));

  TVector3 Normal = U.Cross(V);

  return (Normal.Unit());
}

std::vector<std::pair<int, std::pair<unsigned int, unsigned int>>>
Must2Detector::Match_Si_CsI(std::vector<std::pair<unsigned int, unsigned int>>
                                Array_of_good_XY_couples) {
  // New Match Si_CsI->Takes into account possible events with an overlap in 2
  // CsI vector of matching pairs: first is CsI index, second is the pair of XY
  // indexes
  std::vector<std::pair<int, std::pair<unsigned int, unsigned int>>>
      Array_of_matches;
  Array_of_matches.reserve(Array_of_good_XY_couples.size());
  std::vector<std::vector<
      std::pair<int, std::pair<unsigned int, unsigned int>>>::iterator>
      Wrong_matches;
  for (unsigned int i = 0; i < m_CalData->GetMMCsIEMult(); i++) {
    for (auto couple : Array_of_good_XY_couples) {
      unsigned int Xindex = couple.first;
      unsigned int Yindex = couple.second;
      // a couple is already matched in detector
      unsigned int XYdet  = m_CalData->GetMMStripXEDetectorNbr(Xindex);
      unsigned int Xstrip = m_CalData->GetMMStripXEStripNbr(Xindex);
      unsigned int Ystrip = m_CalData->GetMMStripYEStripNbr(Yindex);

      unsigned int CsIdet   = m_CalData->GetMMCsIEDetectorNbr(i);
      double       CsIRaw_E = m_CalData->GetMMCsIEEnergy(i);
      if (CsIRaw_E < 8210) {
        cout << "MATCHING energy: " << std::endl;
      }

      // Checking if detnbr matches
      if (XYdet == CsIdet) {
        unsigned int CsINbr = m_CalData->GetMMCsIECristalNbr(i);

        // Checking if the projection of XY position on CsI matches
        if (abs((double)m_CsI_MatchingX[CsINbr - 1] + 0.5 - (double)Xstrip)
                <= (double)m_CsI_Size / 2.
            && abs((double)m_CsI_MatchingY[CsINbr - 1] + 0.5 - (double)Ystrip)
                   <= (double)m_CsI_Size / 2.) {

          // It is a priori possible that 2 XY pairs match with the same CsI (in
          // particular with large CsI sizes) We need to check if 2 XY pairs
          // match with the same CsI, and we then reject the 2 XY pairs AND the
          // CsI event

          // iterator that points to an element of Array_of_matches if, in the
          // same detector, this CsI or this couple has already been matched.
          // Else it points to end.
          auto it = std::find_if(
              Array_of_matches.begin(), Array_of_matches.end(),
              [i, couple](
                  const std::pair<int, std::pair<unsigned int, unsigned int>>&
                      match) {
                return (match.first == i || match.second.first == couple.first
                        || match.second.second == couple.second);
              });

          // if this CsI has not been found, we can add the CsI XY match to
          // Array_of_matches
          if (it == Array_of_matches.end()) {
            Array_of_matches.push_back(std::make_pair(i, couple));
          }
          // if this CsI has been found, the previous match is kept but the
          // associated iterator is saved in a vector to be deleted at the end
          // of the loop
          else {
            // Creating a vector of wrong matches to delete them from the vector
            // after looping over all events
            if (std::find(Wrong_matches.begin(), Wrong_matches.end(), it)
                == Wrong_matches.end())
              Wrong_matches.push_back(it);
          }
        }
      }
    }
  }
  std::sort(Wrong_matches.rbegin(), Wrong_matches.rend());
  for (auto it : Wrong_matches) {
    Array_of_matches.erase(it);
  }
  // However, when we deleted all these wrong matches, we removed events that we
  // cant match well to a CsI (double matches) or events that stop in the Si
  // Removing all these events will make false multiplicities. We add these
  // events again with index -1 to indicate that these events are physical
  // events in DSSD that we cant match to a CsI

  for (auto couple : Array_of_good_XY_couples) {
    // iterator that points to an element of Array_of_matches if an X Y couple
    // has been matched, else it points to end
    auto it = std::find_if(
        Array_of_matches.begin(), Array_of_matches.end(),
        [couple](const std::pair<int, std::pair<unsigned int, unsigned int>>&
                     match) {
          return (match.second.first == couple.first
                  || match.second.second == couple.second);
        });

    if (it == Array_of_matches.end()) {
      Array_of_matches.push_back(std::make_pair(-1, couple));
    }
  }
  return Array_of_matches;
}

std::vector<std::pair<unsigned int, unsigned int>> Must2Detector::Match_X_Y() {
  // Simplified Match_X_Y to get all possible matching energy couples in same
  // detector The ony dubious case is 2 events in the same detector with the
  // same energy, giving 4 possiblilities If one of the matching couples finds a
  // corresponding CsI in Matchi_Si_CsI, then the problem is solved If none of
  // the 4 couples finds a matching CsI, 2 Si only events are added to preserve
  // good multiplicity Thus, the multiplicity given by Match_X_Y is not a
  // physical multiplicity, but a vector of all possible matches The good
  // physical multiplicity is given by Match_Si_CsI
  std::vector<std::pair<unsigned int, unsigned int>> ArrayOfGoodCouple;
  ArrayOfGoodCouple.clear();
  m_StripXEMult      = m_CalData->GetMMStripXEMult();
  m_StripYEMult      = m_CalData->GetMMStripYEMult();
  double matchSigma  = m_StripEnergyMatchingSigma;
  double NmatchSigma = m_StripEnergyMatchingNumberOfSigma;

  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if (m_StripXEMult > m_MaximumStripMultiplicityAllowed
      || m_StripYEMult > m_MaximumStripMultiplicityAllowed) {
    return ArrayOfGoodCouple;
  }
  for (unsigned int i = 0; i < m_StripXEMult; i++) {
    for (unsigned int j = 0; j < m_StripYEMult; j++) {
      // Declaration of variable for clarity
      int StripXDetNbr = m_CalData->GetMMStripXEDetectorNbr(i);
      int StripYDetNbr = m_CalData->GetMMStripYEDetectorNbr(j);
      //   if same detector check energy
      if (StripXDetNbr == StripYDetNbr) {

        int DetNbr = StripXDetNbr;
        // Declaration of variable for clarity
        double StripXEnergy = m_CalData->GetMMStripXEEnergy(i);
        double StripYEnergy = m_CalData->GetMMStripYEEnergy(j);
        //   Look if energy match
        if (abs((StripXEnergy - StripYEnergy) / 2.)
            < NmatchSigma * matchSigma) {
          ArrayOfGoodCouple.push_back(std::make_pair(i, j));
        }
      } // if same detector
    } // loop on StripY Mult
  } // loop on StripX Mult
  return ArrayOfGoodCouple;
}

void Must2Detector::BuildPhysicalEvent() {
  PreTreat();

  std::vector<unsigned int> Raw_Si_X_E;
  std::vector<unsigned int> Raw_Si_Y_E;
  Raw_Si_X_E.clear();
  Raw_Si_Y_E.clear();
  // Treating again raw data to get raw Si energy in the data: should be removed
  // at some point Added because I did cut on wrong calib and now need raw Si to
  // apply CUTS
  for (unsigned int i = 0; i < m_RawData->GetMMStripXEMult(); ++i) {
    if (m_RawData->GetMMStripXEEnergy(i) > m_Si_X_E_RAW_Threshold
        && IsValidChannel(0, m_RawData->GetMMStripXEDetectorNbr(i),
                          m_RawData->GetMMStripXEStripNbr(i))) {
      double EX = fSi_X_E(i);
      if (EX > m_Si_X_E_Threshold)
        Raw_Si_X_E.push_back(m_RawData->GetMMStripXEEnergy(i));
    }
  }

  for (unsigned int i = 0; i < m_RawData->GetMMStripYEMult(); ++i) {
    if (m_RawData->GetMMStripYEEnergy(i) < m_Si_Y_E_RAW_Threshold
        && IsValidChannel(1, m_RawData->GetMMStripYEDetectorNbr(i),
                          m_RawData->GetMMStripYEStripNbr(i))) {
      double EY = fSi_Y_E(i);
      if (EY > m_Si_Y_E_Threshold)
        Raw_Si_Y_E.push_back(m_RawData->GetMMStripYEEnergy(i));
    }
  }

  bool check_SILI = false;
  bool check_CSI  = false;
  m_StripXEMult   = m_CalData->GetMMStripXEMult();
  m_StripYEMult   = m_CalData->GetMMStripYEMult();
  m_StripXTMult   = m_CalData->GetMMStripXTMult();
  m_StripYTMult   = m_CalData->GetMMStripYTMult();
  m_SiLiEMult     = m_CalData->GetMMSiLiEMult();
  m_SiLiTMult     = m_CalData->GetMMSiLiTMult();
  m_CsIEMult      = m_CalData->GetMMCsIEMult();
  m_CsITMult      = m_CalData->GetMMCsITMult();
  // Just a sanity check, to be sure that mult is consistent
  if (m_StripXEMult != Raw_Si_X_E.size())
    std::cout << "Si_X_E mult not consistent" << std::endl;
  if (m_StripYEMult != Raw_Si_Y_E.size())
    std::cout << "Si_Y_E mult not consistent" << std::endl;
  // Returns the matching couples and set the type
  // of matching. In case of a multiple match in one
  // detector, m_match_type is set to 1, else it is
  // set to 2
  auto couple = Match_X_Y();
  auto match  = Match_Si_CsI(couple);
  /////////////////////////////////////////////////
  for (auto event : match) {
    unsigned int Xindex   = event.second.first;
    unsigned int Yindex   = event.second.second;
    int          CsIindex = event.first;

    int N = m_CalData->GetMMStripXEDetectorNbr(Xindex);
    int X = m_CalData->GetMMStripXEStripNbr(Xindex);
    int Y = m_CalData->GetMMStripYEStripNbr(Yindex);

    m_PhysicsData->M2_TS.push_back(m_RawData->GetMMTS(Xindex));

    if (CsIindex >= 0) {
      m_PhysicsData->CsI_N.push_back(m_CalData->GetMMCsIECristalNbr(CsIindex));
      m_PhysicsData->CsI_E_Raw.push_back(m_CalData->GetMMCsIEEnergy(CsIindex));
      m_PhysicsData->CsI_E.push_back(fCsI_E2(CsIindex));
      m_PhysicsData->CsI_T.push_back(-1000);

      // Look for associate Time
      for (unsigned int k = 0; k < m_CsITMult; ++k) {
        // Same Cristal, Same Detector
        if (N == m_CalData->GetMMCsITDetectorNbr(k)
            && m_CalData->GetMMCsIECristalNbr(CsIindex)
                   == m_CalData->GetMMCsITCristalNbr(k)) {
          m_PhysicsData->CsI_T[m_PhysicsData->CsI_T.size() - 1]
              = m_CalData->GetMMCsITTime(CsIindex);
          break;
        }
      }
    } else {
      m_PhysicsData->CsI_N.push_back(-1000);
      m_PhysicsData->CsI_E_Raw.push_back(-1000);
      m_PhysicsData->CsI_E.push_back(-1000);
      m_PhysicsData->CsI_T.push_back(-1000);
    }
    double Si_X_T = -1000;
    for (unsigned int t = 0; t < m_StripXTMult; ++t) {
      if (N == m_CalData->GetMMStripXTDetectorNbr(t)
          && X == m_CalData->GetMMStripXTStripNbr(t)) {
        Si_X_T = m_CalData->GetMMStripXTTime(t);
        break;
      }
    }
    double Si_Y_T = -1000;
    for (unsigned int t = 0; t < m_StripYTMult; ++t) {
      if (N == m_CalData->GetMMStripYTDetectorNbr(t)
          && Y == m_CalData->GetMMStripYTStripNbr(t)) {
        Si_Y_T = m_CalData->GetMMStripYTTime(t);
        break;
      }
    }
    double Si_X_E     = m_CalData->GetMMStripXEEnergy(Xindex);
    double Si_Y_E     = m_CalData->GetMMStripYEEnergy(Yindex);
    double Si_X_E_Raw = Raw_Si_X_E[Xindex];
    double Si_Y_E_Raw = Raw_Si_Y_E[Yindex];
    m_PhysicsData->TelescopeNumber.push_back(N);
    m_PhysicsData->Si_X.push_back(X);
    m_PhysicsData->Si_Y.push_back(Y);
    // Store the other value for checking purpose
    m_PhysicsData->Si_EX.push_back(Si_X_E);
    m_PhysicsData->Si_TX.push_back(Si_X_T);
    m_PhysicsData->Si_EY.push_back(Si_Y_E);
    m_PhysicsData->Si_TY.push_back(Si_Y_T);
    if (m_Take_E_Y) {
      m_PhysicsData->Si_E.push_back(Si_Y_E);
    } else {
      m_PhysicsData->Si_E.push_back(Si_X_E);
    }
    if (m_Take_T_Y)
      m_PhysicsData->Si_T.push_back(Si_Y_T);
    else
      m_PhysicsData->Si_T.push_back(Si_X_T);

  }
  return;
}

bool Must2Detector::IsValidChannel(const int& DetectorType,
                                   const int& telescope, const int& channel) {

  if (DetectorType == 0)
    return *(m_XChannelStatus[telescope - 1].begin() + channel - 1);

  else if (DetectorType == 1)
    return *(m_YChannelStatus[telescope - 1].begin() + channel - 1);

  else if (DetectorType == 2)
    return *(m_SiLiChannelStatus[telescope - 1].begin() + channel - 1);

  if (DetectorType == 3)
    return *(m_CsIChannelStatus[telescope - 1].begin() + channel - 1);

  else
    return false;
}

//   DSSD
//   X
double Must2Detector::fSi_X_E(const int& i) {
  static string name;
  name = "MUST2_T";
  name += nptool::itoa(m_RawData->GetMMStripXEDetectorNbr(i));
  name += "_Si_X";
  name += nptool::itoa(m_RawData->GetMMStripXEStripNbr(i));
  name += "_E";
  return m_Cal.ApplyCalibration(name, m_RawData->GetMMStripXEEnergy(i));
}

double Must2Detector::fSi_X_T(const int& i) {
  static string name;
  name = "MUST2_T";
  name += nptool::itoa(m_RawData->GetMMStripXTDetectorNbr(i));
  name += "_Si_X";
  name += nptool::itoa(m_RawData->GetMMStripXTStripNbr(i));
  name += "_T";
  return m_Cal.ApplyCalibration(name, m_RawData->GetMMStripXTTime(i));
}

//   Y
double Must2Detector::fSi_Y_E(const int& i) {
  static string name;
  name = "MUST2_T";
  name += nptool::itoa(m_RawData->GetMMStripYEDetectorNbr(i));
  name += "_Si_Y";
  name += nptool::itoa(m_RawData->GetMMStripYEStripNbr(i));
  name += "_E";
  return m_Cal.ApplyCalibration(name, m_RawData->GetMMStripYEEnergy(i));
}

double Must2Detector::fSi_Y_T(const int& i) {
  static string name;
  name = "MUST2_T";
  name += nptool::itoa(m_RawData->GetMMStripYTDetectorNbr(i));
  name += "_Si_Y";
  name += nptool::itoa(m_RawData->GetMMStripYTStripNbr(i));
  name += "_T";
  return m_Cal.ApplyCalibration(name, m_RawData->GetMMStripYTTime(i));
}

//   SiLi
double Must2Detector::fSiLi_E(const int& i) {
  static string name;
  name = "MUST2_T";
  name += nptool::itoa(m_RawData->GetMMSiLiEDetectorNbr(i));
  name += "_SiLi";
  name += nptool::itoa(m_RawData->GetMMSiLiEPadNbr(i));
  name += "_E";

  return m_Cal.ApplyCalibration(name, m_RawData->GetMMSiLiEEnergy(i));
}

double Must2Detector::fSiLi_T(const int& i) {
  static string name;
  name = "MUST2_T";
  name += nptool::itoa(m_RawData->GetMMSiLiTDetectorNbr(i));
  name += "_SiLi";
  name += nptool::itoa(m_RawData->GetMMSiLiTPadNbr(i));
  name += "_T";
  return m_Cal.ApplyCalibration(name, m_RawData->GetMMSiLiTTime(i));
}

//   CsI
double Must2Detector::fCsI_E(const int& i) {
  static string name;
  name = "MUST2_T";
  name += nptool::itoa(m_RawData->GetMMCsIEDetectorNbr(i));
  name += "_CsI";
  name += nptool::itoa(m_RawData->GetMMCsIECristalNbr(i));
  name += "_E";
  return m_Cal.ApplyCalibration(name, m_RawData->GetMMCsIEEnergy(i));
}

double Must2Detector::fCsI_E2(const int& i) {
  static string name;
  name = "MUST2_T";
  name += nptool::itoa(m_CalData->GetMMCsIEDetectorNbr(i));
  name += "_CsI";
  name += nptool::itoa(m_CalData->GetMMCsIECristalNbr(i));
  name += "_E";
  return m_Cal.ApplyCalibration(name, m_CalData->GetMMCsIEEnergy(i));
}

double Must2Detector::fCsI_T(const int& i) {
  // static string name;
  // name = "MUST2_T";
  // name += nptool::itoa(m_RawData->GetMMCsITDetectorNbr(i));
  // name += "_CsI";
  // name += nptool::itoa(m_RawData->GetMMCsITCristalNbr(i));
  // name += "_T";
  // return m_Cal.ApplyCalibration(name, m_RawData->GetMMCsITTime(i));
  return m_RawData->GetMMCsITTime(i);
}

////////////////////////////////////////////////////////////////////////////////
void Must2Detector::InitializeDataInputRaw(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("must2", "must2::Must2Data", &m_RawData);
}
////////////////////////////////////////////////////////////////////////////////
void Must2Detector::InitializeDataInputPhysics(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("must2", "must2::Must2Physics", &m_PhysicsData);
}
////////////////////////////////////////////////////////////////////////////////
void Must2Detector::InitializeDataOutputRaw(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("must2", "must2::Must2Data", &m_RawData);
}
////////////////////////////////////////////////////////////////////////////////
void Must2Detector::InitializeDataOutputPhysics(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("must2", "must2::Must2Physics", &m_PhysicsData);
}

////////////////////////////////////////////////////////////////////////////////
void Must2Detector::InitSimulation(std::string simtype) {
  // store the loaded simulation
  m_simtype = simtype;

  // load the plugin
  auto handle = nptool::Application::GetApplication()->LoadPlugin(
      "must2-" + m_simtype, true);
  // build the class
#ifdef Geant4_FOUND
  auto func = (std::shared_ptr<nptool::geant4::VDetector>(*)())dlsym(
      handle, "ConstructDetectorSimulation");
  if (func)
    m_Geant4 = (*func)();
  else
    throw(nptool::Error("Must2Detector", "Fail to load Geant4 module"));
#endif
}
////////////////////////////////////////////////////////////////////////////////
void Must2Detector::ConstructGeometry() {
#ifdef Geant4_FOUND
  if (m_Geant4) {
    // should load the library here and find the external constructor
    std::cout << "I CONSTRUCT GEOMETRY" << std::endl;
    m_Geant4->ConstructDetector();
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void Must2Detector::InitSpectra() {
  m_Spectra = std::make_shared<must2::Must2Spectra>();
};
////////////////////////////////////////////////////////////////////////////////
void Must2Detector::FillSpectra() {
  m_Spectra->FillRaw();
  m_Spectra->FillPhy();
};
////////////////////////////////////////////////////////////////////////////////
void Must2Detector::WriteSpectra() {};
////////////////////////////////////////////////////////////////////////////////
void Must2Detector::CheckSpectra() {};
////////////////////////////////////////////////////////////////////////////////
void Must2Detector::ClearSpectra() { m_Spectra->Clear(); };
////////////////////////////////////////////////////////////////////////////////
extern "C" {
shared_ptr<nptool::VDetector> ConstructDetector() {
  return make_shared<must2::Must2Detector>();
};
}

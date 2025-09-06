#include "CatsDetector.h"

#include <iostream>

#include "CatsSpectra.h"
#include "NPFunction.h"
#include "NPRootPlugin.h"
using namespace cats;
using namespace std;
using namespace ROOT::Math;

////////////////////////////////////////////////////////////////////////////////
CatsDetector::CatsDetector() {
  m_RawData = new cats::CatsData();
  m_CalData = new cats::CatsData();
  m_PhysicsData = new cats::CatsPhysics();
  m_Cal.InitCalibration();
  m_NumberOfCATS = 0;
}

CatsDetector::~CatsDetector() {}

////////////////////////////////////////////////////////////////////////////////
void CatsDetector::ReadConfiguration(nptool::InputParser parser) {
  auto blocks = parser.GetAllBlocksWithToken("cats");
  auto blocksMask = parser.GetAllBlocksWithToken("mask");
  faction_file_initialized = false;

  vector<string> token = {"X01_Y01", "X28_Y01", "X01_Y28", "X28_Y28",
                          "CATSNumber"};
  vector<string> tokenMask = {"Z", "MaskNumber"};

  for (auto block : blocks) {
    if (block->HasTokenList(token)) {
      auto A = nptool::ConvertXYZVector(block->GetVector3("X01_Y01", "mm"));
      auto B = nptool::ConvertXYZVector(block->GetVector3("X28_Y01", "mm"));
      auto C = nptool::ConvertXYZVector(block->GetVector3("X01_Y28", "mm"));
      auto D = nptool::ConvertXYZVector(block->GetVector3("X28_Y28", "mm"));
      auto N = block->GetInt("CATSNumber");

      AddCATS(A, B, C, D, N);
      if (block->HasToken("ActionFile") && !faction_file_initialized) {
        ReadActionFile(block->GetString("ActionFile"));
      }
    } else {
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
  for (auto blockMask : blocksMask) {
    if (blockMask->HasTokenList(tokenMask)) {
      AddMask(blockMask->GetDouble("Z", "mm"), blockMask->GetInt("MaskNumber"));
    } else {
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  // get_config_file
  auto app = nptool::Application::GetApplication();
  if (app->HasFlag("--cats_cfg")) {
    auto arg = app->GetVectorArg("--cats_cfg");
    std::cout << arg[0] << std::endl;
    if (arg.size()) {
      m_config_file = arg[0];
    }
  }

  // get_action_file
  if (app->HasFlag("--action")) {
    auto arg = app->GetVectorArg("--action");
    std::cout << arg[0] << std::endl;
    if (arg.size()) {
      ReadActionFile(arg[0]);
    }
  }

  InitializeStandardParameter();
  ReadAnalysisConfig();
}

////////////////////////////////////////////////////////////////////////////////
void CatsDetector::ReadActionFile(std::string ActionFile) {
  faction_file_initialized = true;
  std::string LabelName;
  ifstream file(ActionFile, std::ifstream::in);
  int Label, c, d;
  if (!file) exit(1);
  while (file >> LabelName >> Label >> c >> d) {
    fLabelMap[Label] = LabelName;
  }
}

/////////////////////////////////////////////////////////////////////
void CatsDetector::AddCATS(XYZVector C_X1_Y1, XYZVector C_X28_Y1,
                           XYZVector C_X1_Y28, XYZVector C_X28_Y28,
                           UShort_t N) {
  m_NumberOfCATS++;

  // remove warning
  C_X28_Y28 *= 1;

  //	Vector U on Telescope Face (paralelle to Y Strip)
  XYZVector U = C_X28_Y1 - C_X1_Y1;
  U = U.Unit();

  //	Vector V on Telescope Face (parallele to X Strip)
  XYZVector V = C_X1_Y28 - C_X1_Y1;
  V = V.Unit();

  //	Position Vector of Strip Center
  XYZVector StripCenter;
  //	Position Vector of X=1 Y=1 Strip
  XYZVector Strip_1_1;

  //	Geometry Parameters
  double Face = 71.12;  // mm
  double NumberOfStrip = 28;
  double StripPitch = Face / NumberOfStrip;  // mm

  //	Buffer object to fill Position Array
  vector<double> lineX;
  vector<double> lineY;

  vector<vector<double>> OneDetectorStripPositionX;
  vector<vector<double>> OneDetectorStripPositionY;
  double OneDetectorStripPositionZ;

  //	Moving StripCenter to 1.1 corner (strip center!) :
  Strip_1_1 = C_X1_Y1 + (U + V) * (StripPitch / 2);

  for (int i = 0; i < 28; i++) {
    lineX.clear();
    lineY.clear();

    for (int j = 0; j < 28; j++) {
      StripCenter = Strip_1_1 + StripPitch * (i * U + j * V);
      lineX.push_back(StripCenter.x());
      lineY.push_back(StripCenter.y());
    }

    OneDetectorStripPositionX.push_back(lineX);
    OneDetectorStripPositionY.push_back(lineY);
  }

  OneDetectorStripPositionZ = C_X1_Y1.Z();

  StripPositionX[N] = OneDetectorStripPositionX;
  StripPositionY[N] = OneDetectorStripPositionY;
  StripPositionZ[N] = OneDetectorStripPositionZ;
}

void CatsDetector::AddMask(Double_t Z, UShort_t MaskNumber) {
  if (MaskNumber == 1) {
    Mask1_Z = Z;
  } else if (MaskNumber == 2) {
    Mask2_Z = Z;
  } else {
    std::cout << "Wrong Number for MASKS" << std::endl;
  }
  std::cout << ">>>>>>>>>> DEBUG : " << 0 << std::endl;
  std::cout << Mask1_Z << std::endl;
  std::cout << Mask2_Z << std::endl;
  std::cout << ">>>>>>>>>> DEBUG : " << 1 << std::endl;
  // exit(1);
}

void CatsDetector::BuildRawEvent(const std::string& daq_name,
                                 const std::string& st_type_key,
                                 void* commonframe) {
#ifdef MFM_FOUND
  int type_key = ((MFMCommonFrame*)commonframe)->GetFrameType();
  if (type_key == MFM_EBY_EN_TS_FRAME_TYPE) {
    TreatFrame((MFMCommonFrame*)commonframe);
  }
#endif
}

void CatsDetector::TreatFrame(void* commonframe) {
#ifdef MFM_FOUND
  unsigned short NItems = 0;
  unsigned short value, label_id;
  std::string label;
  unsigned short LblData[2];
  unsigned long long TS;

  std::shared_ptr<MFMEbyedatFrame> EbyEframe =
      std::make_shared<MFMEbyedatFrame>();
  EbyEframe->SetAttributs(((MFMCommonFrame*)commonframe)->GetPointHeader());

  NItems = EbyEframe->GetNbItems();
  TS = EbyEframe->GetTimeStamp();
  if (TS == 0) {
    std::cout << "/////////////////////////////" << std::endl;
    std::cout << "   CATS  WARNING TS IS 0     " << std::endl;
    std::cout << "/////////////////////////////" << std::endl;
  }

  // std::cout << "///////// NEW BLOCK" << std::endl;
  for (unsigned short i = 0; i < NItems; i++) {
    EbyEframe->EbyedatGetParameters(i, &label_id, &value);
    LblData[0] = label_id;
    LblData[1] = value;

    // cout<<"label_id is : "<<label_id<<endl;
    // cout<<"LblData[0] is : "<<LblData[0]<<endl;

    label = fLabelMap[label_id];
    // cout<<"label is : "<<label<<endl;
    if (label.compare(0, 4, "CATS") == 0) {

      double det = atoi(label.substr(4, 1).c_str());
      double channel = atoi(label.substr(7).c_str());
      // cout<<"det is : "<<det<<" channel is : "<<channel<<endl;
      if (label.compare(5, 1, "X") == 0) {
        m_RawData->SetStripX(det, channel + 1, value);
        // FIXME: Should be a pair! Fix for now online data e869
        m_RawData->SetCatsTS(TS);
        
        // cout<<"RAW: charge is: "<<value<<" strip: "<<channel+1<<"det: "<<det<<endl;
      } else if (label.compare(5, 1, "Y") == 0) {
        m_RawData->SetStripY(det, channel + 1, value);
      } else if (label.compare(5, 4, "QFIL") == 0) {
        m_RawData->SetQ(det, value);
      }
    }
  }

#endif
}

////////////////////////////////////////////////////////////////////////////////
void CatsDetector::BuildPhysicalEvent() {
  MapX.clear();
  MapY.clear();

  MaxQX.clear();
  MaxQY.clear();

  QSumX.clear();
  QSumY.clear();

  MultX.clear();
  MultY.clear();
  MapTSX.clear();
  MapTSY.clear();
  Positions.clear();

  DetectorHitX.clear();
  DetectorHit.clear();

  // cout<<"\033[31m"<<"build physical event running"<<endl<<"\033[0m";
  PreTreat();                                                                                         //Sets the data to the CAL_DATA

  unsigned int sizeX = m_CalData->GetCATSMultX();                                                  //already calibrated data
  for (unsigned short i = 0; i < sizeX; i++) {
    // Insert detector number in the set, if the key already exist, do nothing
    DetectorHitX.insert(m_CalData->GetCATSDetX(i));                                                   //the x-strips that triggered belonged to which cats (1 or 2?)
  }

  // Correspond to CATS with both X and Y
  unsigned int sizeY = m_CalData->GetCATSMultY();
  for (unsigned short i = 0; i < sizeY; i++) {
    // Insert detector number in the set, if the key already exist, do nothing
    // Only if the detector was hit on X as well
    if (DetectorHitX.find(m_CalData->GetCATSDetY(i)) != DetectorHitX.end())
      DetectorHit.insert(m_CalData->GetCATSDetY(i));                                                    //DetectorHit contains only Y hits of the events that triggered both CATS
  }

  // The number of CATS hit, i.e. the number of CATS that we are going to
  // analyse
  for (unsigned short i = 0; i < sizeX; i++) {
    StrX = m_CalData->GetCATSStripX(i);                                                                 //StrX has the strip that triggered in X only (both CATS1 and CATS2)
    NX = m_CalData->GetCATSDetX(i);                                                                     //NX has the CATS that was triggered (either 1 or 2)
    CATS_X_Q = m_CalData->GetCATSChargeX(i);                                                            //CATS_X_Q has the charge (both 1 and 2)

    m_PhysicsData->CalStripNumberX.push_back(m_CalData->GetCATSStripX(i));
    m_PhysicsData->CalDetNumberX.push_back(m_CalData->GetCATSDetX(i));
    m_PhysicsData->CalChargeX.push_back(m_CalData->GetCATSChargeX(i));                                   //pushes the same values into the cal branches...
    
    if (DetectorHit.find(NX) != DetectorHit.end()) {                                                     //if the CATS that was triggered is in the DetectorHit (i.e. it was hit on Y as well)
      MapX[NX].push_back(std::make_pair(StrX, CATS_X_Q));                                                //pushes the pair of the strip and charge into the MapX 
      QSumX[NX] += CATS_X_Q;                                                                             //QSumX has the sum of the charge for each CATS
      MapTSX[NX] = m_CalData->GetCatsTS(i);                                                              //MapTSX has the timestamp of the CATS that was triggered
      // std::cout << sizeX << std::endl;
      // if (m_CalData->GetCatsTS(i) > 1000e12) {
      //   std::cout << sizeX << " "<< m_CalData->GetCatsTS(i) / 1e12 <<
      //   std::endl;
      // }
      if (MaxQX.find(NX) == MaxQX.end() || MaxQX[NX].second < CATS_X_Q) {                                 //if the CATS that was triggered is not in MaxQX or the charge is larger than the one already in MaxQX
        MaxQX[NX] = make_pair(StrX, CATS_X_Q);                                                            //MaxQX has the pair of the strip and charge for the CATS that was triggered with the largest charge
      }
    }
  }

  for (unsigned short i = 0; i < sizeY; i++) {
    StrY = m_CalData->GetCATSStripY(i);
    NY = m_CalData->GetCATSDetY(i);
    CATS_Y_Q = m_CalData->GetCATSChargeY(i);
    m_PhysicsData->CalStripNumberY.push_back(m_CalData->GetCATSStripY(i));
    m_PhysicsData->CalDetNumberY.push_back(m_CalData->GetCATSDetY(i));
    m_PhysicsData->CalChargeY.push_back(m_CalData->GetCATSChargeY(i));
    if (DetectorHit.find(NY) != DetectorHit.end()) {
      MapY[NY].push_back(std::make_pair(StrY, CATS_Y_Q));
      QSumY[NY] += CATS_Y_Q;
      MapTSY[NY] = m_CalData->GetCatsTS(i);
      if (MaxQY.find(NY) == MaxQY.end() || MaxQY[NY].second < CATS_Y_Q) {
        MaxQY[NY] = make_pair(StrY, CATS_Y_Q);
      }
    }
  }

  for (unsigned int i = 1; i < 3; i++) {
    std::sort(MapX[i].begin(), MapX[i].end(), [](auto& left, auto& right) { return left.first < right.first; });    // Sorts the MapX by the strip number
    MultX[i] = FindMult(MapX[i], MaxQX[i].first);                                                                   // FindMult is a function that returns the multiplicity of the CATS, i.e. how many strips were hit in X
    std::sort(MapY[i].begin(), MapY[i].end(), [](auto& left, auto& right) { return left.first < right.first; });
    MultY[i] = FindMult(MapY[i], MaxQY[i].first);
  }
  for (auto& DetN : DetectorHit) {                                                                                  // Loop over the CATS that were hit on both X and Y
    // Return the position in strip unit
    // Convention: the collected charge is atrributed to the center of the strip
    // (histogram convention) so that a reconstructed position for a single
    // strip goes from strip index -0.5 to strip index +0.5
    double PosX =
        ReconstructionFunctionX[DetN](MaxQX[DetN], MapX[DetN], QSumX[DetN]);                                         // ReconstructionFunctionX is a function that returns the position in strip unit for X
    double PosY =
        ReconstructionFunctionY[DetN](MaxQY[DetN], MapY[DetN], QSumY[DetN]);
                                                                                                                      //Inversion in X can be done here by changing PosX to -PosX
    // a shift - -1 is made to have PosX in between -0.5 and 27.5
    // for the following calculation of the position in the lab.
    PosX = PosX;      //  FIXME: PS : added replicating np3... remove later if doesn't work
    PosY = PosY;      //  FIXME: PS : added replicating np3... remove later if doesn't work

    // std::cout << DetN << " " << MapTSX[DetN] << " " << MapTSY[DetN]
    //           << std::endl;

                                                                                                                      // sx and sy are the X and Y strip number between which the PosX and PosY
    // are
    int sx0 = (int)PosX;
    int sx1 = sx0 + 1;
    int sy0 = (int)PosY;
    int sy1 = sy0 + 1;

    // PUSHING DETNUMBER ALWAYS TO USE TS
    m_PhysicsData->CATS_TS.push_back(MapTSX[DetN]);
    // if(MapTSX[DetN]>1000e12){
    //   std::cout << MapTSX[DetN]/1e12 << std::endl;
    // }
    // m_PhysicsData->DetNumber.push_back(DetN);  //FIXME: PS: was here originally... now below
    if (PosX > -1000 && PosY > -1000 && sx0 > -1 && sx1 < 28 &&  sy0 > -1 &&
        sy1 < 28) {
      // px and py are the x and y coordinate of strip sx and sy
      m_PhysicsData->StripNumberX.push_back(PosX);
      m_PhysicsData->StripNumberY.push_back(PosY);
      m_PhysicsData->DetNumber.push_back(DetN);  //FIXME: PS: was above originally... now here
      //The above prob is fixed by choosing to fill DetNumber here... command like PhysicsTree->
          //Draw("PositionY:PositionX","DetNumber==1", ""); filled CATS1 twice.
      m_PhysicsData->ChargeX.push_back(QSumX[DetN]);
      m_PhysicsData->ChargeY.push_back(QSumY[DetN]);
      double px0 = StripPositionX[DetN][sx0][sy0];
      // double px1 = StripPositionX[DetN][sx1][sy1];
      double px1 = StripPositionX[DetN][sx1][sy1];

                                                
      double py0 = StripPositionY[DetN][sx0][sy0];
      // double py1 = StripPositionY[DetN][sx1][sy1];
      double py1 = StripPositionY[DetN][sx1][sy1];


      Positions[DetN] = make_pair(StripPositionZ[DetN], make_pair(px0 + (px1 - px0) * (PosX - sx0), py0 + (py1 - py0) * (PosY - sy0)));
      m_PhysicsData->PositionX.push_back(px0 + (px1 - px0) * (PosX - sx0)); 
      m_PhysicsData->PositionY.push_back(py0 + (py1 - py0) * (PosY - sy0));
      m_PhysicsData->PositionZ.push_back(StripPositionZ[DetN]);
    } else {
      // // ADDING THIS TO KEEP DIMENSIONS OF DETN
      // m_PhysicsData->StripNumberX.push_back(-1000);
      // m_PhysicsData->StripNumberY.push_back(-1000);
      // m_PhysicsData->ChargeX.push_back(-1000);
      // m_PhysicsData->ChargeY.push_back(-1000);
      // m_PhysicsData->PositionX.push_back(-1000);
      // m_PhysicsData->PositionY.push_back(-1000);
      // m_PhysicsData->PositionZ.push_back(-1000);
    }
  }

  for (auto it : MultX) {
    if (it.first == 1) m_PhysicsData->Mult1X.push_back(it.second);
    if (it.first == 2) m_PhysicsData->Mult2X.push_back(it.second);
  }
  for (auto it : MultY) {
    if (it.first == 1) m_PhysicsData->Mult1Y.push_back(it.second);
    if (it.first == 2) m_PhysicsData->Mult2Y.push_back(it.second);
  }

  // At least two CATS need to gave back position in order to reconstruct on
  // Target
  if (Positions.size() == 2) {
    double t = (m_PhysicsData->m_Zproj - Positions[1].first) / (Positions[2].first - Positions[1].first);
    m_PhysicsData->PositionOnTargetX = Positions[1].second.first + (Positions[2].second.first - Positions[1].second.first) * t;
    m_PhysicsData->PositionOnTargetY = Positions[1].second.second + (Positions[2].second.second - Positions[1].second.second) * t;

    if (Mask1_Z != 0 && Mask2_Z != 0) {
      double tmask1 = (Positions[1].first - Mask1_Z) / (Positions[2].first - Positions[1].first);
      double tmask2 = (Positions[2].first - Mask2_Z) / (Positions[2].first - Positions[1].first);

      m_PhysicsData->PositionOnMask1X = Positions[1].second.first - (Positions[2].second.first - Positions[1].second.first) * tmask1;
      m_PhysicsData->PositionOnMask1Y = Positions[1].second.second -(Positions[2].second.second - Positions[1].second.second) * tmask1;
      m_PhysicsData->PositionOnMask2X = Positions[2].second.first - (Positions[2].second.first - Positions[1].second.first) * tmask2;
      m_PhysicsData->PositionOnMask2Y = Positions[2].second.second - (Positions[2].second.second - Positions[1].second.second) * tmask2;
    } else {
      m_PhysicsData->PositionOnMask1X = -1000;
      m_PhysicsData->PositionOnMask1Y = -1000;
      m_PhysicsData->PositionOnMask2X = -1000;
      m_PhysicsData->PositionOnMask2Y = -1000;
    }
  } else {
    BeamDirection = XYZVector(1, 0, 0);
    m_PhysicsData->PositionOnTargetX = -1000;
    m_PhysicsData->PositionOnTargetY = -1000;
    m_PhysicsData->PositionOnMask1X = -1000;
    m_PhysicsData->PositionOnMask1Y = -1000;
    m_PhysicsData->PositionOnMask2X = -1000;
    m_PhysicsData->PositionOnMask2Y = -1000;
  }
  BeamDirection = GetBeamDirection();

  // Does not meet the conditions for target position and beam direction
  return;
}

void CatsDetector::TestCalibration() {
  
    // cout<<"\033[31m"<<"test calibration running"<<endl<<"\033[0m";
  ClearEventPhysics();
  // Testing Hugo's test
  //  Method to be used to test calibrations
  //  Thus does not match X and Y strips
  // #ifdef npcalibration
  PreTreat();

  unsigned int sizeX = m_CalData->GetCATSMultX();
  unsigned int sizeY = m_CalData->GetCATSMultY();

  for (unsigned short i = 0; i < sizeX; i++) {
    m_PhysicsData->CalStripNumberX.push_back(m_CalData->GetCATSStripX(i));
    m_PhysicsData->CalDetNumberX.push_back(m_CalData->GetCATSDetX(i));
    m_PhysicsData->CalChargeX.push_back(m_CalData->GetCATSChargeX(i));
  }

  for (unsigned short i = 0; i < sizeY; i++) {
    m_PhysicsData->CalStripNumberY.push_back(m_CalData->GetCATSStripY(i));
    m_PhysicsData->CalDetNumberY.push_back(m_CalData->GetCATSDetY(i));
    m_PhysicsData->CalChargeY.push_back(m_CalData->GetCATSChargeY(i));
  }
  BeamDirection = TVector3(1, 0, 0);
  m_PhysicsData->CalPositionOnTargetX = -1000;
  m_PhysicsData->CalPositionOnTargetY = -1000;
  m_PhysicsData->CalPositionOnMask1X = -1000;
  m_PhysicsData->CalPositionOnMask1Y = -1000;
  m_PhysicsData->CalPositionOnMask2X = -1000;
  m_PhysicsData->CalPositionOnMask2Y = -1000;
  // #endif
}

////////////////////////////////////////////////////////////////////////////////
void CatsDetector::PreTreat() {
  ClearPreTreatedData();

  gRandom->SetSeed(0);
  // X
  unsigned int sizeX = m_RawData->GetCATSMultX();
  if (sizeX > 0)
    for (unsigned int i = 0; i < sizeX; i++) {
      // Valid Channel X
      if (IsValidChannel("X", m_RawData->GetCATSDetX(i),
                         m_RawData->GetCATSStripX(i))) {
                          // cout<<"is a Valid Channel X: "<<i<<endl;
        if (fCATS_Threshold_X(i)) {       
          // cout<<"fCATS_Threshold_X cleared by: "<<i<<endl;
          // if (true) {
          // cout<<"charge before calibration: "<<m_RawData->GetCATSChargeX(i);
          double QX = fCATS_X_Q(i);
          // cout<<" and after: "<<QX<<" for strip: "<<m_RawData->GetCATSStripX(i)<<" and det: "<<m_RawData->GetCATSDetX(i)<<"\n"<<endl;
          // cout<<"after calibration value is: "<<QX<<endl;
          unsigned int stripX = -1;
          // Inversion X
          if (*(m_CATSXInversion[m_RawData->GetCATSDetX(i) - 1].begin() +
                m_RawData->GetCATSStripX(i) - 1) !=
              m_RawData->GetCATSStripX(i)) {
                // cout<<"stripX is not equal to CATSStripX: "<<i<<endl;
            stripX = *(m_CATSXInversion[m_RawData->GetCATSDetX(i) - 1].begin() +
                       m_RawData->GetCATSStripX(i) - 1);

                      //  cout<<"\033[31m"<<"stripX is not equal to CATSStripX: "<<i<<endl<<"\033[0m";
          } else {
            stripX = m_RawData->GetCATSStripX(i);
            // cout<<"stripX is equal to CATSStripX: "<<i<<endl;
          }
          if (QX < 0) QX = 0;
          // if(m_RawData->GetCATSDetX(i) == 2)
          m_CalData->SetStripX(m_RawData->GetCATSDetX(i), stripX,
                               (unsigned int)QX);
          // if (m_RawData->GetCatsTS(i) > 1000e12) {
          // }
          m_CalData->SetCatsTS(m_RawData->GetCatsTS(i));
        }
        // if(m_RawData->GetCATSDetX(i) == 2)
      }
    }

  // Y
  unsigned int sizeY = m_RawData->GetCATSMultY();
  for (unsigned int i = 0; i < sizeY; i++) {
    // Valid Channel Y
    if (IsValidChannel("Y", m_RawData->GetCATSDetY(i),
                       m_RawData->GetCATSStripY(i))) {
      if (fCATS_Threshold_Y(i)) {
        double QY = fCATS_Y_Q(i);
        unsigned int stripY = -1;
        // Inversion Y
        if (*(m_CATSYInversion[m_RawData->GetCATSDetY(i) - 1].begin() +
              m_RawData->GetCATSStripY(i) - 1) != m_RawData->GetCATSStripY(i)) {
          stripY = *(m_CATSYInversion[m_RawData->GetCATSDetY(i) - 1].begin() +
                     m_RawData->GetCATSStripY(i) - 1);
        } else {
          stripY = m_RawData->GetCATSStripY(i);
        }
        if (QY < 0) QY = 0;
        m_CalData->SetStripY(m_RawData->GetCATSDetY(i), stripY,
                             (unsigned int)QY);
      }
    }
  }
  return;
}

void CatsDetector::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(m_config_file.c_str());
  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigCATS.dat found: Default parameter loaded for Analayis "
         << m_config_file << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from " << m_config_file << endl;

  // // Save it in a TAsciiFile
  // TAsciiFile* asciiConfig
  //     = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  // asciiConfig->AppendLine("%%% ConfigCATS.dat %%%");
  // asciiConfig->Append(config_file.c_str());
  // asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer, DataBuffer, whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 10, "ConfigCATS") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus) {
      whatToDo = "";
      AnalysisConfigFile >> whatToDo;
      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n');
      }

      else if (whatToDo == "DISABLE_CHANNEL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int Detector = atoi(DataBuffer.substr(4, 1).c_str());
        int channel = -1;
        if (DataBuffer.compare(5, 4, "STRX") == 0) {
          channel = atoi(DataBuffer.substr(9).c_str());
          *(m_XChannelStatus[Detector - 1].begin() + channel - 1) = false;
        }

        else if (DataBuffer.compare(5, 4, "STRY") == 0) {
          channel = atoi(DataBuffer.substr(9).c_str());
          *(m_YChannelStatus[Detector - 1].begin() + channel - 1) = false;
        }

        else
          cout << "Warning: detector type for CATS unknown!" << endl;
      }

      else if (whatToDo == "INVERSION") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer;
        int Detector = atoi(DataBuffer.substr(4, 1).c_str());
        int channel1 = -1;
        int channel2 = -1;
        AnalysisConfigFile >> DataBuffer;
        cout << " " << DataBuffer;
        if (DataBuffer.compare(0, 4, "STRX") == 0) {
          channel1 = atoi(DataBuffer.substr(4).c_str());
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          channel2 = atoi(DataBuffer.substr(4).c_str());
          *(m_CATSXInversion[Detector - 1].begin() + channel1 - 1) = channel2;
          *(m_CATSXInversion[Detector - 1].begin() + channel2 - 1) = channel1;
        }
        if (DataBuffer.compare(0, 4, "STRY") == 0) {
          channel1 = atoi(DataBuffer.substr(4).c_str());
          AnalysisConfigFile >> DataBuffer;
          cout << " " << DataBuffer << endl;
          channel2 = atoi(DataBuffer.substr(4).c_str());
          *(m_CATSYInversion[Detector - 1].begin() + channel1 - 1) = channel2;
          *(m_CATSYInversion[Detector - 1].begin() + channel2 - 1) = channel1;
        }
      }

      else if (whatToDo == "INVERSIONX") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer;
        int Detector = atoi(DataBuffer.substr(4, 1).c_str());
        for (unsigned int strip = 0; strip < 28; strip++) {
          *(m_CATSXInversion[Detector - 1].begin() + strip) = 27 - strip;

          // *(m_CATSXInversion[Detector - 1].begin() + strip) = 28 - strip;

        }
      }

      else if (whatToDo == "INVERSIONY") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer;
        int Detector = atoi(DataBuffer.substr(4, 1).c_str());
        for (unsigned int strip = 0; strip < 28; strip++) {
          *(m_CATSYInversion[Detector - 1].begin() + strip) = 27 - strip;
        }
      }

      else if (whatToDo == "RECONSTRUCTION_METHOD") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer;
        // DataBuffer is of form CATSNX
        // Look for the CATS Number removing the first 4 letters and the
        // trailling letter
        string Duplicate = DataBuffer.substr(4);  // Duplicate is of form NX
        Duplicate.resize(Duplicate.size() - 1);   // Duplicate is of form
        unsigned int CATSNumber = atoi(Duplicate.c_str());

        // Look for the X or Y part of the Detector, Basically the last
        // character
        string XorY(string(1, DataBuffer[DataBuffer.size() - 1]));
        // Get the Reconstruction Methods Name
        AnalysisConfigFile >> DataBuffer;
        // Set the Reconstruction Methods using above information
        SetReconstructionMethod(CATSNumber, XorY, DataBuffer);
      }

      else if (whatToDo == "ZPROJ") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        m_PhysicsData->m_Zproj = atoi(DataBuffer.c_str());
      }

      else {
        ReadingStatus = false;
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////
double CatsDetector::AnalyticHyperbolicSecant(
    std::pair<UShort_t, UShort_t>& MaxQ,
    std::vector<std::pair<UShort_t, UShort_t>>& Map, Double_t QSum) {
  double sech = -1000;

  // std::cout << "test AnH 1" << std::endl;
  if (MaxQ.second > 0 && MaxQ.first > 2 && MaxQ.first < 27) {
    //      if(Buffer_Q[MaxQ.first-1+1]==0||Buffer_Q[MaxQ.first-1-1]==0)
    //        return sech;
    // std::cout << "test AnH 2" << std::endl;
    double q2 = MaxQ.second;
    double q1 = 0, q3 = 0;
    for (auto& strip : Map) {
      if (strip.first == MaxQ.first - 1) {
        q1 = strip.second;
      } else if (strip.first == MaxQ.first + 1) {
        q3 = strip.second;
      }
    }

    // if(MaxQ.first==15 && q1 > 0){
    //   std::cout << "max q " << MaxQ.first << std::endl;
    //   std::cout << "test q " << q1 << " " << q2 << " " << q3 << std::endl <<
    //   std::endl;
    // }

    double vs[6];
    // std::cout << "test AnH 3" << std::endl;
    if (q1 > 0 && q3 > 0) {
      // QsumSample[DetNum].push_back(QSum);
      vs[0] = sqrt(q2 / q3);
      vs[1] = sqrt(q2 / q1);
      vs[2] = 0.5 * (vs[0] + vs[1]);
      vs[3] = log(vs[2] + sqrt(vs[2] * vs[2] - 1.0));
      vs[4] = abs((vs[0] - vs[1]) / (2.0 * sinh(vs[3])));
      vs[5] = 0.5 * log((1.0 + vs[4]) / (1.0 - vs[4]));
      // std::cout << "test AnH 4" << std::endl;

      if (q3 > q1)
        sech = MaxQ.first + vs[5] / vs[3];
      else
        sech = MaxQ.first - vs[5] / vs[3];
      // std::cout << "test sech " << sech << std::endl;
    }
  }

  return sech;
}
unsigned int CatsDetector::FindMult(
    std::vector<std::pair<unsigned short, unsigned short>> vector,
    unsigned int StripMax) {
  std::vector<unsigned int> StripGroup;
  unsigned int mult = 0;

  for (auto it : vector) {
    if (std::find(StripGroup.begin(), StripGroup.end(), it.first - 1) !=
        StripGroup.end()) {
      StripGroup.push_back(it.first);
    } else {
      if (std::find(StripGroup.begin(), StripGroup.end(), StripMax) !=
          StripGroup.end()) {
        mult = StripGroup.size();
      }
      StripGroup.clear();
      StripGroup.push_back(it.first);
    }
  }
  if (std::find(StripGroup.begin(), StripGroup.end(), StripMax) !=
      StripGroup.end())
    mult = StripGroup.size();
  return mult;
}

/////////////////////////////////////////////////////////////////////
double CatsDetector::FittedHyperbolicSecant(
    std::pair<UShort_t, UShort_t>& MaxQ,
    std::vector<std::pair<UShort_t, UShort_t>>& Map, Double_t QSum) {
  // Warning: should not delete static variable
  static TF1* f = new TF1(
      "sechs",
      "[0]/(cosh(TMath::Pi()*(x-[1])/[2])*cosh(TMath::Pi()*(x-[1])/[2]))", 1,
      28);

  // Help the fit by computing the position of the maximum by analytic method
  double StartingPoint = AnalyticHyperbolicSecant(MaxQ, Map, QSum);
  // if analytic method fails then the starting point in strip max
  if (StartingPoint == -1000) StartingPoint = MaxQ.first;

  // Maximum is close to charge max, Mean value is close to Analytic one,
  // typical width is 3.8 strip
  f->SetParameters(MaxQ.second, StartingPoint, 3.8);

  static vector<double> y;
  static vector<double> q;
  y.clear();
  q.clear();
  double final_size = 0;
  unsigned int sizeQ = Map.size();

  for (unsigned int i = 0; i < sizeQ; i++) {
    if (Map[i].second > MaxQ.second * 0.2) {
      q.push_back(Map[i].second);
      y.push_back(i + 1);
      final_size++;
    }
  }

  // requiered at least 3 point to perfom a fit
  if (final_size < 3) {
    return -1000;
  }

  TGraph* g = new TGraph(q.size(), &y[0], &q[0]);
  g->Fit(f, "QN0");
  delete g;
  return f->GetParameter(1);
  delete f;

  return 0;
}

////////////////////////////////////////////////////////////////////////////
bool CatsDetector::IsValidChannel(const string& DetectorType,
                                  const int& Detector, const int& channel) {
  if (DetectorType == "X")
    return *(m_XChannelStatus[Detector - 1].begin() + channel - 1);

  else if (DetectorType == "Y")
    return *(m_YChannelStatus[Detector - 1].begin() + channel - 1);

  else
    return false;
}

///////////////////////////////////////////////////////////////
XYZVector CatsDetector::GetBeamDirection() {
  XYZVector Direction;
  if (Positions.size() < 2)
    return Direction;

  else {
    Direction =
        TVector3(Positions[2].second.first - Positions[1].second.first,
                 Positions[2].second.second - Positions[1].second.second,
                 Positions[2].first - Positions[1].first);
    Direction.Unit();
  }

  return (Direction);
}

////////////////////////////////////////////////////////////////
void CatsDetector::SetReconstructionMethod(unsigned int CATSNumber, string XorY,
                                           string MethodName) {
  if (XorY == "X") {
    if (MethodName == "ASECH") {
      ReconstructionFunctionX[CATSNumber] = std::bind(
          &CatsDetector::AnalyticHyperbolicSecant, this, std::placeholders::_1,
          std::placeholders::_2, std::placeholders::_3);
    } else if (MethodName == "FSECH") {
      ReconstructionFunctionX[CATSNumber] = std::bind(
          &CatsDetector::FittedHyperbolicSecant, this, std::placeholders::_1,
          std::placeholders::_2, std::placeholders::_3);
    } else {
      cout << "WARNING: Wrong name for reconsctuction Method, using default "
              "AGAUSS"
           << endl;
    }
  }
  if (XorY == "Y") {
    if (MethodName == "ASECH") {
      ReconstructionFunctionY[CATSNumber] = std::bind(
          &CatsDetector::AnalyticHyperbolicSecant, this, std::placeholders::_1,
          std::placeholders::_2, std::placeholders::_3);
    } else if (MethodName == "FSECH") {
      ReconstructionFunctionY[CATSNumber] = std::bind(
          &CatsDetector::FittedHyperbolicSecant, this, std::placeholders::_1,
          std::placeholders::_2, std::placeholders::_3);
    } else {
      cout << "WARNING: Wrong name for reconsctuction Method, using default "
              "AGAUSS"
           << endl;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////
void CatsDetector::InitializeStandardParameter() {
  //   Enable all channel and no inversion
  vector<bool> ChannelStatus;
  vector<int> InversionStatus;
  m_XChannelStatus.clear();
  m_YChannelStatus.clear();
  m_CATSXInversion.clear();
  m_CATSYInversion.clear();

  ChannelStatus.resize(28, true);
  InversionStatus.resize(28);
  for (unsigned int j = 0; j < InversionStatus.size(); j++) {
    InversionStatus[j] = j + 1;
  }

  for (int i = 0; i < m_NumberOfCATS; ++i) {
    m_XChannelStatus[i] = ChannelStatus;
    m_YChannelStatus[i] = ChannelStatus;
    m_CATSXInversion[i] = InversionStatus;
    m_CATSYInversion[i] = InversionStatus;
    SetReconstructionMethod(i + 1, "X", "AGAUSS");
    SetReconstructionMethod(i + 1, "Y", "AGAUSS");
  }

  return;
}

////////////////////////////////////////////////////////////////////////
double CatsDetector::fCATS_X_Q(const int& i) {
  static string name;
  name = "CATS_D";
  name += nptool::itoa(m_RawData->GetCATSDetX(i));
  name += "_X";
  name += nptool::itoa(m_RawData->GetCATSStripX(i));
  name += "_Q";
  return m_Cal.ApplyCalibration(name,
                                m_RawData->GetCATSChargeX(i) + gRandom->Rndm());
}
////////////////////////////////////////////////////////////////////////
double CatsDetector::fCATS_Y_Q(const int& i) {
  static string name;
  name = "CATS_D";
  name += nptool::itoa(m_RawData->GetCATSDetY(i));
  name += "_Y";
  name += nptool::itoa(m_RawData->GetCATSStripY(i));
  name += "_Q";
  return m_Cal.ApplyCalibration(name,
                                m_RawData->GetCATSChargeY(i) + gRandom->Rndm());
}
////////////////////////////////////////////////////////////////////////
bool CatsDetector::fCATS_Threshold_X(const int& i) {
  static string name;
  name = "CATS_D";
  name += nptool::itoa(m_RawData->GetCATSDetX(i));
  name += "_X";
  name += nptool::itoa(m_RawData->GetCATSStripX(i));
  return m_Cal.ApplyThreshold(name, m_RawData->GetCATSChargeX(i));
}
////////////////////////////////////////////////////////////////////////
bool CatsDetector::fCATS_Threshold_Y(const int& i) {
  static string name;
  name = "CATS_D";
  name += nptool::itoa(m_RawData->GetCATSDetY(i));
  name += "_Y";
  name += nptool::itoa(m_RawData->GetCATSStripY(i));
  return m_Cal.ApplyThreshold(name, m_RawData->GetCATSChargeY(i));
}
////////////////////////////////////////////////////////////////////////
double CatsDetector::fCATS_Ped_X(const int& i) {
  static string name;
  name = "CATS_D";
  name += nptool::itoa(m_RawData->GetCATSDetX(i));
  name += "_X";
  name += nptool::itoa(m_RawData->GetCATSStripX(i));
  return m_Cal.GetPedestal(name);
}
////////////////////////////////////////////////////////////////////////
double CatsDetector::fCATS_Ped_Y(const int& i) {
  static string name;
  name = "CATS_D";
  name += nptool::itoa(m_RawData->GetCATSDetY(i));
  name += "_Y";
  name += nptool::itoa(m_RawData->GetCATSStripY(i));
  return m_Cal.GetPedestal(name);
}

////////////////////////////////////////////////////////////////////////////////
void CatsDetector::InitializeDataInputRaw(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("cats", "cats::CatsData", &m_RawData);
}
////////////////////////////////////////////////////////////////////////////////
void CatsDetector::InitializeDataInputPhysics(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("cats", "cats::CatsPhysics", &m_PhysicsData);
}
////////////////////////////////////////////////////////////////////////////////
void CatsDetector::InitializeDataOutputRaw(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("cats", "cats::CatsData", &m_RawData);
}
////////////////////////////////////////////////////////////////////////////////
void CatsDetector::InitializeDataOutputPhysics(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("cats", "cats::CatsPhysics", &m_PhysicsData);
}

////////////////////////////////////////////////////////////////////////////////
void CatsDetector::InitSpectra() {
  m_Spectra = std::make_shared<cats::CatsSpectra>();
};
////////////////////////////////////////////////////////////////////////////////
void CatsDetector::FillSpectra() {
  auto app = nptool::Application::GetApplication();
  if (app->HasFlag("--input-raw")) m_Spectra->FillRaw();
  if (app->HasFlag("--input-phy")) m_Spectra->FillPhy();
};
////////////////////////////////////////////////////////////////////////////////
void CatsDetector::WriteSpectra(){};
////////////////////////////////////////////////////////////////////////////////
void CatsDetector::CheckSpectra(){};
////////////////////////////////////////////////////////////////////////////////
void CatsDetector::ClearSpectra() { m_Spectra->Clear(); };

////////////////////////////////////////////////////////////////////////////////
extern "C" {
shared_ptr<nptool::VDetector> ConstructDetector() {
  return make_shared<cats::CatsDetector>();
};
}

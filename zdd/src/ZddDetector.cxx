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
#include "ZddDetector.h"

#include <iostream>

#include "NPFunction.h"
#include "NPRootPlugin.h"
#include "ZddSpectra.h"

using namespace zdd;
using namespace std;
using namespace ROOT::Math;

////////////////////////////////////////////////////////////////////////////////
ZddDetector::ZddDetector() {
  std::cout << "////////////////////////////////////////////// test INIT "
               "////////////////////////////////////////////////"
            << std::endl;
  m_RawData = new zdd::ZddData();
  m_CalData = new zdd::ZddData();
  m_PhysicsData = new zdd::ZddPhysics();
  std::cout << "Adresse phy " << m_PhysicsData << std::endl;
  m_Cal.InitCalibration();
}

ZddDetector::~ZddDetector() {}

////////////////////////////////////////////////////////////////////////////////
void ZddDetector::ReadConfiguration(nptool::InputParser parser) {
  auto blocks = parser.GetAllBlocksWithToken("zdd");
  std::vector<std::string> token = {"Type", "Board", "NUMEXOChannel"};

  for (unsigned int i = 0; i < blocks.size(); i++) {
    if (blocks[i]->HasTokenList(token)) {
      std::string Type = blocks[i]->GetString("Type");
      unsigned int Board = blocks[i]->GetInt("Board");

      std::vector<int> NUMEXOChannel = blocks[i]->GetVectorInt("NUMEXOChannel");
      unsigned int NUMEXOChannel_size = NUMEXOChannel.size();

      for (unsigned int j = 0; j < NUMEXOChannel_size; j++) {
        unsigned int index = Board * 1e3 + (unsigned int)NUMEXOChannel[j];
        SetNumexoMap(Type, Board, (unsigned int)NUMEXOChannel[j]);
        // m_BoardDataTypeMap[index] = "ZDD";
      }
    }
  }

  ReadAnalysisConfig();
}

////////////////////////////////////////////////////////////////////////////////
void ZddDetector::SetNumexoMap(std::string Type, unsigned int Board,
                               unsigned int NUMEXOChannel) {
  unsigned int index = Board * 1e3 + NUMEXOChannel;
  m_NumexoMap[index] = Type;
}

////////////////////////////////////////////////////////////////////////////////
std::string ZddDetector::GetTypeFromNumexoMap(unsigned int Board,
                                              unsigned int Channel) {
  unsigned int index = Board * 1e3 + Channel;
  std::string Type = m_NumexoMap[index];
  return Type;
}

////////////////////////////////////////////////////////////////////////////////
void ZddDetector::BuildRawEvent(const std::string& daq_name,
                                const std::string& st_type_key,
                                void* commonframe) {
#ifdef MFM_FOUND
  int type_key = ((MFMCommonFrame*)commonframe)->GetFrameType();
  if (type_key == MFM_REA_GENE_FRAME_TYPE) {
    TreatFrame((MFMCommonFrame*)commonframe);
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void ZddDetector::TreatFrame(void* commonframe) {
#ifdef MFM_FOUND

  std::shared_ptr<MFMReaGenericFrame> ReaFrame =
      std::make_shared<MFMReaGenericFrame>();
  ReaFrame->SetAttributs(((MFMCommonFrame*)commonframe)->GetPointHeader());

  int Board = -1;
  int Channel = -1;
  unsigned short E = 0;
  unsigned long long TS = 0;
  int Time = -1;

  Board = ReaFrame->GetBoardId();
  Channel = ReaFrame->GetTGCristalId();
  E = ReaFrame->GetEnergy();
  TS = ReaFrame->GetTimeStamp();
  Time = ReaFrame->GetTime();

  //std::cout << "CAN I GET REA??" << std::endl;
  //std::cout << Time << std::endl;

  std::string Type = GetTypeFromNumexoMap(Board, Channel);

  // int i = 0;

  if (Type == "PL") {
    m_RawData->SetZDDPL(Channel, E, TS);
    // i++;
    // std::cout << " PL " << i << std::endl;
  } else if (Type == "IC") {
    // std::cout << Channel << " " << E << " " << TS << std::endl;
    m_RawData->SetZDDIC(Channel, E, TS);
    // i++;
    // std::cout << " IC " << i << std::endl;
  } else if (Type == "DC") {
    m_RawData->SetZDDDC(Channel, E, TS);
    // i++;
    // std::cout << " DC " << i <<std::endl;
  } else if (Type == "EXO") {
    m_RawData->SetZDDEXO(Channel, E, TS, Time);
    // cout<<"setting zdd exo: "<<Channel<<" "<<E<<" "<<TS<<" "<<Time<<endl;
    // std::cout << " EX ";
  }


#endif
}

void ZddDetector::BuildPhysicalEvent() {
  //   if (NPOptionManager::getInstance()->IsReader() == true) {
  //     m_RawData = &(**r_ReaderEventData);
  //   }

  // apply thresholds and calibration
  PreTreat();

  // match energy and time together
  Match_IC();
  // if (m_PhysicsData->IC_Nbr.size() > 0)
  Match_PL();

  Match_Exo();
  Match_DC();
  SortIC.clear();
  SortPL.clear();
  SortDC.clear();
}

///////////////////////////////////////////////////////////////////////////
void ZddDetector::Match_Exo(){
    m_PhysicsData->Exo_Nbr.clear();
    m_PhysicsData->Exo_E.clear();
    m_PhysicsData->Exo_TS.clear();
    for(auto it = SortEXO.begin(); it != SortEXO.end(); ++it){
      for (const auto& data: it->second){
          m_PhysicsData->Exo_Nbr.push_back(it->first);
          m_PhysicsData->Exo_E.push_back(data.first);
          m_PhysicsData->Exo_TS.push_back(data.second);
      }
 }
}

///////////////////////////////////////////////////////////////////////////
void ZddDetector::Match_DC() {
  m_PhysicsData->DC_Nbr.clear();
  m_PhysicsData->DC_E.clear();
  m_PhysicsData->DC_TS.clear();
  for (auto it = SortDC.begin(); it != SortDC.end(); ++it) {
    for (auto& data : it->second) {
      m_PhysicsData->DC_Nbr.push_back(it->first);
      m_PhysicsData->DC_E.push_back(data.first);
      m_PhysicsData->DC_TS.push_back(data.second);
    }
  }
}
/*SortIC2.clear();

  for (int ps = 0; ps<6; ps++){ //0->5 5CHIOs ZDD + 1 CHIO E9
    for (int i=0; i<m_RawData->GetZDD_IC_Mult(ps); i++){  //[0-PS]th det. : 0,1,2,size(0th det)
      if (m_RawData->GetZDD_IC_E(ps, i) >= m_IC_E_RAW_Threshold) {

        if(!RefTS_Name.empty()){
          std::string TSName = "IC_"+std::to_string(ps);
          //cout<<"the TSNAME is "<<TSName<<"should be : "<<ps<<endl;
          TSEvent->AddTimeStamp(TSName,m_RawData->GetZDD_IC_TS(ps,i));
          TSEvent->AddTimeStamp(RefTS_Name, RefTS);
          //cout<<"debug: IC ts: "<<TSName<<" "<<m_RawData->GetZDD_IC_TS(ps,i)<<" refts "<<RefTS_Name<<" "<<RefTS<<endl;
          if (TSEvent->MatchTS(TSName)){
                //Trying feeding directly to a map!!
                //cout<<"made it"<<endl;
                SortIC2[ps].push_back(std::make_pair(m_RawData->GetZDD_IC_E(ps,i), m_RawData->GetZDD_IC_TS(ps,i)));
          }
          TSEvent->ClearTimeStamps();
        }
        // else, all datas are filled
        else{
          //m_PreTreatedData->SetZDDIC(ps, m_RawData->GetZDD_IC_E(ps,i), m_RawData->GetZDD_IC_TS(ps,i));
          //cout<<" IC "<<ps+1<<" energy "<<m_RawData->GetZDD_IC_E(ps,i)<<" ts "<<m_RawData->GetZDD_IC_TS(ps,i)<<endl;
          SortIC2[ps].push_back(std::make_pair(m_RawData->GetZDD_IC_E(ps,i), m_RawData->GetZDD_IC_TS(ps,i)));
        }
      }
    }
  }*/

void ZddDetector::Match_IC() {
  unsigned long long prevTS = 0;
  unsigned int multTS = m_CalData->GetZDD_ICMult();
  std::map<unsigned int, std::pair<unsigned int, unsigned long long>> loc_map;
  if (multTS > 0)
    for (unsigned int i = 0; i < multTS; i++) {
      // pair of E and TS
      std::pair<unsigned int, unsigned long long> tmp_pair;
      tmp_pair.first = m_CalData->GetZDD_ICE(i);
      tmp_pair.second = m_CalData->GetZDD_ICTS(i);

      long long diffTS = static_cast<long long>(m_CalData->GetZDD_ICTS(i)) -
                         static_cast<long long>(prevTS);

      prevTS = m_CalData->GetZDD_ICTS(i);

      // 8 ticks is 80ns? Close to beam bursts?
      if (diffTS > 8) {
        // If very large it is the first event
        if (diffTS < 1e11) {
          SortIC.push_back(loc_map);
          loc_map.clear();

          // Fill the map if in the coincidence window = same IC event
          loc_map[m_CalData->GetZDD_ICN(i)] = tmp_pair;
        } else {
          // Fill the map if in the coincidence window = same IC event
          loc_map[m_CalData->GetZDD_ICN(i)] = tmp_pair;
        }
      } else {
        // Fill the map if in the coincidence window = same IC event
        loc_map[m_CalData->GetZDD_ICN(i)] = tmp_pair;
      }
      if (i == multTS - 1) {
        SortIC.push_back(loc_map);
        loc_map.clear();
      }
    }

  unsigned short locGroup = 0;
  for (const auto& map : SortIC) {
    locGroup++;
    double locSum;
    unsigned short locMult = 0;
    unsigned long long locTS;
    unsigned int locHere = 0;
    for (const auto& [key, pair] : map) {
      if (pair.first > 0) {
        locSum += pair.first;
        locMult++;
        locTS = pair.second;
        locHere += pow(10, key);
        m_PhysicsData->IC_Nbr.push_back(key);
        m_PhysicsData->IC_E.push_back(pair.first);
        m_PhysicsData->IC_TS.push_back(pair.second);
      }
    }
    m_PhysicsData->ICSum.push_back(locSum);
    m_PhysicsData->ICSumMult.push_back(locMult);
    m_PhysicsData->ICSumTS.push_back(locTS);
    m_PhysicsData->ICSumHere.push_back(locHere);
  }
  m_PhysicsData->ICSumGroupMult.push_back(locGroup);
}
  //////////////////////////////////////VALERIAN's METHOD//////////////////////
/* void ZddDetector::Match_PL() {
  for (unsigned int i = 0; i < m_CalData->GetZDD_PLMult(); i++) {
    SortPL[m_CalData->GetZDD_PLN(i) % 5].push_back(
        std::make_pair(m_CalData->GetZDD_PLE(i), m_CalData->GetZDD_PLTS(i)));
  }
  for (auto it = SortPL.begin(); it != SortPL.end(); ++it) {
    if (it->second.size() == 2) {
      m_PhysicsData->PL_Nbr.push_back(it->first);
      m_PhysicsData->PL_E.push_back(
          sqrt((it->second)[0].first * (it->second)[1].first ));
      m_PhysicsData->PL_TS.push_back(
          ((it->second)[0].second + (it->second)[1].second) / 2);
    }
  }
} */
//////////////////////////////////////////OLD method PS modified//////////////


void ZddDetector::Match_PL(){
    m_PhysicsData->PL_Nbr.clear();
    m_PhysicsData->PL_E.clear();
    m_PhysicsData->PL_TS.clear();
    for(auto it = SortPL.begin(); it != SortPL.end(); ++it){
      for (const auto& data: it->second){
          m_PhysicsData->PL_Nbr.push_back(it->first);
          m_PhysicsData->PL_E.push_back(data.first);
          m_PhysicsData->PL_TS.push_back(data.second);
      }
 }
}

void ZddDetector::PreTreat() {
  ClearPreTreatedData();
  m_IC_Mult = m_RawData->GetZDD_ICMult();
  m_PL_Mult = m_RawData->GetZDD_PLMult();
  if(m_PL_Mult>0)
  //cout<<"plastic mult is: "<<m_PL_Mult<<endl;
  m_DC_Mult = m_RawData->GetZDD_DCMult();
  m_EXO_Mult = m_RawData->GetZDD_EXOMult();
  // cout<<"pretreat ran with m_EXO_mult "<<m_EXO_Mult<<endl;

  // if(m_RawData->GetZDD_DCMult()>0){
  // std::cout << "before = " << m_RawData->GetZDD_DCMult() << " " <<
  // m_CalData->GetZDD_DCMult() << std::endl;
  // }

  for (unsigned int i = 0; i < m_IC_Mult; ++i) {
    // if (m_RawData->GetZDD_ICE(i) > m_IC_E_RAW_Threshold) {
    // std::cout << m_RawData->GetZDD_ICN(i) << " " <<
    // m_RawData->GetZDD_ICE(i)
    //           << " " << Map_IC[m_RawData->GetZDD_ICN(i)] << std::endl;
    // std::cout << m_IC_E_RAW_Threshold << std::endl;
    m_CalData->SetZDDIC(Map_IC[m_RawData->GetZDD_ICN(i)],
                        m_RawData->GetZDD_ICE(i), m_RawData->GetZDD_ICTS(i));
    // }
  }
  SortPL.clear();
  for(int ps = 0; ps<10; ps++){
    for (int i = 0; i<m_RawData->GetZDD_PL_Mult(ps); i++){
      if (m_RawData->GetZDD_PL_E(ps,i) > m_PL_E_RAW_Threshold){
        /* if(!RefTS_Name.empty()){
          std::string TSName = "PL_"+std::to_string(ps);
          TSEvent->AddTimeStamp(TSName,m_RawData->GetZDD_PL_TS(ps,i));
          TSEvent->AddTimeStamp(RefTS_Name, RefTS);

          if (TSEvent->MatchTS(TSName)){
            //m_PreTreatedData->SetZDDPL(ps,m_RawData->GetZDD_PL_E(ps,i), m_RawData->GetZDD_PL_TS(ps,i));
            SortPL[ps].push_back(std::make_pair(m_RawData->GetZDD_PL_E(ps,i), m_RawData->GetZDD_PL_TS(ps,i)));
          }
          TSEvent->ClearTimeStamps();
        }
        // else, all datas are filled
        else{ */
          //m_PreTreatedData->SetZDDPL(ps, m_RawData->GetZDD_PL_E(ps,i), m_RawData->GetZDD_PL_TS(ps,i));
          SortPL[ps].push_back(std::make_pair(m_RawData->GetZDD_PL_E(ps,i), m_RawData->GetZDD_PL_TS(ps,i)));
        //}
      }
    }
  }
  


  SortDC.clear();
  for(int ps = 0; ps<4; ps++){
    for (int i = 0; i<m_RawData->GetZDD_DC_Mult(ps); i++){
      if (m_RawData->GetZDD_DC_E(ps,i)>m_DC_E_RAW_Threshold){
        /*if(!RefTS_Name.empty()){
          std::string TSName = "DC_"+std::to_string(ps);
          TSEvent->AddTimeStamp(TSName,m_RawData->GetZDD_DC_TS(ps,i));
          TSEvent->AddTimeStamp(RefTS_Name, RefTS);

          if (TSEvent->MatchTS(TSName)){
            m_PreTreatedData->SetZDDDC(ps,m_RawData->GetZDD_DC_E(ps,i), m_RawData->GetZDD_DC_TS(ps,i));
          }
          TSEvent->ClearTimeStamps();
        }
        // else, all datas are filled
        else{*/
          //m_PreTreatedData->SetZDDDC(ps, m_RawData->GetZDD_DC_E(ps,i), m_RawData->GetZDD_DC_TS(ps,i));
            SortDC[ps].push_back(std::make_pair(m_RawData->GetZDD_DC_E(ps,i), m_RawData->GetZDD_DC_TS(ps,i)));
        //}
      }
    }
  }

  SortEXO.clear();
  for(int ps = 0; ps<4; ps++){
    for (int i = 0; i<m_RawData->GetZDD_EXO_Mult(ps); i++){
      if (m_RawData->GetZDD_EXO_E(ps,i)>m_EXO_E_RAW_Threshold){
        /*if(!RefTS_Name.empty()){
          std::string TSName = "EXO_"+std::to_string(ps);
          TSEvent->AddTimeStamp(TSName,m_RawData->GetZDD_EXO_TS(ps,i));
          TSEvent->AddTimeStamp(RefTS_Name, RefTS);

          if (TSEvent->MatchTS(TSName)){
            m_PreTreatedData->SetZDDEXO(ps,m_RawData->GetZDD_EXO_E(ps,i), m_RawData->GetZDD_EXO_TS(ps,i));
          }
          TSEvent->ClearTimeStamps();
        }
        // else, all datas are filled
        else{*/
          //m_PreTreatedData->SetZDDEXO(ps, m_RawData->GetZDD_EXO_E(ps,i), m_RawData->GetZDD_EXO_TS(ps,i));
          SortEXO[ps].push_back(std::make_pair(m_RawData->GetZDD_EXO_E(ps,i), m_RawData->GetZDD_EXO_TS(ps,i)));
        //}
      }
    }
  }

  // if(m_RawData->GetZDD_DCMult()>0){
  // std::cout << "after = " << m_RawData->GetZDD_DCMult() << " " <<
  // m_CalData->GetZDD_DCMult() << std::endl;
  // }
}

///////////////////////////////////////////////////////////////////////////
void ZddDetector::PreTreatEnergy(std::string Detector) {
  /*unsigned int mysize = m_RawData->GetMultEnergy(Detector);
    for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_RawData->Get_Energy(Detector, i) > m_E_RAW_Threshold) {
    Double_t Energy =
    Cal->ApplyCalibration("ZDD/ENERGY"+nptool::itoa(m_RawData->GetE_DetectorNbr(Detector,
    i)),m_RawData->Get_Energy(Detector, i)); if (Energy > m_E_Threshold) {
    if(Detector == "IC")
    m_CalData->Set_IC_Energy(m_RawData->GetE_DetectorNbr(Detector, i),
    Energy);

    else if(Detector == "Plastic")
    m_CalData->Set_Plastic_Energy(m_RawData->GetE_DetectorNbr(Detector,
    i), Energy);
    }
    }
    }
    */
}

///////////////////////////////////////////////////////////////////////////
void ZddDetector::PreTreatTime(std::string Detector) {
  /*  unsigned int mysize = m_RawData->GetMultTime(Detector);
      for (UShort_t i = 0; i < mysize; ++i) {
      Double_t Time=
      Cal->ApplyCalibration("ZDD/TIME"+nptool::itoa(m_RawData->GetT_DetectorNbr(Detector,
      i)),m_RawData->Get_Time(Detector,i)); if(Detector == "IC")
      m_CalData->Set_IC_Time(m_RawData->GetT_DetectorNbr(Detector, i), Time);
      else if(Detector == "Plastic")
      m_CalData->Set_Plastic_Time(m_RawData->GetT_DetectorNbr(Detector, i),
      Time); else if(Detector == "DC")
      m_CalData->Set_DC_Time(m_RawData->GetT_DetectorNbr(Detector, i), Time);
      }
      */
}

///////////////////////////////////////////////////////////////////////////
void ZddDetector::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigZDD.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigZDD.dat found: Default parameter loaded for Analayis "
         << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigZDD.dat " << endl;

  //// Save it in a TAsciiFile
  // TAsciiFile* asciiConfig =
  // RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  // asciiConfig->AppendLine("%%% ConfigZDD.dat %%%");
  // asciiConfig->Append(FileName.c_str());
  // asciiConfig->AppendLine("");
  //  read analysis config file
  string LineBuffer, DataBuffer, whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigZDD";
    if (LineBuffer.compare(0, name.length(), name) == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus) {
      whatToDo = "";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n');
      }

      else if (whatToDo == "IC_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_IC_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_IC_E_RAW_Threshold << endl;
      } else if (whatToDo == "PL_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_PL_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_PL_E_RAW_Threshold << endl;
      } else if (whatToDo == "DC_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_DC_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_DC_E_RAW_Threshold << endl;
      } else if (whatToDo == "EXO_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_EXO_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_EXO_E_RAW_Threshold << endl;
      } else if (whatToDo == "MAP_IC") {
        AnalysisConfigFile >> DataBuffer;
        Map_IC[atoi(DataBuffer.substr(0, 1).c_str())] =
            atoi(DataBuffer.substr(1, 1).c_str());
        cout << whatToDo << " " << atoi(DataBuffer.substr(0, 1).c_str()) << " "
             << atoi(DataBuffer.substr(1, 1).c_str()) << endl;
      } else if (whatToDo == "E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_E_Threshold << endl;
      } else {
        ReadingStatus = false;
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
//	tranform an integer to a string
double ZddDetector::fZDDEXO_E(const unsigned int& i) {
  static string name;
  name = "ZDDEXO_E";
  name += nptool::itoa(m_RawData->GetZDD_EXON(i));
  return m_Cal.ApplyCalibration(name, m_RawData->GetZDD_EXOE(i), 1);
}

////////////////////////////////////////////////////////////////////////////////
double ZddDetector::fZDDEXO_Time(const unsigned int& i) {
  static string name;
  name = "ZDDEXO_Time";
  name += nptool::itoa(m_RawData->GetZDD_EXON(i));
  return m_Cal.ApplyCalibration(name, m_RawData->GetZDD_EXOTime(i), 1);
}

////////////////////////////////////////////////////////////////////////////////
void ZddDetector::InitializeDataInputRaw(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("zdd", "zdd::ZddData", &m_RawData);
}
////////////////////////////////////////////////////////////////////////////////
void ZddDetector::InitializeDataInputPhysics(
    std::shared_ptr<nptool::VDataInput> input) {
  // input->Attach("zdd", "zdd::ZddData", &m_RawData);
  input->Attach("zdd", "zdd::ZddPhysics", &m_PhysicsData);
}
////////////////////////////////////////////////////////////////////////////////
void ZddDetector::InitializeDataOutputRaw(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("zdd", "zdd::ZddData", &m_RawData);
}
////////////////////////////////////////////////////////////////////////////////
void ZddDetector::InitializeDataOutputPhysics(
    std::shared_ptr<nptool::VDataOutput> output) {
  // output->Attach("zdd", "zdd::ZddData", &m_RawData);
  output->Attach("zdd", "zdd::ZddPhysics", &m_PhysicsData);
}

////////////////////////////////////////////////////////////////////////////////
void ZddDetector::InitSpectra() {
  m_Spectra = std::make_shared<zdd::ZddSpectra>();
};
////////////////////////////////////////////////////////////////////////////////
void ZddDetector::FillSpectra() {
  auto app = nptool::Application::GetApplication();

  if (app->HasFlag("--input-raw")) m_Spectra->FillRaw();
  if (app->HasFlag("--input-phy")) m_Spectra->FillPhy();
};
////////////////////////////////////////////////////////////////////////////////
void ZddDetector::WriteSpectra(){};
////////////////////////////////////////////////////////////////////////////////
void ZddDetector::CheckSpectra(){};
////////////////////////////////////////////////////////////////////////////////
void ZddDetector::ClearSpectra() { m_Spectra->Clear(); };

////////////////////////////////////////////////////////////////////////////////
extern "C" {
shared_ptr<nptool::VDetector> ConstructDetector() {
  return make_shared<zdd::ZddDetector>();
};
}

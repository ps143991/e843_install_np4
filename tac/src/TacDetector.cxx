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
#include "TacDetector.h"
#include "NPFunction.h"
#include "NPRootPlugin.h"
#include <iostream>
#include "TacSpectra.h"
using namespace tac;
using namespace std;
using namespace ROOT::Math;

////////////////////////////////////////////////////////////////////////////////
TacDetector::TacDetector() {
  m_RawData     = new tac::TacData();
  m_CalData     = new tac::TacData();
  m_PhysicsData = new tac::TacPhysics();
  m_Cal.InitCalibration();
}

TacDetector::~TacDetector() {}

////////////////////////////////////////////////////////////////////////////////
void TacDetector::ReadConfiguration(nptool::InputParser parser) {

  auto                     blocks = parser.GetAllBlocksWithToken("tac");
  std::vector<std::string> token
      = {"Board", "NUMEXOChannel", "NUMEXOChannelName"};

  for (unsigned int i = 0; i < blocks.size(); i++) {
    cout << i << endl;
    if (blocks[i]->HasTokenList(token)) {

      std::string  Type  = "TAC";
      unsigned int Board = blocks[i]->GetInt("Board");

      std::vector<int> NUMEXOChannel = blocks[i]->GetVectorInt("NUMEXOChannel");

      unsigned int NUMEXOChannel_size = NUMEXOChannel.size();

      std::vector<std::string> NUMEXOChannelName
          = blocks[i]->GetVectorString("NUMEXOChannelName");
      unsigned int NUMEXOChannelName_size = NUMEXOChannel.size();

      if (NUMEXOChannel_size != NUMEXOChannelName_size) {
        std::cout
            << "Number of TAC channel different than number of TAC name!!!"
            << std::endl;
        exit(1);
      }

      for (unsigned int j = 0; j < NUMEXOChannel_size; j++) {
        unsigned int index       = Board * 1e3 + (unsigned int)NUMEXOChannel[j];
        SetNumexoMap(Board, (unsigned int)NUMEXOChannel[j],
                                     NUMEXOChannelName[j]);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void TacDetector::SetNumexoMap(unsigned int Board, unsigned int NUMEXOChannel,
                               std::string NUMEXOChannelName) {
  unsigned int index = Board * 1e3 + NUMEXOChannel;
  m_NumexoMap[index] = NUMEXOChannelName;
}

////////////////////////////////////////////////////////////////////////////////
std::string TacDetector::GetNameFromNumexoMap(unsigned int Board,
                                              unsigned int Channel) {
  unsigned int index = Board * 1e3 + Channel;
  std::string  Name  = m_NumexoMap[index];
  return Name;
}

////////////////////////////////////////////////////////////////////////////////
void TacDetector::BuildRawEvent(const std::string& daq_name,
                                const std::string& st_type_key,
                                void*              commonframe) {
#ifdef MFM_FOUND
  int type_key = ((MFMCommonFrame*)commonframe)->GetFrameType();
  if (type_key == MFM_REA_GENE_FRAME_TYPE) {
    TreatFrame((MFMCommonFrame*)commonframe);
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void TacDetector::TreatFrame(void* commonframe) {
#ifdef MFM_FOUND

  std::shared_ptr<MFMReaGenericFrame> ReaFrame = std::make_shared<MFMReaGenericFrame>();
  ReaFrame->SetAttributs(((MFMCommonFrame*)commonframe)->GetPointHeader());

  int                Board   = -1;
  int                Channel = -1;
  unsigned short     E       = 0;
  unsigned long long TS      = 0;
  int                Time    = -1;

  Board   = ReaFrame->GetBoardId();
  Channel = ReaFrame->GetTGCristalId();
  E       = ReaFrame->GetEnergy();
  TS      = ReaFrame->GetTimeStamp();
  Time    = ReaFrame->GetTime();

  std::string name = GetNameFromNumexoMap(Board, Channel);
  if (name!=""){
    m_RawData->SetTAC(Channel, Time, TS, name);
  }

#endif
}

void TacDetector::BuildPhysicalEvent() {
  PreTreat();
  Match_TAC();
}

void TacDetector::PreTreat() {
  ClearPreTreatedData();

  m_TAC_Mult = m_RawData->GetTAC_Mult();
  for (unsigned int i = 0; i < m_TAC_Mult; ++i) {
    if (m_RawData->GetTAC_Time(i) > m_TAC_Time_RAW_Threshold) {
      m_CalData->SetTAC(m_RawData->GetTAC_N(i), m_RawData->GetTAC_Time(i),
                        m_RawData->GetTAC_TS(i), m_RawData->GetTAC_Name(i));
    }
  }
}

void TacDetector::Match_TAC() {
  for (unsigned int i = 0; i < m_CalData->GetTAC_Mult(); i++) {
    SortTAC[m_CalData->GetTAC_Name(i)]
        = std::make_pair(m_CalData->GetTAC_Time(i), m_CalData->GetTAC_TS(i));
  }
  for (auto it = SortTAC.begin(); it != SortTAC.end(); ++it) {
    m_PhysicsData->TAC_Name.push_back(it->first);
    m_PhysicsData->TAC_Time.push_back((it->second).first);
    m_PhysicsData->TAC_TS.push_back((it->second).second);
  }
}

////////////////////////////////////////////////////////////////////////////////
void TacDetector::InitializeDataInputRaw(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("tac", "tac::TacData", &m_RawData);
}
////////////////////////////////////////////////////////////////////////////////
void TacDetector::InitializeDataInputPhysics(
    std::shared_ptr<nptool::VDataInput> input) {
  input->Attach("tac", "tac::TacPhysics", &m_PhysicsData);
}
////////////////////////////////////////////////////////////////////////////////
void TacDetector::InitializeDataOutputRaw(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("tac", "tac::TacData", &m_RawData);
}
////////////////////////////////////////////////////////////////////////////////
void TacDetector::InitializeDataOutputPhysics(
    std::shared_ptr<nptool::VDataOutput> output) {
  output->Attach("tac", "tac::TacPhysics", &m_PhysicsData);
}
////////////////////////////////////////////////////////////////////////////////
void TacDetector::InitSpectra() {
  m_Spectra = std::make_shared<tac::TacSpectra>();
};
////////////////////////////////////////////////////////////////////////////////
void TacDetector::FillSpectra() {
  m_Spectra->FillRaw();
  m_Spectra->FillPhy();
};
////////////////////////////////////////////////////////////////////////////////
void TacDetector::WriteSpectra() {};
////////////////////////////////////////////////////////////////////////////////
void TacDetector::CheckSpectra() {};
////////////////////////////////////////////////////////////////////////////////
void TacDetector::ClearSpectra() { m_Spectra->Clear(); };

extern "C" {
shared_ptr<nptool::VDetector> ConstructDetector() {
  return make_shared<tac::TacDetector>();
};
}

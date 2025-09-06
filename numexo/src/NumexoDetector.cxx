#include "NumexoDetector.h"

#include <iostream>

#include "NPFunction.h"
#include "NPRootPlugin.h"
#include "NumexoSpectra.h"
using namespace numexo;
using namespace std;
using namespace ROOT::Math;

////////////////////////////////////////////////////////////////////////////////
NumexoDetector::NumexoDetector() {}

NumexoDetector::~NumexoDetector() {}

////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::ReadConfiguration(nptool::InputParser parser) {
  auto blocks = parser.GetAllBlocksWithToken("numexo");
  std::vector<std::string> token = {"Board", "NUMEXOChannel",
                                    "NUMEXOChannelName"};

  for (unsigned int i = 0; i < blocks.size(); i++) {
    if (blocks[i]->HasTokenList(token)) {
      unsigned int Board = blocks[i]->GetInt("Board");
      unsigned int NUMEXOChannel = blocks[i]->GetInt("NUMEXOChannel");

      std::string NUMEXOChannelName = blocks[i]->GetString("NUMEXOChannelName");

      std::string branch_name = NUMEXOChannelName;
      std::string branch_nameTS = branch_name + "_TS";
      GenericRawBranch[branch_name] = new std::vector<unsigned int>;
      GenericRawBranchTS[branch_nameTS] = new std::vector<unsigned long long>;
      GenericPhysicsBranch[branch_name] = new std::vector<unsigned int>;
      GenericPhysicsBranchTS[branch_nameTS] =
          new std::vector<unsigned long long>;
      unsigned int index = Board * 1e3 + (unsigned int)NUMEXOChannel;
      SetNumexoMap(Board, (unsigned int)NUMEXOChannel, NUMEXOChannelName);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::SetNumexoMap(unsigned int Board,
                                  unsigned int NUMEXOChannel,
                                  std::string NUMEXOChannelName) {
  unsigned int index = Board * 1e3 + NUMEXOChannel;
  m_NumexoMap[index] = NUMEXOChannelName;
}

////////////////////////////////////////////////////////////////////////////////
std::string NumexoDetector::GetNameFromNumexoMap(unsigned int Board,
                                                 unsigned int Channel) {
  unsigned int index = Board * 1e3 + Channel;
  std::string Name = m_NumexoMap[index];
  return Name;
}

void NumexoDetector::BuildRawEvent(const std::string& daq_name,
                                   const std::string& st_type_key,
                                   void* commonframe) {
#ifdef MFM_FOUND
  int type_key = ((MFMCommonFrame*)commonframe)->GetFrameType();
  if (type_key == MFM_REA_GENE_FRAME_TYPE) {
    TreatFrame((MFMCommonFrame*)commonframe);
  }
#endif
}

void NumexoDetector::TreatFrame(void* commonframe) {
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
  if (TS == 0) {
    std::cout << "/////////////////////////////" << std::endl;
    std::cout << "  NUMEXO  WARNING TS IS 0    " << std::endl;
    std::cout << "/////////////////////////////" << std::endl;
  }
 

  std::string name = GetNameFromNumexoMap(Board, Channel);
  if (name != "") {
    if (GenericRawBranch.find(name) != GenericRawBranch.end()) {
      if (E > 0) {
        (GenericRawBranch[name])->push_back((unsigned int)E);
      } else if (Time > 0) {
        (GenericRawBranch[name])->push_back((unsigned int)Time);
      }
      (GenericRawBranchTS[(name + "_TS")])->push_back((unsigned long long)TS);
    }
  }
#endif
}

void NumexoDetector::PreTreat() {}

void NumexoDetector::BuildPhysicalEvent() {
  ClearEventPhysics();

  for (auto& it : GenericRawBranch) {
    *GenericPhysicsBranch[it.first] = *(it.second);
  }

  for (auto& it : GenericRawBranchTS) {
    *GenericPhysicsBranchTS[it.first] = *(it.second);
  }
}

////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::InitializeDataInputRaw(
    std::shared_ptr<nptool::VDataInput> input) {
  for (auto& it : GenericRawBranch) {
    std::string class_name = "std::vector<unsigned int>";
    input->Attach(it.first, class_name, &(it.second));
  }
  for (auto& it : GenericRawBranchTS) {
    std::string class_name = "std::vector<unsigned long long>";
    input->Attach(it.first, class_name, &(it.second));
  }
}
////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::InitializeDataInputPhysics(
    std::shared_ptr<nptool::VDataInput> input) {
  for (auto& it : GenericPhysicsBranch) {
    std::string class_name = "std::vector<unsigned int>";
    input->Attach(it.first, class_name, &(it.second));
  }
  for (auto& it : GenericPhysicsBranchTS) {
    std::string class_name = "std::vector<unsigned long long>";
    input->Attach(it.first, class_name, &(it.second));
  }
}
////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::InitializeDataOutputRaw(
    std::shared_ptr<nptool::VDataOutput> output) {
  for (auto it : GenericRawBranch) {
    std::string class_name = "std::vector<unsigned int>";
    output->Attach(it.first, class_name, &it.second[0]);
  }
  for (auto it : GenericRawBranchTS) {
    std::string class_name = "std::vector<unsigned long long>";
    output->Attach(it.first, class_name, &it.second[0]);
  }
}
////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::InitializeDataOutputPhysics(
    std::shared_ptr<nptool::VDataOutput> output) {
  for (auto it : GenericPhysicsBranch) {
    std::string class_name = "std::vector<unsigned int>";
    output->Attach(it.first, class_name, &it.second[0]);
  }
  for (auto it : GenericPhysicsBranchTS) {
    std::string class_name = "std::vector<unsigned long long>";
    output->Attach(it.first, class_name, &it.second[0]);
  }
}

void NumexoDetector::ClearEventData() {
  for (auto it : GenericRawBranch) (it.second)->clear();
  for (auto it : GenericRawBranchTS) (it.second)->clear();
}

void NumexoDetector::ClearEventPhysics() {
  for (auto it : GenericPhysicsBranch) (it.second)->clear();
  for (auto it : GenericPhysicsBranchTS) (it.second)->clear();
}

////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::InitSpectra() {
  m_Spectra = std::make_shared<numexo::NumexoSpectra>();
};
////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::FillSpectra() {
  m_Spectra->FillRaw();
  m_Spectra->FillPhy();
};
////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::WriteSpectra(){};
////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::CheckSpectra(){};
////////////////////////////////////////////////////////////////////////////////
void NumexoDetector::ClearSpectra() { m_Spectra->Clear(); };
////////////////////////////////////////////////////////////////////////////////
extern "C" {
shared_ptr<nptool::VDetector> ConstructDetector() {
  return make_shared<numexo::NumexoDetector>();
};
}

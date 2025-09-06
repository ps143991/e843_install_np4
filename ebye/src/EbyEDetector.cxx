#include "EbyEDetector.h"
#include "NPFunction.h"
#include "NPRootPlugin.h"
#include <iostream>
using namespace ebye;
using namespace std;
using namespace ROOT::Math;

////////////////////////////////////////////////////////////////////////////////
EbyEDetector::EbyEDetector() {}

EbyEDetector::~EbyEDetector() {}

////////////////////////////////////////////////////////////////////////////////
void EbyEDetector::ReadConfiguration(nptool::InputParser parser) {
  auto blocks = parser.GetAllBlocksWithToken("ebye");
  std::vector<std::string> token = {"BranchName"};
  for (auto block : blocks) {
    if (block->HasTokenList(token)) {
      std::string branch_name = block->GetString("BranchName"); 
      std::string branch_nameTS = branch_name + "TS"; 
      GenericRawBranch[branch_name] = new unsigned int;
      GenericPhysicsBranch[branch_name] = new unsigned int;
      GenericRawBranchTS[branch_nameTS] = new unsigned long long;
      GenericPhysicsBranchTS[branch_nameTS] = new unsigned long long;
    }
    if (block->HasToken("ActionFile") /*&& !faction_file_initialized*/) {
      ReadActionFile(block->GetString("ActionFile"));
    }
  }

  // get_action_file
  auto app = nptool::Application::GetApplication();
  if (app->HasFlag("--action")) {
    auto arg = app->GetVectorArg("--action");
    std::cout << arg[0] << std::endl;
    if (arg.size()) {
      ReadActionFile(arg[0]);
    }
  }
}

void EbyEDetector::BuildRawEvent(const std::string& daq_name,
    const std::string& st_type_key,
    void*              commonframe) {
#ifdef MFM_FOUND
  int type_key = ((MFMCommonFrame*)commonframe)->GetFrameType();
  if (type_key == MFM_EBY_EN_TS_FRAME_TYPE) {
    TreatFrame((MFMCommonFrame*)commonframe);
  }
#endif
}

void EbyEDetector::TreatFrame(void* commonframe) {
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
    std::cout << "   EBYE  WARNING TS IS 0     " << std::endl;
    std::cout << "/////////////////////////////" << std::endl;
  }
  for (unsigned short i = 0; i < NItems; i++) {
    EbyEframe->EbyedatGetParameters(i, &label_id, &value);
    LblData[0] = label_id;
    LblData[1] = value;

    label = fLabelMap[label_id];              //PS: in nptool3 (in npmfm) file npmfmebyeframes.cxx line pGATCONF.push_back(value); flters only those 
    //that have "MM", "MG", or "CATS" in them
    if (GenericRawBranch.find(label) != GenericRawBranch.end()) {
      // if(TS == 0){
      // }
      // std::cout << "val: " << value << std::endl;
      // std::cout << "label: " << label << std::endl;
      // std::cout << "TS: " << TS << std::endl;
      if ((unsigned int)value>100)
      *(GenericRawBranch[label]) = 64;
      else
      *(GenericRawBranch[label]) = (unsigned int)value;
      //if(value > 100)   //TODO: remove this :PS
      //std::cout<<""<<(unsigned int)value<<std::endl;
     
      *(GenericRawBranchTS[(label+"TS")]) = (unsigned long long)TS;
    }
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////
void EbyEDetector::ReadActionFile(std::string ActionFile) {
  faction_file_initialized = true;
  std::string LabelName;
  ifstream    file(ActionFile, std::ifstream::in);
  int         Label, c, d;
  while (file >> LabelName >> Label >> c >> d) {
    fLabelMap[Label] = LabelName;
  }
}
void EbyEDetector::PreTreat() {}

void EbyEDetector::BuildPhysicalEvent() {

  for(auto it: GenericPhysicsBranch)
    *(it.second)= 0;
  for(auto it: GenericPhysicsBranchTS)
    *(it.second)= 0;

  for(auto it: GenericRawBranch){
    if ((unsigned int)(*it.second) > 0){
      // std::cout << it.first << " " << (unsigned int)(*it.second) << std::endl;
      // std::cout << it.first << " " << it.second << std::endl;
      if((unsigned int)(*it.second) > 100)
      {cout<<"\033[1;31mwarning gatconf went more than 100, so changed to 64\033[0m"<<endl; *(GenericPhysicsBranch[it.first]) = 64;}
    else
      *(GenericPhysicsBranch[it.first]) = (unsigned int)(*it.second);}
  }
  for(auto it: GenericRawBranchTS){
    if ((unsigned int)(*it.second) > 0)
    *(GenericPhysicsBranchTS[it.first]) = (unsigned long long)(*it.second);
  }
};

////////////////////////////////////////////////////////////////////////////////
void EbyEDetector::InitializeDataInputRaw(
    std::shared_ptr<nptool::VDataInput> input) {
  for (auto it : GenericRawBranch) {
    std::string class_name = it.first + "/i";
    input->Attach(it.first, class_name, it.second);
  }
  for (auto it : GenericRawBranchTS) {
    std::string class_name = it.first + "/l";
    input->Attach(it.first, class_name, it.second);
  }
}
////////////////////////////////////////////////////////////////////////////////
void EbyEDetector::InitializeDataInputPhysics(
    std::shared_ptr<nptool::VDataInput> input) {
  for (auto it : GenericPhysicsBranch){
    std::string class_name = it.first + "/i";
    input->Attach(it.first, class_name, it.second);
  }
  for (auto it : GenericPhysicsBranchTS) {
    std::string class_name = it.first + "/l";
    input->Attach(it.first, class_name, it.second);
  }
}
////////////////////////////////////////////////////////////////////////////////
void EbyEDetector::InitializeDataOutputRaw(
    std::shared_ptr<nptool::VDataOutput> output) {
  for (auto it : GenericRawBranch) {
    std::string class_name = it.first + "/i";
    output->Attach(it.first, class_name, it.second);
  }
  for (auto it : GenericRawBranchTS) {
    std::string class_name = it.first + "/l";
    output->Attach(it.first, class_name, it.second);
  }
}
////////////////////////////////////////////////////////////////////////////////
void EbyEDetector::InitializeDataOutputPhysics(
    std::shared_ptr<nptool::VDataOutput> output) {
  for (auto it : GenericPhysicsBranch){
    std::string class_name = it.first + "/i";
    output->Attach(it.first, class_name, it.second);
  }
  for (auto it : GenericPhysicsBranchTS) {
    std::string class_name = it.first + "/l";
    output->Attach(it.first, class_name, it.second);
  }
}

void EbyEDetector::ClearEventData() {
  for(auto it: GenericRawBranch)
    *(it.second)= 0;
  for(auto it: GenericRawBranchTS)
    *(it.second)= 0;
}

void EbyEDetector::ClearEventPhysics() {
  for(auto it: GenericPhysicsBranch)
    *(it.second)= 0;
  for(auto it: GenericPhysicsBranchTS)
    *(it.second)= 0;
}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
  shared_ptr<nptool::VDetector> ConstructDetector() {
    return make_shared<ebye::EbyEDetector>();
  };
}

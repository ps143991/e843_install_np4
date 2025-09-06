#include "Analysis_ZDD.h"
#include "NPApplication.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_ZDD::Init() {
  auto app = nptool::Application::GetApplication();

    tac = std::dynamic_pointer_cast<tac::TacDetector>(app->GetDetector("tac"));

}


////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_ZDD::TreatEvent() {
    Clear();
}


void user_analysis::Analysis_ZDD::SetDataOutput(std::shared_ptr<nptool::VDataOutput> output) {
  auto Tree = std::dynamic_pointer_cast<nptool::RootOutput>(output)->GetTree();

  
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_ZDD::Clear() {

}



////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_ZDD::End() {}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserAnalysis> ConstructUserAnalysis() { return std::make_shared<user_analysis::Analysis_ZDD>(); }
}

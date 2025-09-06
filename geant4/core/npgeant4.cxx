#include "NPApplication.h"
#include "NPException.h"
#include "NPG4Session.h"
#include "NPVDataOutput.h"

int main(int argc, char** argv) {
  try {
    // instantiate an application
    auto app = nptool::Application::InitApplication(argc, argv);
    app->Start();
    ////////////////////////////////////////////////////////////////////////////
    // Data Output
    std::shared_ptr<nptool::VDataOutput> output;
    if (app->HasFlag("--output")) {
      auto output_arg = app->GetVectorArg("--output");
      if (output_arg.size()) {
        output = app->ConstructDataOutput(output_arg[0]);
        if (output) {
          output->Init(output_arg);
        }
        else {
          throw(nptool::Error("npgeant4", "Fail to construct data output " + output_arg[0]));
        }
      }
      else
        throw(nptool::Error("npgeant4", "--output flag provided with no arguments, at least one needed"));
    }
    // app->InitializeDataOutputRaw(output);
    ////////////////////////////////////////////////////////////////////////////
    // start a geant4 instance
    auto g4session = nptool::geant4::Session::GetSession();
    g4session->Start();

    // end of app
    app->Stop();
  }

  // Catch nptool exception
  catch (nptool::Warning& ex) {
    nptool::DisplayException(ex);
  }
  catch (nptool::Error& ex) {
    nptool::DisplayException(ex);
    return 1;
  }

  // any other exception
  catch (...) {
    return 1;
  }
  return 0;
}

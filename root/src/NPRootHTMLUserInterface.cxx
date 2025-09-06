#include "NPRootHTMLUserInterface.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TKey.h"
#include <filesystem>
namespace fs = std::filesystem;

////////////////////////////////////////////////////////////////////////////////
void RootProxyClearSpectra() { nptool::Application::GetApplication()->ClearSpectra(); }

////////////////////////////////////////////////////////////////////////////////
void nptool::root::RootHTLMUserInterface::Init(std::vector<std::string> arg) {
  if (arg.size() > 1) {
    // start the server on requested port
    std::string address = "http:" + arg[1] + ";noglobal?top=nponline&thrds=2&websocket_disable";
    m_http = std::make_shared<THttpServer>(address.c_str());
    if (arg.size() > 2) {
      nptool::message("green", "root", "nptool::RootHTMLUserInterface", "Loading custom user interface at " + arg[2]);
      m_http->SetDefaultPage(arg[2]);
    }
    // m_http->CreateServerThread();
    m_http->SetReadOnly(kFALSE);
    // link a command to a function
    m_http->RegisterCommand("/Command/ClearSpectra", "RootProxyClearSpectra()", "button;rootsys/icons/ed_delete.png");

    // Add Command ad a quick access icon
    m_http->SetItemField("/Command/ClearSpectra", "_fastcmd", "true");
    // hide the command folder for nite interface
    m_http->Hide("/Command");
    // Set the refresh rate to 500ms
    m_http->SetItemField("/", "_monitoring", "500"); // monitoring interval in ms

    nptool::message("green", "root", "nptool::RootHTMLUserInterface", "starting html interface on port " + arg[1]);
  }
};
////////////////////////////////////////////////////////////////////////////////
void nptool::root::RootHTLMUserInterface::ExecuteMacro(const std::string& path) {
  static std::string fullpath;
  static fs::directory_entry d(path);
  if (d.is_directory()) {
    for (const auto& entry : fs::directory_iterator(path)) {
      fullpath = entry.path().string();
      if (fullpath.compare(fullpath.size() - 4, 4, ".cxx") == 0) {
        fullpath = ".x " + fullpath;
        gROOT->ProcessLine(fullpath.c_str());
      }
    }
  }
  else if (d.is_regular_file()) {
  }
}

////////////////////////////////////////////////////////////////////////////////
void nptool::root::RootHTLMUserInterface::WriteSpectra(const std::string& path) {
  std::cout << "writing spectra ... ";
  auto List = gDirectory->GetList();
  auto file = new TFile(path.c_str(), "RECREATE");
  auto size = List->GetEntries();
  for (unsigned int i = 0; i < size; i++) {
    auto obj = List->At(i);
    if (obj->IsA()->InheritsFrom("TH1")) {
      if (i % 2 == 0)
        std::cout << "\rwriting spectra ... |" << std::flush;
      else
        std::cout << "\rwriting spectra ... /" << std::flush;
      obj->Write();
    }
  }

  std::cout << "\r                     " << std::flush;
  ;
  file->Close();
}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserInterface> ConstructUserInterface() {
  return std::make_shared<nptool::root::RootHTLMUserInterface>();
}
}

#ifndef NPRootHTLMUserInterface_h
#define NPRootHTLMUserInterface_h 1
// C++ header
#include <map>
#include <memory>
#include <string>
#include <vector>

// nptool
#include "NPApplication.h"
#include "NPException.h"
#include "NPFunction.h"
#include "NPVUserInterface.h"

// root
#include "TError.h"
#include "THttpServer.h"
#include "TNamed.h"
#include "TROOT.h"
#include "TSystem.h"
////////////////////////////////////////////////////////////////////////////////
// Command for the HTML interface
void RootProxyClearSpectra();

////////////////////////////////////////////////////////////////////////////////
namespace nptool {
  namespace root {
    //////////////////////////////////////////////////////////////////////////////
    class RootHTLMUserInterface : public nptool::VUserInterface {
     public:
      RootHTLMUserInterface() { gErrorIgnoreLevel = kError; };
      ~RootHTLMUserInterface(){};

     public: // inheritted from VUserUserInterface, must be implemented
      // Initialise the input
      void Init(std::vector<std::string> arg);
      bool WaitEvent() { return !gSystem->ProcessEvents(); }
      void ExecuteMacro(const std::string& path);
      void WriteSpectra(const std::string& path);

     private:
      std::shared_ptr<THttpServer> m_http;
    };
  } // namespace root
} // namespace nptool
#endif

#ifndef NPG4VPrimaryGenerator_h
#define NPG4VPrimaryGenerator_h

#include "G4Event.hh"
#include "NPInputParser.h"
#include "NPVPrimaryGenerator.h"
namespace nptool {
  namespace geant4 {
    class VPrimaryGenerator : virtual public nptool::VPrimaryGenerator {
     public:
      VPrimaryGenerator() = default;
      ~VPrimaryGenerator() = default;

     public:
      // Init variable
      virtual void ReadConfiguration(nptool::InputParser&) = 0;
      // Generate Event
      virtual void GeneratePrimaries(void*) = 0;
      // dependencies of the plugin
      virtual std::vector<std::string> GetDependencies() = 0;
    };
  } // namespace geant4
} // namespace nptool
#endif


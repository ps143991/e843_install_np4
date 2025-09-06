#ifndef NPG4VBeamProcess_h
#define NPG4VBeamProcess_h
// nptool
#include "NPInputParser.h"
#include "NPVReactionGenerator.h"
// geant4
#include "G4FastStep.hh"
#include "G4FastTrack.hh"
#include "G4ParticleDefinition.hh"
#include "G4RegionStore.hh"
#include "G4VFastSimulationModel.hh"

namespace nptool {
  namespace geant4 {
    class VBeamProcess : public nptool::VReactionGenerator, public G4VFastSimulationModel {
     public:
      VBeamProcess()
          : G4VFastSimulationModel("NPBeamProcess", G4RegionStore::GetInstance()->FindOrCreateRegion("NPFastProcess")) {
      }
      VBeamProcess(std::string name)
          : G4VFastSimulationModel(name.c_str(), G4RegionStore::GetInstance()->FindOrCreateRegion("NPFastProcess")){};
      ~VBeamProcess(){};

     public:
      virtual void ReadConfiguration(std::string& file) {
        nptool::InputParser parser(file);
        ReadConfiguration(parser);
      };
      virtual void ReadConfiguration(nptool::InputParser& parser) = 0;
      virtual G4bool IsApplicable(const G4ParticleDefinition&) = 0;
      virtual G4bool ModelTrigger(const G4FastTrack&) = 0;
      virtual void DoIt(const G4FastTrack&, G4FastStep&) = 0;
    };
  } // namespace geant4
} // namespace nptool

#endif


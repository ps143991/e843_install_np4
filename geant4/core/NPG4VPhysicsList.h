#ifndef NPG4PhysicsList_h
#define NPG4PhysicsList_h 1

// nptool
#include "NPVPhysicsProcess.h"

// nptool geant4 plugin
#include "NPG4VBeamProcess.h"

// G4
#include "G4VModularPhysicsList.hh"

namespace nptool {
  namespace geant4 {
    class VPhysicsList : public virtual nptool::VPhysicsProcess, public virtual G4VModularPhysicsList {
     public:
      VPhysicsList() = default;
      virtual ~VPhysicsList() = default;
      void AddFastProcessManager();
      void ReadFastProcessConfiguration(std::string& file);
      //  virtual void ConstructParticle() { std::cout << "Virtual 1 " << std::endl; };
      //  virtual void ConstructProcess() { std::cout << "Virtual 2 " << std::endl; };
     private:
      G4Region* m_ReactionRegion;

     private:
      std::vector<std::shared_ptr<nptool::geant4::VBeamProcess>> m_fast_process;
    };
  } // namespace geant4
} // namespace nptool
#endif


#ifndef NPG4BasePhysicsList_h
#define NPG4BasePhysicsList_h

#include "NPG4VPhysicsList.h"

#include "G4EmConfigurator.hh"
namespace nptool {
  namespace geant4 {
    class BasePhysicsList : public virtual nptool::geant4::VPhysicsList {

     public:
      BasePhysicsList();
      ~BasePhysicsList(){};

      void ConstructParticle();
      void ConstructProcess();

     private: // Physics List
      G4VPhysicsConstructor* emPhysicsList;
      G4EmConfigurator* emConfig;

      // Decay
      G4VPhysicsConstructor* decay_List;
      G4VPhysicsConstructor* radioactiveDecay_List;

    };
  } // namespace geant4
} // namespace nptool

#endif

#ifndef NPG4PrimaryGeneratorAction_h
#define NPG4PrimaryGeneratorAction_h
// C++ headers
#include <string>

// G4 headers
#include "G4Event.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

// npgeant4

// using namespace CLHEP;
namespace nptool {
  namespace geant4 {

    class VPrimaryGenerator;
    class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
     public:
      PrimaryGeneratorAction();
      ~PrimaryGeneratorAction();

     public:
      void GeneratePrimaries(G4Event*);

     public:
      void ReadConfiguration(std::string file);
      void ClearEventGenerator();

     private:
      std::vector<std::shared_ptr<nptool::geant4::VPrimaryGenerator>> m_primary;
    };
  } // namespace geant4
} // namespace nptool
#endif


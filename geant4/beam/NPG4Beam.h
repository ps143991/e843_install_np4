#ifndef NPG4Beam_h
#define NPG4Beam_h

// nptool
#include "NPInputParser.h"
#include "NPVPrimaryGenerator.h"

// Physics Essential
#include "NPBeam.h"
#include "NPParticle.h"

// npgeant4
#include "NPG4VPrimaryGenerator.h"
// geant4
#include "G4Event.hh"
// std
#include <string>
#include <vector>

using namespace std;

namespace nptool {
  namespace geant4 {
    class Beam : public nptool::geant4::VPrimaryGenerator {
     public: // Constructor and destructor
      Beam();
      virtual ~Beam();

     public:
      void ReadConfiguration(nptool::InputParser&);
      void GeneratePrimaries(void*);
      // void InitializeRootOutput();
      //  void SetTarget(Target* Target);
      std::vector<std::string> GetDependencies();

     private:                           // Source parameter
      G4ParticleDefinition* m_particle; // Kind of particle to shoot
      nptool::Beam* m_Beam;

     private: // Target Value
              //  Target* m_Target;
    };
  } // namespace geant4
} // namespace nptool
#endif

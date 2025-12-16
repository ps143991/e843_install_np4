#ifndef NPG4Source_h
#define NPG4Source_h

// nptool
#include "NPInputParser.h"
#include "NPVPrimaryGenerator.h"

// npgeant4
#include "NPG4VPrimaryGenerator.h"
// geant4
#include "G4Event.hh"
// std
#include <string>
#include <vector>

namespace nptool {
  namespace geant4 {
    class SourceParameter {
     public:
      SourceParameter(G4ThreeVector pos, std::string& n, std::vector<double> e, double tmin, double tmax)
          : position(pos), particle_name(n), energies(e), particle(nullptr), theta_min(tmin), theta_max(tmax){};

     public:
      G4ThreeVector position;
      std::string particle_name;
      G4ParticleDefinition* particle;
      std::vector<double> energies;
      double theta_min;
      double theta_max;
    };
    class Source : public nptool::geant4::VPrimaryGenerator {
     public:
      Source() = default;
      ~Source() = default;

     public:
      void ReadConfiguration(nptool::InputParser&);
      // Generate Event
      void GeneratePrimaries(void*);
      std::vector<std::string> GetDependencies();

     private:
      std::vector<SourceParameter> m_sources;
    };

  } // namespace geant4
} // namespace nptool
#endif

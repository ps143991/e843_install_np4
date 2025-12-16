// nptool
#include "NPApplication.h"
#include "NPException.h"
#include "NPFunction.h"

// Physics Essential
#include "NPParticle.h"

// npgeant4
#include "NPG4Source.h"

// Geant4
#include "G4IonTable.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Run.hh"
#include "Randomize.hh"

// STD
#include <iostream>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
void nptool::geant4::Source::ReadConfiguration(nptool::InputParser& parser) {
  auto                     blocks = parser.GetAllBlocksWithToken("source");
  std::vector<std::string> info   = {"position", "particle", "energies"};
  std::vector<std::string> opt    = {"half-angle-min", "half-angle-max"};
  for (auto block : blocks) {
    if (block->HasTokenList(info)) {
      nptool::message("green", "", "", "Source event generator");
      std::vector<double> pos           = block->GetVector3("position", "cm");
      auto                particle_name = block->GetString("particle");
      std::vector<double> energies = block->GetVectorDouble("energies", "MeV");

      // position conversion
      auto                  g4pos = G4ThreeVector(pos[0], pos[1], pos[2]);
      G4ParticleDefinition* p     = nullptr;
      // particle name conversion
      if (particle_name == "gamma" || particle_name == "neutron"
          || particle_name == "opticalphoton" || particle_name == "mu+"
          || particle_name == "mu-" || particle_name == "e-") {
        p = G4ParticleTable::GetParticleTable()->FindParticle(
            particle_name.c_str());
      } else {
        if (particle_name == "proton")
          particle_name = "1H";
        else if (particle_name == "deuton")
          particle_name = "2H";

        else if (particle_name == "triton")
          particle_name = "3H";

        else if (particle_name == "alpha")
          particle_name = "4He";
      }

      // optional token
      double tmin = 0;
      double tmax = 180;
      if (block->HasTokenList(opt)) {
        tmin = block->GetDouble("half-angle-min", "deg");
        tmax = block->GetDouble("half-angle-max", "deg");
      }
      m_sources.push_back(
          SourceParameter(g4pos, particle_name, energies, tmin, tmax));
    }

    else {
      std::string msg = "Missing token for Source event generator  blocks";
      std::string tip = "Expected token list: ";
      for (auto token : info)
        tip += token + ",";
      throw(nptool::Error("geant4::Source", msg, tip));
    }
  }
};

// Generate Event
void nptool::geant4::Source::GeneratePrimaries(void* anEvent) {
  for (auto& source : m_sources) {
    auto m_particleGun = new G4ParticleGun(1);
    if (!source.particle) {
      auto N = new nptool::Particle(source.particle_name);
      source.particle
          = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(
              N->GetZ(), N->GetA(), 0);
      delete N;
    }
    m_particleGun->SetParticleDefinition(source.particle);
    // randomize direction
    double cos_theta_min = cos(source.theta_min);
    double cos_theta_max = cos(source.theta_max);
    double cos_theta
        = cos_theta_min
          + (cos_theta_max - cos_theta_min) * CLHEP::RandFlat::shoot();
    double theta = acos(cos_theta);
    double phi   = CLHEP::RandFlat::shoot() * 2 * pi;
    auto   dir   = G4ThreeVector(0, 0, 1);
    dir.setTheta(theta);
    dir.setPhi(phi);
    m_particleGun->SetParticleMomentumDirection(dir);

    m_particleGun->SetParticlePosition(source.position);

    // randomize energy
    unsigned int rand
        = floor(CLHEP::RandFlat::shoot() * source.energies.size());
    m_particleGun->SetParticleEnergy(source.energies[rand]);

    // generate particle
    m_particleGun->GeneratePrimaryVertex((G4Event*)anEvent);
  }
};

// From VPlugin
std::vector<std::string> nptool::geant4::Source::GetDependencies() {
  return {"geant4", "physics-essential"};
};

extern "C" {
std::shared_ptr<nptool::VPrimaryGenerator> ConstructPrimaryGenerator() {
  return std::make_shared<nptool::geant4::Source>();
}
}
/*__attribute__((constructor)) void LoadDependencies() {
  nptool::Application::GetApplication()->LoadPlugin("physics-essential");
}*/

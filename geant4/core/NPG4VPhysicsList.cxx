// nptool
#include "NPApplication.h"
#include "NPInputParser.h"

// nptool geant4 plugin
#include "NPG4Session.h"
#include "NPG4VPhysicsList.h"

// Geant4
#include "G4FastSimulationManager.hh"
#include "G4FastSimulationManagerProcess.hh"
#include "G4ProcessManager.hh"
#include "G4RegionStore.hh"
#include "G4StepLimiter.hh"
void nptool::geant4::VPhysicsList::AddFastProcessManager() {
  // Use the store to insure the Region is the same in here and in the Detector Plugin
  m_ReactionRegion = G4RegionStore::GetInstance()->FindOrCreateRegion("NPFastProcess");
  //  G4FastSimulationManager* BeamReactionManager = m_ReactionRegion->GetFastSimulationManager();
  G4FastSimulationManagerProcess* BeamReactionManager = new G4FastSimulationManagerProcess("NPFastProcess");

  auto app = nptool::Application::GetApplication();
  if (app->HasFlag("--event-generator")) {
    auto event_files = app->GetVectorArg("--event-generator");
    for (auto file : event_files)
      ReadFastProcessConfiguration(file);
  }
  if (app->HasFlag("-E")) {
    auto event_files = app->GetVectorArg("-E");
    for (auto file : event_files)
      ReadFastProcessConfiguration(file);
  }

  G4ParticleTable::G4PTblDicIterator* theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    std::string name = particle->GetParticleName();
    pmanager->AddDiscreteProcess(BeamReactionManager);
    // Add a Step limiter to the beam particle.
    // This will be used to limit the step of the beam in the target
    pmanager->AddProcess(new G4StepLimiter, -1, -1, 5);
  }
}

////////////////////////////////////////////////////////////////////////////////
void nptool::geant4::VPhysicsList::ReadFastProcessConfiguration(std::string& file) {
  nptool::InputParser parser(file);
  auto blocks_token = parser.GetAllBlocksToken();
  auto app = nptool::Application::GetApplication();
  std::set<std::string> viewed;
  for (const auto& token : blocks_token) {
    if (viewed.find(token) == viewed.end()) {
      viewed.insert(token);
      app->LoadPlugin(token, true);
      auto fast_process =
          std::dynamic_pointer_cast<nptool::geant4::VBeamProcess>(app->ConstructReactionGenerator(token));
      if (fast_process) {
        fast_process->ReadConfiguration(parser);
        m_fast_process.push_back(fast_process);
      }
      else {
        message("yellow", "core", "nptool::geant4::Physics list", token + " does not name a beam process");
      }
    }
  }
  return;
};

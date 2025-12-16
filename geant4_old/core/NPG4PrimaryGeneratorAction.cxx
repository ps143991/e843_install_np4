// STL
#include "math.h"
#include <cstdlib>
#include <memory>

// G4
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

// CLHEP
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandGeneral.h"

// Geant4
#include "NPG4PrimaryGeneratorAction.h"
#include "NPG4VPrimaryGenerator.h"
// nptool
#include "NPApplication.h"
#include "NPInputParser.h"
using namespace nptool::geant4;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  for (auto& p : m_primary) {
    p->GeneratePrimaries(anEvent);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::ReadConfiguration(std::string file) {
  nptool::InputParser parser(file);
  auto blocks_token = parser.GetAllBlocksToken();
  auto app = nptool::Application::GetApplication();
  std::set<std::string> viewed;
  for (const auto& token : blocks_token) {
    if (viewed.find(token) == viewed.end()) {
      viewed.insert(token);
      app->LoadPlugin(token, true);
      auto primary =
          std::dynamic_pointer_cast<nptool::geant4::VPrimaryGenerator>(app->ConstructPrimaryGenerator(token));
      if (primary) {
        primary->ReadConfiguration(parser);
        m_primary.push_back(primary);
      }
      else {
        message("yellow", "core", "nptool::geant4::PrimaryGeneratorAction",
                token + " does not name a primary generator");
      }
    }
  }
  return;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::ClearEventGenerator() {}


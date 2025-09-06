#include "NPG4BasePhysicsList.h"
#include "NPFunction.h"

// G4
#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronElasticProcess.hh"
#include "G4He3.hh"
#include "G4IonConstructor.hh"
#include "G4Ions.hh"
#include "G4LossTableManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
/////////////////////////////////////////////////////////////////////////////
nptool::geant4::BasePhysicsList::BasePhysicsList() : G4VUserPhysicsList() {
  nptool::message("green", "", "", "Loading Base Physics List");
  emPhysicsList = new G4EmStandardPhysics_option4();
  emConfig = G4LossTableManager::Instance()->EmConfigurator();
}

/////////////////////////////////////////////////////////////////////////////
void nptool::geant4::BasePhysicsList::ConstructParticle() {
  // If decay is not activated we have to declare the particle ourself
  G4He3::He3Definition();
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4Alpha::AlphaDefinition();
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  G4Gamma::GammaDefinition();
  //  mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4Eta::EtaDefinition();
  G4EtaPrime::EtaPrimeDefinition();
  //  G4RhoZero::RhoZeroDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4AntiKaonZero::AntiKaonZeroDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();
  // Ion
  G4IonConstructor ionConstructor;
  ionConstructor.ConstructParticle();
}
/////////////////////////////////////////////////////////////////////////////
void nptool::geant4::BasePhysicsList::ConstructProcess() {
  // Transportation
  AddTransportation();
  // Electromagnetic physics
  emPhysicsList->ConstructProcess();
  return;
}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VPhysicsProcess> ConstructPhysicsProcess() {
  return std::make_shared<nptool::geant4::BasePhysicsList>();
};
}

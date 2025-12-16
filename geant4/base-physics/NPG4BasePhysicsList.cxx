#include "NPG4BasePhysicsList.h"
#include "NPFunction.h"


// G4
#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronElasticProcess.hh"
#include "G4He3.hh"
#include "G4IonConstructor.hh"
#include "G4Ions.hh"
#include "G4LossTableManager.hh"

// Decay
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronicParameters.hh"
#include "G4NuclideTable.hh"

#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"
/////////////////////////////////////////////////////////////////////////////
nptool::geant4::BasePhysicsList::BasePhysicsList() : G4VUserPhysicsList()
{
  emPhysicsList = new G4EmStandardPhysics_option4();
  emConfig = G4LossTableManager::Instance()->EmConfigurator();

  std::cout << "//// Radioactive decay activated ////" << std::endl;
  decay_List = new G4DecayPhysics();
  radioactiveDecay_List = new G4RadioactiveDecayPhysics();
  // Half-life threshold set at 5 times the Half-life of 239Pu
  G4HadronicParameters::Instance()->SetTimeThresholdForRadioactiveDecay(2.0e+05 * CLHEP::year);
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1 * CLHEP::picosecond);
}

/////////////////////////////////////////////////////////////////////////////
void nptool::geant4::BasePhysicsList::ConstructParticle()
{
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
  // Decay
  decay_List->ConstructParticle();
  radioactiveDecay_List->ConstructParticle();
}
/////////////////////////////////////////////////////////////////////////////
void nptool::geant4::BasePhysicsList::ConstructProcess()
{
  // Transportation
  AddTransportation();
  // Electromagnetic physics
  emPhysicsList->ConstructProcess();
  // Decay
  decay_List->ConstructProcess();
  radioactiveDecay_List->ConstructProcess();
  return;
}
////////////////////////////////////////////////////////////////////////////////
extern "C"
{
  std::shared_ptr<nptool::VPhysicsProcess> ConstructPhysicsProcess()
  {
    return std::make_shared<nptool::geant4::BasePhysicsList>();
  };
}

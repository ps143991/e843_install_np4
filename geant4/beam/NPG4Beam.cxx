// C++
#include <limits>

// G4 header
#include "G4IonTable.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// npgeant4
#include "NPG4Beam.h"

// nptool header
// #include "RootOutput.h"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
nptool::geant4::Beam::Beam() {
  // m_ParticleStack = ParticleStack::getInstance();
  // m_Target         = NULL    ;
  m_particle = NULL;
  m_Beam = new nptool::Beam();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
nptool::geant4::Beam::~Beam() { delete m_Beam; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void   EventGeneratorBeam::SetTarget(Target* Target){
//   if(Target!=0)
//     m_Target = Target;

//   // Set the target parameter for the internal event generator of m_Beam
//   m_Beam->SetTargetSize(m_Target->GetTargetRadius());
//   m_Beam->SetTargetAngle(m_Target->GetTargetAngle());
//   m_Beam->SetTargetThickness(m_Target->GetTargetThickness());
//   m_Beam->SetTargetZ(m_Target->GetTargetZ());

// }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void nptool::geant4::Beam::ReadConfiguration(nptool::InputParser& parser) { m_Beam->ReadConfiguration(parser); }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void nptool::geant4::Beam::GeneratePrimaries(void* anEvent) {
  if (((G4Event*)anEvent)->GetEventID() == 0) {
    // Define the particle to be shoot
    if (m_Beam->GetZ() == 0 && m_Beam->GetA() == 1)
      m_particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
    else if (m_Beam->GetName() == "electron")
      m_particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    else if (m_Beam->GetName() == "gamma")
      m_particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");

    else
      m_particle = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(m_Beam->GetZ(), m_Beam->GetA(),
                                                                              m_Beam->GetExcitationEnergy());
  }

  ///////////////////////////////////////////////////////////////////////
  ///// Calculate the incident beam direction as well as the vertex /////
  ///// of interaction in target and Energy Loss of the beam within /////
  ///// the target.                                                 /////
  ///////////////////////////////////////////////////////////////////////
  // G4ThreeVector InterCoord;

  auto m_particleGun = std::make_shared<G4ParticleGun>(1);
  double InitialBeamEnergy, x0, y0, z0, Beam_thetaX, Beam_phiY;

  m_Beam->GenerateRandomEvent(InitialBeamEnergy, x0, y0, z0, Beam_thetaX, Beam_phiY);
  double Xdir = tan(Beam_thetaX); // tan(thetax)= px/pz
  double Ydir = tan(Beam_phiY);   // tan(phiy)= py/pz
  double Zdir = 1;                // fix pz=1 arbitrarily
  G4ThreeVector BeamDir(Xdir, Ydir, Zdir);
  BeamDir = BeamDir.unit();
  G4ThreeVector BeamPos(x0, y0, z0);

  ///////////////////////////////////////////////////////
  ///// Add the Beam particle to the particle Stack /////
  ///////////////////////////////////////////////////////
  // nptool::Particle BeamParticle(m_particle, InitialBeamEnergy,
  //                               InitialBeamEnergy, BeamDir.unit(), BeamPos, 1);

  m_particleGun->SetParticleDefinition(m_particle);
  m_particleGun->SetParticleMomentumDirection(BeamDir);
  m_particleGun->SetParticlePosition(BeamPos);
  m_particleGun->SetParticleEnergy(InitialBeamEnergy);
  m_particleGun->GeneratePrimaryVertex((G4Event*)anEvent);
  // m_ParticleStack->AddBeamParticleToStack(BeamParticle);
}

// From VPlugin
std::vector<std::string> nptool::geant4::Beam::GetDependencies() { return {"geant4", "physics-essential"}; };

extern "C" {
std::shared_ptr<nptool::VPrimaryGenerator> ConstructPrimaryGenerator() {
  return std::make_shared<nptool::geant4::Beam>();
}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void EventGeneratorBeam::InitializeRootOutput(){
// }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

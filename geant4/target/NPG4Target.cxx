// C++ header
#include <fstream>
#include <limits>

// G4
#include "G4Colour.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4EmCalculator.hh"
#include "G4FastSimulationManager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4PVPlacement.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4RegionStore.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "Randomize.hh"
using namespace CLHEP;

// npgeant4
#include "NPG4MaterialManager.h"
#include "NPG4Session.h"
#include "NPG4Target.h"

// std
#include <dlfcn.h>

// Physics Process
// #include "BeamReaction.hh"
// #include "Decay.hh"
// #include "FissionDecay.hh"
// #include "ThreeBody.hh"

// NPL
#include "NPInputParser.h"
#include "NPOptionManager.h"
using namespace std;
using namespace nptool::geant4;
// using namespace target;

G4Target* G4Target::TargetInstance = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Specific Method of this class
G4Target::G4Target() {
  m_target =
      std::dynamic_pointer_cast<nptool::geant4::Target>(nptool::Application::GetApplication()->GetDetector("target"));
  InitializeTarget();
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Target::~G4Target() {
  unsigned int size = m_ReactionModel.size();
  for (unsigned int i = 0; i < size; i++) {
    delete m_ReactionModel[i];
  }
  m_ReactionModel.clear();
}

void G4Target::InitializeTarget() {

  if (m_target->m_TargetType) { // standard target
    // Standard parameter
    m_TargetThickness = m_target->m_TargetThickness;
    m_TargetRadius = m_target->m_TargetRadius;
    m_TargetAngle = m_target->m_TargetAngle;
    m_TargetNbSlices = m_target->m_TargetNbSlices;

    m_TargetBackingThickness = m_target->m_TargetBackingThickness;

    m_TargetMaterial = GetMaterialFromLibrary(m_target->m_TargetMaterial, m_target->m_TargetDensity);
    if (m_TargetBackingThickness > 0)
      m_TargetBackingMaterial = GetMaterialFromLibrary(m_target->m_TargetBackingMaterial);
  }
  else { // cryogenic target

    m_TargetDensity = m_target->m_TargetDensity;
    m_FrontDeformation = m_target->m_FrontDeformation;
    m_FrontThickness = m_target->m_FrontThickness;
    m_FrontRadius = m_target->m_FrontRadius;
    m_BackDeformation = m_target->m_BackDeformation;
    m_BackRadius = m_target->m_BackRadius;
    m_BackThickness = m_target->m_BackThickness;
    m_FrameRadius = m_target->m_FrameRadius;
    m_FrameThickness = m_target->m_FrameThickness;
    m_FrontCone = m_target->m_FrontCone;
    m_BackCone = m_target->m_BackCone;
    m_ShieldInnerRadius = m_target->m_ShieldInnerRadius;
    m_ShieldOuterRadius = m_target->m_ShieldOuterRadius;
    m_ShieldTopLength = m_target->m_ShieldTopLength;
    m_ShieldFrontRadius = m_target->m_ShieldFrontRadius;
    m_ShieldBackRadius = m_target->m_ShieldBackRadius;
    m_ShieldBottomLength = m_target->m_ShieldBottomLength;

    m_FrontMaterial = GetMaterialFromLibrary(m_target->m_FrontMaterial);
    m_BackMaterial = GetMaterialFromLibrary(m_target->m_BackMaterial);
    m_FrameMaterial = GetMaterialFromLibrary(m_target->m_FrameMaterial);
    m_ShieldMaterial = GetMaterialFromLibrary(m_target->m_ShieldMaterial);

    m_TargetMaterial = nptool::geant4::MaterialManager::GetMaterialManager()->GetMaterialFromLibrary(
        m_target->m_TargetMaterial, m_target->m_TargetDensity);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Material* G4Target::GetMaterialFromLibrary(G4String MaterialName, double density) {
  return nptool::geant4::MaterialManager::GetMaterialManager()->GetMaterialFromLibrary(MaterialName, density);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
// void G4Target::ConstructDetector(G4LogicalVolume* world) {
void G4Target::ConstructDetector() {
  auto g4session = nptool::geant4::Session::GetSession();
  auto world = g4session->GetWorldLogicalVolume();

  if (m_target->m_TargetType) { // case of standard target
    if (m_TargetThickness > 0) {
      m_TargetSolid = new G4Tubs("solidTarget", 0, m_TargetRadius, 0.5 * m_TargetThickness, 0 * deg, 360 * deg);
      m_TargetLogic = new G4LogicalVolume(m_TargetSolid, m_TargetMaterial, "logicTarget");

      if (m_TargetBackingThickness > 0) {
        m_TargetBackingSolid =
            new G4Tubs("solidTargetBacking", 0, m_TargetRadius, 0.5 * m_TargetBackingThickness, 0 * deg, 360 * deg);
        m_TargetBackingLogic = new G4LogicalVolume(m_TargetBackingSolid, m_TargetBackingMaterial, "logicTargetBacking");
      }
      // rotation of target
      G4RotationMatrix* rotation = new G4RotationMatrix();
      rotation->rotateY(m_TargetAngle);

      new G4PVPlacement(rotation, G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ), m_TargetLogic, "Target", world, false,
                        0);

      G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(1, 0.4, 0.4));
      m_TargetLogic->SetVisAttributes(TargetVisAtt);

      if (m_TargetBackingThickness > 0) {
        new G4PVPlacement(
            rotation,
            G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ + m_TargetThickness * 0.5 + m_TargetBackingThickness * 0.5),
            m_TargetBackingLogic, "TargetBacking", world, false, 0);
        G4VisAttributes* TargetBackingVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0));
        m_TargetBackingLogic->SetVisAttributes(TargetBackingVisAtt);
      }
    }
  }
  /*
    else { // case of cryogenic target
      // X-Z target profile
      unsigned int        size = 100;
      std::vector<double> OuterRadius;
      std::vector<double> InnerRadius;
      std::vector<double> Z;

      // Front Bulge
      if (m_FrontDeformation != 0) {
        double step = m_FrontRadius / size;
        for (unsigned int i = 0; i < size; i++) {
          OuterRadius.push_back(i * step);
          Z.push_back(FrontProfile(i * step, m_TargetThickness * 0.5,
                                   m_FrontDeformation, m_FrontRadius));
        }
      }

      // Nominal Part
      OuterRadius.push_back(m_FrontRadius);
      Z.push_back(0.5 * m_TargetThickness);

      OuterRadius.push_back(m_TargetRadius);
      Z.push_back(0.5 * m_TargetThickness);

      OuterRadius.push_back(m_TargetRadius);
      Z.push_back(-0.5 * m_TargetThickness);

      OuterRadius.push_back(m_BackRadius);
      Z.push_back(-0.5 * m_TargetThickness);

      // Back Bulge
      if (m_BackDeformation != 0) {
        double step = m_BackRadius / size;
        for (int i = size - 1; i >= 0; i--) {
          OuterRadius.push_back(i * step);
          Z.push_back(BackProfile(i * step, m_TargetThickness * 0.5,
                                  m_BackDeformation, m_BackRadius));
        }
      }

      InnerRadius.resize(Z.size(), 0);

      m_CryoTargetSolid = new G4Polycone("solidTarget", 0, 360 * deg, Z.size(),
                                         &Z[0], &InnerRadius[0], &OuterRadius[0]);

      m_TargetLogic = new G4LogicalVolume(m_CryoTargetSolid, m_TargetMaterial,
                                          "logicTarget");

      G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));
      m_TargetLogic->SetVisAttributes(TargetVisAtt);

      /////// Creating the windows volume
      // X-Z target profile
      OuterRadius.clear();
      InnerRadius.clear();
      Z.clear();

      // Front Bulge
      if (m_FrontDeformation != 0) {
        double step = m_FrontRadius / size;
        for (unsigned int i = 0; i < size; i++) {
          OuterRadius.push_back(i * step + m_FrontThickness);
          Z.push_back(FrontProfile(i * step,
                                   m_TargetThickness * 0.5 + m_FrontThickness,
                                   m_FrontDeformation, m_FrontRadius));
        }
      }

      // Nominal Part (same as target)
      OuterRadius.push_back(m_FrontRadius);
      Z.push_back(0.5 * m_TargetThickness);

      OuterRadius.push_back(m_TargetRadius);
      Z.push_back(0.5 * m_TargetThickness);

      OuterRadius.push_back(m_TargetRadius);
      Z.push_back(-0.5 * m_TargetThickness);

      OuterRadius.push_back(m_BackRadius);
      Z.push_back(-0.5 * m_TargetThickness);

      // Back Bulge
      if (m_BackDeformation != 0) {
        double step = m_BackRadius / size;
        for (int i = size - 1; i >= 0; i--) {
          OuterRadius.push_back(i * step + m_BackThickness);
          Z.push_back(BackProfile(i * step,
                                  m_TargetThickness * 0.5 + m_BackThickness,
                                  m_BackDeformation, m_BackRadius));
        }
      }

      InnerRadius.resize(Z.size(), 0);

      G4Polycone* WindowsSolid
          = new G4Polycone("solidWindows", 0, 360 * deg, Z.size(), &Z[0],
                           &InnerRadius[0], &OuterRadius[0]);

      G4LogicalVolume* WindowsLogic
          = new G4LogicalVolume(WindowsSolid, m_FrontMaterial, "logicWindows");

      new G4PVPlacement(0, G4ThreeVector(0, 0, 0), WindowsLogic, "TargetWindows",
                        world, false, 0);

      // Place the windows
      G4VisAttributes* WindowsVisAtt
          = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
      WindowsLogic->SetVisAttributes(WindowsVisAtt);

      // Place the target inside the windows
      new G4PVPlacement(0, G4ThreeVector(0, 0, 0), m_TargetLogic, "Target",
                        WindowsLogic, false, 0);

      // Frame
      OuterRadius.clear();
      InnerRadius.clear();
      Z.clear();
      double FrontInner = 0.5 * (m_FrameThickness - m_TargetThickness)
                          / tan(0.5 * M_PI - m_FrontCone);
      double BackInner = 0.5 * (m_FrameThickness - m_TargetThickness)
                         / tan(0.5 * M_PI - m_BackCone);

      Z.push_back(m_FrameThickness * 0.5);
      InnerRadius.push_back(FrontInner + m_FrontRadius);
      OuterRadius.push_back(m_FrameRadius);
      Z.push_back(m_TargetThickness * 0.5);
      InnerRadius.push_back(m_FrontRadius);
      OuterRadius.push_back(m_FrameRadius);
      Z.push_back(m_TargetThickness * 0.5);
      InnerRadius.push_back(m_TargetRadius);
      OuterRadius.push_back(m_FrameRadius);
      Z.push_back(-m_TargetThickness * 0.5);
      InnerRadius.push_back(m_TargetRadius);
      OuterRadius.push_back(m_FrameRadius);
      Z.push_back(-m_TargetThickness * 0.5);
      InnerRadius.push_back(m_BackRadius);
      OuterRadius.push_back(m_FrameRadius);
      Z.push_back(-m_FrameThickness * 0.5);
      InnerRadius.push_back(BackInner + m_BackRadius);
      OuterRadius.push_back(m_FrameRadius);

      G4Polycone* FrameSolid
          = new G4Polycone("solidFrame", 0, 360 * deg, Z.size(), &Z[0],
                           &InnerRadius[0], &OuterRadius[0]);

      G4LogicalVolume* FrameLogic
          = new G4LogicalVolume(FrameSolid, m_FrameMaterial, "logicFrame");

      new G4PVPlacement(0, G4ThreeVector(0, 0, 0), FrameLogic, "FrameTarget",
                        world, false, 0);
      G4VisAttributes* FrameVisAtt
          = new G4VisAttributes(G4Colour(0.3, 0.4, 0.4, 1));
      FrameLogic->SetVisAttributes(FrameVisAtt);

      G4RotationMatrix* Rotation = new G4RotationMatrix();
      Rotation->rotateX(90 * deg);

      // Heat Shield
      G4Tubs* ShieldMain
          = new G4Tubs("HeatShieldMain", m_ShieldInnerRadius, m_ShieldOuterRadius,
                       m_ShieldBottomLength + m_ShieldTopLength, 0, 360);

      G4Tubs* ShieldBack = new G4Tubs("HeatShieldBack", 0, m_ShieldBackRadius,
                                      m_ShieldInnerRadius, 0, 360);

      G4Tubs* ShieldFront = new G4Tubs("HeatShieldFront", 0, m_ShieldFrontRadius,
                                       m_ShieldInnerRadius, 0, 360);

      G4RotationMatrix Rot;
      Rot.rotateX(90 * deg);

      G4Transform3D transformBack(
          Rot, G4ThreeVector(0, m_ShieldInnerRadius, -m_ShieldTopLength));

      G4SubtractionSolid* subtraction = new G4SubtractionSolid(
          "Shield-Back", ShieldMain, ShieldBack, transformBack);

      G4Transform3D transformFront(
          Rot, G4ThreeVector(0, -m_ShieldInnerRadius, -m_ShieldTopLength));

      G4SubtractionSolid* ShieldSolid = new G4SubtractionSolid(
          "Shield-Front", subtraction, ShieldFront, transformFront);

      G4LogicalVolume* ShieldLogic
          = new G4LogicalVolume(ShieldSolid, m_ShieldMaterial, "logicShield");

      new G4PVPlacement(Rotation, G4ThreeVector(0, m_ShieldTopLength, 0),
                        ShieldLogic, "ShieldTarget", world, false, 0);
      G4VisAttributes* ShieldVisAtt
          = new G4VisAttributes(G4Colour(0.7, 0.9, 0.9, 0.5));
      ShieldLogic->SetVisAttributes(ShieldVisAtt);
    }
  */
  SetReactionRegion();
}
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4Target::SetReactionRegion() {
  // Use the store to insure the Region is the same in here and in VPhysicsList
  m_ReactionRegion = G4RegionStore::GetInstance()->FindOrCreateRegion("NPFastProcess");
  m_ReactionRegion->AddRootLogicalVolume(m_TargetLogic);
  m_ReactionRegion->SetUserLimits(new G4UserLimits(m_TargetThickness / m_TargetNbSlices));
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//// Add Detector branch to the EventTree.
//// Called After DetectorConstruction::AddDetector Method
// void G4Target::InitializeRootOutput()
//{}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//// Read sensitive part and fill the Root tree.
//// Called at in the EventAction::EndOfEventAction
// void G4Target::ReadSensitive(const G4Event*)
//{}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void G4Target::RandomGaussian2D(double MeanX, double MeanY, double SigmaX,
// double SigmaY, double &X, double &Y, double NumberOfSigma){
//   if (SigmaX != 0) {
//     X = 2 * NumberOfSigma*SigmaX;
//     while (X > NumberOfSigma*SigmaX) X = RandGauss::shoot(MeanX, SigmaX);

//    double a = NumberOfSigma * SigmaX/2;
//    double b = NumberOfSigma * SigmaY/2;
//    double SigmaYPrim = b * sqrt(1 - X*X / (a*a));

//    SigmaYPrim = 2*SigmaYPrim / NumberOfSigma;
//    Y = RandGauss::shoot(MeanY, SigmaYPrim);
//  }
//  else {
//    X = MeanX;
//    Y = RandGauss::shoot(MeanY, SigmaY);
//  }
//}

extern "C" {
std::shared_ptr<nptool::geant4::VDetector> ConstructDetectorSimulation() {
  return make_shared<nptool::geant4::G4Target>();
};
}

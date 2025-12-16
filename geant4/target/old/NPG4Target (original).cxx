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
  m_TargetType             = true;
  m_TargetThickness        = 0;
  m_TargetAngle            = 0;
  m_TargetRadius           = 0;
  m_TargetDensity          = 0;
  m_TargetNbSlices         = 100.; // Number of sslices/steps by default
  m_TargetBackingThickness = 0;
  m_ReactionRegion         = NULL;

  m_TargetDensity      = 0;
  m_FrontDeformation   = 0;
  m_FrontThickness     = 0;
  m_FrontRadius        = 0;
  m_FrontMaterial      = 0;
  m_BackDeformation    = 0;
  m_BackRadius         = 0;
  m_BackThickness      = 0;
  m_BackMaterial       = 0;
  m_FrameRadius        = 0;
  m_FrameThickness     = 0;
  m_FrontCone          = 0;
  m_BackCone           = 0;
  m_FrameMaterial      = 0;
  m_ShieldInnerRadius  = 0;
  m_ShieldOuterRadius  = 0;
  m_ShieldBottomLength = 0;
  m_ShieldTopLength    = 0;
  m_ShieldFrontRadius  = 0;
  m_ShieldBackRadius   = 0;
  m_ShieldMaterial     = 0;

  TargetInstance = this;
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Target::~G4Target() {
  unsigned int size = m_ReactionModel.size();
  for (unsigned int i = 0; i < size; i++) {
    delete m_ReactionModel[i];
  }
  m_ReactionModel.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Material* G4Target::GetMaterialFromLibrary(G4String MaterialName) {
  return nptool::geant4::MaterialManager::GetMaterialManager()
      ->GetMaterialFromLibrary(MaterialName, m_TargetDensity);
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//// Inherite Method of Vdetector Class

//// Read stream at Configfile to pick-up parameters of detector (Position,...)
//// Called in DetecorConstruction::ReadDetextorConfiguration Method
void G4Target::ReadConfiguration(nptool::InputParser parser) {
  auto starget_blocks = parser.GetAllBlocksWithToken("target");
  auto ctarget_blocks = parser.GetAllBlocksWithToken("cryogenic_target");

  for (auto starget : starget_blocks) {
    cout << "////       TARGET      ////" << endl;
    cout << "//// Solid Target found " << endl;
    vector<string> token
        = {"thickness", "radius", "material", "angle", "x", "y", "z"};
    if (starget->HasTokenList(token)) {
      m_TargetThickness = starget->GetDouble("thickness", "micrometer");
      m_TargetAngle     = starget->GetDouble("angle", "deg");
      m_TargetRadius    = starget->GetDouble("radius", "mm");
      m_TargetMaterial = GetMaterialFromLibrary(starget->GetString("material"));
      m_TargetX        = starget->GetDouble("x", "mm");
      m_TargetY        = starget->GetDouble("y", "mm");
      m_TargetZ        = starget->GetDouble("z", "mm");
    } else {
      cout << "ERROR: Target token list incomplete, check your input file"
           << endl;
      exit(1);
    }

    if (starget->HasToken("NbSlices"))
      m_TargetNbSlices = starget->GetInt("NbSlices");

    if (starget->HasToken("BackingMaterial")
        && starget->HasToken("BackingThickness")) {
      m_TargetBackingMaterial
          = GetMaterialFromLibrary(starget->GetString("BackingMaterial"));
      m_TargetBackingThickness
          = starget->GetDouble("BackingThickness", "micrometer");
    }
  }

  for (auto ctarget : ctarget_blocks) {
    m_TargetType = false;
    cout << "//// Cryogenic Target found " << endl;

    vector<string> CoreToken  = {"NominalThickness",
                                 "Material",
                                 "Density",
                                 "Radius",
                                 "Angle",
                                 "X",
                                 "Y",
                                 "Z"};
    vector<string> FrontToken = {"FrontDeformation", "FrontThickness",
                                 "FrontRadius", "FrontMaterial"};
    vector<string> BackToken
        = {"BackDeformation", "BackThickness", "BackRadius", "BackMaterial"};
    vector<string> FrameToken  = {"FrameRadius", "FrameThickness", "FrontCone",
                                  "BackCone", "FrameMaterial"};
    vector<string> ShieldToken = {"ShieldInnerRadius",
                                  "ShieldOuterRadius"
                                  "ShieldBottomLength",
                                  "ShieldTopLength",
                                  "ShieldFrontRadius",
                                  "ShieldBackRadius",
                                  "ShieldMaterial"};

    if (ctarget->HasTokenList(CoreToken)) {
      // Target
      m_TargetThickness = ctarget->GetDouble("NominalThickness", "micrometer");
      m_TargetAngle     = ctarget->GetDouble("Angle", "deg");
      m_TargetMaterial  = nptool::geant4::MaterialManager::GetMaterialManager()
                             ->GetMaterialFromLibrary(
                                 ctarget->GetString("Material"),
                                 ctarget->GetDouble("Density", "g/cm3"));
      m_TargetRadius  = ctarget->GetDouble("Radius", "mm");
      m_TargetX       = ctarget->GetDouble("X", "mm");
      m_TargetY       = ctarget->GetDouble("Y", "mm");
      m_TargetZ       = ctarget->GetDouble("Z", "mm");
      m_TargetDensity = ctarget->GetDouble("Density", "g/cm3");
      m_TargetRadius  = ctarget->GetDouble("Radius", "mm");

      // Front Window
      m_FrontDeformation = ctarget->GetDouble("FrontDeformation", "mm");
      m_FrontThickness   = ctarget->GetDouble("FrontThickness", "micrometer");
      m_FrontRadius      = ctarget->GetDouble("FrontRadius", "mm");
      m_FrontMaterial
          = nptool::geant4::MaterialManager::GetMaterialManager()
                ->GetMaterialFromLibrary(ctarget->GetString("FrontMaterial"));

      // Back Window
      m_BackDeformation = ctarget->GetDouble("BackDeformation", "mm");
      m_BackRadius      = ctarget->GetDouble("BackRadius", "mm");
      m_BackThickness   = ctarget->GetDouble("BackThickness", "micrometer");
      m_BackMaterial
          = nptool::geant4::MaterialManager::GetMaterialManager()
                ->GetMaterialFromLibrary(ctarget->GetString("BackMaterial"));

      // Cell Frame
      m_FrameRadius    = ctarget->GetDouble("FrameRadius", "mm");
      m_FrameThickness = ctarget->GetDouble("FrameThickness", "mm");
      m_FrontCone      = ctarget->GetDouble("FrontCone", "deg");
      m_BackCone       = ctarget->GetDouble("BackCone", "deg");
      m_FrameMaterial
          = nptool::geant4::MaterialManager::GetMaterialManager()
                ->GetMaterialFromLibrary(ctarget->GetString("FrameMaterial"));
      // Heat Shield
      m_ShieldInnerRadius  = ctarget->GetDouble("ShieldInnerRadius", "mm");
      m_ShieldOuterRadius  = ctarget->GetDouble("ShieldOuterRadius", "mm");
      m_ShieldBottomLength = ctarget->GetDouble("ShieldBottomLength", "mm");
      m_ShieldTopLength    = ctarget->GetDouble("ShieldTopLength", "mm");
      m_ShieldFrontRadius  = ctarget->GetDouble("ShieldFrontRadius", "mm");
      m_ShieldBackRadius   = ctarget->GetDouble("ShieldBackRadius", "mm");
      m_ShieldMaterial
          = nptool::geant4::MaterialManager::GetMaterialManager()
                ->GetMaterialFromLibrary(ctarget->GetString("ShieldMaterial"));
    } else {
      cout << "ERROR: Target token list incomplete, check your input file"
           << endl;
      exit(1);
    }

    if (ctarget->HasToken("NbSlices"))
      m_TargetNbSlices = ctarget->GetInt("NbSlices");
  }
  if (starget_blocks.size() > 1 || ctarget_blocks.size() > 1) {
    cout << "ERROR: One and only one target shall be declared in your "
            "detector file "
         << endl;
    exit(1);
  }
  // } else {
  //   cout << "ERROR: One and only one target shall be declared in your
  //   detector
  //       file " << endl;
  //       exit(1);
  // }
}

////////////////////////////////////////////////////////////////////////////////
void G4Target::InitSimulation(std::string simtype) {
  // store the loaded simulation
  m_simtype = simtype;

  // load the plugin
  auto handle = nptool::Application::GetApplication()->LoadPlugin(
      "target-" + m_simtype, true);
  // build the class
  #ifdef Geant4_FOUND
  auto func = (std::shared_ptr<nptool::geant4::VDetector>(*)())dlsym(
      handle, "ConstructDetectorSimulation");
  if (func)
    m_Geant4 = (*func)();
  else
    throw(nptool::Error("Target", "Fail to load Geant4 module"));
  #endif
}

void G4Target::ConstructGeometry() {
#ifdef Geant4_FOUND
  if (m_Geant4) {
    // should load the library here and find the external constructor
    m_Geant4->ConstructDetector();
  }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void G4Target::ConstructDetector(G4LogicalVolume* world) {
  if (m_TargetType) { // case of standard target

    if (m_TargetThickness > 0) {
      m_TargetSolid = new G4Tubs("solidTarget", 0, m_TargetRadius,
                                 0.5 * m_TargetThickness, 0 * deg, 360 * deg);
      m_TargetLogic
          = new G4LogicalVolume(m_TargetSolid, m_TargetMaterial, "logicTarget");

      if (m_TargetBackingThickness > 0) {
        m_TargetBackingSolid
            = new G4Tubs("solidTargetBacking", 0, m_TargetRadius,
                         0.5 * m_TargetBackingThickness, 0 * deg, 360 * deg);
        m_TargetBackingLogic
            = new G4LogicalVolume(m_TargetBackingSolid, m_TargetBackingMaterial,
                                  "logicTargetBacking");
      }

      // rotation of target
      G4RotationMatrix* rotation = new G4RotationMatrix();
      rotation->rotateY(m_TargetAngle);

      new G4PVPlacement(rotation,
                        G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ),
                        m_TargetLogic, "Target", world, false, 0);

      G4VisAttributes* TargetVisAtt
          = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4));
      m_TargetLogic->SetVisAttributes(TargetVisAtt);

      if (m_TargetBackingThickness > 0) {
        new G4PVPlacement(rotation,
                          G4ThreeVector(m_TargetX, m_TargetY,
                                        m_TargetZ + m_TargetThickness * 0.5
                                            + m_TargetBackingThickness * 0.5),
                          m_TargetBackingLogic, "TargetBacking", world, false,
                          0);
        G4VisAttributes* TargetBackingVisAtt
            = new G4VisAttributes(G4Colour(0.5, 0.5, 0));
        m_TargetBackingLogic->SetVisAttributes(TargetBackingVisAtt);
      }
    }
  }

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
}
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// void G4Target::SetReactionRegion(){
//   m_ReactionRegion =
//   G4RegionStore::GetInstance()->FindOrCreateRegion("NPSimulationProcess");
//   m_ReactionRegion->AddRootLogicalVolume(m_TargetLogic);
//   m_ReactionRegion->SetUserLimits(new
//   G4UserLimits(m_TargetThickness/m_TargetNbSlices));
//   G4FastSimulationManager* mng =
//   m_ReactionRegion->GetFastSimulationManager();

//  unsigned int size = m_ReactionModel.size();
//  for(unsigned int i = 0 ; i < size ; i++)
//  mng->RemoveFastSimulationModel(m_ReactionModel[i]);

//  m_ReactionModel.clear();
//  G4VFastSimulationModel* fsm;
//  fsm = new NPS::BeamReaction("BeamReaction",m_ReactionRegion);
//  ((NPS::BeamReaction*)
//  fsm)->SetStepSize(m_TargetThickness/m_TargetNbSlices);
//  m_ReactionModel.push_back(fsm);
//  fsm = new NPS::Decay("Decay",m_ReactionRegion);
//  m_ReactionModel.push_back(fsm);
//  fsm = new NPS::FissionDecay("FissionDecay",m_ReactionRegion);
//  m_ReactionModel.push_back(fsm);
//  fsm = new NPS::ThreeBody("ThreeBody",m_ReactionRegion);
//  ((NPS::ThreeBody*) fsm)->SetStepSize(m_TargetThickness/100.);
//  m_ReactionModel.push_back(fsm);

//}

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
std::shared_ptr<nptool::VDetector> ConstructDetector() {
  return make_shared<nptool::geant4::G4Target>();
};
}

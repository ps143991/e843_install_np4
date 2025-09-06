// C++ header
#include <fstream>
#include <limits>

#include "Randomize.hh"
using namespace CLHEP;

// npgeant4
// For Geant4 Simulation
#ifdef Geant4_FOUND
#include "NPTarget.h"
#endif

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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Specific Method of this class
Target::Target() {
  m_TargetType = true;
  m_TargetThickness = 0;
  m_TargetAngle = 0;
  m_TargetRadius = 0;
  m_TargetDensity = 0;
  m_TargetNbSlices = 100.; // Number of sslices/steps by default
  m_TargetBackingThickness = 0;

  m_TargetDensity = 0;
  m_FrontDeformation = 0;
  m_FrontThickness = 0;
  m_FrontRadius = 0;
  m_FrontMaterial = "";
  m_BackDeformation = 0;
  m_BackRadius = 0;
  m_BackThickness = 0;
  m_BackMaterial = "";
  m_FrameRadius = 0;
  m_FrameThickness = 0;
  m_FrontCone = 0;
  m_BackCone = 0;
  m_FrameMaterial = "";
  m_ShieldInnerRadius = 0;
  m_ShieldOuterRadius = 0;
  m_ShieldBottomLength = 0;
  m_ShieldTopLength = 0;
  m_ShieldFrontRadius = 0;
  m_ShieldBackRadius = 0;
  m_ShieldMaterial = "";
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Target::~Target() {
  // unsigned int size = m_ReactionModel.size();
  // for (unsigned int i = 0; i < size; i++) {
  //   delete m_ReactionModel[i];
  // }
  // m_ReactionModel.clear();
}

////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//// Inherite Method of Vdetector Class

//// Read stream at Configfile to pick-up parameters of detector (Position,...)
//// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Target::ReadConfiguration(nptool::InputParser parser) {
  auto starget_blocks = parser.GetAllBlocksWithToken("target");
  auto ctarget_blocks = parser.GetAllBlocksWithToken("cryogenic_target");

  for (auto starget : starget_blocks) {
    cout << "////       TARGET      ////" << endl;
    cout << "//// Solid Target found " << endl;
    vector<string> token = {"thickness", "radius", "material", "angle", "x", "y", "z"};
    if (starget->HasTokenList(token)) {
      m_TargetThickness = starget->GetDouble("thickness", "micrometer");
      m_TargetAngle = starget->GetDouble("angle", "deg");
      m_TargetRadius = starget->GetDouble("radius", "mm");
      m_TargetMaterial = starget->GetString("material");
      m_TargetX = starget->GetDouble("x", "mm");
      m_TargetY = starget->GetDouble("y", "mm");
      m_TargetZ = starget->GetDouble("z", "mm");
      m_TargetType = true;
    }
    else {
      cout << "ERROR: Target token list incomplete, check your input file" << endl;
      exit(1);
    }

    if (starget->HasToken("NbSlices"))
      m_TargetNbSlices = starget->GetInt("NbSlices");

    if (starget->HasToken("BackingMaterial") && starget->HasToken("BackingThickness")) {
      m_TargetBackingMaterial = starget->GetString("BackingMaterial");
      m_TargetBackingThickness = starget->GetDouble("BackingThickness", "micrometer");
    }
  }

  for (auto ctarget : ctarget_blocks) {
    m_TargetType = false;
    cout << "//// Cryogenic Target found " << endl;

    vector<string> CoreToken = {"NominalThickness", "Material", "Density", "Radius", "Angle", "X", "Y", "Z"};
    vector<string> FrontToken = {"FrontDeformation", "FrontThickness", "FrontRadius", "FrontMaterial"};
    vector<string> BackToken = {"BackDeformation", "BackThickness", "BackRadius", "BackMaterial"};
    vector<string> FrameToken = {"FrameRadius", "FrameThickness", "FrontCone", "BackCone", "FrameMaterial"};
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
      m_TargetAngle = ctarget->GetDouble("Angle", "deg");
      m_TargetMaterial = ctarget->GetString("Material");
      m_TargetDensity = ctarget->GetDouble("Density", "g/cm3");
      m_TargetRadius = ctarget->GetDouble("Radius", "mm");
      m_TargetX = ctarget->GetDouble("X", "mm");
      m_TargetY = ctarget->GetDouble("Y", "mm");
      m_TargetZ = ctarget->GetDouble("Z", "mm");
      m_TargetDensity = ctarget->GetDouble("Density", "g/cm3");
      m_TargetRadius = ctarget->GetDouble("Radius", "mm");

      // Front Window
      m_FrontDeformation = ctarget->GetDouble("FrontDeformation", "mm");
      m_FrontThickness = ctarget->GetDouble("FrontThickness", "micrometer");
      m_FrontRadius = ctarget->GetDouble("FrontRadius", "mm");
      m_FrontMaterial = ctarget->GetString("FrontMaterial");

      // Back Window
      m_BackDeformation = ctarget->GetDouble("BackDeformation", "mm");
      m_BackRadius = ctarget->GetDouble("BackRadius", "mm");
      m_BackThickness = ctarget->GetDouble("BackThickness", "micrometer");
      m_BackMaterial = ctarget->GetString("BackMaterial");

      // Cell Frame
      m_FrameRadius = ctarget->GetDouble("FrameRadius", "mm");
      m_FrameThickness = ctarget->GetDouble("FrameThickness", "mm");
      m_FrontCone = ctarget->GetDouble("FrontCone", "deg");
      m_BackCone = ctarget->GetDouble("BackCone", "deg");
      m_FrameMaterial = ctarget->GetString("FrameMaterial");
      // Heat Shield
      m_ShieldInnerRadius = ctarget->GetDouble("ShieldInnerRadius", "mm");
      m_ShieldOuterRadius = ctarget->GetDouble("ShieldOuterRadius", "mm");
      m_ShieldBottomLength = ctarget->GetDouble("ShieldBottomLength", "mm");
      m_ShieldTopLength = ctarget->GetDouble("ShieldTopLength", "mm");
      m_ShieldFrontRadius = ctarget->GetDouble("ShieldFrontRadius", "mm");
      m_ShieldBackRadius = ctarget->GetDouble("ShieldBackRadius", "mm");
      m_ShieldMaterial = ctarget->GetString("ShieldMaterial");
    }
    else {
      cout << "ERROR: Target token list incomplete, check your input file" << endl;
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
void Target::InitSimulation(std::string simtype) {
  // store the loaded simulation
  m_simtype = simtype;

  // load the plugin
  auto handle = nptool::Application::GetApplication()->LoadPlugin("target-" + m_simtype, true);
// build the class
#ifdef Geant4_FOUND
  auto func = (std::shared_ptr<nptool::geant4::VDetector>(*)())dlsym(handle, "ConstructDetectorSimulation");
  if (func)
    m_Geant4 = (*func)();
  else
    throw(nptool::Error("Target", "Fail to load Geant4 module"));
#endif
}

void Target::ConstructGeometry() {
#ifdef Geant4_FOUND
  if (m_Geant4) {
    // should load the library here and find the external constructor
    m_Geant4->ConstructDetector();
  }
#endif
}

extern "C" {
std::shared_ptr<nptool::VDetector> ConstructDetector() { return make_shared<nptool::geant4::Target>(); };
}

#ifndef NPG4Target_h
#define NPG4Target_h 1

// C++ headers
#include <cmath>
#include <string>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4VFastSimulationModel.hh"

// NPTool headers
#include "NPFunction.h"
#include "NPApplication.h"
#include "NPG4VDetector.h"
#include "NPInputParser.h"
#include "NPVDetector.h"

using namespace CLHEP;

namespace nptool {
namespace geant4 {
// namespace target {

class G4Target : public nptool::VDetector {
public:
  G4Target();
  ~G4Target();

public: // inherrited from nptool::VPlugin
  std::vector<std::string> GetDependencies() { return {"root"}; };

public: // inherrited from nptool::VDetector
        //  Read stream at ConfigFile to pick-up parameters of detector
        //  (Position,...) using Token
  void ReadConfiguration(nptool::InputParser);

  //   Initialize the standard parameter for analysis
  //   ie: all channel enable, maximum multiplicity for strip = number of
  //   telescope
  void InitializeStandardParameter();

  //   Read the user configuration file; if no file found, load standard one
  void ReadAnalysisConfig();

  //  Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager() {};

  void InitializeDataInputConversion(std::shared_ptr<nptool::VDataInput>){};

  //  Activated associated Branches and link it to the private member
  //  DetectorData address In this method mother Branches (Detector) AND
  //  daughter leaf (fDetector_parameter) have to be activated
  void InitializeDataInputRaw(std::shared_ptr<nptool::VDataInput>){};

  //  Activated associated Branches and link it to the private member
  //  DetectorPhysics address In this method mother Branches (Detector) AND
  //  daughter leaf (parameter) have to be activated
  void InitializeDataInputPhysics(std::shared_ptr<nptool::VDataInput>){};

  //  Create associated branches and associated private member DetectorPhysics
  //  address
  void InitializeDataOutputRaw(std::shared_ptr<nptool::VDataOutput>){};

  //  Create associated branches and associated private member DetectorPhysics
  //  address
  void InitializeDataOutputPhysics(std::shared_ptr<nptool::VDataOutput>){};

  //  This method is called at each event read from the Input Tree. Aim is to
  //  build treat Raw dat in order to extract physical parameter.
  void BuildPhysicalEvent() {};
  //   Remove bad channel, calibrate the data and apply threshold
  void PreTreat() {};
  void ClearPreTreatedData() {}

  //  Those two method all to clear the Event Physics or Data
  void ClearEventPhysics() {};
  void ClearEventData() {};
  //   Read stream at Configfile to pick-up parameters of detector
  //   (Position,...) Called in DetecorConstruction::ReadDetextorConfiguration
  //   Method

  // From version based on NG4VDetector
  // void ReadConfiguration(nptool::InputParser);

  // //   Construct detector and inialise sensitive part.
  // //   Called After DetecorConstruction::AddDetector Method

  // //   Add Detector branch to the EventTree.
  // //   Called After DetecorConstruction::AddDetector Method
  // void InitializeRootOutput();

  // //   Read sensitive part and fill the Root tree.
  // //   Called at in the EventAction::EndOfEventAction
  // void ReadSensitive(const G4Event* event);

  // simulation
public:
  void        InitSimulation(std::string simtype);
  void        ConstructGeometry();
  void ConstructDetector(G4LogicalVolume* world);
  std::string m_simtype;

#ifdef Geant4_FOUND
private:
  std::shared_ptr<nptool::geant4::VDetector> m_Geant4;
#endif

public:
  //   method for debug purpose (still to be implemented)
  //   This method should check if the results of the beam interaction within
  //   the target (interaction coordinates) are well located inside the target
  //   volume
  bool IsInsideTarget() {
    return false; };

  // Used to simulate beam emmitance effect
  void RandomGaussian2D(double MeanX, double MeanY, double SigmaX,
                        double SigmaY, double& X, double& Y,
                        double NumberOfSigma = 10000);

public:
  //   Return Material from the Target Material Library
  G4Material* GetMaterialFromLibrary(G4String MaterialName);

public:
  G4double      GetTargetThickness() {
    return m_TargetThickness; }
  G4Material*   GetTargetMaterial() {
    return m_TargetMaterial; }
  G4double      GetTargetRadius() {
    return m_TargetRadius; }
  G4double      GetTargetAngle() {
    return m_TargetAngle; }
  G4double      GetTargetX() {
    return m_TargetX; }
  G4double      GetTargetY() {
    return m_TargetY; }
  G4double      GetTargetZ() {
    return m_TargetZ; }
  G4ThreeVector GetTargetPosition() {
    return G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ);
  }
  G4double         GetTargetNbSlices() {
    return m_TargetNbSlices; }
  G4Tubs*          GetTargetSolid() {
    return m_TargetSolid; }
  G4LogicalVolume* GetTargetLogic() {
    return m_TargetLogic; }

private:
  // Target type : true = normal ; false = cryo
  bool m_TargetType;

  // Solid and Logic Volume
  G4Tubs*          m_TargetSolid;
  G4LogicalVolume* m_TargetLogic;
  G4Tubs*          m_TargetBackingSolid;
  G4LogicalVolume* m_TargetBackingLogic;

  // Standard parameter
  G4double    m_TargetThickness;
  G4double    m_TargetRadius;
  G4double    m_TargetAngle;
  G4Material* m_TargetMaterial;
  G4double    m_TargetNbSlices;
  G4Material* m_TargetBackingMaterial;
  G4double    m_TargetBackingThickness;

  // For Cryo Target
  // this fonction generate a deformed target window shape
  //  inline double FrontProfile(double x, double offset, double b, double R)
  //  {return (offset+b+1)- cosh(x/(R/acosh(b+1)));} inline double
  //  BackProfile(double x, double offset, double b, double R)  {return
  //  cosh(x/(R/acosh(b+1)))-(offset+b+1);}
  inline double HyperbolicProfile(double x, double offset, double b, double R) {
    return (offset + b + 1) - cosh(x / (R / acosh(b + 1)));
  }
  inline double FrontProfile(double x, double offset, double b, double R) {
    return HyperbolicProfile(x, offset, b, R);
  }
  inline double BackProfile(double x, double offset, double b, double R) {
    return -HyperbolicProfile(x, offset, b, R);
  }

  G4double    m_TargetDensity;
  double      m_FrontDeformation;
  double      m_FrontThickness;
  double      m_FrontRadius;
  G4Material* m_FrontMaterial;
  double      m_BackDeformation;
  double      m_BackRadius;
  double      m_BackThickness;
  G4Material* m_BackMaterial;
  double      m_FrameRadius;
  double      m_FrameThickness;
  double      m_FrontCone;
  double      m_BackCone;
  G4Material* m_FrameMaterial;
  double      m_ShieldInnerRadius;
  double      m_ShieldOuterRadius;
  double      m_ShieldBottomLength;
  double      m_ShieldTopLength;
  double      m_ShieldFrontRadius;
  double      m_ShieldBackRadius;
  G4Material* m_ShieldMaterial;
  G4Polycone* m_CryoTargetSolid;
  // Positioning
  G4double m_TargetX;
  G4double m_TargetY;
  G4double m_TargetZ;

private:
  // Region were reaction can occure:
  G4Region*                       m_ReactionRegion;
  std::vector<G4VFastSimulationModel*> m_ReactionModel;

public: // Region were reaction can occure
  void                    SetReactionRegion();
  inline G4Region*        GetReactionRegion() {
    return m_ReactionRegion; };
  inline G4LogicalVolume* GetLogicalVolume() {
    return m_TargetLogic; };

private:
  static G4Target* TargetInstance;

public:
  static G4Target* GetTarget() {
    return TargetInstance; }
};
} // namespace geant4
} // namespace nptool
// } // namespace target

#endif

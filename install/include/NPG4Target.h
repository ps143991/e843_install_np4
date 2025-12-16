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
#include "NPApplication.h"
#include "NPFunction.h"
#include "NPG4VDetector.h"
#include "NPInputParser.h"

#include "NPTarget.h"

using namespace CLHEP;

namespace nptool {
  namespace geant4 {
    // namespace target {

    class G4Target : public nptool::geant4::VDetector {
     public:
      G4Target();
      ~G4Target();

      // simulation
     public:
      // void ConstructDetector(G4LogicalVolume* world);
      void ConstructDetector();

     private:
      // std::shared_ptr<nptool::geant4::VDetector> m_Geant4;
      std::shared_ptr<nptool::geant4::Target> m_target;

     public:
      //   method for debug purpose (still to be implemented)
      //   This method should check if the results of the beam interaction within
      //   the target (interaction coordinates) are well located inside the target
      //   volume
      bool IsInsideTarget() { return false; };

      // Used to simulate beam emmitance effect
      void RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double& X, double& Y,
                            double NumberOfSigma = 10000);

      void InitializeTarget();

     public:
      //   Return Material from the Target Material Library
      G4Material* GetMaterialFromLibrary(G4String MaterialName, double density = 0);

     public:
      G4double GetTargetThickness() { return m_TargetThickness; }
      G4Material* GetTargetMaterial() { return m_TargetMaterial; }
      G4double GetTargetRadius() { return m_TargetRadius; }
      G4double GetTargetAngle() { return m_TargetAngle; }
      G4double GetTargetX() { return m_TargetX; }
      G4double GetTargetY() { return m_TargetY; }
      G4double GetTargetZ() { return m_TargetZ; }
      G4ThreeVector GetTargetPosition() { return G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ); }
      G4double GetTargetNbSlices() { return m_TargetNbSlices; }
      G4Tubs* GetTargetSolid() { return m_TargetSolid; }
      G4LogicalVolume* GetTargetLogic() { return m_TargetLogic; }

     public:
      // Target type : true = normal ; false = cryo
      // bool m_TargetType;

      // Solid and Logic Volume
      G4Tubs* m_TargetSolid;
      G4LogicalVolume* m_TargetLogic;
      G4Tubs* m_TargetBackingSolid;
      G4LogicalVolume* m_TargetBackingLogic;

      // Standard parameter
      G4double m_TargetThickness;
      G4double m_TargetRadius;
      G4double m_TargetAngle;
      G4Material* m_TargetMaterial;
      G4double m_TargetNbSlices;
      G4Material* m_TargetBackingMaterial;
      G4double m_TargetBackingThickness;

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

      G4double m_TargetDensity;
      double m_FrontDeformation;
      double m_FrontThickness;
      double m_FrontRadius;
      G4Material* m_FrontMaterial;
      double m_BackDeformation;
      double m_BackRadius;
      double m_BackThickness;
      G4Material* m_BackMaterial;
      double m_FrameRadius;
      double m_FrameThickness;
      double m_FrontCone;
      double m_BackCone;
      G4Material* m_FrameMaterial;
      double m_ShieldInnerRadius;
      double m_ShieldOuterRadius;
      double m_ShieldBottomLength;
      double m_ShieldTopLength;
      double m_ShieldFrontRadius;
      double m_ShieldBackRadius;
      G4Material* m_ShieldMaterial;
      G4Polycone* m_CryoTargetSolid;
      // Positioning
      G4double m_TargetX;
      G4double m_TargetY;
      G4double m_TargetZ;

     private:
      // Region were reaction can occure:
      G4Region* m_ReactionRegion;
      std::vector<G4VFastSimulationModel*> m_ReactionModel;

     public: // Region were reaction can occure
      void SetReactionRegion();
      inline G4Region* GetReactionRegion() { return m_ReactionRegion; };
      inline G4LogicalVolume* GetLogicalVolume() { return m_TargetLogic; };

     private:
      static G4Target* TargetInstance;

     public:
      static G4Target* GetTarget() { return TargetInstance; }
    };
  } // namespace geant4
} // namespace nptool
// } // namespace target

#endif

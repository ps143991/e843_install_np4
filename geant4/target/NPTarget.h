#ifndef NPTarget_h
#define NPTarget_h 1

// C++ headers
#include <cmath>
#include <string>

// G4 header defining G4 types
#ifdef Geant4_FOUND
#include "NPG4VDetector.h"
#include "globals.hh"
#endif

// NPTool headers
#include "NPApplication.h"
#include "NPFunction.h"
#include "NPInputParser.h"
#include "NPVDetector.h"

using namespace CLHEP;

namespace nptool {
  namespace geant4 {
    // namespace target {

    class Target : public nptool::VDetector {
     public:
      Target();
      ~Target();

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
      void AddParameterToCalibrationManager(){};

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
      void BuildPhysicalEvent(){};
      //   Remove bad channel, calibrate the data and apply threshold
      void PreTreat(){};
      void ClearPreTreatedData() {}

      //  Those two method all to clear the Event Physics or Data
      void ClearEventPhysics(){};
      void ClearEventData(){};
      //   Read stream at Configfile to pick-up parameters of detector
      //   (Position,...) Called in DetecorConstruction::ReadDetextorConfiguration
      //   Method

      // simulation
     public:
      void InitSimulation(std::string simtype);
      void ConstructGeometry();
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
      bool IsInsideTarget() { return false; };

      // Used to simulate beam emmitance effect
      void RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double& X, double& Y,
                            double NumberOfSigma = 10000);

     public:
      double GetTargetThickness() { return m_TargetThickness; }
      std::string GetTargetMaterial() { return m_TargetMaterial; }
      double GetTargetRadius() { return m_TargetRadius; }
      double GetTargetAngle() { return m_TargetAngle; }
      double GetTargetX() { return m_TargetX; }
      double GetTargetY() { return m_TargetY; }
      double GetTargetZ() { return m_TargetZ; }
      double GetTargetNbSlices() { return m_TargetNbSlices; }

     public:
      // Target type : true = normal ; false = cryo
      bool m_TargetType;

      // Standard parameter
      double m_TargetThickness;
      double m_TargetRadius;
      double m_TargetAngle;
      std::string m_TargetMaterial;
      double m_TargetNbSlices;
      std::string m_TargetBackingMaterial;
      double m_TargetBackingThickness;

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

      double m_TargetDensity;
      double m_FrontDeformation;
      double m_FrontThickness;
      double m_FrontRadius;
      std::string m_FrontMaterial;
      double m_BackDeformation;
      double m_BackRadius;
      double m_BackThickness;
      std::string m_BackMaterial;
      double m_FrameRadius;
      double m_FrameThickness;
      double m_FrontCone;
      double m_BackCone;
      std::string m_FrameMaterial;
      double m_ShieldInnerRadius;
      double m_ShieldOuterRadius;
      double m_ShieldBottomLength;
      double m_ShieldTopLength;
      double m_ShieldFrontRadius;
      double m_ShieldBackRadius;
      std::string m_ShieldMaterial;

      // Positioning
      double m_TargetX;
      double m_TargetY;
      double m_TargetZ;
    };
  } // namespace geant4
} // namespace nptool
// } // namespace target

#endif

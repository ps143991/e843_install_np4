#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1
class G4LogicalVolume;
class G4VPhysicalVolume;

// C++ headers
#include <cmath>
#include <vector>

// G4 headers
// #include "DetectorMessenger.hh"
#include "G4Event.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"

namespace nptool {
  namespace geant4 {
    //   typedef void (NPS::VDetector::*VDetector_FuncPtr)(const G4Event*);

    class DetectorConstruction : public G4VUserDetectorConstruction {
     public:
      DetectorConstruction();
      virtual ~DetectorConstruction();

      G4VPhysicalVolume* Construct();
      G4LogicalVolume* GetWorldLogic();
      // Delete every existing bit
      void ClearGeometry();
      // Reload the file configuration
      void RedefineGeometry(std::string file = "");
      // Export the geometry to a gdml file
      void ExportGeometry(std::string);

     public:
      G4VPhysicalVolume* ReadConfigurationFile();
      void ReadAllSensitive(const G4Event*);
      /* void ClearInteractionCoordinates() {
         if (m_Detectors.size() > 0)
           m_Detectors[0]->GetInterCoordPointer()->Clear();
       };*/

     private:
      // Solid world
      G4Box* m_world_box;

      // Logical volumes
      G4LogicalVolume* m_world_log;

      // Physical volumes
      G4VPhysicalVolume* m_world_phys;

     public:
      G4LogicalVolume* GetWorldLogicalVolume() { return m_world_log; };

     private: // VDetector function pointer
              //      VDetector_FuncPtr m_ReadSensitivePtr;
      /// private: // Data about the target
      // Target* m_Target;
      // Chamber* m_Chamber;
      // DetectorMessenger* m_Messenger;

      //   public: // Getter and Setter
      //   Target* GetTarget() { return m_Target; }
      //   Chamber* GetChamber() { return m_Chamber; }
    };
  } // namespace geant4
} // namespace nptool

#endif

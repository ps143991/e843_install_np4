#include "NPG4DetectorConstruction.h"

// G4
#include "G4GeometryManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RegionStore.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4ThreeVector.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"

#ifdef NPS_GDML
#include "G4GDMLParser.hh"
#endif

// STL
#include <cstdlib>
#include <fstream>
#include <set>
#include <string>

// NPG4 plugin
#include "NPG4MaterialManager.h"

// nptool
#include "NPApplication.h"
using namespace CLHEP;
using namespace nptool;
using namespace geant4;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction() : m_world_log(nullptr), m_world_phys(nullptr), m_world_box(nullptr) {
  // m_Target = NULL;
  // m_Chamber = NULL;
  //  m_Messenger = new DetectorMessenger(this);
  // m_ReadSensitivePtr = &NPS::VDetector::ReadSensitive;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Construct the World volume //
  // world volume material
  auto Vacuum = nptool::geant4::MaterialManager::GetMaterialManager()->GetMaterialFromLibrary("Vacuum");

  // world volume dimension
  G4double world_x = 100.0 * m;
  G4double world_y = 100.0 * m;
  G4double world_z = 100.0 * m;
  // world volume solid
  if (!m_world_box)
    m_world_box = new G4Box("m_world_box", world_x, world_y, world_z);

  // world volume logical
  if (!m_world_log) {
    m_world_log = new G4LogicalVolume(m_world_box, Vacuum, "m_world_log", 0, 0, 0);
  }
  // world volume physical
  if (!m_world_phys)
    m_world_phys = new G4PVPlacement(0, G4ThreeVector(), m_world_log, "world", 0, false, 0);

  // world volume visual attribute
  auto VisAtt = new G4VisAttributes(G4VisAttributes::GetInvisible());
  m_world_log->SetVisAttributes(VisAtt);
  // Require all detector to build their own geometry
  nptool::Application::GetApplication()->InitSimulation("geant4");

  // Initialiaze scorer for each detector
  nptool::Application::GetApplication()->InitializeScorers();

  // Build geometry for each detector
  nptool::Application::GetApplication()->ConstructGeometry();

  return m_world_phys;
}


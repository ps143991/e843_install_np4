#ifndef NPG4MaterialManager_h
#define NPG4MaterialManager_h

// Geant4
#include "G4Element.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4ParticleDefinition.hh"

// std
#include <map>
#include <set>
#include <string>
namespace nptool {
  namespace geant4 {
    class MaterialManager {
     public:
      MaterialManager();
      ~MaterialManager(){};

      // Emulated a singleton like behaviour when using the GetInstance method
     private: // pointer to the single instance
      static std::shared_ptr<MaterialManager> m_instance;

     public: // interface to instance
      static std::shared_ptr<MaterialManager> GetMaterialManager();

      // Map of element and material:
      std::map<std::string, G4Material*> m_Material;

     private:
      G4Element* m_D;
      G4Element* m_T;
      G4Element* m_He3;
      G4Element* m_Li6;
      G4Element* m_Li7;

     public:
      // clear all exising material from the library
      void ClearMaterialLibrary();

      // Look for a known material named Name.
      // If the Material is known but not instantiate it is created and return
      // If the Material is instantiate it return it
      G4Material* GetMaterialFromLibrary(std::string Name, double density = 0);

      // Look for a known gas
      // Compute the correct density using Gas Law and density at STP
      G4Material* GetGasFromLibrary(std::string Name, double Pressure, double Temperature);

      // Same as above but for Element.
      G4Element* GetElementFromLibrary(std::string Name);

      // Let the user directly add a custom material to the library
      // It is howver overwritting existing material having the same name
      void AddMaterialToLibrary(G4Material*);

      // Create tiny block of active material so the DEDX tables are generated
      // prevent crash if the user define material but don't use it
      void CreateSampleVolumes(G4LogicalVolume* world_log);

      // Write the DEDx table for all material instantiate in the MaterialManager
      // for a given particle
      void WriteDEDXTable(G4ParticleDefinition* Particle, G4double Emin, G4double Emax);

      // Write the DEDx table for all material instantiate in the MaterialManager
      // for a list of particle name
      void WriteDEDXTable(std::set<std::string> Particle, G4double Emin, G4double Emax);

      // Write the Cross Section table for all material instantiate in the MaterialManager
      // for a given particle
      void WriteCrossSectionTable(G4ParticleDefinition* Particle, G4double Emin, G4double Emax);

      // Write the Cross Section table for all material instantiate in the MaterialManager
      // for a list of particle name
      void WriteCrossSectionTable(std::set<std::string> Particle, G4double Emin, G4double Emax);
    };
  } // namespace geant4
} // namespace nptool
#endif


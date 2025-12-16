#ifndef NPG4Session_h
#define NPG4Session_h

// Geant4
#include "G4PhysListFactory.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4UItcsh.hh"
#include "G4UIterminal.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

// nptool geant4 plugin
#include "NPG4DetectorConstruction.h"
#include "NPG4RunAction.h"
#include "NPG4EventAction.h"
#include "NPG4PrimaryGeneratorAction.h"
#include "NPG4VPhysicsList.h"
#include "NPVPlugin.h"

// nptool
#include "NPVDataOutput.h"

namespace nptool
{
  namespace geant4
  {
    class Session : nptool::VPlugin
    {
    public:
      Session();
      ~Session();

    public:
      void Start(std::shared_ptr<nptool::VDataOutput>);
      void Stop() {};

      // Emulated a singleton like behaviour when using the GetInstance method
    private: // pointer to the single instance
      static std::shared_ptr<Session> m_instance;
      bool already_started;

    public: // interface to instance
      static std::shared_ptr<Session> GetSession();

      // return true if this plugin is a service
      bool IsService() { return true; };
      // return true if this plugin is a detector
      bool IsDetector() { return false; };
      // Return the list of other plugin necessary for correct operation
      std::vector<std::string> GetDependencies() { return std::vector<std::string>(); };

    private:
      G4UIExecutive *m_UI;
      G4VisManager *m_VisManager;
      G4RunManager *m_RunManager;
      std::shared_ptr<nptool::geant4::VPhysicsList> m_PhysicsList;
      nptool::geant4::PrimaryGeneratorAction *m_PrimaryGenerator;
      nptool::geant4::DetectorConstruction *m_DetectorConstruction;
      nptool::geant4::RunAction *m_RunAction;
      nptool::geant4::EventAction *m_EventAction;
      std::shared_ptr<nptool::VDataOutput> m_output;

    public:
      G4LogicalVolume *GetWorldLogicalVolume() { return m_DetectorConstruction->GetWorldLogicalVolume(); };
      const G4Event *GetEvent() { return m_EventAction->GetEvent(); }
    };
  } // namespace geant4
} // namespace nptool
#endif

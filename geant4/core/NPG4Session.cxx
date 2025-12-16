// geant4
#include "G4RunManagerFactory.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

// nptool geant4 plugin
#include "NPG4PrimaryGeneratorAction.h"
#include "NPG4Session.h"

// nptool
#include "NPApplication.h"
#include "NPFunction.h"

using namespace nptool::geant4;
// Static pointer to the current application
std::shared_ptr<Session> Session::m_instance = nullptr;

////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Session> Session::GetSession() {
  if (!m_instance) {
    m_instance = std::make_shared<Session>();
  }
  return m_instance;
}
////////////////////////////////////////////////////////////////////////////////
Session::Session() {
  already_started = false;
  // get the application
  auto app = nptool::Application::GetApplication();

  // Initialize the state of geant4 random generator
  if (app->HasFlag("--seed")) {
    std::string msg = "Seeds for random generators set to: " + app->GetArg("--seed");
    message("green", "geant4", "nptool::geant4::Session", msg);
    CLHEP::HepRandom::setTheSeed(app->GetIntArg("--seed"), 3);
  }
  m_UI = nullptr;
  m_VisManager = nullptr;
  m_RunManager = nullptr;
  m_PhysicsList = nullptr;
  m_PrimaryGenerator = nullptr;
  m_DetectorConstruction = nullptr;

  // Run Manager
  m_RunManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);
}
////////////////////////////////////////////////////////////////////////////////
void Session::Start(std::shared_ptr<nptool::VDataOutput> output) {
  m_output = output;
  // avoid multiple start
  if (already_started)
    return;
  else
    already_started = true;

  // get the application
  auto app = nptool::Application::GetApplication();
  /////////////////////////////////////////////////////////////////////////////
  // UI
  /////////////////////////////////////////////////////////////////////////////
  char** argv;
  int argc = app->GetArgcArgv(argv);
  if (!app->HasFlag("--batch"))
    m_UI = new G4UIExecutive(argc, argv);

  ////////////////////////////////////////////////////////////////////////////
  // Detector Construction interface to app
  ////////////////////////////////////////////////////////////////////////////
  m_DetectorConstruction = new DetectorConstruction;
  m_RunManager->SetUserInitialization(m_DetectorConstruction);

  ////////////////////////////////////////////////////////////////////////////
  // Physics List
  ////////////////////////////////////////////////////////////////////////////
  std::string physics_list = "base-physics-list";
  if (app->HasFlag("--physics-list")) {
    physics_list = app->GetArg("--physics-list");
  }

  m_PhysicsList = std::dynamic_pointer_cast<nptool::geant4::VPhysicsList>(app->ConstructPhysicsProcess(physics_list));
  m_RunManager->SetUserInitialization(m_PhysicsList.get());

  ////////////////////////////////////////////////////////////////////////////
  // Run Action
  ////////////////////////////////////////////////////////////////////////////
  m_RunAction = new nptool::geant4::RunAction();
  m_RunManager->SetUserAction(m_RunAction);
  
  ////////////////////////////////////////////////////////////////////////////
  // Event Action
  ////////////////////////////////////////////////////////////////////////////
  m_EventAction = new nptool::geant4::EventAction(m_output);
  m_EventAction->SetDetector(m_DetectorConstruction);
  m_RunManager->SetUserAction(m_EventAction);

  ////////////////////////////////////////////////////////////////////////////
  // Primary Generator
  ////////////////////////////////////////////////////////////////////////////

  m_PrimaryGenerator = new nptool::geant4::PrimaryGeneratorAction();
  if (app->HasFlag("--event-generator")) {
    auto event_files = app->GetVectorArg("--event-generator");
    for (auto file : event_files)
      m_PrimaryGenerator->ReadConfiguration(file);
  }
  if (app->HasFlag("-E")) {
    auto event_files = app->GetVectorArg("-E");
    for (auto file : event_files)
      m_PrimaryGenerator->ReadConfiguration(file);
  }
  m_RunManager->SetUserAction(m_PrimaryGenerator);

  // Initialisation
  m_RunManager->Initialize();

  m_PhysicsList->AddFastProcessManager();

  // UI
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (!app->HasFlag("--batch")) {
    UImanager->ApplyCommand("/control/execute macro/geant4/verbose.mac");
    UImanager->ApplyCommand("/control/execute macro/geant4/aliases.mac");
    m_VisManager = new G4VisExecutive("Quiet");
    m_VisManager->Initialize();
    UImanager->ApplyCommand("/control/execute macro/geant4/vis.mac");
    if (m_UI->IsGUI()) {
      UImanager->ApplyCommand("/control/execute macro/geant4/gui.mac");
    }
  }

  else { // if batch mode do not accumulate any track
    UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate 0");
  }

  // Execute user macro
  if (app->HasFlag("--macro")) {
    auto macro = app->GetArg("--macro");
    UImanager->ApplyCommand("/control/execute " + macro);
  }

  // Start the session
  if (m_UI) {
    m_UI->SessionStart();
    delete m_UI;
  }
  /*
      if (m_VisManager)
        delete m_VisManager;
      if (m_RunManager)
        delete m_RunManager;
        */
}
////////////////////////////////////////////////////////////////////////////////
Session::~Session() {}

// g4 headers
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"

// nptool geant4
#include "NPG4EventAction.h"
#include "NPG4RunAction.h"

// nptool
#include "NPApplication.h"

// stl
#include <iostream>

using namespace nptool::geant4;

EventAction *EventAction::m_EventAction = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventAction::EventAction(std::shared_ptr<nptool::VDataOutput> output)
{
  m_EventAction = this;
  m_output = output;
  begin = clock();
  treated = 0;
  inter = 0;
  total = 0;
  mean_rate = 0;
  displayed = 0;
  m_event = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventAction::~EventAction() {
  if (progress)
    delete progress;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::BeginOfEventAction(const G4Event *event)
{
  m_event = event;
  treated = event->GetEventID() + 1;
  if (treated == 1) {
    total = RunAction::GetInstance()->GetNumberOfEvent();
    progress = new nptool::ProgressDisplay(total, -1);
  }
  else if (progress)
    progress->AttemptDisplay(treated);
  //     SaveRandomGeneratorInitialState();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event *event)
{
  m_event = event;
  nptool::Application::GetApplication()->ReadSensitive();
  m_output->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetDetector(DetectorConstruction *detector) { m_detector = detector; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SaveRandomGeneratorInitialState()
{
  // This allow to restore the geant4 random generator status for problematic
  // event
  /*  CLHEP::HepRandom::saveFullState(m_Geant4RandomFullState);
    m_G4State = m_Geant4RandomFullState.str();
    m_Geant4RandomFullState.str("");
    m_Geant4RandomFullState.clear();
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetRunLength(int length) { total = length; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventAction *EventAction::GetInstance() { return m_EventAction; }
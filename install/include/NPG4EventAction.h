#ifndef NPG4EventAction_h
#define NPG4EventAction_h 1

// geant4
#include "G4UserEventAction.hh"
#include "globals.hh"

// nptool geant4 plugin
#include "NPG4DetectorConstruction.h"

// nptool
#include "NPVDataOutput.h"
#include "NPProgressDisplay.h"

// stl
#include <iostream>
#include <string>
#include <time.h>
namespace nptool
{
  namespace geant4
  {
    class EventAction : public G4UserEventAction
    {
    public:
      EventAction(std::shared_ptr<nptool::VDataOutput>);
      ~EventAction();

    public:
      void BeginOfEventAction(const G4Event *);
      void EndOfEventAction(const G4Event *);
      void SetDetector(DetectorConstruction *detector);
      void SaveRandomGeneratorInitialState();
      void SetRunLength(int);
      const G4Event *GetEvent() { return m_event; }

    private: // Progress Display
      ProgressDisplay *progress;
      clock_t begin;
      clock_t end;
      unsigned int treated;
      unsigned int inter;
      unsigned int total;
      double mean_rate;
      int displayed;

    private: // data output
      std::shared_ptr<nptool::VDataOutput> m_output;
      const G4Event *m_event;

    private: // Random state
      std::ostringstream m_Geant4RandomFullState;
      std::string m_G4State;

    private:
      DetectorConstruction *m_detector;
      static EventAction *m_EventAction;

    public:
      static EventAction *GetInstance();
    };
  } // namespace geant4
} // namespace nptool
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

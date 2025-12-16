#ifndef NPG4RunAction_h
#define NPG4RunAction_h 1

// G4 headers
#include "G4UserRunAction.hh"
#include "globals.hh"
#include <set>
#include <string>

class G4Run;

namespace nptool
{
  namespace geant4
  {
    class RunAction : public G4UserRunAction
    {
    public:
      RunAction();
      virtual ~RunAction();

      virtual void BeginOfRunAction(const G4Run *);
      virtual void EndOfRunAction(const G4Run *);

    private:
      int m_RunNumber;
      std::set<std::string> m_Particle;
      int m_EventCount;

    private:
      static RunAction *m_RunAction;

    public:
      static RunAction *GetInstance();
      int GetNumberOfEvent() { return m_EventCount; };
    };
  } // namespace geant4
} // namespace nptool

#endif

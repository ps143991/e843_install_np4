#ifndef NPG4VDetector_h
#define NPG4VDetector_h
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "InteractionCoordinates.h"
#include "NPApplication.h"

namespace nptool {
namespace geant4 {
class VDetector {
public:
  //  Default Constructor and destructor
  VDetector();
  virtual ~VDetector() {};

  // Construct detector and inialise sensitive part.
  // Called After DetecorConstruction::AddDetector Method
  virtual void ConstructDetector() {};

  // Read sensitive part and fill the Root tree.
  // Called at in the EventAction::EndOfEventAvtion
  virtual void ReadSensitive() {};
  virtual void InitializeScorers() {};

  // Check if the scorer exist
  // if yes return the already existing one, otherwise return a new created one
  G4MultiFunctionalDetector* CheckScorer(std::string name, bool& exist) {
    exist                          = true;
    G4MultiFunctionalDetector* ptr = NULL;

    ptr = (G4MultiFunctionalDetector*)G4SDManager::GetSDMpointer()
              ->FindSensitiveDetector(name, false);

    if (!ptr) {
      ptr   = new G4MultiFunctionalDetector(name.c_str());
      exist = false;
    }
    return ptr;
  }

public:
  InteractionCoordinates* GetInterCoordPointer() { return ms_InterCoord; };

public:
  // Class to store the interaction data (i.e. X,Y,Z,theta,phi)
  static InteractionCoordinates* ms_InterCoord;
};
} // namespace geant4
} // namespace nptool
#endif

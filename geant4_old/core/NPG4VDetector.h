#ifndef NPG4VDetector_h
#define NPG4VDetector_h
namespace nptool {
  namespace geant4 {
    class VDetector {
     public:
      //  Default Constructor and destructor
      VDetector(){};
      virtual ~VDetector(){};

      // Construct detector and inialise sensitive part.
      // Called After DetecorConstruction::AddDetector Method
      virtual void ConstructDetector(){};

      // Read sensitive part and fill the Root tree.
      // Called at in the EventAction::EndOfEventAvtion
      //   virtual void ReadSensitive(const G4Event*) = 0;
    };
  } // namespace geant4
} // namespace nptool
#endif


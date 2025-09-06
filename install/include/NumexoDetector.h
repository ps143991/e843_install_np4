#ifndef NumexoDetector_h
#define NumexoDetector_h

#include <map>
#include <stdlib.h>
#include <vector>

#include "Math/Vector3D.h"
#include "NPCalibrationManager.h"
#include "NPApplication.h"
#include "NPVDetector.h"

// ROOT
#include "TGraphErrors.h"
#include "TH1.h"
#include "TObject.h"
#include "TVector2.h"
#include "TVector3.h"

// MFM
#ifdef MFM_FOUND
#include "MFMAllFrames.h"
#endif

namespace numexo {

class NumexoSpectra;
class NumexoDetector : public nptool::VDetector {
public: // Constructor and Destructor
  NumexoDetector();
  ~NumexoDetector();

public:
  std::shared_ptr<numexo::NumexoSpectra> m_Spectra;
  std::map<std::string, std::vector<unsigned int>*>       GenericRawBranch;
  std::map<std::string, std::vector<unsigned long long>*> GenericRawBranchTS;
  std::map<std::string, std::vector<unsigned int>*>       GenericPhysicsBranch;
  std::map<std::string, std::vector<unsigned long long>*>
      GenericPhysicsBranchTS;

  // parameters used in the analysis
private:
public: // inherrited from nptool::VPlugin
  std::vector<std::string> GetDependencies() { return {"root"}; };

public: // inherrited from nptool::VDetector
        //  Read stream at ConfigFile to pick-up parameters of detector
        //  (Position,...) using Token
  void ReadConfiguration(nptool::InputParser);

  //  Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager() {};

  //  Activated associated Branchhes and link it to the private member
  //  DetectorData address In this method mother Branchhes (Detector) AND
  //  daughter leaf (fDetector_parameter) have to be activated
  void InitializeDataInputRaw(std::shared_ptr<nptool::VDataInput>);

  //  Activated associated Branchhes and link it to the private member
  //  DetectorPhysics address In this method mother Branchhes (Detector) AND
  //  daughter leaf (parameter) have to be activated
  void InitializeDataInputPhysics(std::shared_ptr<nptool::VDataInput>);

  //  Create associated branches and associated private member DetectorPhysics
  //  address
  void InitializeDataOutputRaw(std::shared_ptr<nptool::VDataOutput>);

  //  Create associated branches and associated private member DetectorPhysics
  //  address
  void InitializeDataOutputPhysics(std::shared_ptr<nptool::VDataOutput>);

  void BuildRawEvent(const std::string& daq_name, const std::string& label,
                     void* commonframe);

  void TreatFrame(void* commonframe);

#ifdef MFM_FOUND
  MFMMergeFrame*  MergeFrame;
  MFMCommonFrame* InsideFrame;
#endif

  std::map<int, std::string> fLabelMap;

  void SetNumexoMap(unsigned int Board, unsigned int NUMEXOChannel,
                    std::string NUMEXOChannelName);

  std::string GetNameFromNumexoMap(unsigned int Board, unsigned int Channel);

  std::map<unsigned int, std::string> m_NumexoMap;

  //  This method is called at each event read from the Input Tree. Aim is to
  //  build treat Raw dat in order to extract physical parameter.
  void BuildPhysicalEvent();
  //   Remove bad channel, calibrate the data and apply threshold
  void PreTreat();
  void ClearPreTreatedData() {}
  //  Those two method all to clear the Event Physics or Data
  void ClearEventPhysics();
  void ClearEventData();

  /// A usefull method to bundle all operation to add a detector
  void AddDetector(TVector3 POS, std::string shape);
  void AddDetector(double R, double Theta, double Phi, std::string shape);

  // Method related to the TSpectra classes, aimed at providing a framework for
  // online applications Instantiate the Spectra class and the histogramm
  // throught it
  void InitSpectra();
  // Fill the spectra hold by the spectra class
  void FillSpectra();
  // Write the spectra to a file
  void WriteSpectra();
  // Used for Online mainly, perform check on the histo and for example change
  // their color if issues are found
  void CheckSpectra();
  // Used for Online only, clear all the spectra hold by the Spectra class
  void ClearSpectra();
  // Used for interoperability with other framework
  void SetRawDataPointer(void*) {};
  // needed for online analysis for example
  void SetRawDataPointer() {}

private:
private: //   Parameter used in the analysis
};

} // namespace numexo
#endif

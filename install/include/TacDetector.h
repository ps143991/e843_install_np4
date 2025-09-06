/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girard-alcindor@ijclab.in2p3.fr                          *
 *                                                                           *
 * Creation Date  :    08/03/24                                              *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#ifndef TacDetector_h
#define TacDetector_h

#include <map>
#include <stdlib.h>
#include <vector>

#include "Math/Vector3D.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "TacData.h"
#include "TacPhysics.h"

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


namespace tac {

class TacSpectra;
class TacDetector : public nptool::VDetector {
public: // Constructor and Destructor
  TacDetector();
  ~TacDetector();

public: // Data member
  tac::TacData*              m_RawData;
  tac::TacData*              m_CalData;
  tac::TacPhysics*           m_PhysicsData;
  nptool::CalibrationManager m_Cal;
  std::shared_ptr<tac::TacSpectra> m_Spectra;
  // getters for raw and pre-treated data object
public:
  TacData* GetRawData() const { return m_RawData; }
  TacData* GetPreTreatedData() const { return m_CalData; }

  // parameters used in the analysis
private:
  // thresholds
  double       m_TAC_Time_RAW_Threshold = 0; //!
  unsigned int m_TAC_Mult; //!
  std::map<std::string, std::pair<unsigned int, unsigned long long>>
      SortTAC; //!

public: // inherrited from nptool::VPlugin
  std::vector<std::string> GetDependencies() { return {"root"}; };

public: // inherrited from nptool::VDetector
        //  Read stream at ConfigFile to pick-up parameters of detector
        //  (Position,...) using Token
  void ReadConfiguration(nptool::InputParser);

  //  Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager(){};

  //  Activated associated Branches and link it to the private member
  //  DetectorData address In this method mother Branches (Detector) AND
  //  daughter leaf (fDetector_parameter) have to be activated
  void InitializeDataInputRaw(std::shared_ptr<nptool::VDataInput>);

  //  Activated associated Branches and link it to the private member
  //  DetectorPhysics address In this method mother Branches (Detector) AND
  //  daughter leaf (parameter) have to be activated
  void InitializeDataInputPhysics(std::shared_ptr<nptool::VDataInput>);

  //  Create associated branches and associated private member DetectorPhysics
  //  address
  void InitializeDataOutputRaw(std::shared_ptr<nptool::VDataOutput>);

  //  Create associated branches and associated private member DetectorPhysics
  //  address
  void InitializeDataOutputPhysics(std::shared_ptr<nptool::VDataOutput>);

  //  This method is called at each event of a conversion to fill in the data
  //  class
  void BuildRawEvent(const std::string& daq_name, const std::string& label,
                     void* commonframe);

  void SetNumexoMap(unsigned int Board,
                    unsigned int NUMEXOChannel,std::string NUMEXOChannelName);

  std::string GetNameFromNumexoMap(unsigned int Board, unsigned int Channel);
  void TreatFrame(void* commonframe);

#ifdef MFM_FOUND
  MFMMergeFrame*  MergeFrame;
  MFMCommonFrame* InsideFrame;
#endif

  std::map<unsigned int, std::string> m_NumexoMap;

  //  This method is called at each event read from the Input Tree. Aim is to
  //  build treat Raw dat in order to extract physical parameter.
  void BuildPhysicalEvent();
  //   Remove bad channel, calibrate the data and apply threshold
  void PreTreat();
  void ClearPreTreatedData() { m_CalData->Clear(); }
  //  Those two method all to clear the Event Physics or Data
  void ClearEventPhysics() { m_PhysicsData->Clear(); };
  void ClearEventData() { m_RawData->Clear(); };

  /// A usefull method to bundle all operation to add a detector
  void AddDetector(TVector3 POS, std::string shape);
  void AddDetector(double R, double Theta, double Phi, std::string shape);
  void Match_TAC();

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
  void SetRawDataPointer(void*){};
  // give and external TTACData object to TTACPhysics.
  // needed for online analysis for example
  void SetRawDataPointer(TacData* rawDataPointer) {
    m_RawData = rawDataPointer;
  }

private:
private: //   Parameter used in the analysis
};

} // namespace tac
#endif

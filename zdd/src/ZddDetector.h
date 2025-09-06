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
 * This class is heavily based on the nptool v3 ZDD detector                 *
 *                                                                           *
 *****************************************************************************/
#ifndef ZddDetector_h
#define ZddDetector_h

#include <map>
#include <stdlib.h>
#include <vector>

#include "Math/Vector3D.h"
#include "NPApplication.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "ZddData.h"
#include "ZddPhysics.h"

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

namespace zdd {

class ZddSpectra;
class ZddDetector : public nptool::VDetector {
public: // Constructor and Destructor
  ZddDetector();
  ~ZddDetector();

public: // Data member
  zdd::ZddData*              m_RawData;
  zdd::ZddData*              m_CalData;
  zdd::ZddPhysics*           m_PhysicsData;
  nptool::CalibrationManager m_Cal;
  std::shared_ptr<zdd::ZddSpectra> m_Spectra;


public: // inherrited from nptool::VPlugin
  std::vector<std::string> GetDependencies() { return {"root"}; };

public: // inherrited from nptool::VDetector
        //  Read stream at ConfigFile to pick-up parameters of detector
        //  (Position,...) using Token
  void ReadConfiguration(nptool::InputParser);

  //  Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager() {};

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

  void SetNumexoMap(std::string Type, unsigned int Board,
      unsigned int NUMEXOChannel);

  std::string GetTypeFromNumexoMap(unsigned int Board, unsigned int Channel);
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

  void Match_DC();
  void Match_Exo();
  // Matching IC
  void Match_IC();

  void Match_PL();
  // bool CheckGoodEvent();

  // PreTreating Energy for IC and Plastic
  void PreTreatEnergy(std::string Detector);

  // Same for time
  void PreTreatTime(std::string Detector);

  // read the user configuration file. If no file is found, load standard one
  void ReadAnalysisConfig();

  // give and external TZDDData object to TZDDPhysics.
  // needed for online analysis for example
  void SetRawDataPointer(ZddData* rawDataPointer) {
    m_RawData = rawDataPointer;
  }

  void Add_ZDD(double R, double Theta) {
    m_R     = R;
    m_Theta = Theta;
    m_NumberOfDetectors++;
  }
  bool fSortIC(int i, int j);

  // parameters used in the analysis
private:
  // thresholds
  double                               m_IC_E_RAW_Threshold; //!
  double                               m_PL_E_RAW_Threshold; //!
  double                               m_DC_E_RAW_Threshold; //!
  double                               m_EXO_E_RAW_Threshold; //!
  double                               m_E_Threshold; //!
  double                               m_R; //!
  double                               m_Theta; //!
  unsigned int                         m_IC_Mult; //!
  unsigned int                         m_PL_Mult; //!
  unsigned int                         m_DC_Mult; //!
  unsigned int                         m_EXO_Mult; //!
  std::map<unsigned int, unsigned int> Map_IC; //!
  std::vector<std::map<unsigned int, std::pair<unsigned int, unsigned long long>>>
    SortIC; //!

  // std::map<unsigned int, std::pair<unsigned int, unsigned long long>>
  //   SortIC; //!
  int m_NumberOfDetectors; //!
    std::map<unsigned int,std::vector<std::pair<unsigned int, unsigned long long>>> SortPL;//!
    //std::map<unsigned int,std::vector<std::pair<unsigned int, long long unsigned int>>> SortIC2;
    std::map<unsigned int,std::vector<std::pair<unsigned int, long long unsigned int>>> SortDC;//!
    std::map<unsigned int,std::vector<std::pair<unsigned int, long long unsigned int>>> SortEXO;//!
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

  // ApplyCalibration
  double fZDDEXO_E(const unsigned int& i);
  double fZDDEXO_Time(const unsigned int& i);

private: //   Parameter used in the analysis
};

} // namespace zdd
#endif

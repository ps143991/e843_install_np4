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
 * This class is heavily based on the nptool v3 EXOGAM detector              *
 *                                                                           *
 *****************************************************************************/
#ifndef ExogamDetector_h
#define ExogamDetector_h

#include <map>
#include <stdlib.h>
#include <vector>

#include "ExogamData.h"
#include "ExogamPhysics.h"
#include "Geometry_Clover_Exogam.h"
//#include "Geometry_Exogam_100mmOffset.h"
#include "ExogamSimulatedGeometry.h"
#include "Math/Vector3D.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"
#include "NPApplication.h"

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

namespace exogam {

class ExogamSpectra;
class ExogamDetector : public nptool::VDetector {
public: // Constructor and Destructor
  ExogamDetector();
  ~ExogamDetector();

public: // Data member
  exogam::ExogamData*        m_RawData;
  exogam::ExogamData*        m_CalData;
  exogam::ExogamPhysics*     m_PhysicsData;
  exogam::ExogamSimulatedGeometry*     m_SimGeo;
  nptool::CalibrationManager m_Cal;
  std::shared_ptr<exogam::ExogamSpectra> m_Spectra;

public: // inherrited from nptool::VPlugin
  std::vector<std::string> GetDependencies() { return {"root"}; };

public: // inherrited from nptool::VDetector
        //  Read stream at ConfigFile to pick-up parameters of detector
        //  (Position,...) using Token
  void ReadConfiguration(nptool::InputParser);

  void SetExogamNumexoMap(unsigned int Board, unsigned int Flange,
                          unsigned int NUMEXOChannel,
                          unsigned int NUMEXOChannelGroup);

  unsigned int GetCrystalFromNumexoMap(unsigned int Board,
                                       unsigned int Channel);

public:
  std::map<unsigned int, unsigned int> m_ExogamNumexoCrystalMap;
  std::map<unsigned int, unsigned int> m_ExogamNumexoGroupMap;

  //  Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager() {};

  void InitializeDataInputConversion(std::shared_ptr<nptool::VDataInput>){};

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

  void TreatFrame(void* commonframe);
#ifdef MFM_FOUND
  MFMMergeFrame*  MergeFrame;
  MFMCommonFrame* InsideFrame;
#endif

  //  This method is called at each event read from the Input Tree. Aim is to
  //  build treat Raw dat in order to extract physical parameter.
  void BuildPhysicalEvent();
  //   Remove bad channel, calibrate the data and apply threshold
  void PreTreat();
  void ClearPreTreatedData() { m_CalData->Clear(); }
  void ResetPreTreatVariable(); //!

  //  Those two method all to clear the Event Physics or Data
  void ClearEventPhysics() { m_PhysicsData->Clear(); };
  void ClearEventData() { m_RawData->Clear(); };

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

  //	Add a Clover
  // void AddClover(string AngleFile);
  void AddClover(int Board, int Flange, int Channel0, int Channel1); //!

  // Give and external TMustData object to TExogamPhysics. Needed for online
  // analysis for example.
  void SetRawDataPointer(ExogamData* rawDataPointer) {
    m_RawData = rawDataPointer;
  } //!
  // Retrieve raw and pre-treated data
  ExogamData* GetRawData() const { return m_RawData; } //!
  ExogamData* GetPreTreatedData() const { return m_CalData; } //!
  void        ReadAnalysisConfig(); //!

  double ComputeMeanFreePath(double Energy); //!
  std::map<double, double> GetDefaultPhotonCS(); //!
  unsigned int GetFlangeNbr(unsigned int crystal_nbr); //!
  int    GetMaxOuter(unsigned int EventId); //!
  double GetDoppler(double Energy, unsigned int Flange, unsigned int Crystal,
                    unsigned int Outer); //!
  double GetDopplerSimple(double E, double Angle); //!
  double GetDoppler_Sim100mm(double energy, unsigned int flange, unsigned int crystal, unsigned int segment); //!
  double GetDoppler_SimBoost(double energy, unsigned int flange, unsigned int crystal, unsigned int segment); //!

  void SetBeta(double beta){m_beta = beta;};
  double GetBeta(){return m_beta;};

private: // Variables for analysis
  unsigned int             m_EXO_Mult; //!
  double                   m_EXO_OuterLw_RAW_Threshold; //!
  double                   m_EXO_OuterUp_RAW_Threshold; //!
  double                   m_EXO_E_RAW_Threshold; //!
  double                   m_EXO_E_Threshold; //!
  double                   m_EXO_EHG_RAW_Threshold; //!
  double                   m_EXO_TDC_RAW_Threshold; //!
  double                   m_EXO_BGO_RAW_Threshold; //!
  double                   m_EXO_CsI_RAW_Threshold; //!
  double                   EXO_E; //!
  double                   EXO_EHG; //!
  double                   EXO_TDC; //!
  double                   EXO_Outer1; //!
  double                   EXO_Outer2; //!
  double                   EXO_Outer3; //!
  double                   EXO_Outer4; //!
  unsigned int             flange_nbr; //!
  unsigned int             crystal_nbr; //!
  double                   GeDensity = 0.005323; //! g/mm3
  std::map<double, double> Map_PhotonCS; //!
  double                   m_beta = -1000.; //!

  Clover_struc Exogam_struc; //!

  std::map<unsigned int, std::pair<unsigned int, unsigned int>>
      MapCrystalFlangeClover; //! Map key is raw crystal nbr, pair associated is
                              //! flange nbr and crystal nbr in the flange

private: // Variables for checks and alarms
  unsigned int count_GammaHit;
  unsigned int count_CoreNoSeg;
  unsigned int count_SegNoCore;
  unsigned int count_AntiCompton;
  double limit_CoreNoSeg;
  double limit_SegNoCore;
  double limit_AntiCompton;
  unsigned long long lastTS;

private: // Crystal and segment activation/deactivation
  bool IsCloverActive[16] = {false};
  bool IsCrystalActive[16*4] = {false};
  bool IsFlangeCrystalActive[16][4] = {false};
  bool IsSegmentActive[16*4][4] = {false};

  void SetCloverActive(int flange);
  void SetCloverInactive(int flange);

  void SetCrystalActive(int rawcrystal);
  void SetCrystalInactive(int rawcrystal);

  void SetSegmentActive(int rawcrystal, int seg);
  void SetSegmentInactive(int rawcrystal, int seg);

private: // Parameters used in the analysis
  double fEXO_E(const unsigned int& i);
  double fEXO_EHG(const unsigned int& i);
  double fEXO_T(const unsigned int& i);
  double fEXO_Outer(const unsigned int& i, const unsigned int OuterNumber);

};

} // namespace exogam
#endif

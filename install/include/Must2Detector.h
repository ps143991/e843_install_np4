#ifndef Must2Detector_h
#define Must2Detector_h

#include <map>
#include <stdlib.h>
#include <vector>

#include "Math/Vector3D.h"
#include "Must2Data.h"
#include "Must2Physics.h"
#include "NPApplication.h"
#include "NPCalibrationManager.h"
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

#ifdef Geant4_FOUND
#include "NPG4VDetector.h"
#endif

namespace must2 {

class Must2Spectra;
class Must2Detector : public nptool::VDetector {
public: // Constructor and Destructor
  Must2Detector();
  ~Must2Detector();

public: // Data member
  must2::Must2Data*                    m_RawData;
  must2::Must2Data*                    m_CalData;
  must2::Must2Physics*                 m_PhysicsData;
  nptool::CalibrationManager           m_Cal;
  std::shared_ptr<must2::Must2Spectra> m_Spectra;

public: // inherrited from nptool::VPlugin
  std::vector<std::string> GetDependencies() { return {"root"}; };

public: // inherrited from nptool::VDetector
        //  Read stream at ConfigFile to pick-up parameters of detector
        //  (Position,...) using Token
  void ReadConfiguration(nptool::InputParser);

  //   Initialize the standard parameter for analysis
  //   ie: all channel enable, maximum multiplicity for strip = number of
  //   telescope
  void InitializeStandardParameter();

  //   Read the user configuration file; if no file found, load standard one
  void ReadAnalysisConfig();

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
  void ReadActionFile(std::string action_file);
  bool faction_file_initialized;
  std::map<int, std::string> fLabelMap;

  //  This method is called at each event read from the Input Tree. Aim is to
  //  build treat Raw dat in order to extract physical parameter.
  void BuildPhysicalEvent();
  //   Remove bad channel, calibrate the data and apply threshold
  void PreTreat();
  void ClearPreTreatedData() { m_CalData->Clear(); }

  //   Return false if the channel is disabled by user
  //   Frist argument is either 0 for X,1 Y,2 SiLi, 3 CsI
  bool IsValidChannel(const int& DetectorType, const int& telescope,
                      const int& channel);

  // Match XY events based on the energy lost in the Si
  // Returns an array of good X,Y pairs (matching in energy)
  std::vector<std::pair<unsigned int, unsigned int>> Match_X_Y(); //!

  // Returns a pair <CsI,pixel> if a good match is found
  std::pair<unsigned int, unsigned int>
  Match_Pixel(std::pair<unsigned int, unsigned int> XY_couple,
              unsigned int                          CsI_size); //!

  bool Match_Si_CsI(int X, int Y, int CristalNbr, int DetectorNbr); //!
  std::vector<std::pair<int, std::pair<unsigned int, unsigned int>>>
  Match_Si_CsI(std::vector<std::pair<unsigned int, unsigned int>>
                   Array_of_good_XY_couples); //!

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

private:
  void AddTelescope(ROOT::Math::XYZVector C_X1_Y1,
                    ROOT::Math::XYZVector C_X128_Y1,
                    ROOT::Math::XYZVector C_X1_Y128,
                    ROOT::Math::XYZVector C_X128_Y128);

public:
  std::vector<ROOT::Math::XYZVector> m_X1_Y1; //!
  std::vector<ROOT::Math::XYZVector> m_X128_Y1; //!
  std::vector<ROOT::Math::XYZVector> m_X1_Y128; //! 
  std::vector<ROOT::Math::XYZVector> m_X128_Y128; //!
  std::map<int, bool>                m_SiPresent; //!
  std::map<int, bool>                m_CsIPresent; //!
  std::map<int, bool>                m_SiLiPresent; //!

private: //   Parameter used in the analysis
  int                                           m_NumberOfTelescope; //!
  std::vector<std::vector<std::vector<double>>> m_StripPositionX; //!
  std::vector<std::vector<std::vector<double>>> m_StripPositionY; //!
  std::vector<std::vector<std::vector<double>>> m_StripPositionZ; //!

  // By default take EX and TY.
  bool m_Take_E_Y; //!
  bool m_Take_T_Y; //!

  // Size Container to be used in the different loop of the analysis (value must
  // be given locally)
  unsigned int m_StripXEMult; //!
  unsigned int m_StripYEMult; //!
  unsigned int m_StripXTMult; //!
  unsigned int m_StripYTMult; //!
  unsigned int m_SiLiEMult; //!
  unsigned int m_SiLiTMult; //!
  unsigned int m_CsIEMult; //!
  unsigned int m_CsITMult; //!

  //   Event over this value after pre-treatment are not treated / avoid long
  //   treatment time on spurious event
  unsigned int m_MaximumStripMultiplicityAllowed; //!
                                                  //   Give the allowance in
                                                  //   percent of the difference
                                                  //   in energy between X and Y
  double m_StripEnergyMatchingSigma; //!
  double m_StripEnergyMatchingNumberOfSigma; //!

  // Raw Threshold
  int m_Si_X_E_RAW_Threshold; //!
  int m_Si_Y_E_RAW_Threshold; //!
  int m_SiLi_E_RAW_Threshold; //!
  int m_CsI_E_RAW_Threshold; //!

  // Calibrated Threshold
  double m_Si_X_E_Threshold; //!
  double m_Si_Y_E_Threshold; //!
  double m_SiLi_E_Threshold; //!
  double m_CsI_E_Threshold; //!

  // Geometric Matching
  // size in strip of a pad
  int m_SiLi_Size; //!
                   // center position of the pad on X
  std::vector<int> m_SiLi_MatchingX; //!
                                     // center position of the pad on Y
  std::vector<int> m_SiLi_MatchingY; //!
                                     // size in strip of a cristal
  int m_CsI_Size; //!
                  // center position of the cristal on X
  std::vector<int> m_CsI_MatchingX; //!
  std::map<int, std::pair<int, int>>
      m_ZeroDegree_CsI_MatchingX; //!
                                  // center position of the cristal on X
  std::vector<int>                   m_CsI_MatchingY; //!
  std::map<int, std::pair<int, int>> m_ZeroDegree_CsI_MatchingY; //!

  // If set to true, all event that do not come in front of a cristal will be
  // ignore all time (crossing or not),
  // Warning, this option reduce statistic, however it help eliminating
  // unrealevent event that cross the DSSD
  // And go between pad or cristal.
  bool m_Ignore_not_matching_SiLi; //!
  bool m_Ignore_not_matching_CsI; //!

  // Prevent to treat event with ambiguous matching beetween X and Y
  std::vector<int>   m_match_type; //!
  std::map<int, int> m_NMatchDet; //!
  std::map<int, int> m_StripXMultDet; //!
  std::map<int, int> m_StripYMultDet; //!
  std::map<int, int> m_NMatchX; //!
  std::map<int, int> m_NMatchY; //!

  //   Map of activated channel
  std::map<int, std::vector<bool>> m_XChannelStatus; //!
  std::map<int, std::vector<bool>> m_YChannelStatus; //!
  std::map<int, std::vector<bool>> m_SiLiChannelStatus; //!
  std::map<int, std::vector<bool>> m_CsIChannelStatus; //!

  //   DSSD
  //   X
  double fSi_X_E(const int& i);
  double fSi_X_T(const int& i);

  //   Y
  double fSi_Y_E(const int& i);
  double fSi_Y_T(const int& i);

  //   SiLi
  double fSiLi_E(const int& i);
  double fSiLi_T(const int& i);

  //   CsI
  double fCsI_E(const int& i);
  double fCsI_E2(const int& i);
  double fCsI_T(const int& i);

public:
  TVector3 GetPositionOfInteraction(const int i) const;
  TVector3 GetTelescopeNormal(const int i) const;

  double GetNumberOfTelescopes() const { return m_NumberOfTelescope; };

  // Use to access the strip position
  double GetStripPositionX(const int N, const int X, const int Y) const {
    return m_StripPositionX[N - 1][X - 1][Y - 1];
  };
  double GetStripPositionY(const int N, const int X, const int Y) const {
    return m_StripPositionY[N - 1][X - 1][Y - 1];
  };
  double GetStripPositionZ(const int N, const int X, const int Y) const {
    return m_StripPositionZ[N - 1][X - 1][Y - 1];
  };

  // simulation
public:
  void        InitSimulation(std::string simtype);
  void        ConstructGeometry();
  std::string m_simtype;

#ifdef Geant4_FOUND
private:
  std::shared_ptr<nptool::geant4::VDetector> m_Geant4;
#endif
};

} // namespace must2
#endif

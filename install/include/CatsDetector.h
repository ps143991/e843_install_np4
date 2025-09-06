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
 * This class is heavily based on the nptool v3 CATS detector                *
 *                                                                           *
 *****************************************************************************/
#ifndef CatsDetector_h
#define CatsDetector_h

#include <stdlib.h>

#include <map>
#include <vector>

#include "CatsData.h"
#include "CatsPhysics.h"
#include "Math/Vector3D.h"
#include "NPApplication.h"
#include "NPCalibrationManager.h"
#include "NPVDetector.h"

// ROOT
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TObject.h"
#include "TVector2.h"

// MFM
#ifdef MFM_FOUND
#include "MFMAllFrames.h"
#endif

namespace cats {

class CatsSpectra;
class CatsDetector : public nptool::VDetector {
 public:  // Constructor and Destructor
  CatsDetector();
  ~CatsDetector();


 public:  // Data member
  cats::CatsData* m_RawData;
  cats::CatsData* m_CalData;
  cats::CatsPhysics* m_PhysicsData;
  nptool::CalibrationManager m_Cal;
  std::shared_ptr<cats::CatsSpectra> m_Spectra;

      // getters for raw and pre-treated data object
    public:
      CatsData* GetRawData() const { return m_RawData; }
      CatsData* GetPreTreatedData() const { return m_CalData; }
    
      // parameters used in the analysis
    private:
      // thresholds
      double       m_cats_Time_RAW_Threshold = 0; //!
      unsigned int m_cats_Mult; //!
      std::map<std::string, std::pair<unsigned int, unsigned long long>>
        Sortcats; //!

 public:  // inherrited from nptool::VPlugin
  std::vector<std::string> GetDependencies() { return {"root"}; };

 public:  // inherrited from nptool::VDetector
          //  Read stream at ConfigFile to pick-up parameters of detector
          //  (Position,...) using Token
  void ReadConfiguration(nptool::InputParser);

  //  Add Parameter to the CalibrationManger
  void AddParameterToCalibrationManager(){};

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

      // Method to test calibration
      void TestCalibration();

      //   Remove bad channel, calibrate the data and apply threshold
      void PreTreat();
      void ClearPreTreatedData() { m_CalData->Clear(); }
      //  Those two method all to clear the Event Physics or Data
      void ClearEventPhysics() { m_PhysicsData->Clear(); };
      void ClearEventData() { m_RawData->Clear(); };

  //   Return false if the channel is disabled by user
  bool IsValidChannel(const std::string& DetectorType, const int& Detector,
                      const int& channel);
  void InitializeStandardParameter();
  void ReadAnalysisConfig();
  void AddCATS(ROOT::Math::XYZVector C_X1_Y1, ROOT::Math::XYZVector C_X28_Y1,
               ROOT::Math::XYZVector C_X1_Y28, ROOT::Math::XYZVector C_X28_Y28,
               unsigned short N);
  void AddMask(double Z, unsigned short N);

 public:
  ROOT::Math::XYZVector GetBeamDirection();
  ROOT::Math::XYZVector GetPositionOnTarget();

  double GetPositionOnTargetX() { return m_PhysicsData->PositionOnTargetX; }
  double GetPositionOnTargetY() { return m_PhysicsData->PositionOnTargetY; }

 private:
  // unsigned int sizeY; //!
  // unsigned int sizeX; //!
  unsigned int sizeDet;  //!
  double Mask1_Z = 0;    //!
  double Mask2_Z = 0;    //!

  int counter = 0;
  unsigned short StrX;      //!
  unsigned short NX;        //!
  unsigned short CATS_X_Q;  //!
  unsigned short StrY;      //!
  unsigned short NY;        //!
  unsigned short CATS_Y_Q;  //!
                            // Look how many CATS were fired
                            // use a set to identify which detector has been hit
  std::set<int> DetectorHitX;                                //!
  std::set<int> DetectorHit;                                 //!
  std::map<unsigned short, std::vector<double>> QsumSample;  //!

  // Debugging
  // unsigned long counter;//!
  unsigned long long time_elapsed1;  //!
  unsigned long long time_elapsed2;  //!
  unsigned long long time_elapsed3;  //!

 public:
  ROOT::Math::XYZVector BeamDirection;  //!
  std::map<unsigned short,
           std::vector<std::pair<unsigned short, unsigned short>>>
      MapX;  //!
  std::map<unsigned short,
           std::vector<std::pair<unsigned short, unsigned short>>>
      MapY;  //!
  std::map<unsigned short, std::pair<unsigned short, unsigned short>>
      MaxQX;  //!
  std::map<unsigned short, std::pair<unsigned short, unsigned short>>
      MaxQY;                               //!
  std::map<unsigned short, double> QSumX;  //!
  std::map<unsigned short, double> QSumY;  //!
  std::map<unsigned short, std::pair<double, std::pair<double, double>>>
      Positions;                                   //!
  std::map<unsigned short, unsigned short> MultX;  //!
  std::map<unsigned short, unsigned short> MultY;  //!
  std::map<unsigned short, unsigned long long> MapTSX;  //!
  std::map<unsigned short, unsigned long long> MapTSY;  //!
  std::map<unsigned short, std::vector<std::vector<double>>>
      StripPositionX;  //!
  std::map<unsigned short, std::vector<std::vector<double>>>
      StripPositionY;                               //!
  std::map<unsigned short, double> StripPositionZ;  //!
  int m_NumberOfCATS;                               //!
  double m_TargetAngle;                             //!
  double m_TargetThickness;                         //!

 private:
  //   Map of activated channel
  std::map<int, std::vector<bool>> m_XChannelStatus;  //!
  std::map<int, std::vector<bool>>
      m_YChannelStatus;  //!
                         //   Map of inverted channel
  std::map<int, std::vector<int>> m_CATSXInversion;  //!
  std::map<int, std::vector<int>> m_CATSYInversion;  //!

  // Those two vector contain a pointer to the reconstruction function that
  // should be used for each detector Method take as argument a vector<double>
  // representing the Charge distribution and uint giving the Strip with Max Q
  // It returns a double, corresponding to the position in Strip unit Methods
  // are declared in the CATS namespace outside the class CINT can't handle such
  // complex vector, so the ifndef part is there to hide it away
#ifndef __CINT__
  // std::map<unsigned short,
  //          double (*)(std::pair<unsigned short, unsigned short>&,
  //                     std::vector<std::pair<unsigned short, unsigned
  //                     short>>&, double)>
  //     ReconstructionFunctionY; //!

  std::map<
      unsigned short,
      std::function<double(
          std::pair<unsigned short, unsigned short>&,
          std::vector<std::pair<unsigned short, unsigned short>>&, double)>>
      ReconstructionFunctionY;  //!

  // std::map<unsigned short,
  //          double (*)(std::pair<unsigned short, unsigned short>&,
  //                     std::vector<std::pair<unsigned short, unsigned
  //                     short>>&, double)>
  //     ReconstructionFunctionX; //!

  std::map<
      unsigned short,
      std::function<double(
          std::pair<unsigned short, unsigned short>&,
          std::vector<std::pair<unsigned short, unsigned short>>&, double)>>
      ReconstructionFunctionX;  //!

  // std::map<unsigned short, double (*)(std::function<void*>)>
  // ReconstructionFunctionX; //!
#endif /* __CINT __ */
 public:
  // Set the reconstruction Method used for the CATS plane

  unsigned int FindMult(
      std::vector<std::pair<unsigned short, unsigned short>> vector,
      unsigned int StripMax);

  void SetReconstructionMethod(unsigned int CATSNumber, std::string XorY,
                               std::string MethodName);  //!

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
  // give and external TcatsData object to TcatsPhysics.
  // needed for online analysis for example
  void SetRawDataPointer(CatsData* rawDataPointer) {
    m_RawData = rawDataPointer;
  }

  // The reconstruction methods need to be outside the class for practical
  // purpose of dealing with the function pointer
  double AnalyticHyperbolicSecant(
      std::pair<unsigned short, unsigned short>&,
      std::vector<std::pair<unsigned short, unsigned short>>&, double);
  double FittedHyperbolicSecant(
      std::pair<unsigned short, unsigned short>&,
      std::vector<std::pair<unsigned short, unsigned short>>&, double);

  double fCATS_X_Q(const int& i);
  double fCATS_Y_Q(const int& i);
  bool fCATS_Threshold_X(const int& i);
  bool fCATS_Threshold_Y(const int& i);
  double fCATS_Ped_X(const int& i);
  double fCATS_Ped_Y(const int& i);

  std::string m_config_file;

 private:
 private:  //   Parameter used in the analysis
};

}  // namespace cats
#endif

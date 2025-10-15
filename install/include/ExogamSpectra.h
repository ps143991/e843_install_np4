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
#ifndef ExogamSpectra_h
#define ExogamSpectra_h
// nebula-plus
#include "ExogamDetector.h"
// root
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
#include "TROOT.h"
// std
#include <map>
#include <memory>
namespace exogam {

  // forward declaration is necessary
  // class ExogamDetector;
  class ExogamSpectra {
   public:
    ExogamSpectra();
    ~ExogamSpectra(){};

   private:
    TFolder* exofolder;
    std::shared_ptr<exogam::ExogamDetector> m_detector;
    exogam::ExogamData* m_RawData;
    exogam::ExogamPhysics* m_PhysicsData;
    std::map<std::string, TH1*> m_raw_hist_1D;
    std::map<std::string, TH2*> m_raw_hist_2D;
    std::map<std::string, TH1*> m_raw_hist_V1D;
    std::map<std::string, TH1*> m_phy_hist_1D;
    std::map<std::string, TH2*> m_phy_hist_2D;
    std::map<std::string, TH1*> m_phy_hist_V1D;
    std::map<std::string, TCanvas*> m_canvas;
    unsigned long long ExoRefTS;
    unsigned long long ExoLastTS;
    long long ExoTSDiff;

   public:
    void FillRaw();
    void FillPhy();
    void Clear();

   // Getters & setters for reference values for constructing spectra
   public:
    inline void SetRefTS(unsigned long long buffer){ExoRefTS=buffer;}
    inline void SetLastTS(unsigned long long buffer){ExoLastTS=buffer;}
    inline unsigned long long GetRefTS(){return ExoRefTS;}
    inline unsigned long long GetLastTS(){return ExoLastTS;}
   
   public:
    bool haveRaw = false;
    bool havePhy = false;
    bool diagcanvas = false;
  
  };

} // namespace exogam
#endif

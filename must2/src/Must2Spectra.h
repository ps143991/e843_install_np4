#ifndef Must2Spectra_h
#define Must2Spectra_h

/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girard-alcindor@ijclab.in2p3.fr                          *
 *                                                                           *
 * Creation Date  : 08/03/24                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This class hold MUST2 Spectra definitions                                 * 
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This class is heavily based on the nptool v3 MUST2 detector               *
 *                                                                           *
 *****************************************************************************/

#include "Must2Detector.h"
// root
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
// std
#include <map>
#include <memory>
namespace must2 {

  // forward declaration is necessary
  // class Must2Detector;
  class Must2Spectra {
   public:
    Must2Spectra();
    ~Must2Spectra(){};

   private:
    std::shared_ptr<must2::Must2Detector> m_detector;
    must2::Must2Data* m_RawData;
    must2::Must2Physics* m_PhysicsData;
    std::map<std::string, TH1*> m_raw_hist;
    std::map<std::string, TCanvas*> m_raw_canvas;
    std::map<std::string, TH1*> m_phy_hist;
    std::map<std::string, TCanvas*> m_phy_canvas;
    std::shared_ptr<nptool::Application> m_app;
    // Boolean used to display more spectra
    bool is_expert;

   public:
    void FillRaw();
    void FillPhy();
    void Clear();
  };

} // namespace must2
#endif

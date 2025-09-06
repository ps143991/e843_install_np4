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
#ifndef ZddSpectra_h
#define ZddSpectra_h
#include "ZddDetector.h"
// root
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
// std
#include <map>
#include <memory>
namespace zdd {

  // forward declaration is necessary
  // class ZddDetector;
  class ZddSpectra {
   public: 
    ZddSpectra();
    ~ZddSpectra(){};

   private:

    std::shared_ptr<zdd::ZddDetector> m_detector;
    zdd::ZddData* m_RawData;
    zdd::ZddPhysics* m_PhysicsData;
    std::map<std::string,TH2*> m_raw_hist;
    std::map<std::string,std::vector<unsigned int>> m_raw_channels;
    // std::map<std::string,std::map<unsigned int, TH1*>> m_phy_hist;
    std::map<std::string, TH1*> m_phy_hist;
    // Double_t dt_x = -1.;
    // Double_t dt_y = -1.;
    long long dt_x=0;
    long long dt_y=0;
    double dl_x1 = 0.0;
    double dl_y1 = 0.0;
    double dl_x2 = 0.0;
    double dl_y2 = 0.0;

    Double_t dx_off[2]={+53.055,-54.09 };
    Double_t dx_gain[2]={-0.33057,0.41992};
    Double_t dy_off[2]={-68.3353,76.538};
    Double_t dy_gain[2]={0.4735,-0.43558};
    unsigned long long Central_Right_PL_TS = -1;
   public:
    void FillRaw();
    void FillPhy();
    void Clear();
  };

} // namespace zdd
#endif
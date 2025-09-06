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
#ifndef TacSpectra_h
#define TacSpectra_h
// nebula-plus
#include "TacDetector.h"
// root
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
// std
#include <map>
#include <memory>
namespace tac {

  // forward declaration is necessary
  // class TacDetector;
  class TacSpectra {
   public:
    TacSpectra();
    ~TacSpectra(){};

   private:
    std::shared_ptr<tac::TacDetector> m_detector;
    tac::TacData* m_RawData;
    tac::TacPhysics* m_PhysicsData;
    std::map<std::string,TH2*> m_raw_hist;
    std::map<std::string,unsigned int> m_bin_label;
    std::map<std::string,std::vector<std::pair<unsigned int,std::string>>> m_raw_channels;
    std::map<std::string, std::pair<unsigned int, TH1*>> m_phy_hist;

   public:
    void FillRaw();
    void FillPhy();
    void Clear();
  };

} // namespace tac
#endif

/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * connumexot address: girard-alcindor@ijclab.in2p3.fr                          *
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
#ifndef NumexoSpectra_h
#define NumexoSpectra_h
// nebula-plus
#include "NumexoDetector.h"
// root
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
// std
#include <map>
#include <memory>
namespace numexo {

  // forward declaration is necessary
  // class NumexoDetector;
  class NumexoSpectra {
   public:
    NumexoSpectra();
    ~NumexoSpectra(){};

   private:
    std::shared_ptr<numexo::NumexoDetector> m_detector;
    std::map<std::string,TH1*> m_raw_hist;
    std::map<std::string, TCanvas*> m_raw_canvas;
    std::map<std::string,unsigned int> m_bin_label;
    std::map<std::string,std::vector<std::pair<unsigned int,std::string>>> m_raw_channels;
    std::map<std::string, std::pair<unsigned int, TH1*>> m_phy_hist;

   public:
    void FillRaw();
    void FillPhy();
    void Clear();
  };

} // namespace numexo
#endif

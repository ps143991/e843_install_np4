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
#ifndef CatsSpectra_h
#define CatsSpectra_h
// nebula-plus
#include "CatsDetector.h"
// root
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
// std
#include <map>
#include <memory>
namespace cats {

  // forward declaration is necessary
  // class CatsDetector;
  class CatsSpectra {
   public:
    CatsSpectra();
    ~CatsSpectra(){};

   private:
    std::shared_ptr<cats::CatsDetector> m_detector;
    cats::CatsData* m_RawData;
    cats::CatsData* m_CalData;
    cats::CatsPhysics* m_PhysicsData;
    std::map<std::string, TH1*> m_raw_hist;
    std::map<std::string, TH1*> m_phy_hist;

    unsigned int fStripsNumber {28};

   public:
    void FillRaw();
    void FillPhy();
    void Clear();
  };

} // namespace cats
#endif

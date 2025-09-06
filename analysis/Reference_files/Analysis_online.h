#ifndef Analysis_h
#define Analysis_h

#include "NPVUserAnalysis.h"
#include "Must2Detector.h"
#include "CatsDetector.h"
#include "ZddDetector.h"
#include "TacDetector.h"
#include "ExogamDetector.h"
#include "ExogamDetector.h"
#include "EbyEDetector.h"
#include "NumexoDetector.h"

#include "TH2F.h"

#include <map>


// #include "MyDetector.h"
namespace user_analysis {
  class Analysis : public nptool::VUserAnalysis {
    public:
      Analysis(){};
      ~Analysis(){};

    public:
      void Init();
      void TreatEvent();
      // if this method return false, the event is discarded
      // Caution : this change the size of the tree,
      // so the friend mecanism no longer work
      bool FillOutputCondition() { return true; };
      void InitializeDataOuput(std::shared_ptr<nptool::VDataOutput>){};
      void InitializeDataInput(std::shared_ptr<nptool::VDataInput>){};
      void End();

    private:
      std::shared_ptr<cats::CatsDetector> cats;
      std::shared_ptr<must2::Must2Detector> must2;
      std::shared_ptr<zdd::ZddDetector> zdd;
      std::shared_ptr<exogam::ExogamDetector> exogam;
      std::shared_ptr<ebye::EbyEDetector> gatconf;
      std::shared_ptr<numexo::NumexoDetector> numexo;

    private:
      std::map<std::string, unsigned int> m_bin_label;
      std::map<std::string, TH2F*>        m_phy_hist_2D;
      //  std::shared_ptr<my_detector::MyDetector> mydetector;
  };
} // namespace user_analysis
#endif

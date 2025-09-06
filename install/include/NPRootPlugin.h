// nptool
#include "NPFunction.h"
#include "NPVPlugin.h"
// Root
#include "Math/Vector3D.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TRandom.h"
#include "TRandom2.h"
#include "TVector3.h"
//  stl
#include <string>
#include <vector>

namespace nptool {
  // Check the type of Filename (root or ASCII) and extract build/extract a 1D histogramm
  TH1F* Read1DProfile(std::string filename, std::string HistName);
  // Check the type of Filename (root or ASCII) and extract build/extract a 2D histogramm
  TH2F* Read2DProfile(std::string filename, std::string HistName);
  // Open a file at Filename after checking the type of file it is
  // true for a ASCII file
  // False for a Root file
  bool OpenASCIIorROOTFile(std::string filename, std::ifstream& ASCII, TFile& ROOT);

  // generetate random (X,Y) based on prodivded 2D gaussian parameter
  void RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double& X, double& Y);

  // plugin class
  class RootPlugin : public nptool::VPlugin {
   public:
    RootPlugin(){};
    ~RootPlugin(){};

   public:
    std::vector<std::string> GetDependencies() { return {}; };
  };

  TVector3 ConvertTVector3(std::vector<double> v);

  ROOT::Math::XYZVector ConvertXYZVector(std::vector<double> v);
} // namespace nptool

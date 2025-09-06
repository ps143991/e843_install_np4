#include "NPRootPlugin.h"
#include <sstream>
#include <fstream>

// Check the type of Filename (root or ASCII) and extract build/extract a 1D histogramm
TH1F* nptool::Read1DProfile(std::string filename, std::string HistName) {
  std::ifstream ASCII;
  TFile ROOT;
  TH1F* h;

  // test whether file format is ASCII or ROOT
  bool type = OpenASCIIorROOTFile(filename, ASCII, ROOT);

  // ASCII File case
  if (type) {
    std::string LineBuffer;

    // storing std::vector
    std::vector<double> x, w;

    // variable buffer
    double xb, wb;

    // read the file
    Double_t xmin = 200;
    Double_t xmax = -200;
    int mysize = 0;
    while (getline(ASCII, LineBuffer)) {
      std::stringstream iss(LineBuffer);
      if (!(iss >> xb >> wb)) {
        continue;
      } // skip comment lines
      // fill std::vectors
      x.push_back(xb);
      w.push_back(wb);
      //       std::cout << xb << "\t" << wb << std::endl;
      // compute xmin / xmax / size of x array
      if (xb > xmax)
        xmax = xb;
      if (xb < xmin)
        xmin = xb;
      mysize++;
    }
    Double_t dx = (xmax - xmin) / (mysize);
    //    std::cout << xmin << "\t" << xmax << "\t" << mysize << "\t" << dx << std::endl;

    // fill histo
    h = new TH1F(HistName.c_str(), HistName.c_str(), mysize, xmin, xmax + dx);
    for (unsigned int i = 0; i < mysize; i++) {
      int bin = h->FindBin(x[i]);
      h->SetBinContent(bin, w[i]);
      //      std::cout << i << "\t" << x[i] << "\t" << bin << "\t" << w[i] << "\t" << h->GetBinContent(bin) <<
      //      std::endl;
    }
  }

  // ROOT File case
  else {
    h = (TH1F*)gDirectory->FindObjectAny(HistName.c_str());
    if (!h) {
      std::cout << "Error: Histogram " << HistName << " not found in file " << filename << std::endl;
      exit(1);
    }
  }

  return h;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Check the type of Filename (root or ASCII) and extract build/extract a 2D histogramm
TH2F* nptool::Read2DProfile(std::string filename, std::string HistName) {

  std::ifstream ASCII;
  TFile ROOT;
  TH2F* h;

  bool type = OpenASCIIorROOTFile(filename, ASCII, ROOT);

  // ASCII File case
  if (type) {
    std::string LineBuffer;

    // storing std::vector
    std::vector<double> x, y, w;

    // variable buffer
    double xb, yb, wb;

    // Read the file
    while (!ASCII.eof()) {
      getline(ASCII, LineBuffer);
      std::stringstream LineStream(LineBuffer);
      // ignore comment line
      if (LineBuffer.compare(0, 1, "%") != 0) {
        LineStream >> xb >> yb >> wb;
        x.push_back(xb);
        y.push_back(yb);
        w.push_back(wb);
      }
    }

    // Look for the step size, min and max of the distribution
    double xmin = 0;
    double xmax = 0;
    unsigned int xsize = x.size();

    double ymin = 0;
    double ymax = 0;
    unsigned int ysize = y.size();

    if (xsize > 0) {
      xmin = x[0];
      xmax = x[0];
    }

    if (ysize > 0) {
      ymin = y[0];
      ymax = y[0];
    }

    for (unsigned int i = 0; i < xsize; i++) {
      if (x[i] > xmax)
        xmax = x[i];
      if (x[i] < xmin)
        xmin = x[i];
    }

    for (unsigned int i = 0; i < ysize; i++) {
      if (y[i] > ymax)
        ymax = y[i];
      if (y[i] < ymin)
        ymin = y[i];
    }

    h = new TH2F(HistName.c_str(), HistName.c_str(), xsize, xmin, xmax, ysize, ymin, ymax);

    for (unsigned int i = 0; i < xsize; i++) {
      int bin = h->FindBin(x[i], y[i]);
      h->SetBinContent(bin, w[i]);
    }
  }

  // ROOT File case
  else {
    h = (TH2F*)gDirectory->FindObjectAny(HistName.c_str());
    if (!h) {
      std::cout << "Error: Histogramm " << HistName << " not found in file " << filename << std::endl;
      exit(1);
    }
  }
  return h;
}

// Open a file at Filename after checking the type of file it is
// true for a ASCII file
// False for a Root file
bool nptool::OpenASCIIorROOTFile(std::string filename, std::ifstream& ASCII, TFile& ROOT) {

  // look for .root extension
  size_t pos = filename.find(".root");

  std::string GlobalPath = getenv("NPTOOL");
  std::string StandardPath = GlobalPath + "/Inputs/CrossSection/" + filename;

  // extension not found, file is assume to be a ASCII file
  if (pos > filename.size()) {
    ASCII.open(filename.c_str());

    if (!ASCII.is_open()) {
      ASCII.open(StandardPath.c_str());
      if (!ASCII.is_open()) {
        std::cout << "Error, file " << filename << " not found " << std::endl;
        exit(1);
      }
    }

    return true;
  }

  else {
    if (ROOT.Open(filename.c_str(), "READ")) {
      return false;
    }

    else {

      if (ROOT.Open(StandardPath.c_str(), "READ")) {
        return false;
      }
      else {
        std::cout << "Error, file " << StandardPath << " not found " << std::endl;
        exit(1);
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void nptool::RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double& X, double& Y) {
  if (SigmaX != 0 && SigmaY != 0) {
    X = gRandom->Gaus(MeanX, SigmaX);
    // double a = SigmaX/2.;
    // double b = SigmaY/2.;
    // double SigmaYPrim = b * sqrt(1 - X*X / (a*a));
    double SigmaYPrim = SigmaY; // This is not right a better solution needed
    Y = gRandom->Gaus(MeanY, SigmaYPrim);
  }

  else if (SigmaX == 0 && SigmaY != 0) {
    X = MeanX;
    Y = gRandom->Gaus(MeanY, SigmaY);
  }

  else if (SigmaX != 0 && SigmaY == 0) {
    Y = MeanY;
    X = gRandom->Gaus(MeanX, SigmaX);
  }

  else {
    X = MeanX;
    Y = MeanY;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 nptool::ConvertTVector3(std::vector<double> v) {
  nptool::message("red", "root", "nptool::ConvertTVector3", "TVector3 is a legacy class, prefere XYZVector");
  if (v.size() == 3)
    return TVector3(v[0], v[1], v[2]);
  return TVector3(-1000, -1000, -1000);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ROOT::Math::XYZVector nptool::ConvertXYZVector(std::vector<double> v) {
  if (v.size() == 3)
    return ROOT::Math::XYZVector(v[0], v[1], v[2]);
  return ROOT::Math::XYZVector(-1000, -1000, -1000);
};


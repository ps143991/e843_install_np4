// nptool headers
#include "NPParticle.h"

using namespace nptool;

// ROOT headers
#include "TPRegexp.h"
#include "TString.h"

// C++ headers
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

ClassImp(Particle)

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    Particle::Particle() {
  //----------- Default Constructor ----------
  fName = "XX DEFAULT XX";
  fCharge = 0;
  fAtomicWeight = 0;
  fMassExcess = 0;
  fMass = 0;
  fExcitationEnergy = 0;
  fSpinParity = "";
  fSpin = 0;
  fParity = "";
  fLifeTime = -1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Particle::Particle(string name) { SetUp(name); }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Particle::Particle(string name, const string& path) {
  SetUp(name);
  LoadENSDF(name, path);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Particle::Particle(string name, vector<string> subpart, double binding, double Ex, string SpinParity, double Spin,
                   string Parity, double LifeTime) {
  fName = name;
  fCharge = 0;
  fAtomicWeight = 0;
  unsigned int size = subpart.size();
  double Mass = 0;
  for (unsigned int i = 0; i < size; i++) {
    Particle N = Particle(subpart[i]);
    Mass += N.Mass();
    fAtomicWeight += N.GetA();
    fCharge += N.GetZ();
  }

  Mass -= binding;
  SetMassExcess(1000 * (Mass - fAtomicWeight * amu_c2 + fCharge * electron_mass_c2));

  fExcitationEnergy = Ex;
  fSpinParity = SpinParity;
  fSpin = Spin;
  fParity = Parity;
  fLifeTime = LifeTime;
  fMass = Mass;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::LoadENSDF(const string& isotope, const string& pathENSDF) {
  // open file to read
  TString fileName = Form("%s/AR_%s.ens", pathENSDF.c_str(), isotope.c_str());
  ifstream inputFile(fileName.Data());
  if (!inputFile)
    cout << "Problem opening file " << fileName << endl;

  // read file
  TString lineFile;
  TString isComment, isWhitespace, isLevel;
  TString energy, uncertainty;
  TString tempIsotopeName = isotope;
  // calculate size of file name
  Int_t size = tempIsotopeName.Length();
  // get position of first '_' in file name
  Int_t pos = tempIsotopeName.First("_");
  if (pos != -1) {
    // remove everything after first '_'
    tempIsotopeName.Remove(pos, size - pos);
    // size now corresponds to the length of the isotope name
    size = tempIsotopeName.Length();
  }
  // find length of mass number
  //   TString tempIsotopeName = pathENSDF;
  TPRegexp("\\D").Substitute(tempIsotopeName, "",
                             "g"); // suppress all alpha characters
  Int_t nbDigit = tempIsotopeName.Length();
#ifdef DEBUG
  cout << "size, nbDigit:\t" << size << "\t" << nbDigit << endl;
#endif
  while (!inputFile.eof()) {
    lineFile.ReadLine(inputFile);
    isWhitespace = lineFile(size, 1);
    isComment = lineFile(size + 1, 1);
    isLevel = lineFile(size + 2, 1);
    if (size == 3 && nbDigit == 2) { // case of a few isotopes, e.g. 12C, 10B, ...
      isComment = lineFile(size + 2, 1);
      isLevel = lineFile(size + 3, 1);
    }
    if (size == 2 && nbDigit == 1) { // case of a few isotopes, e.g. 1H ...
      isComment = lineFile(size + 2, 1);
      isLevel = lineFile(size + 3, 1);
    }
#ifdef DEBUG
    cout << "isComment, isLevel:\t" << isComment << "\t" << isLevel << endl;
#endif
    if (isWhitespace.IsWhitespace() && isComment.CompareTo("c") && !isLevel.CompareTo("L")) {
      energy = lineFile(size + 4, 10);
      energy = energy.Strip(); // remove trailing white spaces
      uncertainty = lineFile(size + 14, 2);
      if (size == 3 && nbDigit == 2) {
        energy = lineFile(size + 5, 10);
        energy = energy.Strip(); // remove trailing white spaces
        uncertainty = lineFile(size + 15, 2);
      }
      if (size == 2 && nbDigit == 1) { // case of a few isotopes, e.g. 1H ...
        energy = lineFile(size + 6, 10);
        energy = energy.Strip(); // remove trailing white spaces
        uncertainty = lineFile(size + 16, 2);
      }
      if (energy.IsFloat()) {
        // energy set to MeV units
        fLevelEnergy.push_back(energy.Atof() * 1e-3);
        // transform uncertainty to MeV units
        UInt_t nElementsAfterComma = 0;
        if (energy.First(".") > 0)
          nElementsAfterComma = energy.Length() - energy.First(".") - 1;
        Double_t uncertain = uncertainty.Atof();
        uncertain /= TMath::Power(10, (Int_t)nElementsAfterComma);
        fLevelEnergyUncertainty.push_back(uncertain * 1e-3);
      }
    }
  }

  // close input file
  inputFile.close();

  // sort level energies
  // !! use with caution since if uncertainty are defined they are not sorted
  // accordingly to the energies
  sort(fLevelEnergy.begin(), fLevelEnergy.end());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::SetUp(string name) {
  if (name == "electron") {
    fName = "electron";
    fCharge = -1;
    fAtomicWeight = 0;
    fMassExcess = 0;
    fMass = electron_mass_c2;
    fExcitationEnergy = 0;
    fSpinParity = "0.5";
    fSpin = 0.5;
    fParity = "";
    fLifeTime = -1;
    return;
  }

  else if (name == "gamma") {
    fName = "gamma";
    fCharge = 0;
    fAtomicWeight = 0;
    fMassExcess = 0;
    fMass = 0;
    fExcitationEnergy = 0;
    fSpinParity = "0";
    fSpin = 0;
    fParity = "";
    fLifeTime = -1;
    return;
  }

  //----------- Constructor Using nubtab16.asc by name----------
  // open the file to read and check if it is open
  fExcitationEnergy = 0;
  // Replace the n,p,d,t,a by there standard name:
  if (name == "p")
    name = "1H";
  else if (name == "n")
    name = "1n";
  else if (name == "d")
    name = "2H";
  else if (name == "t")
    name = "3H";
  else if (name == "a")
    name = "4He";
  else if (name == "n")
    name = "1n";
  else if (name == "neutron")
    name = "1n";
  else if (name == "g")
    name = "gamma";
  else if (name == "gamma")
    name = "gamma";
  else if (name == "4n") { // tetraneutron @Eres = 0
    string line = "004 0000   4n      32285.268   0.0005                       219.4    "
                  "ys 0.6    1/2+          00 02PaDGt   B-=100";
    Extract(line.data());
    return;
  }

  ifstream inFile;
  string path1 = getenv("NPTOOL_HOME");
  string path2 = getenv("NPTOOL_ENV");
  string FileName = path1 + "/" + path2 + "/physics-essential/sources/nubtab16.asc";

  inFile.open(FileName.c_str());

  // reading the file
  string line, s_name;
  size_t space;
  if (inFile.is_open()) {
    while (!inFile.eof()) {
      getline(inFile, line);

      s_name = line.substr(11, 7);
      space = s_name.find_first_of(" ");
      s_name.resize(space);

      if (s_name.find(name) != string::npos && s_name.length() == name.length())
        break;
    }
    Extract(line.data());
  }
  else
    cout << "Unable to open file nuclear data base file " << FileName << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Particle::Particle(int Z, int A) {
  //----------- Constructor Using nubtab16.asc by Z and A----------

  fExcitationEnergy = 0;

  if (Z == 0 && A == 4) {
    SetUp("4n");
    return;
  }
  // open the file to read and check if it is open
  ifstream inFile;
  string path1 = getenv("NPTOOL_HOME");
  string path2 = getenv("NPTOOL_ENV");
  string FileName = path1 + "/" + path2 + "/physics-essential/sources/nubtab16.asc";

  inFile.open(FileName.c_str());

  // reading the file
  string line, s_charge, s_mass;
  int charge, mass;
  bool check = false;
  // read the data
  if (inFile.is_open()) {
    while (!inFile.eof()) {
      getline(inFile, line);
      if (inFile.eof())
        break;
      s_mass = line.substr(0, 3);
      s_charge = line.substr(4, 4);
      charge = atoi(s_charge.data());
      mass = atoi(s_mass.data());
      if (mass == A && charge == Z * 10) {
        check = true;
        break;
      }
    }
    if (check)
      Extract(line.data());
  }
  else
    cout << "Unable to open file nuclear data base file " << FileName << endl;

  // close the file
  inFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Particle::~Particle() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TGraph* Particle::GetStableNuclei() {
  TGraph* gStableNuclei = new TGraph();
  ifstream inFile;
  string path1 = getenv("NPTOOL_HOME");
  string path2 = getenv("NPTOOL_ENV");
  string FileName = path1 + "/" + path2 + "/physics-essential/sources/nubtab16.asc";

  inFile.open(FileName.c_str());

  // reading the file
  string line, s_name;
  size_t space;
  int i_stable = 1;
  if (inFile.is_open()) {
    while (!inFile.eof()) {
      getline(inFile, line);

      if (line.size() > 72) {
        s_name = line.substr(11, 7);
        space = s_name.find_first_of(" ");
        s_name.resize(space);

        Extract(line.data());
        if (fLifeTime == -1) {
          gStableNuclei->SetPoint(i_stable, fAtomicWeight - fCharge, fCharge);
          i_stable++;
        }
      }
    }
  }
  else
    cout << "Unable to open file nuclear data base file " << FileName << endl;

  return gStableNuclei;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::Extract(string line) {
  // name of the isotope
  fName = line.substr(11, 7);
  // charge and mass
  string s_mass = line.substr(0, 3);
  string s_charge = line.substr(4, 4);
  fAtomicWeight = atoi(s_mass.data());
  fCharge = atoi(s_charge.data());
  fCharge /= 10;

  // mass excess
  string s_excess = line.substr(18, 10);
  fMassExcess = atof(s_excess.data());

  // life time
  string s_lt_units = line.substr(69, 3);
  string s_LifeTime = line.substr(57, 12);
  string s_LifeTimeErr = line.substr(72, 7);
  // Remove star
  replace(s_LifeTime.begin(), s_LifeTime.end(), '*', ' ');
  // Remove <
  replace(s_LifeTime.begin(), s_LifeTime.end(), '<', ' ');
  // Remove >
  replace(s_LifeTime.begin(), s_LifeTime.end(), '>', ' ');
  // Remove &
  replace(s_LifeTime.begin(), s_LifeTime.end(), '&', ' ');
  // Remove #
  replace(s_LifeTime.begin(), s_LifeTime.end(), '#', ' ');

  // Remove space
  s_LifeTime.erase(std::remove_if(s_LifeTime.begin(), s_LifeTime.end(), ::isspace), s_LifeTime.end());
  s_LifeTimeErr.erase(std::remove_if(s_LifeTimeErr.begin(), s_LifeTimeErr.end(), ::isspace), s_LifeTimeErr.end());
  s_lt_units.erase(std::remove_if(s_lt_units.begin(), s_lt_units.end(), ::isspace), s_lt_units.end());

  if (s_LifeTime == "stbl") {
    fLifeTime = -1;
    fLifeTimeErr = -1;
  }
  else {
    fLifeTime = atof(s_LifeTime.data());
    fLifeTimeErr = atof(s_LifeTimeErr.data());
  }
  if (s_lt_units == "ms") {
    fLifeTime *= 1e-3;
    fLifeTimeErr *= 1e-3;
  }
  else if (s_lt_units == "us") {
    fLifeTime *= 1e-6;
    fLifeTimeErr *= 1e-6;
  }
  else if (s_lt_units == "ns") {
    fLifeTime *= 1e-9;
    fLifeTimeErr *= 1e-9;
  }
  else if (s_lt_units == "ps") {
    fLifeTime *= 1e-12;
    fLifeTimeErr *= 1e-12;
  }
  else if (s_lt_units == "fs") {
    fLifeTime *= 1e-15;
    fLifeTimeErr *= 1e-15;
  }
  else if (s_lt_units == "as") {
    fLifeTime *= 1e-18;
    fLifeTimeErr *= 1e-18;
  }
  else if (s_lt_units == "zs") {
    fLifeTime *= 1e-21;
    fLifeTimeErr *= 1e-21;
  }
  else if (s_lt_units == "ys") {
    fLifeTime *= 1e-24;
    fLifeTimeErr *= 1e-24;
  }
  else if (s_lt_units == "m") {
    fLifeTime *= 60;
    fLifeTimeErr *= 60;
  }
  else if (s_lt_units == "h") {
    fLifeTime *= 3600;
    fLifeTimeErr *= 3600;
  }
  else if (s_lt_units == "d") {
    fLifeTime *= 3600 * 24;
    fLifeTimeErr *= 3600 * 24;
  }
  else if (s_lt_units == "y") {
    fLifeTime *= 3600 * 24 * 365.25;
    fLifeTimeErr *= 3600 * 24 * 365.25;
  }
  else if (s_lt_units == "ky") {
    fLifeTime *= 3600 * 24 * 365.25 * 1e3;
    fLifeTimeErr *= 3600 * 24 * 365.25 * 1e3;
  }
  else if (s_lt_units == "My") {
    fLifeTime *= 3600 * 24 * 365.25 * 1e6;
    fLifeTimeErr *= 3600 * 24 * 365.25 * 1e6;
  }
  else if (s_lt_units == "Gy") {
    fLifeTime *= 3600 * 24 * 365.25 * 1e9;
    fLifeTimeErr *= 3600 * 24 * 365.25 * 1e6;
  }
  else if (s_lt_units == "Py") {
    fLifeTime *= 3600 * 24 * 365.25 * 1e12;
    fLifeTimeErr *= 3600 * 24 * 365.25 * 1e12;
  }

  // spin and parity
  string s_spinparity = line.substr(79, 14);
  fSpinParity = s_spinparity.data();
  size_t found_p = s_spinparity.find("+");
  size_t found_m = s_spinparity.find("-");
  // parity
  if (found_p != string::npos)
    fParity = "+";
  if (found_m != string::npos)
    fParity = "-";
  // spin
  if (s_spinparity.find("0") != string::npos)
    fSpin = 0;
  if (s_spinparity.find("1") != string::npos)
    fSpin = 1;
  if (s_spinparity.find("2") != string::npos)
    fSpin = 2;
  if (s_spinparity.find("3") != string::npos)
    fSpin = 3;
  if (s_spinparity.find("4") != string::npos)
    fSpin = 4;
  if (s_spinparity.find("5") != string::npos)
    fSpin = 5;
  if (s_spinparity.find("6") != string::npos)
    fSpin = 6;
  if (s_spinparity.find("7") != string::npos)
    fSpin = 7;
  if (s_spinparity.find("8") != string::npos)
    fSpin = 8;
  if (s_spinparity.find("9") != string::npos)
    fSpin = 9;
  if (s_spinparity.find("10") != string::npos)
    fSpin = 10;
  if (s_spinparity.find("1/2") != string::npos)
    fSpin = 0.5;
  if (s_spinparity.find("3/2") != string::npos)
    fSpin = 1.5;
  if (s_spinparity.find("5/2") != string::npos)
    fSpin = 2.5;
  if (s_spinparity.find("7/2") != string::npos)
    fSpin = 3.5;
  if (s_spinparity.find("9/2") != string::npos)
    fSpin = 4.5;
  if (s_spinparity.find("11/2") != string::npos)
    fSpin = 5.5;
  if (s_spinparity.find("13/2") != string::npos)
    fSpin = 6.5;
  if (s_spinparity.find("15/2") != string::npos)
    fSpin = 7.5;
  if (s_spinparity.find("17/2") != string::npos)
    fSpin = 8.5;
  if (s_spinparity.find("19/2") != string::npos)
    fSpin = 9.5;
  if (s_spinparity.find("21/2") != string::npos)
    fSpin = 10.5;
  GetParticleName();
  fMass = Mass();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::Print() const {
  //------------ Imprime a l'ecran les caracteristiques d'un noyau -------
  cout << endl;
  cout << "Isotope: " << fName << endl;
  cout << "Z = " << fCharge << "   A = " << fAtomicWeight << endl;
  cout << "Mass Excess: " << fMassExcess << " keV" << endl;
  cout << "Jpi = " << fSpinParity << " (" << fSpin << fParity << ")" << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::GetParticleName() { fParticleName.assign(fName); }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::EnergyToBrho(double Q) {
  if (Q == -1000)
    Q = GetZ();

  EnergyToBeta();
  BetaToGamma();
  // fBrho = sqrt(pow(fKineticEnergy,2) + 2*fKineticEnergy*Mass()) * 1e6 * e_SI
  // / (c_light*1e6) / (Q * e_SI);
  fBrho = 3.107 * GetA() / Q * fBeta * fGamma;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::EnergyToTof() {
  fTimeOfFlight =
      1 / sqrt(1 - (Mass() * Mass()) / (fKineticEnergy + Mass()) / (fKineticEnergy + Mass())) / (c_light * 1e6);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::TofToEnergy() {
  double Energy = sqrt(Mass() * Mass() / (1 - pow((1 / ((c_light * 1e6) * fTimeOfFlight)), 2)));
  fKineticEnergy = Energy - Mass();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::BrhoToEnergy(double Q) {
  if (Q == -1000)
    Q = GetZ();

  fKineticEnergy = sqrt(pow((fBrho * (c_light * 1e6) * Q * e_SI) / (1e6 * e_SI), 2) + pow(Mass(), 2)) - Mass();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::EnergyToBeta() {
  fBeta = sqrt(pow(fKineticEnergy, 2) + 2 * fKineticEnergy * Mass()) / (fKineticEnergy + Mass());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::BetaToEnergy() { fKineticEnergy = Mass() / sqrt(1 - pow(fBeta, 2)) - Mass(); }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::BetaToGamma() { fGamma = 1 / sqrt(1 - pow(fBeta, 2)); }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::BetaToVelocity() { fVelocity = c_light * fBeta; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::DopplerCorrection(double EnergyLabGamma, double ThetaLabGamma) {
  double EnergyGammaCorrected = EnergyLabGamma * (1 - fBeta * cos(ThetaLabGamma)) / (sqrt(1 - pow(fBeta, 2)));

  return EnergyGammaCorrected;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetEnergyCM(double EnergyLab, double ThetaLab, double PhiLab, double relativisticboost) {
  SetKineticEnergy(EnergyLab);
  double EnergyCM;
  double ImpulsionLab;
  double ImpulsionLabX;
  double ImpulsionLabY;
  double ImpulsionLabZ;
  TVector3 VImpulsionLAB;
  TLorentzVector LVEnergyImpulsionLAB;
  TLorentzVector LVEnergyImpulsionCM;

  ImpulsionLab = sqrt(EnergyLab * EnergyLab + 2 * EnergyLab * Mass());
  ImpulsionLabX = ImpulsionLab * sin(ThetaLab) * cos(PhiLab);
  ImpulsionLabY = ImpulsionLab * sin(ThetaLab) * sin(PhiLab);
  ImpulsionLabZ = ImpulsionLab * cos(ThetaLab);

  VImpulsionLAB = TVector3(ImpulsionLabX, ImpulsionLabY, ImpulsionLabZ);
  LVEnergyImpulsionLAB = TLorentzVector(VImpulsionLAB, Mass() + EnergyLab);

  LVEnergyImpulsionCM = LVEnergyImpulsionLAB;
  LVEnergyImpulsionCM.Boost(0, 0, -relativisticboost);

  EnergyCM = LVEnergyImpulsionCM.Energy() - Mass();

  return EnergyCM;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetThetaCM(double EnergyLab, double ThetaLab, double PhiLab, double relativisticboost) {
  SetKineticEnergy(EnergyLab);
  double EnergyCM;
  double ThetaCM;
  double ImpulsionLab;
  double ImpulsionLabX;
  double ImpulsionLabY;
  double ImpulsionLabZ;
  TVector3 VImpulsionLAB;
  TLorentzVector LVEnergyImpulsionLAB;
  TLorentzVector LVEnergyImpulsionCM;

  ImpulsionLab = sqrt(EnergyLab * EnergyLab + 2 * EnergyLab * Mass());
  ImpulsionLabX = ImpulsionLab * sin(ThetaLab) * cos(PhiLab);
  ImpulsionLabY = ImpulsionLab * sin(ThetaLab) * sin(PhiLab);
  ImpulsionLabZ = ImpulsionLab * cos(ThetaLab);

  VImpulsionLAB = TVector3(ImpulsionLabX, ImpulsionLabY, ImpulsionLabZ);
  LVEnergyImpulsionLAB = TLorentzVector(VImpulsionLAB, Mass() + EnergyLab);

  LVEnergyImpulsionCM = LVEnergyImpulsionLAB;
  LVEnergyImpulsionCM.Boost(0, 0, -relativisticboost);

  EnergyCM = LVEnergyImpulsionCM.Energy() - Mass();
  ThetaCM = LVEnergyImpulsionCM.Theta();

  return ThetaCM;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::DefineMassByThreshold(const vector<string>& v) {
  // Define the mass as the sum of the mass of the particle named in v
  unsigned int size = v.size();
  vector<nptool::Particle> N;
  for (unsigned int i = 0; i < size; i++)
    N.push_back(nptool::Particle(v[i]));

  DefineMassByThreshold(N);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::DefineMassByThreshold(const vector<nptool::Particle>& N) {
  // Define the mass as the sum of the mass of the particle defined in N
  unsigned int size = N.size();
  double Mass = 0;
  unsigned int A = 0;
  unsigned int Z = 0;
  for (unsigned int i = 0; i < size; i++) {
    Mass += N[i].Mass();
    A += N[i].GetA();
    Z += N[i].GetZ();
  }
  // Check the threshold make any sense (same A, same Z):
  if (A != GetA() || Z != GetZ()) {
    /*nptool::SendWarning("nptool::Particle",
                        "Mass and charge is not conserved in "
                        "DefineMassByThreshold! Doing no change to particle");
    */
    return;
  }

  SetExcitationEnergy(0);
  SetMassExcess(1000 * (Mass - fAtomicWeight * amu_c2 + fCharge * electron_mass_c2));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetSXn(unsigned int X) const {
  Particle N(GetZ(), GetA() - X);
  return N.Mass() + X * neutron_mass_c2 - Mass();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetSXp(unsigned int X) const {
  Particle N(GetZ() - X, GetA() - X);
  return N.Mass() + X * proton_mass_c2 - Mass();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetSn() const { return GetSXn(1); }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetSp() const { return GetSXp(1); }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetS2n() const { return GetSXn(2); }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetS2p() const { return GetSXp(2); }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetSt() const {
  Particle N(GetZ() - 1, GetA() - 3);
  Particle A(1, 3);
  return N.Mass() + A.Mass() - Mass();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetS3He() const {
  Particle N(GetZ() - 2, GetA() - 3);
  Particle A(2, 3);
  return N.Mass() + A.Mass() - Mass();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double Particle::GetSa() const {
  Particle N(GetZ() - 2, GetA() - 4);
  Particle A(2, 4);
  return N.Mass() + A.Mass() - Mass();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Particle::PrintThreshold() const {
  cout << GetName() << " thresholds : " << endl;
  cout << "  Sn   : " << GetSn() << " MeV" << endl;
  cout << "  Sp   : " << GetSp() << " MeV" << endl;
  cout << "  S2n  : " << GetS2n() << " MeV" << endl;
  cout << "  S2p  : " << GetS2p() << " MeV" << endl;
  cout << "  St   : " << GetSt() << " MeV" << endl;
  cout << "  S3He : " << GetS3He() << " MeV" << endl;
  cout << "  Sa   : " << GetSa() << " MeV" << endl;
}
////////////////////////////////////////////////////////////////////////////////
std::string nptool::ChangeNameToG4Standard(std::string OriginalName, bool excited) {
  std::string NumberOfMass;
  std::string Nucleid;

  for (unsigned int i = 0; i < OriginalName.length(); i++) {
    std::ostringstream character;
    character << OriginalName[i];
    if (character.str() == "0")
      NumberOfMass += "0";
    else if (character.str() == "1")
      NumberOfMass += "1";
    else if (character.str() == "2")
      NumberOfMass += "2";
    else if (character.str() == "3")
      NumberOfMass += "3";
    else if (character.str() == "4")
      NumberOfMass += "4";
    else if (character.str() == "5")
      NumberOfMass += "5";
    else if (character.str() == "6")
      NumberOfMass += "6";
    else if (character.str() == "7")
      NumberOfMass += "7";
    else if (character.str() == "8")
      NumberOfMass += "8";
    else if (character.str() == "9")
      NumberOfMass += "9";

    else if (character.str() == "A")
      Nucleid += "A";
    else if (character.str() == "B")
      Nucleid += "B";
    else if (character.str() == "C")
      Nucleid += "C";
    else if (character.str() == "D")
      Nucleid += "D";
    else if (character.str() == "E")
      Nucleid += "E";
    else if (character.str() == "F")
      Nucleid += "F";
    else if (character.str() == "G")
      Nucleid += "G";
    else if (character.str() == "H")
      Nucleid += "H";
    else if (character.str() == "I")
      Nucleid += "I";
    else if (character.str() == "J")
      Nucleid += "J";
    else if (character.str() == "K")
      Nucleid += "K";
    else if (character.str() == "L")
      Nucleid += "L";
    else if (character.str() == "M")
      Nucleid += "M";
    else if (character.str() == "N")
      Nucleid += "N";
    else if (character.str() == "O")
      Nucleid += "O";
    else if (character.str() == "P")
      Nucleid += "P";
    else if (character.str() == "Q")
      Nucleid += "Q";
    else if (character.str() == "R")
      Nucleid += "R";
    else if (character.str() == "S")
      Nucleid += "S";
    else if (character.str() == "T")
      Nucleid += "T";
    else if (character.str() == "U")
      Nucleid += "U";
    else if (character.str() == "V")
      Nucleid += "V";
    else if (character.str() == "W")
      Nucleid += "W";
    else if (character.str() == "X")
      Nucleid += "X";
    else if (character.str() == "Y")
      Nucleid += "Y";
    else if (character.str() == "Z")
      Nucleid += "Z";

    else if (character.str() == "a")
      Nucleid += "a";
    else if (character.str() == "b")
      Nucleid += "b";
    else if (character.str() == "c")
      Nucleid += "c";
    else if (character.str() == "d")
      Nucleid += "d";
    else if (character.str() == "e")
      Nucleid += "e";
    else if (character.str() == "f")
      Nucleid += "f";
    else if (character.str() == "g")
      Nucleid += "g";
    else if (character.str() == "h")
      Nucleid += "h";
    else if (character.str() == "i")
      Nucleid += "i";
    else if (character.str() == "j")
      Nucleid += "j";
    else if (character.str() == "k")
      Nucleid += "k";
    else if (character.str() == "l")
      Nucleid += "l";
    else if (character.str() == "m")
      Nucleid += "m";
    else if (character.str() == "n")
      Nucleid += "n";
    else if (character.str() == "o")
      Nucleid += "o";
    else if (character.str() == "p")
      Nucleid += "p";
    else if (character.str() == "q")
      Nucleid += "q";
    else if (character.str() == "r")
      Nucleid += "r";
    else if (character.str() == "s")
      Nucleid += "s";
    else if (character.str() == "t")
      Nucleid += "t";
    else if (character.str() == "u")
      Nucleid += "u";
    else if (character.str() == "v")
      Nucleid += "v";
    else if (character.str() == "w")
      Nucleid += "w";
    else if (character.str() == "x")
      Nucleid += "x";
    else if (character.str() == "y")
      Nucleid += "y";
    else if (character.str() == "z")
      Nucleid += "z";
  }

  // Special case for light particles
  std::string FinalName = Nucleid + NumberOfMass;

  if (!excited) {
    if (FinalName == "H1")
      FinalName = "proton";
    else if (FinalName == "H2")
      FinalName = "deuteron";
    else if (FinalName == "H3")
      FinalName = "triton";
    else if (FinalName == "He4")
      FinalName = "alpha";
    else if (FinalName == "p")
      FinalName = "proton";
    else if (FinalName == "d")
      FinalName = "deuteron";
    else if (FinalName == "t")
      FinalName = "triton";
    else if (FinalName == "a")
      FinalName = "alpha";
    else if (FinalName == "n")
      FinalName = "neutron";
  }
  return (FinalName);
}
//////////////////////////////////////////////////////////////////////////////
std::string nptool::ChangeNameFromG4Standard(std::string OriginalName) {
  std::string NumberOfMass;
  std::string Nucleid;

  bool g4_exc = false;
  for (unsigned int i = 0; i < OriginalName.length(); i++) {
    std::ostringstream character;
    character << OriginalName[i];
    // std::cout << OriginalName << " "<<  OriginalName[i]<< std::endl;

    // To remove Nuclei excitation from the name
    if (character.str() == "[") {
      g4_exc = true;
    }
    if (character.str() == "]") {
      g4_exc = false;
    }

    if (g4_exc == true) {
      continue;
    }

    if (character.str() == "0")
      NumberOfMass += "0";
    else if (character.str() == "1")
      NumberOfMass += "1";
    else if (character.str() == "2")
      NumberOfMass += "2";
    else if (character.str() == "3")
      NumberOfMass += "3";
    else if (character.str() == "4")
      NumberOfMass += "4";
    else if (character.str() == "5")
      NumberOfMass += "5";
    else if (character.str() == "6")
      NumberOfMass += "6";
    else if (character.str() == "7")
      NumberOfMass += "7";
    else if (character.str() == "8")
      NumberOfMass += "8";
    else if (character.str() == "9")
      NumberOfMass += "9";

    else if (character.str() == "A")
      Nucleid += "A";
    else if (character.str() == "B")
      Nucleid += "B";
    else if (character.str() == "C")
      Nucleid += "C";
    else if (character.str() == "D")
      Nucleid += "D";
    else if (character.str() == "E")
      Nucleid += "E";
    else if (character.str() == "F")
      Nucleid += "F";
    else if (character.str() == "G")
      Nucleid += "G";
    else if (character.str() == "H")
      Nucleid += "H";
    else if (character.str() == "I")
      Nucleid += "I";
    else if (character.str() == "J")
      Nucleid += "J";
    else if (character.str() == "K")
      Nucleid += "K";
    else if (character.str() == "L")
      Nucleid += "L";
    else if (character.str() == "M")
      Nucleid += "M";
    else if (character.str() == "N")
      Nucleid += "N";
    else if (character.str() == "O")
      Nucleid += "O";
    else if (character.str() == "P")
      Nucleid += "P";
    else if (character.str() == "Q")
      Nucleid += "Q";
    else if (character.str() == "R")
      Nucleid += "R";
    else if (character.str() == "S")
      Nucleid += "S";
    else if (character.str() == "T")
      Nucleid += "T";
    else if (character.str() == "U")
      Nucleid += "U";
    else if (character.str() == "V")
      Nucleid += "V";
    else if (character.str() == "W")
      Nucleid += "W";
    else if (character.str() == "X")
      Nucleid += "X";
    else if (character.str() == "Y")
      Nucleid += "Y";
    else if (character.str() == "Z")
      Nucleid += "Z";

    else if (character.str() == "a")
      Nucleid += "a";
    else if (character.str() == "b")
      Nucleid += "b";
    else if (character.str() == "c")
      Nucleid += "c";
    else if (character.str() == "d")
      Nucleid += "d";
    else if (character.str() == "e")
      Nucleid += "e";
    else if (character.str() == "f")
      Nucleid += "f";
    else if (character.str() == "g")
      Nucleid += "g";
    else if (character.str() == "h")
      Nucleid += "h";
    else if (character.str() == "i")
      Nucleid += "i";
    else if (character.str() == "j")
      Nucleid += "j";
    else if (character.str() == "k")
      Nucleid += "k";
    else if (character.str() == "l")
      Nucleid += "l";
    else if (character.str() == "m")
      Nucleid += "m";
    else if (character.str() == "n")
      Nucleid += "n";
    else if (character.str() == "o")
      Nucleid += "o";
    else if (character.str() == "p")
      Nucleid += "p";
    else if (character.str() == "q")
      Nucleid += "q";
    else if (character.str() == "r")
      Nucleid += "r";
    else if (character.str() == "s")
      Nucleid += "s";
    else if (character.str() == "t")
      Nucleid += "t";
    else if (character.str() == "u")
      Nucleid += "u";
    else if (character.str() == "v")
      Nucleid += "v";
    else if (character.str() == "w")
      Nucleid += "w";
    else if (character.str() == "x")
      Nucleid += "x";
    else if (character.str() == "y")
      Nucleid += "y";
    else if (character.str() == "z")
      Nucleid += "z";
  }

  std::string FinalName = NumberOfMass + Nucleid;
  // Special case for light particles
  if (FinalName == "H1")
    FinalName = "proton";
  else if (FinalName == "H2")
    FinalName = "deuteron";
  else if (FinalName == "H3")
    FinalName = "triton";
  else if (FinalName == "He4")
    FinalName = "alpha";
  else if (FinalName == "p")
    FinalName = "proton";
  else if (FinalName == "d")
    FinalName = "deuteron";
  else if (FinalName == "t")
    FinalName = "triton";
  else if (FinalName == "a")
    FinalName = "alpha";
  else if (FinalName == "n")
    FinalName = "neutron";
  return (FinalName);
}

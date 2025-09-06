#ifndef NPParticle_h
#define NPParticle_h

// ROOT headers
#include "TGraph.h"
#include "TLorentzVector.h"

// NPTOOL headers
#include "NPPhysicalConstants.h"
#include "NPSystemOfUnits.h"

// C++ headers
#include <iostream>
#include <string>
#include <vector>
namespace nptool {

  class Particle {
   public:
    Particle();
    Particle(std::string isotope);
    Particle(std::string isotope, const std::string& pathENSDF);
    Particle(int Z, int A);
    Particle(std::string name, std::vector<std::string> subpart, double binding, double Ex = 0,
             std::string SpinParity = "", double Spin = 0, std::string Parity = "", double LifeTime = -1);
    virtual ~Particle();

   public:
    void SetUp(std::string isotope);
    void LoadENSDF(const std::string& isotope, const std::string& pathENSDF);

   private:
    // intrinsic properties
    std::string fName; // Particle name
    std::string fParticleName;
    int fCharge;        // Particle charge
    int fAtomicWeight;  // Particle atomic weight
    double fMassExcess; // Particle mass excess in keV
    double fMass;
    std::string fSpinParity;  // Particle spin and parity
    double fSpin;             // Particle spin
    std::string fParity;      // Particle parity
    double fLifeTime;         // life time
    double fLifeTimeErr;      // life time error
    double fExcitationEnergy; // Excitation Energy
    // kinematic properties
    double fKineticEnergy;
    double fBeta;
    double fGamma;
    double fBrho;
    double fTimeOfFlight;
    double fVelocity;
    TLorentzVector fEnergyImpulsion;
    // ENSDF list of levels
    std::vector<double> fLevelEnergy;            // list of level energies
    std::vector<double> fLevelEnergyUncertainty; // list of level energies uncertainty

   public:
    void EnergyToBrho(double Q = -1000);
    void EnergyToTof();
    void BetaToVelocity();
    void BrhoToEnergy(double Q = -1000);
    void BrhoToTof() {
      BrhoToEnergy();
      EnergyToTof();
    }
    void TofToEnergy();
    void TofToBrho() {
      TofToEnergy();
      EnergyToBrho();
    }
    void EnergyToBeta();
    void BetaToEnergy();
    void BetaToGamma();
    double DopplerCorrection(double EnergyLabGamma, double ThetaLabGamma);

   protected:
    void Extract(std::string line);

   public:
    void GetParticleName();
    std::string GetName() const { return fName; }
    int GetZ() const { return fCharge; }
    int GetA() const { return fAtomicWeight; }
    double GetMassExcess() const { return fMassExcess; }
    std::string GetSpinParity() const { return fSpinParity; }
    double GetSpin() const { return fSpin; }
    std::string GetParity() const { return fParity; }
    double GetLifeTime() const { return fLifeTime; }
    double GetLifeTimeError() const { return fLifeTimeErr; }
    double GetEnergy() const { return fKineticEnergy; }
    double GetBrho() const { return fBrho; }
    double GetTimeOfFlight() const { return fTimeOfFlight; }
    double GetBeta() const { return fBeta; }
    double GetGamma() const { return fGamma; }
    double GetVelocity() const { return fVelocity; }
    TLorentzVector GetEnergyImpulsion() const { return fEnergyImpulsion; }
    TVector3 GetImpulsion() const { return fEnergyImpulsion.Vect(); }
    double GetExcitationEnergy() const { return fExcitationEnergy; }
    void SetName(const char* name) { fName = name; }
    void SetZ(int charge) { fCharge = charge; }
    void SetA(int mass) { fAtomicWeight = mass; }
    void SetMassExcess(double massexcess) { fMassExcess = massexcess; }
    void SetSpinParity(const char* spinparity) { fSpinParity = spinparity; }
    void SetSpin(double spin) { fSpin = spin; }
    void SetParity(const char* parity) { fParity = parity; }
    void SetLifeTime(double LifeTime) { fLifeTime = LifeTime; }
    void SetLifeTimeError(double LifeTimeErr) { fLifeTimeErr = LifeTimeErr; }
    void SetKineticEnergy(double energy) {
      fKineticEnergy = energy;
      EnergyToBrho();
      EnergyToTof();
      EnergyToBeta();
      BetaToGamma();
      BetaToVelocity();
    }
    void SetBrho(double brho) {
      fBrho = brho;
      BrhoToEnergy();
      BrhoToTof();
      EnergyToBeta();
      BetaToGamma();
      BetaToVelocity();
    }
    void SetTimeOfFlight(double tof) {
      fTimeOfFlight = tof;
      TofToEnergy();
      TofToBrho();
      EnergyToBeta();
      BetaToGamma();
      BetaToVelocity();
    }
    void SetEnergyImpulsion(TLorentzVector P) {
      fEnergyImpulsion = P;
      fKineticEnergy = P.E() - Mass();
      EnergyToBrho();
      EnergyToTof();
      EnergyToBeta();
      BetaToGamma();
      BetaToVelocity();
    }

    void SetExcitationEnergy(double Ex) { fExcitationEnergy = Ex; }
    void SetBeta(double beta) {
      fBeta = beta;
      BetaToGamma();
      BetaToEnergy();
      EnergyToTof();
      EnergyToBrho();
      BetaToVelocity();
    }
    double GetEnergyCM(double EnergyLab, double ThetaLab, double PhiLab, double relativisticboost);
    double GetThetaCM(double EnergyLab, double ThetaLab, double PhiLab, double relativisticboost);

    // Nuclear mass in MeV
    double Mass() const {
      return (fAtomicWeight * amu_c2 + fMassExcess / 1000. - fCharge * electron_mass_c2 + fExcitationEnergy);
    }
    double GetBindingEnergy() const {
      return (fCharge * proton_mass_c2 + (fAtomicWeight - fCharge) * neutron_mass_c2 + fCharge * electron_mass_c2 -
              fAtomicWeight * amu_c2 - fMassExcess / 1000);
    }
    void Print() const;

   public:
    void DefineMassByThreshold(const std::vector<std::string>& v);      // Define the mass as the sum of the mass of
                                                                        // the particle named in v
    void DefineMassByThreshold(const std::vector<nptool::Particle>& N); // Define the mass as the sum of the
                                                                        // mass of the particle defined in N

   public:
    TGraph* GetStableNuclei();
    double GetSn() const;
    double GetSp() const;
    double GetS2n() const;
    double GetS2p() const;
    double GetSt() const;
    double GetS3He() const;
    double GetSa() const;
    double GetSXn(unsigned int X) const;
    double GetSXp(unsigned int X) const;
    void PrintThreshold() const;

    // methods for ENSDF
   public:
    unsigned int GetNumberOfLevels() { return fLevelEnergy.size(); }
    std::vector<double> GetLevelEnergyList() { return fLevelEnergy; }
    std::vector<double> GetLevelEnergyUncertaintyList() { return fLevelEnergyUncertainty; }
    double GetLevelEnergy(Int_t i) { return fLevelEnergy[i]; }
    double GetLevelEnergyUncertainty(Int_t i) { return fLevelEnergyUncertainty[i]; }

    ClassDef(Particle, 0)
  };
  //////////////////////////////////////////////////////////////////////////////
  std::string ChangeNameToG4Standard(std::string OriginalName, bool excited);
  //////////////////////////////////////////////////////////////////////////////
  std::string ChangeNameFromG4Standard(std::string OriginalName);
} // namespace nptool
#endif

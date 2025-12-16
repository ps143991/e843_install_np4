#ifndef __INTERACTIONCOORDINATES__
#define __INTERACTIONCOORDINATES__
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    : 01/09/2021 Valerian Alcindor adding particle name to     *
 *                  interaction coordinate for easier g4 analysis simulation *
 *---------------------------------------------------------------------------*
 * Decription: This class mainly records the coordinates of interaction      *
 *             between a particle and a detector.                            *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "NPFunction.h"
#include "TObject.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class InteractionCoordinates : public TObject {
private:
  // TrackID or index for correlations
  vector<int> fDetected_Index;
  // Detected particle properties (before interactions in the target)
  // Energy and Time
  vector<double> fDetected_Energy;
  vector<double> fDetected_Time;
  // Vertex of interaction
  vector<double> fDetected_Position_X;
  vector<double> fDetected_Position_Y;
  vector<double> fDetected_Position_Z;
  // Particle angles
  vector<double> fDetected_Angle_Theta;
  vector<double> fDetected_Angle_Phi;
  // Particle characteristics
  vector<std::string> fDetected_Particle_Name;
  vector<int>         fDetected_A;
  vector<int>         fDetected_Z;
  vector<double>      fDetected_Mass;
  vector<int>         fDetected_Charge;
  vector<double>      fDetected_Brho;
  vector<double>      fDetected_KineticEnergy;

public:
  InteractionCoordinates();
  virtual ~InteractionCoordinates();

  void Clear();
  void Clear(const Option_t*) {};
  void Dump() const;

  void SetInteraction(const double& Energy, const double& Time,
                      const double& PositionX, const double& PositionY,
                      const double& PositionZ, const double& Theta,
                      const double& Phi) {
    fDetected_Energy.push_back(Energy);
    fDetected_Time.push_back(Time);
    fDetected_Position_X.push_back(PositionX);
    fDetected_Position_Y.push_back(PositionY);
    fDetected_Position_Z.push_back(PositionZ);
    fDetected_Angle_Theta.push_back(Theta);
    fDetected_Angle_Phi.push_back(Phi);
  }

  void SetInteraction(const int& Index, const double& Energy,
                      const double& Time, const double& PositionX,
                      const double& PositionY, const double& PositionZ,
                      const double& Theta, const double& Phi) {
    fDetected_Index.push_back(Index);
    fDetected_Energy.push_back(Energy);
    fDetected_Time.push_back(Time);
    fDetected_Position_X.push_back(PositionX);
    fDetected_Position_Y.push_back(PositionY);
    fDetected_Position_Z.push_back(PositionZ);
    fDetected_Angle_Theta.push_back(Theta);
    fDetected_Angle_Phi.push_back(Phi);
  }

  void SetInteraction(const int& Index, const double& Energy,
                      const double& Time, const double& PositionX,
                      const double& PositionY, const double& PositionZ,
                      const double& Theta, const double& Phi,
                      const std::string& ParticleName, const int& A,
                      const int& Z, const double& Mass, const int& Charge,
                      const double& Brho, const double& KineticEnergy) {
    fDetected_Index.push_back(Index);
    fDetected_Energy.push_back(Energy);
    fDetected_Time.push_back(Time);
    fDetected_Position_X.push_back(PositionX);
    fDetected_Position_Y.push_back(PositionY);
    fDetected_Position_Z.push_back(PositionZ);
    fDetected_Angle_Theta.push_back(Theta);
    fDetected_Angle_Phi.push_back(Phi);
    if (ParticleName != "e-" && ParticleName != "e+")
      fDetected_Particle_Name.push_back(ChangeNameFromG4Standard(ParticleName));
    else
      fDetected_Particle_Name.push_back(ParticleName);
    fDetected_A.push_back(A);
    fDetected_Z.push_back(Z);
    fDetected_Mass.push_back(Mass);
    fDetected_Charge.push_back(Charge);
    fDetected_Brho.push_back(Brho);
    fDetected_KineticEnergy.push_back(KineticEnergy);
  }

  /////////////////////           SETTERS           ////////////////////////
  // Incident particle properties (before interactions in the target)
  // Vertex of interaction
  void SetDetectedPositionX(const double& PositionX) {
    fDetected_Position_X.push_back(PositionX);
  } //!
  void SetDetectedPositionY(const double& PositionY) {
    fDetected_Position_Y.push_back(PositionY);
  } //!
  void SetDetectedPositionZ(const double& PositionZ) {
    fDetected_Position_Z.push_back(PositionZ);
  } //!
  // Incident particle angles
  void SetDetectedAngleTheta(const double& AngleTheta) {
    fDetected_Angle_Theta.push_back(AngleTheta);
  } //!
  void SetDetectedAnglePhi(const double& AnglePhi) {
    fDetected_Angle_Phi.push_back(AnglePhi);
  } //!

  void SetDetectedParticleName(const std::string& ParticleName) {
    fDetected_Particle_Name.push_back(ParticleName);
  } //!
  void SetDetectedA(const int& A) { fDetected_A.push_back(A); } //!
  void SetDetectedZ(const int& Z) { fDetected_Z.push_back(Z); } //!
  void SetDetectedMass(const double& Mass) {
    fDetected_Mass.push_back(Mass);
  } //!
  void SetDetectedCharge(const int& Charge) {
    fDetected_Charge.push_back(Charge);
  } //!
  void SetDetectedBrho(const double& Brho) {
    fDetected_Brho.push_back(Brho);
  } //!
  void SetDetectedKineticEnergy(const double& KineticEnergy) {
    fDetected_KineticEnergy.push_back(KineticEnergy);
  } //!

  /////////////////////           GETTERS           ////////////////////////
  // Number of interactions (multiplicity)
  int GetDetectedMultiplicity() const { return fDetected_Position_X.size(); }
  // Incident particle properties (before interactions in the target)
  // Energy and Time
  double GetEnergy(const int& i) const { return fDetected_Energy[i]; } //!
  double GetTime(const int& i) const { return fDetected_Time[i]; } //!
  // Vertex of interaction
  double GetDetectedPositionX(const int& i) const {
    return fDetected_Position_X[i];
  } //!
  double GetDetectedPositionY(const int& i) const {
    return fDetected_Position_Y[i];
  } //!
  double GetDetectedPositionZ(const int& i) const {
    return fDetected_Position_Z[i];
  } //!
  // Incident particle angles
  double GetDetectedAngleTheta(const int& i) const {
    return fDetected_Angle_Theta[i];
  } //!
  double GetDetectedAnglePhi(const int& i) const {
    return fDetected_Angle_Phi[i];
  } //!

  std::string GetParticleName(const int& i) const {
    return fDetected_Particle_Name[i];
  } //!

  int    GetA(const int& i) const { return fDetected_A[i]; } //!
  int    GetZ(const int& i) const { return fDetected_Z[i]; } //!
  double GetMass(const int& i) const { return fDetected_Mass[i]; } //!
  int    GetCharge(const int& i) const { return fDetected_Charge[i]; } //!
  double GetBrho(const int& i) const { return fDetected_Brho[i]; } //!
  double GetKineticEnergy(const int& i) const {
    return fDetected_KineticEnergy[i];
  } //!

  // Vertex of interaction
  vector<double> GetDetected_Position_X() { return fDetected_Position_X; };
  vector<double> GetDetected_Position_Y() { return fDetected_Position_Y; };
  vector<double> GetDetected_Position_Z() { return fDetected_Position_Z; };

  // TODO: Used to be in the NPLib Physics part... moved here for now, to see if
  // useful elsewhere... Should be moved to geant4 itself later on
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  string ChangeNameToG4Standard(string OriginalName, bool excited) {
    string NumberOfMass;
    string Nucleid;

    for (unsigned int i = 0; i < OriginalName.length(); i++) {
      ostringstream character;
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
    string FinalName = Nucleid + NumberOfMass;

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

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  string ChangeNameFromG4Standard(string OriginalName) {
    string NumberOfMass;
    string Nucleid;

    bool g4_exc = false;
    for (unsigned int i = 0; i < OriginalName.length(); i++) {
      ostringstream character;
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

    string FinalName = NumberOfMass + Nucleid;
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

  ClassDef(InteractionCoordinates, 2) // InteractionCoordinates structure
};

#endif

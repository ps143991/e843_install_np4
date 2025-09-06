#ifndef MUST2Geant4_h
#define MUST2Geant4_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This file describe the MUST2 charge particle Detector                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * MUST2 is a modular array made of Telescope (1 to 8 telescope). Each       *
 *  Telescope is made of Three Stage:                                        *
 *  - A 300um Silicium, double-sided strip                                   *
 *  - 16 Si(Li) pad                                                          *
 *  - 16 CsI scintillator Crystal                                            *
 *****************************************************************************/

// G4 headers
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"


// nptool
#include "NPInputParser.h"
#include "NPG4VDetector.h"

// must2
#include "Must2Data.h"
#include "Must2Detector.h"

// stl
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace must2 {
class MUST2Geant4 : public nptool::geant4::VDetector  {
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
 public:
  MUST2Geant4();
  virtual ~MUST2Geant4();

  ////////////////////////////////////////////////////
  //////// Specific Function of this Class ///////////
  ////////////////////////////////////////////////////
 public:
  // Effectively construct Volume
  // Avoid to have two time same code for Angle and Point definition
  void VolumeMaker(G4int TelescopeNumber, G4ThreeVector MMpos, G4RotationMatrix* MMrot, bool wSi, bool wSiLi, bool wCsI,
                   G4LogicalVolume* world);

  ////////////////////////////////////////////////////
  /////////  Inherite from nptool::geant4::VDetector class ///////////
  ////////////////////////////////////////////////////
 public:
  // // Read stream at Configfile to pick-up parameters of detector (Position,...)
  // // Called in DetecorConstruction::ReadDetextorConfiguration Method
  // void ReadConfiguration(NPL::InputParser);

  // Construct detector and inialise sensitive part.
  // Called After DetecorConstruction::AddDetector Method
  void ConstructDetector(/*G4LogicalVolume* world*/);

  // Add Detector branch to the EventTree.
  // Called After DetecorConstruction::AddDetector Method
  void InitializeRootOutput();

  // Read sensitive part and fill the Root tree.
  // Called at in the EventAction::EndOfEventAvtion
  void ReadSensitive(const G4Event* event);

  ////////////////////////////////////////////////////
  ///////////Event class to store Data////////////////
  ////////////////////////////////////////////////////
 private:
  Must2Data* m_Event;
  std::shared_ptr<must2::Must2Detector> m_detector;
  // std::shared_ptr<TMust2Data> m_Event;

  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
 private:
  // True if Define by Position, False is Define by angle
  std::vector<bool> m_DefinitionType;

  // Used for "By Point Definition"
  // std::vector<G4ThreeVector> m_X1_Y1;     // Top Left Corner Position Vector
  // std::vector<G4ThreeVector> m_X1_Y128;   // Bottom Left Corner Position Vector
  // std::vector<G4ThreeVector> m_X128_Y1;   // Bottom Right Corner Position Vector
  // std::vector<G4ThreeVector> m_X128_Y128; // Center Corner Position Vector

  // Used for "By Angle Definition"
  std::vector<G4double> m_R;     //  |
  std::vector<G4double> m_Theta; //  > Spherical coordinate of Strips Silicium Plate
  std::vector<G4double> m_Phi;   //  |

  std::vector<G4double> m_beta_u; //  |
  std::vector<G4double> m_beta_v; //  > Tilt angle of the Telescope
  std::vector<G4double> m_beta_w; //  |

  // If Set to true if you want this stage on you telescope
  std::vector<bool> m_wSi;    // Silicium Strip 300um 128*128 Strip
  std::vector<bool> m_wSiLi;  // Si(Li) 2*4 Pad
  std::vector<bool> m_wCsI;   // CsI 4*4 crystal
  std::vector<bool> m_wAddSi; // Additionnal Thin Silicium Strip

  // Set to true if you want to see Telescope Frame in your visualisation
  bool m_non_sensitive_part_visiualisation;

  std::map<int, bool> m_CsIOffset;

  ////////////////////////////////////////////////////
  ///////////////////// Scorer ///////////////////////
  ////////////////////////////////////////////////////
 private:
  //   Initialize all Scorer used by the MUST2Geant4
  void InitializeScorers();

  //   Silicon Associate Scorer
  G4MultiFunctionalDetector* m_StripScorer;

  //   SiLi Associate Scorer
  G4MultiFunctionalDetector* m_SiLiScorer;

  //   CsI Associate Scorer
  G4MultiFunctionalDetector* m_CsIScorer;

  ////////////////////////////////////////////////////
  //////////////////// Material //////////////////////
  ////////////////////////////////////////////////////
 private:
  //   Declare all material used by the MUST2Geant4
  void InitializeMaterial();
  // Si
  G4Material* m_MaterialSilicon;
  // Al
  G4Material* m_MaterialAluminium;
  // Iron
  G4Material* m_MaterialIron;
  // CsI
  G4Material* m_MaterialCsI;
  //  Vacuum
  G4Material* m_MaterialVacuum;
  //  Mylar
  G4Material* m_MaterialMyl;

 // public:
 //  static nptool::geant4::VDetector* Construct();
};
} // namespace MUST2

extern G4RotationMatrix* Rotation(double tetaX, double tetaY, double tetaZ);
#endif

#include "Analysis.h"
#include "NPApplication.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////
using namespace nptool;
void user_analysis::Analysis::SetDataOutput(std::shared_ptr<nptool::VDataOutput> output) {
  
  // ------------- Attach branches to the ROOT output -------------
  auto Tree = std::dynamic_pointer_cast<nptool::RootOutput>(output)->GetTree();

  // ------------ MUST2 ------------
  Tree->Branch("M2X", &M2X); // Position of the interaction in MUST2 X
  Tree->Branch("M2Y", &M2Y); // Position of the interaction in MUST2 Y
  Tree->Branch("M2Z", &M2Z); // Position of the interaction in MUST2 Z

  // ------------ CsI calibration ------------
  Tree->Branch("CsI_EReconstructed", &CsI_EReconstructed); // Energy reconstructed in CsI

  //   // ------------ Reaction kinematics ------------
  Tree->Branch("ELab", &ELab); // Energy in the lab frame
  Tree->Branch("ELab_corrected", &ELabCorrected); // Energy in the lab frame corrected
  Tree->Branch("ThetaLab", &ThetaLab); // Lab angle of the reaction
  Tree->Branch("ThetaCM_light", &ThetaCM_light); // Lab angle of the reaction
  Tree->Branch("ThetaCM_heavy", &ThetaCM_heavy); // Lab angle of the reaction

  // // ------------ Reaction reconstruction ------------
  Tree->Branch("Ex", &Ex); // Excitation energy
  Tree->Branch("ThetaParticle", &thetaParticle); // Angle of the particle
  Tree->Branch("ThetaRecoil", &thetaRecoil); // Angle of the recoil
  Tree->Branch("PhiRecoil", &phiRecoil); // Phi of the recoil
  Tree->Branch("BetaRecoil", &betaRecoil); // Beta of the recoil
  
  // ------------ EXOGAM ------------
  Tree->Branch("EDC", &EDC); // Doppler corrected energy in EXOGAM
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB // Tree->Branch("EDC_Method1", &EDC_Method1);
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB // Tree->Branch("EDC_Method2", &EDC_Method2);
 
  // ------------ CATS ------------
  // std::cout << "I ATTACH BRANCHES" << std::endl;
}

void user_analysis::Analysis::Init() {
  //std::cout << "test 1" << std::endl;
  auto app = nptool::Application::GetApplication();
  zdd = std::dynamic_pointer_cast<zdd::ZddDetector>(app->GetDetector("zdd"));
  //tac = std::dynamic_pointer_cast<tac::TacDetector>(app->GetDetector("tac"));
  exogam = std::dynamic_pointer_cast<exogam::ExogamDetector>(app->GetDetector("exogam"));
  //gatconf = std::dynamic_pointer_cast<generic_ebye_branch::GenericEbyEBranchDetector>(app->GetDetector("generic_ebye_branch"));
  must2 = std::dynamic_pointer_cast<must2::Must2Detector>(app->GetDetector("must2"));
  cats = std::dynamic_pointer_cast<cats::CatsDetector>(app->GetDetector("cats"));

  // Settings
  TargetThickness = 0.53*micrometer;
  //TargetThickness = 5*micrometer;

  // ------------ Reaction list ------------
  S36pdReaction = Reaction("36S(p,d)35S@1800MeV");    // Take into account the energy loss in the target before reaction
  Si34pdReaction = Reaction("34Si(p,d)33Si@1700MeV"); // Take into account the energy loss in the target before reaction
  Si34ptReaction = Reaction("34Si(p,t)32Si@1700MeV"); // Take into account the energy loss in the target before reaction

  // ------------ DEUTERON Energy Loss ------------
  d_Si = EnergyLoss("./energy_loss/deuteron_Si.G4table", "G4Table", 100); // Energy loss in Silicon (MUST2)
  d_Al = EnergyLoss("./energy_loss/deuteron_Al.G4table", "G4Table", 100); // Energy loss in Aluminium (MUST2)
  d_CH2 = EnergyLoss("./energy_loss/deuteron_CH2.G4table", "G4Table", 100); // Energy loss in CH2 (TARGET)
  // ------------ SILICON Energy Loss ------------
  // si_CH2 = EnergyLoss("./Eloss/Si33_CH2.G4table", "G4Table", 100); // Energy loss in CH2 (TARGET)
  // ------------ SULFUR Energy Loss ------------
  // s_CH2 = EnergyLoss("./Eloss/S33_CH2.G4table", "G4Table", 100); // Energy loss in CH2 (TARGET)

  // ------------ Si thickness ------------
  // Calculated by H. Jacob from data optimization
  si_thickness[1] = 301.1;
  si_thickness[2] = 295.0;
  si_thickness[3] = 296.6;
  si_thickness[4] = 303.2;

  // ------------ Al thickness ------------
  // Calculated by H. Jacob from data optimization
  al_thickness[1] = 0.492;
  al_thickness[2] = 0.740;
  al_thickness[3] = 0.660;
  al_thickness[4] = 0.542;

  // ----------- Get kinematics line -----------
  // 36S(p,d)35S@1800MeV
  S36pdReaction.GetKinematicLine3()->SaveAs("reaction/36Spd_kinematics_GS.root");

  // 34Si(p,d)33Si@1700MeV
  Si34pdReaction.GetKinematicLine3()->SaveAs("reaction/34Sipd_kinematics_GS.root");
  // Set the excitation energy for the Si34pd reaction to 4 MeV
  Si34pdReaction.SetExcitation4(4);
  // Save the kinematic line for the modified Si34pd reaction
  Si34pdReaction.GetKinematicLine3()->SaveAs("reaction/34Sipd_kinematics_4MeV.root");

  // 34Si(p,t)32Si@1700MeV
  Si34ptReaction.GetKinematicLine3()->SaveAs("reaction/34Sipt_kinematics_GS.root");

  double ReactionBeta = 0.315;
}

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::TreatEvent() {

  auto M2 = must2->m_PhysicsData;
  auto CATS = cats->m_PhysicsData;
  auto Exo = exogam->m_PhysicsData;
  auto ZDD = zdd->m_PhysicsData;
  // auto TAC = tac->m_PhysicsData;

  Clear();

  //Filter conditions

  if(M2->Si_E.size()!=1) return;
  if(M2->CsI_E.size()!=1) return;

  // Get the position of the interaction in MUST2
  TVector3 PosM2 {
    must2->GetStripPositionX(M2->TelescopeNumber[0], M2->Si_X[0],M2->Si_Y[0] ),
    must2->GetStripPositionY(M2->TelescopeNumber[0], M2->Si_X[0],M2->Si_Y[0] ),
    must2->GetStripPositionZ(M2->TelescopeNumber[0], M2->Si_X[0],M2->Si_Y[0] )
  };
  M2X.push_back(PosM2.X());
  M2Y.push_back(PosM2.Y());
  M2Z.push_back(PosM2.Z());

  // Beam direction
  TVector3 BeamDirectionDefault(0.,0.,1.); // Beam on Z axis
  auto BeamDirectionXYZ = cats->GetBeamDirection();
  TVector3 BeamDirectionCATS(BeamDirectionXYZ.X(), BeamDirectionXYZ.Y(), BeamDirectionXYZ.Z()); // Beam direction from CATS

  // Get the position of the interaction on TARGET
  TVector3 PosTargetDefault(0.,0.,0.); // Default position
  TVector3 PosTargetCATS(-CATS->PositionOnTargetX,CATS->PositionOnTargetY,0.); // Reconstruted position

  // Get the total energy deposited on MUST2: Si + CsI 
  ELab+=M2->Si_E[0];
  if(M2->CsI_E[0]>0){
    ELab+=M2->CsI_E[0];
  }
  else{
    ELab= -1000.;
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // ------------------------------------ Reaction reconstruction ------------------------------------
  auto ReconstructReaction = [&](Reaction reaction, TVector3 PosOnTarget, TVector3 BeamDirection){
  
    TVector3 HitDirectionRec = PosM2 - PosOnTarget;
    ThetaLab = HitDirectionRec.Angle(BeamDirection);
    double ELabCorrected_tmp = -1000.;

    if(ELab > 0){
      // Energy loss in Al dead layer
      double EbeforeAl = d_Al.EvaluateInitialEnergy(ELab,al_thickness[M2->TelescopeNumber[0]]*um,0.);
      // Energy loss in CH2 target
      double Emidtarget = d_CH2.EvaluateInitialEnergy(EbeforeAl,0.5*TargetThickness,HitDirectionRec.Theta());
      ELabCorrected_tmp = Emidtarget;
    }
  
    // Get the excitation energy
    Ex.push_back(reaction.ReconstructRelativistic(ELabCorrected_tmp, ThetaLab));
    thetaRecoil.push_back(reaction.GetEnergyImpulsionLab_4().Vect().Theta()*180./TMath::Pi());
    phiRecoil.push_back(reaction.GetEnergyImpulsionLab_4().Vect().Phi());
    betaRecoil.push_back(reaction.GetEnergyImpulsionLab_4().Beta());
    ELabCorrected.push_back(ELabCorrected_tmp);
    thetaParticle.push_back(HitDirectionRec.Theta()*180./TMath::Pi());
  };
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  // --------------------------- Reaction reconstruction 34Si(p,d)33Si@1700MeV ---------------------------
  ReconstructReaction(S36pdReaction, PosTargetCATS, BeamDirectionCATS); // Ex[0] 
  ReconstructReaction(S36pdReaction, PosTargetDefault , BeamDirectionDefault); // Ex[1]
  ReconstructReaction(S36pdReaction, PosTargetDefault , BeamDirectionCATS); // Ex[2]
  ReconstructReaction(S36pdReaction, PosTargetCATS, BeamDirectionDefault); // Ex[3]

  ThetaLab = ThetaLab*180./TMath::Pi(); // Convert to deg

  // Reconstruction of the E from the delta E in CsI
  CsI_EReconstructed = -M2->Si_E[0] + d_Al.EvaluateEnergyFromDeltaE(
    M2->Si_E[0],  // Delat E in Si
    si_thickness[M2->TelescopeNumber[0]]*1e-3, // Thickness of Si at 0 degree
    0, // Particle angle
    0.1, // Starting energy
    60, // Maximum energy allowed
    0.003, // Resolution at which function stop
    250 // Maximum number of steps
  ); 

  //// --------------------------- Doppler correction in EXOGAM ---------------------------
  // if(Exo->Exo_EAddBack.size()>0){
  //   exogam->SetBeta(betaRecoil[0]);
  //   double loc_edc = exogam->GetDoppler(Exo->Exo_EAddBack[0], Exo->Exo_FlangeNAddBack[0], Exo->Exo_CrystalNAddBack[0], Exo->Exo_OuterNAddBack[0]);
  //   EDC.push_back(loc_edc); 
  // }

  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB 
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB 
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB 
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB 
  //if(Exo->Exo_EAddBack.size()>0){
  //  exogam->SetBeta(betaRecoil[0]);
  //  double loc_edc = 
  //    exogam->GetDoppler_Sim100mm(
  //          Exo->Exo_EAddBack[0], 
  //          Exo->Exo_FlangeNAddBack[0], 
  //          Exo->Exo_CrystalNAddBack[0], 
  //          Exo->Exo_OuterNAddBack[0]
  //    );
  //  EDC_Method1.push_back(loc_edc); 
  //
  //  loc_edc = 
  //    exogam->GetDoppler_SimBoost(
  //          Exo->Exo_EAddBack[0], 
  //          Exo->Exo_FlangeNAddBack[0], 
  //          Exo->Exo_CrystalNAddBack[0], 
  //          Exo->Exo_OuterNAddBack[0]
  //    );
  //  EDC_Method2.push_back(loc_edc); 
  //}
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB 
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB 
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB 
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB 


}


void user_analysis::Analysis::Clear() {
  // Clear the branches
  M2X.clear();
  M2Y.clear();
  M2Z.clear();

  ELab = 0;
  ThetaLab = 0;

  ThetaCM_light = 0;
  ThetaCM_heavy = 0;

  Ex.clear();
  thetaRecoil.clear();
  phiRecoil.clear();
  betaRecoil.clear();
  ELabCorrected.clear();
  thetaParticle.clear();

  EDC.clear(); 
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB // EDC_Method1.clear(); 
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB // EDC_Method2.clear(); 
}

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::End() {

  // for(auto& telescope: de_e_histos){
  //   for(unsigned int c=0; c<17; ++c){
  //     telescope.second[c]->Write();
  //   }
  // }  
  // for(auto& telescope: csi_calib_histos){
  //   for(unsigned int c=0; c<17; ++c){
  //     telescope.second[c]->Write();
  //   }
  // }

  std::cout << "End of the analysis" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be used by the Factory                   //
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserAnalysis> ConstructUserAnalysis() {
  return std::make_shared<user_analysis::Analysis>();
}
}

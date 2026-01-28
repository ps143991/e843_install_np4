#include "Analysis.h"
#include "NPApplication.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::Init() {
  auto app = nptool::Application::GetApplication();
  
    zdd = std::dynamic_pointer_cast<zdd::ZddDetector>(app->GetDetector("zdd"));
    tac = std::dynamic_pointer_cast<tac::TacDetector>(app->GetDetector("tac"));
    gatconf = std::dynamic_pointer_cast<ebye::EbyEDetector>(app->GetDetector("ebye"));
    must2 = std::dynamic_pointer_cast<must2::Must2Detector>(app->GetDetector("must2"));
    cats = std::dynamic_pointer_cast<cats::CatsDetector>(app->GetDetector("cats"));
    // exogam = std::dynamic_pointer_cast<exogam::ExogamDetector>(app->GetDetector("exogam"));
    mugast = std::dynamic_pointer_cast<mugast::MugastDetector>(app->GetDetector("mugast"));

    //red color output message: hello world

    // cout<<"\033[1;31m works fine till here "<<endl;


    
     // Settings
    
    // ------------ DEUTERON Energy Loss ------------   //FIXME: not defined properly... giving error
    /* d_Si = EnergyLoss("./energy_loss/deuteron_Si.G4table", "G4Table", 100); // Energy loss in Silicon (MUST2)
    d_Al = EnergyLoss("./energy_loss/deuteron_Al.G4table", "G4Table", 100); // Energy loss in Aluminium (MUST2)
    d_CH2 = EnergyLoss("./energy_loss/deuteron_CH2.G4table", "G4Table", 100); // Energy loss in CH2 (TARGET) */
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


    double ReactionBeta = 0.315;

    reaction_dp = new nptool::Reaction("68Ni(d,p)69Ni@1122MeV");
    cout<<reaction_dp->GetBeamEnergy();
    reaction_dp->Print();

    OriginalBeamEnergy = reaction_dp->GetBeamEnergy();
    string Path = "./energy_loss/";
    string TargetMaterial_dp = "CD2";
    string TargetMaterial_pd = "CH2";

    cout<<" TargetMaterial_dp " <<  TargetMaterial_dp << "\n";
    TargetThickness = 5*micrometer;
    string TargetInfo = TargetMaterial_dp + " " + to_string(TargetThickness/micrometer) + " micrometer";
    nptool::message("blue","","",TargetInfo,true);

    string beam = ChangeNameToG4Standard(reaction_dp->GetNucleus1()->GetName(), false);
    string heavy = ChangeNameToG4Standard(reaction_dp->GetNucleus4()->GetName(), false);
    string light = ChangeNameToG4Standard(reaction_dp->GetNucleus3()->GetName(), false);

    Beam_Target = nptool::EnergyLoss("./energy_loss/Ni68_CD2.G4table", "G4Table", 100);
    LightTarget = nptool::EnergyLoss(Path+light + "_" + TargetMaterial_dp + ".G4table", "G4Table", 100);
    HeavyMylar = nptool::EnergyLoss(Path+beam + "_Mylar.G4table", "G4Table", 100);
    LightAl = nptool::EnergyLoss(Path+light + "_Al.G4table", "G4Table", 100);
    // LightSi = nptool::EnergyLoss(light + "_Si.G4table", "G4Table", 100);

    // ReadConfigTS();


}
////////////////////////////////////////////////////////////////////////////////

void user_analysis::Analysis::TreatGATCONF(){

   if(mugast->m_PhysicsData->DSSD_E.size()==1){
    if(mugast->m_PhysicsData->DSSD_E[0]>0.88){
      bMUGAST = true;
    } else{
      bMUGAST = false;
    }
  }
  if(must2->m_PhysicsData->Si_E.size() == 1 && must2->m_PhysicsData->CsI_E.size() == 1 && must2->m_PhysicsData->TelescopeNumber.size() == 1){
    bMUST2 = true;
  } else bMUST2 = false;

   GATCONFMASTER=*(gatconf->GenericRawBranch["GATCONF"]);
   if (GATCONFMASTER==1 /* || GATCONFMASTER==2 || GATCONFMASTER==16 || GATCONFMASTER==32 */){
    if(mugast->m_PhysicsData->DSSD_E.size()==1){
    decider = true;}
        // cout<<"decider "<<decider<<endl;

    GATCONFMASTERTS=*(gatconf->GenericRawBranchTS["GATCONFTS"]);
    // if (GATCONFMASTERTS) cout<< "GATCONFMASTERTS: " << GATCONFMASTERTS << endl;
   }
}


////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::TreatEvent() {
    Clear();
    TreatGATCONF();

    if(decider && bMUGAST){
    TreatCATS();
      if(bCATS){
        // cout<<"bcats is true in analysis "<<endl;
        TreatTOF();
        TreatMugast();
        if (dc_found) {TreatDC();}
      }
    }
    m_keep_event = bCATS && bMUGAST && decider && TAC_TOF ;
}

void user_analysis::Analysis::TreatTOF(){
  
  
  for(unsigned int i = 0; i < tac->m_PhysicsData->TAC_Time.size(); i++){
    if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_CATS_PL") == 0)
        {
            // TAC_CATS_PL += (tac->m_PhysicsData->TAC_Time[i]*0.01276894);
            TAC_CATS_PL.push_back(tac->m_PhysicsData->TAC_Time[i]*0.01276894);
            // cout<<"works well"<<endl;
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_MMG_CATS1") == 0)
        {
            // TAC_MMG_CATS1 += (tac->m_PhysicsData->TAC_Time[i]*0.01327);
            TAC_MMG_CATS1.push_back(tac->m_PhysicsData->TAC_Time[i]*0.01327);
        }
        else if (tac->m_PhysicsData->TAC_Name[i].compare("TAC_D4_CATS1") == 0) {
                TAC_D4_CATS1nc .push_back( tac->m_PhysicsData->TAC_Time[i]);
                if (tac->m_PhysicsData->TAC_Time[i] > 0) {/* cout<<"TAC tof is: "<<tac->m_PhysicsData->TAC_Time[i]<<endl; */ TAC_TOF = true;}
                
      }
  }
}


////////////////////////////////////////////////////////////////////////////////

void user_analysis::Analysis::TreatCATS(){

    if(cats->m_PhysicsData->PositionOnTargetX > -1000 && cats->m_PhysicsData->PositionOnTargetY > -1000){
      // cout<<"CATS position on target valid"<<endl;

        CATS1_XX=0;
        CATS2_XX=0;
        CATS1_YY=0;
        CATS2_YY=0;
        CATS1_XX =  cats->m_PhysicsData->PositionX[0];
        CATS1_YY =  cats->m_PhysicsData->PositionY[0];
        CATS2_XX =  cats->m_PhysicsData->PositionX[1];
        CATS2_YY =  cats->m_PhysicsData->PositionY[1];
    	  BeamImpact = TVector3((cats->m_PhysicsData->PositionOnTargetX),(cats->m_PhysicsData->PositionOnTargetY),0);
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)+1.7,-(cats->m_PhysicsData->PositionOnTargetY)+2.3,0); ///correctone cats3   ====>>>>> OZGE was using this: PS April 23, 2025
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)+1.5,-(cats->m_PhysicsData->PositionOnTargetY)+3,0); ///correctone cats9
        //BeamImpact = TVector3((cats->m_PhysicsData->PositionOnTargetX)-1.835,(cats->m_PhysicsData->PositionOnTargetY)-1.51,0); ///new one cast2
        //BeamImpact = TVector3((cats->m_PhysicsData->PositionOnTargetX),(cats->m_PhysicsData->PositionOnTargetY),0); ///new one cats1
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX),(cats->m_PhysicsData->PositionOnTargetY),0); ///new one cats6
        //BeamImpact = TVector3((cats->m_PhysicsData->PositionOnTargetX)-1.21,(cats->m_PhysicsData->PositionOnTargetY)-4.35,0); ///new one cats4
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX),(cats->m_PhysicsData->PositionOnTargetY)-4,0); ///new one cats5
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)+1.5,(cats->m_PhysicsData->PositionOnTargetY)-3,0); ///new one cats8 target centered
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)-0.5,(cats->m_PhysicsData->PositionOnTargetY)-1.3,0);//cats7
        BeamDirection = TVector3(CATS2_XX - CATS1_XX, CATS2_YY - CATS1_YY, (cats->m_PhysicsData->PositionZ[1] - cats->m_PhysicsData->PositionZ[0]));
                
        bCATS = true;
        // cout<<"bcats returned true"<<endl; countPS_bcats++; cout<<"count ps_bcats: "<<countPS_bcats<<" "<<endl;
        // std::cout << " BeamImpact " << BeamImpact.X() << "  " << BeamImpact.Y() <<  "	"<< BeamImpact.Z() << "\n";
        //  std::cout << " BeamDirection " << BeamDirection.X() << "  " << BeamDirection.Y() <<  "	"<< BeamDirection.Z() << std::endl;
        // cout<<"works well till here cats"<<endl;
      }
    else {
      // cout<<"CATS position on target not valid"<<endl;
      bCATS = false; /*cout<<"bcats returned false"<<endl;*/}
}

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::TreatDC(){
      
}
  // 



void user_analysis::Analysis::TreatMugast(){      //From TreatMugast in 10March, not treatmugast2

  double  Channel_Calib = 0.02554;    // Time Calibration Coefficient 
  double D4_CATS1_L = 2150;
  double Time_0_Ref = 27594;

  if(mugast->m_PhysicsData->DSSD_E.size()==1){
    for(unsigned int countMugast = 0; countMugast < mugast->m_PhysicsData->DSSD_E.size(); countMugast++){
          // if(countMugast>1)cout<<"more than 2 mugast hit!"<<endl;
        // cout<<"time is: "<<mugast->m_PhysicsData->DSSD_T[countMugast]<<" ";
        // if(GATCONFMASTERTS>32650963337861) cout<< "GATCONFMASTERTS: " << GATCONFMASTERTS << endl;

        MG_DetNum.push_back( mugast->m_PhysicsData->TelescopeNumber[countMugast]);
        MG_DetM++;
        thetalab_tmp = 0;
        philab_tmp =0;
        Energycor = 0;
        Energy = 0;
        Energy= mugast->m_PhysicsData->DSSD_E[countMugast];




        // Calculate TOF
        if (TAC_TOF==true){
          // cout<<"Debug PS: true or not? "<< TAC_TOF << endl;
          TOF = (Time_0_Ref - TAC_D4_CATS1nc[0]) * Channel_Calib / D4_CATS1_L / 1e7;  // experiment
          // if(GATCONFMASTERTS>32650963337861) cout<<"TOF is: "<<TOF<<endl;
          reaction_dp->GetNucleus1()->SetTimeOfFlight(TOF);

              // cout<<"Debug PS: TOF "<< TOF << endl;
          
          reaction_dp->SetBeamEnergy(reaction_dp->GetNucleus1()->GetEnergy());
          OriginalBeamEnergy = reaction_dp->GetBeamEnergy();
          if(OriginalBeamEnergy>800){
            OriginalBeamEnergy = HeavyMylar.Slow(OriginalBeamEnergy, 8.4*micrometer, 0);
            OriginalBeamEnergy = Beam_Target.Slow(OriginalBeamEnergy, TargetThickness * 0.5, 0);

            reaction_dp->SetBeamEnergy(OriginalBeamEnergy);

            HitDirectionMG = mugast->GetPositionOfInteraction(countMugast) - BeamImpact;
                // cout<<"DEBUG PS: Beam Impact is: "<<BeamImpact.X()<<" "<<BeamImpact.Y()<<" "<<BeamImpact.Z()<<endl;

                // cout<<"DEBUG PS: Interaction Position is: "<<mugast->GetPositionOfInteraction(countMugast).X()<<" "<<mugast->GetPositionOfInteraction(countMugast).Y()<<" "<<mugast->GetPositionOfInteraction(countMugast).Z()<<endl;
                // cout<<"DEBUG PS: Hit Direction is: "<<HitDirectionMG.X()<<" "<<HitDirectionMG.Y()<<" "<<HitDirectionMG.Z()<<endl;



            thetalab_tmp = HitDirectionMG.Angle(BeamDirection);
            // cout<<"Theta MG calc from TOF: "<< thetalab_tmp << endl;
            philab_tmp = HitDirectionMG.Phi();

            MG_X.push_back(mugast->GetPositionOfInteraction(countMugast).X());
            MG_Y.push_back(mugast->GetPositionOfInteraction(countMugast).Y());
            MG_Z.push_back(mugast->GetPositionOfInteraction(countMugast).Z());



            ThetaMGSurface = HitDirectionMG.Angle(mugast->GetTelescopeNormal(countMugast));
            ThetaNormalTargetMG = HitDirectionMG.Angle(TVector3(0,0,1));

            Energycor = Energy;
            MG_ELabraw.push_back(Energy);

            MG_PhiLab.push_back(HitDirectionMG.Phi()/M_PI*180.);
            MG_ThetaLab.push_back(thetalab_tmp / M_PI * 180.);

            // TLorentzVector PHeavy_dp = reaction_dp->LorentzAfterReaction(Energy, thetalab_tmp);
            // Beta_dp.push_back(PHeavy_dp.Beta());


            Energycor = LightAl.EvaluateInitialEnergy(Energy, 0.4*micrometer, ThetaMGSurface);
            // cout<<"Energy after Al corr: "<<Energycor<<endl;
            
            ELoss_Al.push_back(Energy - Energycor);

            // Target Correction
            double elab_tmp = Energycor;
            Energycor = LightTarget.EvaluateInitialEnergy(Energycor, TargetThickness * 0.5, ThetaNormalTargetMG);
            // cout<<"Energy after Target corr: "<<Energycor<<endl;

            MG_ELab.push_back(Energycor);
            ELoss_Target.push_back(elab_tmp - Energycor);
            ELoss.push_back(Energy - Energycor);

            // Part 3 : Excitation Energy Calculation

            MG_Ex.push_back(reaction_dp->ReconstructRelativistic(Energycor, thetalab_tmp, philab_tmp)); //charlies method
            // MG_Ex_lin.push_back((reaction_dp->ReconstructRelativistic(Energy, thetalab_tmp))-(0.01992656*MG_ThetaLab[countMugast]-2.63549658)); //charlies method without correction
            
                //      // Part 4 : Theta CM Calculation
            MG_ThetaCM.push_back(reaction_dp->EnergyLabToThetaCM(MG_ELab[countMugast], MG_ThetaLab[countMugast])/deg);


            // work by PS::::::::::::::::::: separating it to find easily ::::::::::::::::::

            /* if(MG_ELab[0]>1.0 && abs(TAC_MMG_CATS1[0]-387)>25 && TAC_CATS_PL[0]>300){
                Ex_f.push_back(MG_Ex[countMugast]);
                dc_found = true;
            } */

            // cout<<"Raw Energy MG: "<<Energy<<" Corrected Energy MG: "<<Energycor<<" Theta Lab MG: "<<MG_ThetaLab[countMugast]<<" Ex. true: "<<MG_Ex[countMugast]<<endl;

          }
        // cout<<"***********************************************************************************************"<<endl;

        }
    }
    // cout<<endl;
  }
}





void user_analysis::Analysis::TreatMust2(){}
////////////////////////////////////////////////////////////////////////////////
/* void user_analysis::Analysis::TreatEvent() {


  auto M2 = must2->m_PhysicsData;
  auto cats = cats->m_PhysicsData;
  //auto Exo = exogam->m_PhysicsData;
  auto ZDD = zdd->m_PhysicsData;
  // auto tac = tac->m_PhysicsData;

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
  TVector3 BeamDirectionCATS(BeamDirectionXYZ.X(), BeamDirectionXYZ.Y(), BeamDirectionXYZ.Z()); // Beam direction from cats

  // Get the position of the interaction on TARGET
  TVector3 PosTargetDefault(0.,0.,0.); // Default position
  TVector3 PosTargetCATS(-cats->m_PhysicsData->PositionOnTargetX,cats->m_PhysicsData->PositionOnTargetY,0.); // Reconstruted position

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

} */



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
  Tree->Branch("Ex_f", &Ex_f); // Excitation energy with filter conditions

  Tree->Branch("gf",&gf,"gf/I");
//   Tree->Branch("GATCONF",&GATCONFMASTER);
//   Tree->Branch("GATCONFTS",&GATCONFMASTERTS);
  Tree->Branch("HitDirectionMG",&HitDirectionMG);
  Tree->Branch("BeamDirection",&BeamDirection);
  Tree->Branch("BeamImpact",&BeamImpact);
  Tree->Branch("BeamDirection0",&BeamDirection0);
  Tree->Branch("BeamImpact0",&BeamImpact0);
  Tree->Branch("MG_DetM",&MG_DetM,"MG_DetM/s");
  Tree->Branch("MG_DetNum",&MG_DetNum,"MG_DetNum/s");
  Tree->Branch("MG_Ex",&MG_Ex);
  // Tree->Branch("MG_Ex2",&MG_Ex2);
  // Tree->Branch("MG_Extof",&MG_Ex);
  Tree->Branch("MG_Ex_lin",&MG_Ex_lin);
  Tree->Branch("MG_ELab",&MG_ELab);
  // Tree->Branch("MG_ELab2",&MG_ELab2);
  Tree->Branch("MG_ELabraw",&MG_ELabraw);
  Tree->Branch("ELoss_Al",&ELoss_Al);
  Tree->Branch("ELoss",&ELoss);
  Tree->Branch("ELoss_Target",&ELoss_Target);
  // Tree->Branch("MG_ThetaLab2",&MG_ThetaLab2);
  Tree->Branch("MG_PhiLab",&MG_PhiLab);
  // Tree->Branch("MG_Phi0",&MG_Phi0);
  Tree->Branch("MG_ThetaCM",&MG_ThetaCM);
  Tree->Branch("MG_ThetaLab",&MG_ThetaLab);
  // Tree->Branch("MG_ThetaCM2",&MG_ThetaCM2);
  Tree->Branch("MG_X",&MG_X);
  Tree->Branch("MG_Y",&MG_Y);
  Tree->Branch("MG_Z",&MG_Z);
  
  //////////////////////////////////////EXOGAM Related branch ////////////////////////////////
  Tree->Branch("EXO_Doppler_dp",&EXO_Doppler_dp);
  Tree->Branch("EXO_Doppler_pt",&EXO_Doppler_pt);
  Tree->Branch("EXO_Doppler_p3He",&EXO_Doppler_p3He);
  Tree->Branch("Beta_dp",&Beta_dp);
  Tree->Branch("Beta_p3He",&Beta_p3He);
  Tree->Branch("Beta_pt",&Beta_pt);
  
  Tree->Branch("TOF",&TOF);
  Tree->Branch("OriginalBeamEnergy",&OriginalBeamEnergy);
  Tree->Branch("OriginalBeamEnergy2",&OriginalBeamEnergy2);
  Tree->Branch("OriginalBeamEnergync",&OriginalBeamEnergync);

  Tree->Branch("TAC_CATS_PL",&TAC_CATS_PL);
  Tree->Branch("TAC_MMG_CATS1",&TAC_MMG_CATS1);
  Tree->Branch("TAC_D4_CATS1nc",&TAC_D4_CATS1nc);
  
  // ------------ EXOGAM ------------
  Tree->Branch("EDC", &EDC); // Doppler corrected energy in EXOGAM
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB // Tree->Branch("EDC_Method1", &EDC_Method1);
  // UNCOMMENT WHEN EXOGAM PLUGIN UP-TO-DATE WITH GITLAB // Tree->Branch("EDC_Method2", &EDC_Method2);
 
  // ------------ cats ------------
  // std::cout << "I ATTACH BRANCHES" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::Clear() {
  dc_found = false;

    
  Ex_f.clear();
    TAC_CATS_PL.clear();
    TAC_MMG_CATS1.clear();
    TAC_D4_CATS1nc.clear();
    TAC_TOF = false;
    decider = false;
    bMUGAST = false;
    bMUST2 = false;

    bCATS = false;
    m_keep_event = false;
    MG_DetM= 0;
    MG_DetNum.clear();
    MG_Ex.clear();
    // MG_Ex2.clear();
    // MG_Extof.clear();
    // MG_Ex0tof.clear();
    MG_Ex_lin.clear();
    MG_ELab.clear();
    // MG_ELab2.clear();
    MG_ELabraw.clear();
    ELoss_Al.clear();
    ELoss_Target.clear();
    ELoss.clear();
    MG_ThetaLab.clear();
    MG_PhiLab.clear();
    // MG_Phi0.clear();

    // MG_ThetaLab2.clear();
    MG_ThetaCM.clear();
    // MG_ThetaCM2.clear();
    MG_X.clear();
    MG_Y.clear();
    MG_Z.clear();
    EXO_Doppler_p3He.clear();
    EXO_Doppler_pt.clear();
    EXO_Doppler_dp.clear();

    Beta_p3He.clear();
    Beta_pt.clear();
    Beta_dp.clear();


    // TSEvent->ClearTimeStamps();
    TOF= 0.0;
    velocity = 0.0;
    

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
void user_analysis::Analysis::End() {}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserAnalysis> ConstructUserAnalysis() { return std::make_shared<user_analysis::Analysis>(); }
}

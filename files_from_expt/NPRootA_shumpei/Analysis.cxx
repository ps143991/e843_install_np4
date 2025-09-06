/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  MUST_AND_ZDD analysis project                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include<iostream>
using namespace std;
#include"Analysis.h"
#include"NPAnalysisFactory.h"
#include"NPDetectorManager.h"
////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
  InitialConditions = new TInitialConditions;
  ReactionConditions = new TReactionConditions;
  InitInputBranch();
  InitOutputBranch();
  CATS= (TCATSPhysics*) m_DetectorManager->GetDetector("CATSDetector");
  //ZDD= (TZDDPhysics*) m_DetectorManager->GetDetector("ZDD");
  M2 = (TMust2Physics*)  m_DetectorManager -> GetDetector("M2Telescope");
  MG = (TMugastPhysics*)  m_DetectorManager -> GetDetector("Mugast");
   
  reaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
  OriginalBeamEnergy = reaction->GetBeamEnergy();


  string Path = "../../Inputs/EnergyLoss/";
  string TargetMaterial = m_DetectorManager->GetTargetMaterial();
  TargetThickness = m_DetectorManager->GetTargetThickness();
  string beam=  NPL::ChangeNameToG4Standard(reaction->GetNucleus1()->GetName());
  string heavy_ejectile=  NPL::ChangeNameToG4Standard(reaction->GetNucleus4()->GetName());
  string light=NPL::ChangeNameToG4Standard(reaction->GetNucleus3()->GetName());

  //string beam = "proton";
  //string light = "proton";
  //string heavy_ejectile = "proton";
  //string TargetMaterial = "CH2";
  //Beam_Target = NPL::EnergyLoss(beam+"_"+TargetMaterial+".G4table","G4Table",100);
  //Heavy_Target = NPL::EnergyLoss(heavy_ejectile+"_"+TargetMaterial+".G4table","G4Table",100);
  //LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".G4table","G4Table",100 );
  //LightAl = NPL::EnergyLoss(light+"_Al.G4table","G4Table",100);
  //LightSi = NPL::EnergyLoss(light+"_Si.G4table","G4Table",100);
//
  ProtonSi = NPL::EnergyLoss(Path+ "proton_Si.G4table", "G4Table", 100);

  ///////////////////////////// Initialize some important parameters //////////////////////////////////

//  for(int Telescope = 3; Telescope<5;Telescope++){
//    proton_cut_file[Telescope-1] = new TFile(Form("./CUT/CUT_proton/CUT_T%i_proton.root",Telescope));
//    deuteron_cut_file[Telescope-1] = new TFile(Form("./CUT/CUT_deuteron/CUT_T%i_deuteron.root",Telescope));
//    triton_cut_file[Telescope-1] = new TFile(Form("./CUT/CUT_triton/CUT_T%i_triton.root",Telescope));
//
//    proton_cut[Telescope-1] = (TCutG*)proton_cut_file[Telescope-1]->FindObjectAny(Form("CUT_T%i_proton",Telescope));
//    deuteron_cut[Telescope-1] = (TCutG*)deuteron_cut_file[Telescope-1]->FindObjectAny(Form("CUT_T%i_deuteron",Telescope));
//    triton_cut[Telescope-1] = (TCutG*)triton_cut_file[Telescope-1]->FindObjectAny(Form("CUT_T%i_triton",Telescope));
//  }

  Drift_Speed =  1 * cm / microsecond;
  ZDir.SetXYZ(0.,0.,1.);


  DetectorNumber = 0;
  ThetaNormalTarget = 0;
  ThetaM2Surface = 0;
  ThetaMGSurface = 0;
  Si_E_M2 = 0;
  CsI_E_M2 = 0;
  Energy = 0;
  ThetaGDSurface = 0;
  BeamDirection = TVector3(0,0,1);
  PlasticThreshold = 100;
  /*for(int i =0; i < ZDD->Get_m_Ionisation_Chamber_Z().size(); i++){
    ZDD_Corrected_IC_E.push_back(0);
  }*/
  HeavyEjectile->SetBeta(0.28);
  

  ExogamTopo.open("./calibration/Exogam/ExogamTopology.txt",std::ios::out);
  if(ExogamTopo.is_open()){
    while(ExogamTopo){
      std::getline(ExogamTopo,ExogamLine);
     
      const char* ExogamLine_char = ExogamLine.c_str();
      
      if(sscanf(ExogamLine_char, "Exogam%i : %i",&ExoNumb,&Flange_tmp)){
    
        std::cout << Flange_tmp << std::endl;
        FlangeNumb[ExoNumb-1] = Flange_tmp;

      }
      else{
        std::cout << "Wrong format in Exogam topo" << std::endl;
        break;
      }
    }
  }
  
  DC_calib.open("./calibration/DC/calib/DC_run_0162.txt",std::ios::out);
  if(DC_calib.is_open()){
    while(DC_calib){
      std::getline(DC_calib,DCLine);
     
      const char* DCLine_char = DCLine.c_str();
      
      if(sscanf(DCLine_char, "DC_%c_%i %lf %lf %lf",DC_XY,&DC_numb_tmp, &off_tmp, &cff_tmp ,&sqr_tmp)){
        std::cout << DC_numb_tmp << "  " << DC_XY << "  " <<  off_tmp << " "<< cff_tmp << " "<< sqr_tmp <<"\n";
        if(DC_XY[0] =='Y'){
            off[DC_numb_tmp-1] = off_tmp;
            cff[DC_numb_tmp-1] = cff_tmp;
            sqr[DC_numb_tmp-1] = sqr_tmp;
        }
        else if(DC_XY[0] =='X'){
            std::cout << "coincoin" << std::endl;
            off[DC_numb_tmp+1] = off_tmp;
            cff[DC_numb_tmp+1] = cff_tmp;
            sqr[DC_numb_tmp+1] = sqr_tmp;
        }
        else{
          std::cout << "Wrong format in DC calib 1" << std::endl;
          break;
        }

      }
      else{
        std::cout << "Wrong format in DC calib 2" << std::endl;
        break;
      }
    }
  }
  Plastic_calib.open("./calibration/Plastic/calib/Plastic_r161.txt",std::ios::out);
  if(Plastic_calib.is_open()){
    while(Plastic_calib){
      std::getline(Plastic_calib,PlasticLine);
     
      const char* PlasticLine_char = PlasticLine.c_str();
      
      if(sscanf(PlasticLine_char, "Plastic%i : %lf %lf",&Plastic_Nb_tmp,&Plastic_pedestal_tmp, &Plastic_peak_tmp)){
        std::cout << Plastic_Nb_tmp << "  " << Plastic_pedestal_tmp << "  " <<  Plastic_peak_tmp <<"\n";
        Plastic_pedestal[Plastic_Nb_tmp] = Plastic_pedestal_tmp;
        Plastic_peak[Plastic_Nb_tmp] = Plastic_peak_tmp;
        }
      else{
        std::cout << "Wrong format in Plastic calib" << std::endl;
        break;
      }
    }
  }
  
  IC_calib.open("./calibration/IC/calib/IC_r161.txt",std::ios::out);
  if(IC_calib.is_open()){
    while(IC_calib){
      std::getline(IC_calib,ICLine);
     
      const char* ICLine_char = ICLine.c_str();
      
      if(sscanf(ICLine_char, "IC%i : %lf %lf",&IC_Nb_tmp,&IC_pedestal_tmp, &IC_peak_tmp)){
        std::cout << "Test " <<  IC_Nb_tmp << "  " << IC_pedestal_tmp << "  " <<  IC_peak_tmp <<"\n";
        IC_pedestal[IC_Nb_tmp] = IC_pedestal_tmp;
        IC_peak[IC_Nb_tmp] = IC_peak_tmp;
        }
      else{
        std::cout << "Wrong format in IC calib" << std::endl;
        break;
      }
    }
  }

  for(int i = 0; i <= 11; i++){
    Exogam_Clovers_struc[i] = Ask_For_Angles(FlangeNumb[i]);
  }


  //Beta = 0.28;
  //  Beta = 0.261;
  Beta = 0.181;
  //  Beta = 0.277;

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
    ReInit();
    xtarget = 0;
    ytarget = 0;
    CATSX_diff = 0;
    CATSY_diff = 0;

    xtarget = CATS->GetPositionOnTargetX();
    ytarget = CATS->GetPositionOnTargetY();
//    if(CATS1_X > -1500 && CATS2_X > -1500 && Xf > -1500){
//      CATSX_diff = -(CATS2_X - CATS1_X);
//      xtarget = -Xf;
//    }
//    if(CATS1_Y > -1500 && CATS2_X > -1500 && Yf > -1500){
//      ytarget = Yf;
//      CATSY_diff = CATS2_X - CATS1_X;
//    }
    //////////////////// MUST2 Part ////////////////////

    //    std::cout << "test" << "\n";

    for(unsigned int countMust2 = 0 ; countMust2 < M2->Si_E.size() ; countMust2++){
      M2_TelescopeM++;
      //Part 0 : Get the usefull Data
      // MUST2
      int TelescopeNumber = M2->TelescopeNumber[countMust2];

      /************************************************/
      // Part 1 : Impact Angle
      ThetaM2Surface = 0;
      ThetaNormalTarget = 0;
      BeamImpact = TVector3(xtarget, ytarget,0); 
      BeamDirection = TVector3(CATSX_diff, CATSY_diff, 497.1);
      // std::cout <<  CATS1_X << " " << CATS1_Y << " "<
      // CATS2_X <<" " << CATS2_Y << std::endl;
      
      
      //BeamImpact = TVector3(0,0,0); 
      // BeamDirection = TVector3(0,0,1);
      
      TVector3 HitDirection = M2 -> GetPositionOfInteraction(countMust2) - BeamImpact ;
      M2_ThetaLab.push_back(HitDirection.Angle( BeamDirection ));

      //std::cout <<  M2 -> GetPositionOfInteraction(countMust2).X() << "  " << M2 -> GetPositionOfInteraction(countMust2).Y() << "  "<< M2 -> GetPositionOfInteraction(countMust2).Z() <<  std::endl;
      M2_X.push_back(M2 -> GetPositionOfInteraction(countMust2).X());
      M2_Y.push_back(M2 -> GetPositionOfInteraction(countMust2).Y());
      M2_Z.push_back(M2 -> GetPositionOfInteraction(countMust2).Z());

      ThetaM2Surface = HitDirection.Angle(- M2 -> GetTelescopeNormal(countMust2) );
      ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;

      /************************************************/

      /************************************************/
      // Part 2 : Impact Energy
      Energy = 0;
      Si_E_M2 = 0;
      CsI_E_M2 = 0;

      Si_E_M2 = M2->Si_E[countMust2];
      CsI_E_M2= M2->CsI_E[countMust2];

      Ed = -10.3154 + 76.1818/(Si_E_M2+0.0896792);
      Double_t Ebr2 = 0.3 + 0.8 * Ed;
      Double_t Ebr3 = 2.0 + 1.1 * Ed;

      if(TAC_D4_CATS1>1){
	Double_t beta = 1/(TAC_D4_CATS1*(-0.000307526)+8.97351);
	Double_t gamma = 1/sqrt(1-beta*beta);
	Double_t mf = 62332.093;
	Beam_E = mf * (gamma-1);
	//	std::cout  << Beam_E << " \n";

      }


      if(CsI_E_M2 > Ebr2 && CsI_E_M2 < Ebr3){
	Si_E_d = Si_E_M2;
	CsI_E_d = CsI_E_M2;
	gd = 1;

	//	Etot_d = CsI_E_d; 
	Etot_d = Si_E_d + CsI_E_d; 
	ThetaLab_d = HitDirection.Angle( BeamDirection )/deg; 

	Double_t mp = 938.272;
	Double_t md = 1875.613;
	Double_t mb = 63263.866;
	Double_t mf = 62332.093;
	//	Double_t Ebeam = Beam_E;
	Double_t Ebeam = 38 * 68;

	Ex_d = -mf + TMath::Sqrt(mb*mb + mp*mp + md*md -2*(Ebeam+mb)*(Etot_d+md) +2*mp*(Ebeam+mb-Etot_d-md)
				 + 2*TMath::Cos(ThetaLab_d*deg)*TMath::Sqrt(Ebeam*Etot_d*(Ebeam+2*mb)*(Etot_d+2*md)) ); 
	Ex_dc = -mf + TMath::Sqrt(mb*mb + mp*mp + md*md -2*(Beam_E+mb)*(Etot_d+md) +2*mp*(Beam_E+mb-Etot_d-md)
				 + 2*TMath::Cos(ThetaLab_d*deg)*TMath::Sqrt(Beam_E*Etot_d*(Beam_E+2*mb)*(Etot_d+2*md)) ); 
      }

      //if(Si_E_M2 > 0){
      //  M2_ECsI_from_deltaE.push_back(ProtonSi.EvaluateEnergyFromDeltaE(
      //    Si_E_M2, 300*um, ThetaM2Surface, 6.0 * MeV, 300.0 * MeV,
      //    0.001 * MeV, 10000));
      //}
      // if CsI
      if(CsI_E_M2>0 ){
        // The energy in CsI is calculate form dE/dx Table because
        Energy = CsI_E_M2;
        //Energy = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaM2Surface);
        Energy+=Si_E_M2;
      }

      else
        Energy = Si_E_M2;


      // Evaluate energy using the thickness
      // Energy = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaM2Surface);
      // Target Correction
      //Energy = LightTarget.EvaluateInitialEnergy(Energy ,TargetThickness*0.5, ThetaNormalTarget);
      M2_ELab.push_back(Energy);
      M2_dE.push_back(Si_E_M2);

      /************************************************/

      /************************************************/
      // Part 3 : Excitation Energy Calculation
      M2_Ex.push_back(reaction->ReconstructRelativistic( M2_ELab[countMust2] , M2_ThetaLab[countMust2] ));
      M2_ThetaLab[countMust2]=M2_ThetaLab[countMust2]/deg;

      /************************************************/

      /************************************************/
      // Part 4 : Theta CM Calculation
      M2_ThetaCM.push_back(reaction->EnergyLabToThetaCM( M2_ELab[countMust2] , M2_ThetaLab[countMust2])/deg);
      /************************************************/

      //if(proton_cut[TelescopeNumber]){
      //  std::cout << "Test :" << proton_cut[TelescopeNumber] << " \n";
      //  
      //}
    }//end loop MUST2
    
    
    for(unsigned int countMugast = 0 ; countMugast < MG->DSSD_E.size() ; countMugast++){
      MG_DetM++;
      
      // Part 1 : Impact Angle                                                    
      TVector3 HitDirectionMG = MG -> GetPositionOfInteraction(countMugast) - BeamImpact ;
      MG_ThetaLab.push_back(HitDirectionMG.Angle( BeamDirection )/deg);                             
      
      MG_X.push_back(MG -> GetPositionOfInteraction(countMugast).X());                       
      MG_Y.push_back(MG -> GetPositionOfInteraction(countMugast).Y());                       
      MG_Z.push_back(MG -> GetPositionOfInteraction(countMugast).Z());                       
      
//      ThetaMGSurface.push_back(HitDirectionMG.Angle( TVector3(0,0,1) )) ;                    
//      ThetaNormalTargetMG.push_back(HitDirectionMG.Angle( TVector3(0,0,1) )) ;
      MG_DetM++;
      
      // Part 2 : Impact Energy                                                   
      Energy = 0;                                                          
      Energy = MG->GetEnergyDeposit(countMugast);                                 
      MG_ELab.push_back(Energy);                                                                                 


      if(Energy > 2){
	Etot_p = Energy;
	ThetaLab_p = HitDirectionMG.Angle( BeamDirection )/deg; 

	Double_t mp = 938.272;
	Double_t md = 1875.613;
	Double_t mb = 63263.866;
	Double_t mf = 64198.845;
	//	Double_t Ebeam = Beam_E;
	Double_t Ebeam = 18 * 68;

	Ex_p = -mf + TMath::Sqrt(mb*mb + mp*mp + md*md -2*(Ebeam+mb)*(Etot_p+mp) +2*md*(Ebeam+mb-Etot_p-mp)
				 + 2*TMath::Cos(ThetaLab_p*deg)*TMath::Sqrt(Ebeam*Etot_p*(Ebeam+2*mb)*(Etot_p+2*mp)) ); 
	Ex_pc = -mf + TMath::Sqrt(mb*mb + mp*mp + md*md -2*(Beam_E+mb)*(Etot_p+mp) +2*md*(Beam_E+mb-Etot_p-mp)
				 + 2*TMath::Cos(ThetaLab_p*deg)*TMath::Sqrt(Beam_E*Etot_p*(Beam_E+2*mb)*(Etot_p+2*mp)) ); 
      }


      // ELab = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaMGSurface);
      // Target Correction                                                        
      //ELab   = LightTarget.EvaluateInitialEnergy( Energy,TargetThickness*0.5, ThetaNormalTarget);
                                                                                       
      //if(LightWindow)                                                             
      //ELab = LightWindow->EvaluateInitialEnergy( ELab ,WindowsThickness, ThetaNormalTarget);
                                                                                       
//      // Part 3 : Excitation Energy Calculation                                   
//      MG_Ex.push_back(reaction->ReconstructRelativistic( MG_ELab[countMugast] , MG_ThetaLab[countMugast]));                   
//                                                                                       
//                                                                                         
//      // Part 4 : Theta CM Calculation                                            
//      MG_ThetaCM.push_back(reaction->EnergyLabToThetaCM( MG_ELab[countMugast] , MG_ThetaLab[countMugast])/deg);               
//      MG_ThetaLab[countMugast]=MG_ThetaLab[countMugast]/deg;                                                      
                                                                                       
    }
	

    if(IC_ZDDRaw[0] > 10 && IC_ZDDRaw[1] > 10&& IC_ZDDRaw[2] > 10&& IC_ZDDRaw[3] > 10&& IC_ZDDRaw[4] > 10 ){
      IC_ZDD_sum = IC_ZDDRaw[0] + IC_ZDDRaw[1] + IC_ZDDRaw[2] + IC_ZDDRaw[3] + IC_ZDDRaw[4];
    }
    
    if(PlasticRaw[2] > 10 && PlasticRaw[7] > 10){
      PL_center =  TMath::Sqrt(PlasticRaw[2]*PlasticRaw[7]);
    }
    
    if(PL_center > 8600 && PL_center < 9200 && IC_ZDD_sum > 7600 && IC_ZDD_sum < 8000){
      gf = 1;
    }
    
    /////////////////////////////// Drift Chambers ///////////////////


    if(DCRawM <=2 && DATATRIG_CATSTS[0] > 0){
      for(int DCmult = 0; DCmult < DCRawM; DCmult++){
        DC_Nr = DCRawNr[DCmult];
        
        if(DC_Nr > 2 && DCRawTS[DCmult] > 0){
          dt = DCRawTS[DCmult] - DATATRIG_CATSTS[0];
          DC_X = off[DC_Nr] + cff[DC_Nr]*dt + sqr[DC_Nr]*dt*dt;
          //std::cout << dt << "  " << DC_X << " " << DC_Nr <<  "\n";
          //std::cout << " Test " << DCRawTS[DCmult] << "  " << DATATRIG_CATSTS[0] <<  "\n";
        }
        else if(DC_Nr < 2 && DCRawTS[DCmult] > 0){
          dt = DCRawTS[DCmult] - DATATRIG_CATSTS[0];
          DC_Y = off[DC_Nr] + cff[DC_Nr]*dt + sqr[DC_Nr]*dt*dt;
          //std::cout << dt << "  " << DC_Y <<  "  " << DC_Nr << "\n";
          //std::cout << " Test " << DCRawTS[DCmult] << "  " << DATATRIG_CATSTS[0] <<  "\n";
        }
      }
    }
    

    for(int i = 0; i < 10; i++){
      if(Plastic_peak[i] != 0)
        PlasticCal[i] = (PlasticRaw[i] - Plastic_pedestal[i])*Plastic_peak[2]/Plastic_peak[i]; 
      else
        PlasticCal[i] = 0;
    }
    
    for(int i = 0; i < 4; i++){
      if(IC_peak[i] != 0)
        ICCal[i] = (IC_ZDDRaw[i] - IC_pedestal[i])*IC_peak[0]/IC_peak[i]; 
      else
        ICCal[i] = 0;
    }

    for(int i = 0; i < 5;i++){
      PlasticEner_tmp = sqrt(PlasticCal[i]*PlasticCal[i+5]);
      if(PlasticEner_tmp > PlasticThreshold && PlasticCal[i] > 0 && PlasticCal[i+5] > 0){
        PlasticEner[PlasticCounter] = PlasticEner_tmp;
        PlasticEnerN[PlasticCounter] = i;
        PlasticEnerM++;
        PlasticEnerTS[PlasticCounter] = PlasticRawTS[i];
      }
    }

//    if(EXO_ZDD_A_C > 10 || EXO_ZDD_B_C > 10 || EXO_ZDD_C_C > 10 || EXO_ZDD_D_C > 10 ){
//      std::cout << "test " << EXO_ZDD_A_C << "  " << EXO_ZDD_B_C << "\n";
//      std::cout << "test " << EXO_ZDD_C_C << "  " << EXO_ZDD_D_C << "\n";
//    }
    
    if(EXO_ZDD_A_C > 1){
      EXO_ZDD[0] = EXO_ZDD_A_C;
    }
    if(EXO_ZDD_B_C > 1){
      EXO_ZDD[1] = EXO_ZDD_B_C;
    }
    if(EXO_ZDD_C_C > 1){
      EXO_ZDD[2] = EXO_ZDD_C_C;
    }
    if(EXO_ZDD_D_C > 1){
      EXO_ZDD[3] = EXO_ZDD_D_C;
    }

    unsigned long long time_ref = TAC_CATS_HFTS;

    //    if(time_ref > 10){
    for(int iEXOZDD = 0; iEXOZDD < EXO_ZDDRawM ; iEXOZDD++){
      //      std::cout << "test " << EXO_ZDDRawTS[iEXOZDD] << "  " << iEXOZDD << " " << EXO_ZDDRawNr[iEXOZDD] << "\n";
      if(EXO_ZDDRawNr[iEXOZDD] == 0 && EXO_ZDD[0] > 10){
	t_EXO_ZDD[0] = EXO_ZDDRawTS[iEXOZDD];
      }
      if(EXO_ZDDRawNr[iEXOZDD] == 1 && EXO_ZDD[1] > 10){
	t_EXO_ZDD[1] = EXO_ZDDRawTS[iEXOZDD];
      }
      if(EXO_ZDDRawNr[iEXOZDD] == 2 && EXO_ZDD[2] > 10){
	t_EXO_ZDD[2] = EXO_ZDDRawTS[iEXOZDD];
      }
      if(EXO_ZDDRawNr[iEXOZDD] == 3 && EXO_ZDD[3] > 10){
	t_EXO_ZDD[3] = EXO_ZDDRawTS[iEXOZDD];
      }      
      
    }
      //    }


    /////////////////////////////// EXOGAM /////////////////////////
    //std::cout << "test " << Inner6MVM << "  " << OutersVM << "\n";
    if(Inner6MVM > 0){
      
      int ExoMultMax = 2;
      if(OutersVM < 2){
        ExoMultMax == OutersVM;
      }
      for(int ExoMult = 0; ExoMult < ExoMultMax;ExoMult++){
        ExogamDetNb[ExoMult] = (OutersVN[ExoMult] - 32)/16;
        CristalNb[ExoMult] = (OutersVN[ExoMult] - (2+ ExogamDetNb[ExoMult])*16)/4;
        SegmentNb[ExoMult] = (OutersVN[ExoMult] - 16*(ExogamDetNb[ExoMult] + 2) - 4*(CristalNb[ExoMult]));
      }
      
      if(Inner6MVM == 1){
        if(OutersVM > 0 && BGOV[0] < 20){
            Theta_seg = Exogam_Clovers_struc[ExogamDetNb[0]].Theta_Crystal_Seg[CristalNb[0]][SegmentNb[0]];
            Phi_seg = Exogam_Clovers_struc[ExogamDetNb[0]].Phi_Crystal_Seg[CristalNb[0]][SegmentNb[0]];
            EnergyDoppler = Doppler_Correction(Theta_seg,Phi_seg,0,0,Beta,Inner6MV[0]);
            EnergyAddBackDoppler = EnergyDoppler;
          }
      }
      if(Inner6MVM == 2 && OutersVM > 1 && BGOV[0] < 20 && BGOV[1] < 20){
        //std::cout << Inner6MVN[0]/4 << "  " << Inner6MVN[1]/4 << std::endl;
        //std::cout << Inner6MV[0]/4 << "  " << Inner6MV[1]/4 << std::endl;
        if(Inner6MVN[0]/4 == Inner6MVN[1]/4){
          EnergyAddBack = Inner6MV[0] + Inner6MV[1];
        }
        //if(OutersVM == 2){
        //  if(CheckExoAddBack(ExogamDetNb[0],CristalNb[0],SegmentNb[0],DeltaTV[0],ExogamDetNb[1], CristalNb[1], SegmentNb[1], DeltaTV[1])){
        //    EnergyAddBack = Inner6MV[0] + Inner6MV[1];
        //  }
        //}
        
        //if(OutersVM == 3){
        //  for(int i = 0; i < 3; i++){
        //    if(Inner6MVN[0] == ExogamDetNb[i]){
        //      event1.push_back(i);
        //    }
        //    else{
        //      event2.push_back(i);
        //    }
        //  }
        //  if(event1.size() == 2){
        //    if(CheckExoAddBack(ExogamDetNb[event1[0]],CristalNb[event1[0]],SegmentNb[event1[0]],DeltaTV[event1[0]],ExogamDetNb[event1[1]], CristalNb[event1[1]], SegmentNb[event1[1]], DeltaTV[event1[1]])){
        //      if(CheckExoAddBack(ExogamDetNb[event1[0]],CristalNb[event1[0]],SegmentNb[event1[0]],DeltaTV[event1[0]],ExogamDetNb[event1[2]], CristalNb[event1[2]], SegmentNb[event1[2]], DeltaTV[event1[2]])){
        //        EnergyAddBack = Inner6MV[0] + Inner6MV[1];
        //      }
        //      else if(CheckExoAddBack(ExogamDetNb[event1[1]],CristalNb[event1[1]],SegmentNb[event1[1]],DeltaTV[event1[1]],ExogamDetNb[event1[2]], CristalNb[event1[2]], SegmentNb[event1[2]], DeltaTV[event1[2]])){
        //        EnergyAddBack = Inner6MV[0] + Inner6MV[1];
        //      }
        //    }
        //    }
        //    else{
        //      if(CheckExoAddBack(ExogamDetNb[event2[0]],CristalNb[event2[0]],SegmentNb[event2[0]],DeltaTV[event2[0]],ExogamDetNb[event2[1]], CristalNb[event2[1]], SegmentNb[event2[1]], DeltaTV[event2[1]])){
        //        if(CheckExoAddBack(ExogamDetNb[event2[0]],CristalNb[event2[0]],SegmentNb[event2[0]],DeltaTV[event2[0]],ExogamDetNb[event2[2]], CristalNb[event2[2]], SegmentNb[event2[2]], DeltaTV[event2[2]])){
        //          EnergyAddBack = Inner6MV[0] + Inner6MV[1];
        //        }
        //        else if(CheckExoAddBack(ExogamDetNb[event2[1]],CristalNb[event2[1]],SegmentNb[event2[1]],DeltaTV[event2[1]],ExogamDetNb[event2[2]], CristalNb[event2[2]], SegmentNb[event2[2]], DeltaTV[event2[2]])){
        //          EnergyAddBack = Inner6MV[0] + Inner6MV[1];
        //        }
        //      }
        //    }
        //  }
        //event1.clear();
        //event2.clear();
        }
        if(EnergyAddBack > -1000){
          for(int i = 0;i < ExoMultMax; i++){
            if(OutersV[i] > OutersV[highest_E]){
              highest_E = i;
            }
          }
          Theta_seg = Exogam_Clovers_struc[ExogamDetNb[highest_E]].Theta_Crystal_Seg[CristalNb[highest_E]][SegmentNb[highest_E]];
          Phi_seg = Exogam_Clovers_struc[ExogamDetNb[highest_E]].Phi_Crystal_Seg[CristalNb[highest_E]][SegmentNb[highest_E]];
          EnergyAddBackDoppler = Doppler_Correction(Theta_seg,Phi_seg,0,0,Beta,EnergyAddBack);
    } //for(unsigned int ExoMult; )
  } 
}




bool Analysis::CheckExoAddBack(int ExoDet1, int ExoCristal1, int ExoSegment1, float ExoTime1, int ExoDet2, int ExoCristal2, int ExoSegment2, float ExoTime2){
  bool result = false;
  //if(abs(ExoTime1-ExoTime2) < 5){
    if(ExoDet1 == ExoDet2){
      if(ExoCristal1 == ExoCristal2){
        if((ExoSegment1 == (ExoSegment2+1)%4) || (ExoSegment1 == (ExoSegment2-1)%4) ){
          result = true;
        }      
      }
      else if((ExoCristal1 == (ExoCristal2+1)%4) || (ExoCristal1 == (ExoCristal2-1)%4)){
        if((ExoSegment1 == 2 && ExoSegment1 == ExoSegment2) || (ExoSegment1%2 == 1 && ExoSegment1 == (ExoSegment2+2)%4) ){
          result = true;
        }
      }
    }
  return result;
}

bool Analysis::CheckExoDeltaTV(float ExoTime){
  return(abs(ExoTime) < 5);
}

void Analysis::SetParticles(){
  BeamPart->SetUp(BeamName);
  BeamPart->SetKineticEnergy(InitialConditions->GetIncidentInitialKineticEnergy());
  Beta = BeamPart->GetBeta();
  Gamma = BeamPart->GetGamma();
  Velocity = BeamPart->GetVelocity();
}

bool Analysis::CheckGoodEvent(){

  return //CheckIC() && CheckPlastics() && CheckDC();  
  true;
}

bool Analysis::CheckIC(){

  std::vector<int> IC_vec(ZDD->GetICcounter(), 0);
  bool result = true; 
  for(int i = 0; i < ZDD->IC_DetectorNumber.size(); i++){
    if(ZDD->IC_Energy[i] > 0){
      IC_vec[ZDD->IC_DetectorNumber[i]]++; 
    }

  }
  for(int i = 0; i < ZDD->GetICcounter(); i++){
    if(IC_vec[i] != 1){
      result = false; 
    }
  }
  return result;
}
bool Analysis::CheckPlastics(){
  int counter = 0;
  for(int i = 0; i < ZDD->Plastic_DetectorNumber.size(); i++){
    if(ZDD->Plastic_Energy[i] > 0){
      counter++;
    }
  }
  if(counter == 1){
    return true;
  }
  else{
    return false;
  }  
}

bool Analysis::CheckDC(){
  int DC1_counter = 0, DC2_counter = 0;
  for(int i = 0; i < ZDD->DC_DetectorNumber.size(); i++){
    if(ZDD->DC_DetectorNumber[i] == 1){
      DC1_counter++;
    }
    else if(ZDD->DC_DetectorNumber[i] == 2){
      DC2_counter++;
    }
  }
  return DC2_counter == 1 && DC1_counter == 1;
}

void Analysis::InitOutputBranch() {
  /////////////////////// ZDD related branch ////////////////////////////////
  //RootOutput::getInstance()->GetTree()->Branch("MUST2_Raw",&M2_Raw);
  RootOutput::getInstance()->GetTree()->Branch("ZDD_E_tot",&ZDD_E_tot,"ZDD_E_tot/D");
  RootOutput::getInstance()->GetTree()->Branch("ZDD_Corrected_IC_E",&ZDD_Corrected_IC_E);
  RootOutput::getInstance()->GetTree()->Branch("ZDD_dE_tot",&ZDD_dE_tot,"ZDD_dE_tot/D");
  RootOutput::getInstance()->GetTree()->Branch("ZDD_E_Plastic",&ZDD_E_Plastic,"ZDD_E_Plastic/D");
  RootOutput::getInstance()->GetTree()->Branch("ZDD_DC_X",&ZDD_DC_X,"ZDD_DC_X/D");
  RootOutput::getInstance()->GetTree()->Branch("ZDD_DC_Y",&ZDD_DC_Y,"ZDD_DC_Y/D");
  RootOutput::getInstance()->GetTree()->Branch("xtarget",&xtarget,"xtarget/D");
  RootOutput::getInstance()->GetTree()->Branch("ytarget",&ytarget,"ytarget/D");
  RootOutput::getInstance()->GetTree()->Branch("Beta",&Beta,"Beta/D");
  RootOutput::getInstance()->GetTree()->Branch("ZDD_ThetaAfterTarget",&ZDD_ThetaAfterTarget,"ZDD_ThetaAfterTarget/D");
  RootOutput::getInstance()->GetTree()->Branch("ZDD_ThetaAfterTarget_X",&ZDD_ThetaAfterTarget_X,"ZDD_ThetaAfterTarget_X/D");
  RootOutput::getInstance()->GetTree()->Branch("ZDD_ThetaLab",&ZDD_ThetaLab,"ZDD_ThetaLab/D");
  //RootOutput:: getInstance()->GetTree()->Branch("InitialConditions",&InitialConditions);
  //RootOutput:: getInstance()->GetTree()->Branch("ReactionConditions",&ReactionConditions);
  RootOutput::getInstance()->GetTree()->Branch("IC_ZDD_sum",&IC_ZDD_sum,"IC_ZDD_sum/I");
  RootOutput::getInstance()->GetTree()->Branch("PL_center",&PL_center,"PL_center/D");
  RootOutput::getInstance()->GetTree()->Branch("gf",&gf,"gf/I");

  //////////////////////// MUST related branch ////////////////////////////////
  RootOutput::getInstance()->GetTree()->Branch("M2_TelescopeM",&M2_TelescopeM,"M2_TelescopeM/s");
  RootOutput::getInstance()->GetTree()->Branch("M2_Ex",&M2_Ex);
  //RootOutput::getInstance()->GetTree()->Branch("ExNoBeam",&ExNoBeam,"ExNoBeam/D");
  //RootOutput::getInstance()->GetTree()->Branch("ExNoProton",&ExNoProton,"ExNoProton/D");
  //RootOutput::getInstance()->GetTree()->Branch("EDC",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("M2_ELab",&M2_ELab);
  RootOutput::getInstance()->GetTree()->Branch("M2_ThetaLab",&M2_ThetaLab);
  RootOutput::getInstance()->GetTree()->Branch("M2_ThetaCM",&M2_ThetaCM);
  RootOutput::getInstance()->GetTree()->Branch("M2_X",&M2_X);
  RootOutput::getInstance()->GetTree()->Branch("M2_Y",&M2_Y);
  RootOutput::getInstance()->GetTree()->Branch("M2_Z",&M2_Z);
  RootOutput::getInstance()->GetTree()->Branch("M2_dE",&M2_dE);
  RootOutput::getInstance()->GetTree()->Branch("M2_ECsI_from_deltaE",&M2_ECsI_from_deltaE);

  RootOutput::getInstance()->GetTree()->Branch("MG_DetM",&MG_DetM,"MG_DetM/s");
  RootOutput::getInstance()->GetTree()->Branch("MG_Ex",&MG_Ex);
  RootOutput::getInstance()->GetTree()->Branch("MG_ELab",&MG_ELab);
  RootOutput::getInstance()->GetTree()->Branch("MG_ThetaLab",&MG_ThetaLab);
  RootOutput::getInstance()->GetTree()->Branch("MG_ThetaCM",&MG_ThetaCM);
  RootOutput::getInstance()->GetTree()->Branch("MG_X",&MG_X);
  RootOutput::getInstance()->GetTree()->Branch("MG_Y",&MG_Y);
  RootOutput::getInstance()->GetTree()->Branch("MG_Z",&MG_Z);

  RootOutput::getInstance()->GetTree()->Branch("Ed",&Ed,"Ed/D");
  RootOutput::getInstance()->GetTree()->Branch("Si_E_d",&Si_E_d,"Si_E_d/D");
  RootOutput::getInstance()->GetTree()->Branch("CsI_E_d",&CsI_E_d,"CsI_E_d/D");
  RootOutput::getInstance()->GetTree()->Branch("gd",&gd,"gd/I");

  RootOutput::getInstance()->GetTree()->Branch("Etot_d",&Etot_d,"Etot_d/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab_d",&ThetaLab_d,"ThetaLab_d/D");
  RootOutput::getInstance()->GetTree()->Branch("Ex_d",&Ex_d,"Ex_d/D");
  RootOutput::getInstance()->GetTree()->Branch("Ex_dc",&Ex_dc,"Ex_dc/D");
  RootOutput::getInstance()->GetTree()->Branch("Beam_E",&Beam_E,"Beam_E/D");

  RootOutput::getInstance()->GetTree()->Branch("Etot_p",&Etot_p,"Etot_p/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab_p",&ThetaLab_p,"ThetaLab_p/D");
  RootOutput::getInstance()->GetTree()->Branch("Ex_p",&Ex_p,"Ex_p/D");
  RootOutput::getInstance()->GetTree()->Branch("Ex_pc",&Ex_pc,"Ex_pc/D");

  
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_X_M",&Strip_X_M,"Strip_X_M/I");
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_X_E",Strip_X_E,"Strip_X_E[Strip_X_M]/F");
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_X_T",Strip_X_T,"Strip_X_T[Strip_X_M]/F");
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_X_Nb",Strip_X_Nb,"Strip_X_Nb[Strip_X_M]/i");
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_X_Det",Strip_X_Det,"Strip_X_Det[Strip_X_M]/s");
  
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_Y_M",&Strip_Y_M,"Strip_Y_M/I");
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_Y_E",Strip_Y_E,"Strip_Y_E[Strip_Y_M]/F");
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_Y_T",Strip_Y_T,"Strip_Y_T[Strip_Y_M]/F");
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_Y_Nb",Strip_Y_Nb,"Strip_Y_Nb[Strip_Y_M]/i");
  //RootOutput:: getInstance()->GetTree()->Branch("Strip_Y_Det",Strip_Y_Det,"Strip_Y_Det[Strip_Y_M]/s");
  
  RootOutput:: getInstance()->GetTree()->Branch("Inner6MVM",&Inner6MVM,"Inner6MVM/I");
  RootOutput:: getInstance()->GetTree()->Branch("Inner6MV",Inner6MV,"Inner6MV[Inner6MVM]/F");
  RootOutput:: getInstance()->GetTree()->Branch("Inner6MVN",Inner6MVN,"Inner6MVN[Inner6MVM]/s");
  RootOutput:: getInstance()->GetTree()->Branch("Inner6MVTS",Inner6MVTS,"Inner6MVTS[Inner6MVM]/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("BGOVM",&BGOVM,"BGOVM/I");
  RootOutput:: getInstance()->GetTree()->Branch("BGOV",BGOV,"BGOV[BGOVM]/F");
  RootOutput:: getInstance()->GetTree()->Branch("BGOVN",BGOVN,"BGOVN[BGOVM]/s");
  
  
  RootOutput:: getInstance()->GetTree()->Branch("CATS1_X",&CATS1_X,"CATS1_X/F");
  RootOutput:: getInstance()->GetTree()->Branch("CATS2_X",&CATS2_X,"CATS2_X/F");
  RootOutput:: getInstance()->GetTree()->Branch("CATS1_Y",&CATS1_Y,"CATS1_Y/F");
  RootOutput:: getInstance()->GetTree()->Branch("CATS2_Y",&CATS2_Y,"CATS2_Y/F");
  
  RootOutput:: getInstance()->GetTree()->Branch("Xf",&Xf,"Xf/F");
  RootOutput:: getInstance()->GetTree()->Branch("Yf",&Yf,"Yf/F");
  RootOutput:: getInstance()->GetTree()->Branch("Tf",&Tf,"Tf/F");
  RootOutput:: getInstance()->GetTree()->Branch("Pf",&Pf,"Pf/F");
  
  RootOutput:: getInstance()->GetTree()->Branch("DeltaTVM",&DeltaTVM,"DeltaTVM/I");
  RootOutput:: getInstance()->GetTree()->Branch("DeltaTV",DeltaTV,"DeltaTV[DeltaTVM]/F");
  RootOutput:: getInstance()->GetTree()->Branch("DeltaTVN",DeltaTVN,"DeltaTVN[DeltaTVM]/s");
  RootOutput:: getInstance()->GetTree()->Branch("DeltaTVTS",DeltaTVTS,"DeltaTVTS[DeltaTVM]/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("MUGAST_TS",MUGAST_TS,"MUGAST_TS[1]/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("DATATRIG_CATSTS",DATATRIG_CATSTS,"DATATRIG_CATSTS[1]/l");
  RootOutput:: getInstance()->GetTree()->Branch("DATATRIG_CATS",DATATRIG_CATS,"DATATRIG_CATS[1]/s");
  RootOutput:: getInstance()->GetTree()->Branch("DATATRIG1",DATATRIG1,"DATATRIG1[1]/s");
  RootOutput:: getInstance()->GetTree()->Branch("DATATRIG1TS",DATATRIG1TS,"DATATRIG1TS[1]/l");
  RootOutput:: getInstance()->GetTree()->Branch("GATCONFMASTER",GATCONFMASTER,"GATCONFMASTER[1]/s");
  RootOutput:: getInstance()->GetTree()->Branch("GATCONFMASTERTS",GATCONFMASTERTS,"GATCONFMASTERTS[1]/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_CATS_PL",&TAC_CATS_PL,"TAC_CATS_PL/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_CATS_PLTS",&TAC_CATS_PLTS,"TAC_CATS_PLTS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_CATS_HF",&TAC_CATS_HF,"TAC_CATS_HF/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_CATS_HFTS",&TAC_CATS_HFTS,"TAC_CATS_HFTS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_CATS_EXOGAM",&TAC_CATS_EXOGAM,"TAC_CATS_EXOGAM/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_CATS_EXOGAMTS",&TAC_CATS_EXOGAMTS,"TAC_CATS_EXOGAMTS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_MMG_CATS2",&TAC_MMG_CATS2,"TAC_MMG_CATS2/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_MMG_CATS2TS",&TAC_MMG_CATS2TS,"TAC_MMG_CATS2TS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_MMG_CATS1",&TAC_MMG_CATS1,"TAC_MMG_CATS1/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_MMG_CATS1TS",&TAC_MMG_CATS1TS,"TAC_MMG_CATS1TS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_MMG_EXOGAM",&TAC_MMG_EXOGAM,"TAC_MMG_EXOGAM/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_MMG_EXOGAMTS",&TAC_MMG_EXOGAMTS,"TAC_MMG_EXOGAMTS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_CATS1_CATS2",&TAC_CATS1_CATS2,"TAC_CATS1_CATS2/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_CATS1_CATS2TS",&TAC_CATS1_CATS2TS,"TAC_CATS1_CATS2TS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_D4_CATS1",&TAC_D4_CATS1,"TAC_D4_CATS1/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_D4_CATS1TS",&TAC_D4_CATS1TS,"TAC_D4_CATS1TS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_1",&TAC_PL_1,"TAC_PL_1/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_1TS",&TAC_PL_1TS,"TAC_PL_1TS/l");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_2",&TAC_PL_2,"TAC_PL_2/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_2TS",&TAC_PL_2TS,"TAC_PL_2TS/l");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_3",&TAC_PL_3,"TAC_PL_3/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_3TS",&TAC_PL_3TS,"TAC_PL_3TS/l");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_4",&TAC_PL_4,"TAC_PL_4/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_4TS",&TAC_PL_4TS,"TAC_PL_4TS/l");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_5",&TAC_PL_5,"TAC_PL_5/s");
  RootOutput:: getInstance()->GetTree()->Branch("TAC_PL_5TS",&TAC_PL_5TS,"TAC_PL_5TS/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("EnergyDoppler",&EnergyDoppler,"EnergyDoppler/F");
  //RootOutput:: getInstance()->GetTree()->Branch("EnergyAddBack",&EnergyAddBack,"EnergyAddBack/F");
  RootOutput:: getInstance()->GetTree()->Branch("EnergyAddBackDoppler",&EnergyAddBackDoppler,"EnergyAddBackDoppler/F");
  
  RootOutput:: getInstance()->GetTree()->Branch("OutersVM",&OutersVM,"OutersVM/I");
  RootOutput:: getInstance()->GetTree()->Branch("OutersV",OutersV,"OutersV[OutersVM]/F");
  RootOutput:: getInstance()->GetTree()->Branch("OutersVN",OutersVN,"OutersVN[OutersVM]/s");
  
  RootOutput:: getInstance()->GetTree()->Branch("DCRawM",&DCRawM,"DCRawM/I");
  RootOutput:: getInstance()->GetTree()->Branch("DCRaw",DCRaw,"DCRaw[DCRawM]/s");
  RootOutput:: getInstance()->GetTree()->Branch("DCRawNr",DCRawNr,"DCRawNr[DCRawM]/s");
  RootOutput:: getInstance()->GetTree()->Branch("DCRawTS",DCRawTS,"DCRawTS[DCRawM]/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("DC_X",&DC_X,"DC_X/D");
  RootOutput:: getInstance()->GetTree()->Branch("DC_Y",&DC_Y,"DC_Y/D");
  
  RootOutput:: getInstance()->GetTree()->Branch("PlasticRaw",PlasticRaw,"PlasticRaw[10]/s");
  RootOutput:: getInstance()->GetTree()->Branch("PlasticRawTS",PlasticRawTS,"PlasticRawTS[10]/l");
  RootOutput:: getInstance()->GetTree()->Branch("PlasticCal",PlasticCal,"PlasticCal[10]/F");
  RootOutput:: getInstance()->GetTree()->Branch("PlasticEnerM",&PlasticEnerM,"PlasticEnerM/I");
  RootOutput:: getInstance()->GetTree()->Branch("PlasticEner",PlasticEner,"PlasticEner[PlasticEnerM]/F");
  RootOutput:: getInstance()->GetTree()->Branch("PlasticEnerN",PlasticEnerN,"PlasticEnerN[PlasticEnerM]/s");
  RootOutput:: getInstance()->GetTree()->Branch("PlasticEnerTS",PlasticEnerTS,"PlasticEnerTS[PlasticEnerM]/l");
  
  RootOutput:: getInstance()->GetTree()->Branch("IC_ZDDRaw",IC_ZDDRaw,"IC_ZDDRaw[6]/s");
  RootOutput:: getInstance()->GetTree()->Branch("IC_ZDDRawTS",IC_ZDDRawTS,"IC_ZDDRawTS[6]/l");
  RootOutput:: getInstance()->GetTree()->Branch("ICCal",ICCal,"ICCal[4]/F");

  RootOutput:: getInstance()->GetTree()->Branch("EXO_ZDD",&EXO_ZDD,"EXO_ZDD[4]/F");
  RootOutput:: getInstance()->GetTree()->Branch("t_EXO_ZDD",&t_EXO_ZDD,"t_EXO_ZDD[4]/l");

}

void Analysis::InitInputBranch(){
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("*",false);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("Inner6MVM",&Inner6MVM);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("Inner6MV",Inner6MV);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("Inner6MVN",Inner6MVN);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("Inner6MVTS",Inner6MVTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("BGOVM",&BGOVM);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("BGOV",BGOV);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("BGOVN",BGOVN);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DeltaTVM",&DeltaTVM);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DeltaTV",DeltaTV);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DeltaTVN",DeltaTVN);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DeltaTVTS",DeltaTVTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("CATS1_X",&CATS1_X);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("CATS2_X",&CATS2_X);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("CATS1_Y",&CATS1_Y);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("CATS2_Y",&CATS2_Y);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("Xf",&Xf);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("Yf",&Yf);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("Pf",&Pf);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("Tf",&Tf);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("MUGAST_TS",MUGAST_TS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DATATRIG_CATSTS",DATATRIG_CATSTS);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DATATRIG_CATS",DATATRIG_CATS);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DATATRIG1",DATATRIG1);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DATATRIG1TS",DATATRIG1TS);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("GATCONFMASTER",GATCONFMASTER);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("GATCONFMASTERTS",GATCONFMASTERTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("OutersVM",&OutersVM);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("OutersV",OutersV);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("OutersVN",OutersVN);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DCRawM",&DCRawM);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DCRaw",DCRaw);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DCRawNr",DCRawNr);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("DCRawTS",DCRawTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("PlasticRaw",PlasticRaw);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("PlasticRawTS",PlasticRawTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("IC_ZDDRaw",IC_ZDDRaw);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("IC_ZDDRawTS",IC_ZDDRawTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_CATS_PL",&TAC_CATS_PL);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_CATS_PLTS",&TAC_CATS_PLTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_CATS_HF",&TAC_CATS_HF);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_CATS_HFTS",&TAC_CATS_HFTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_CATS_EXOGAM",&TAC_CATS_EXOGAM);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_CATS_EXOGAMTS",&TAC_CATS_EXOGAMTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_MMG_CATS2",&TAC_MMG_CATS2);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_MMG_CATS2TS",&TAC_MMG_CATS2TS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_MMG_CATS1",&TAC_MMG_CATS1);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_MMG_CATS1TS",&TAC_MMG_CATS1TS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_MMG_EXOGAM",&TAC_MMG_EXOGAM);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_MMG_EXOGAMTS",&TAC_MMG_EXOGAMTS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_CATS1_CATS2",&TAC_CATS1_CATS2);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_CATS1_CATS2TS",&TAC_CATS1_CATS2TS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_D4_CATS1",&TAC_D4_CATS1);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_D4_CATS1TS",&TAC_D4_CATS1TS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_1",&TAC_PL_1);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_1TS",&TAC_PL_1TS);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_2",&TAC_PL_2);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_2TS",&TAC_PL_2TS);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_3",&TAC_PL_3);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_3TS",&TAC_PL_3TS);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_4",&TAC_PL_4);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_4TS",&TAC_PL_4TS);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_5",&TAC_PL_5);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("TAC_PL_5TS",&TAC_PL_5TS);
  
  RootInput:: getInstance()->GetChain()->SetBranchAddress("EXO_ZDD_A_C",&EXO_ZDD_A_C);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("EXO_ZDD_B_C",&EXO_ZDD_B_C);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("EXO_ZDD_C_C",&EXO_ZDD_C_C);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("EXO_ZDD_D_C",&EXO_ZDD_D_C);

  RootInput:: getInstance()->GetChain()->SetBranchAddress("EXO_ZDDRawM",&EXO_ZDDRawM);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("EXO_ZDDRawNr",EXO_ZDDRawNr);
  RootInput:: getInstance()->GetChain()->SetBranchAddress("EXO_ZDDRawTS",EXO_ZDDRawTS);
  
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("Inner6MVM",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("Inner6MV",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("Inner6MVN",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("Inner6MVTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("BGOVM",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("BGOV",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("BGOVN",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DeltaTVM",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DeltaTV",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DeltaTVN",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DeltaTVTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("CATS1_X",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("CATS2_X",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("CATS1_Y",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("CATS2_Y",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("Xf",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("Yf",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("Pf",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("Tf",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("MUGAST_TS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DATATRIG_CATSTS",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DATATRIG_CATS",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DATATRIG1",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DATATRIG1TS",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("GATCONFMASTER",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("GATCONFMASTERTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("OutersVM",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("OutersV",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("OutersVN",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DCRawM",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DCRaw",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DCRawNr",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("DCRawTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("PlasticRaw",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("PlasticRawTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("IC_ZDDRaw",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("IC_ZDDRawTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_CATS_PL",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_CATS_PLTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_CATS_HF",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_CATS_HFTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_CATS_EXOGAM",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_CATS_EXOGAMTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_MMG_CATS2",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_MMG_CATS2TS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_MMG_CATS1",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_MMG_CATS1TS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_MMG_EXOGAM",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_MMG_EXOGAMTS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_CATS1_CATS2",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_CATS1_CATS2TS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_D4_CATS1",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_D4_CATS1TS",true);
  //
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_1",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_1TS",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_2",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_2TS",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_3",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_3TS",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_4",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_4TS",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_5",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("TAC_PL_5TS",true);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("AD.MUST2",true );
  //RootInput:: getInstance()->GetChain()->SetBranchAddress("MUST2",&M2_Raw);
  
  
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
  //RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&InitialConditions);
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("ReactionConditions",true );
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("fRC_*",true );
  //RootInput:: getInstance()->GetChain()->SetBranchAddress("ReactionConditions",&ReactionConditions);
}
////////////////////////////////////////////////////////////////////////////////

void Analysis::ReInit(){
  //ZDD_E_tot = -1000;
  //ZDD_E_Plastic = -1000;
  //ZDD_dE_tot= -1000;
  //ZDD_DC_X = -1000;
  //ZDD_DC_Y = -1000;
  //ZDD_ThetaAfterTarget = -1000;
  //ZDD_ThetaAfterTarget_X = -1000;
  //ZDD_ThetaLab= -1000;
  //ZDD_ThetaIC= -1000;
  xtarget = -1000;
  ytarget = -1000;
  //Beta = -1000;

  Theta_seg = -1000;
  Phi_seg = -1000;
  M2_TelescopeM= 0; 
  M2_Ex.clear();
  M2_ECsI_from_deltaE.clear();
  //ExNoBeam=ExNoProto.clear();
  //EDC.clear();
  M2_ELab.clear();
  //BeamEnergy .clear();
  M2_ThetaLab.clear();
  M2_ThetaCM.clear();
  M2_X.clear();
  M2_Y.clear();
  M2_Z.clear();
  M2_dE.clear();

  MG_DetM= 0; 
  MG_Ex.clear();
  MG_ELab.clear();
  //BeamEnergy .clear();
  MG_ThetaLab.clear();
  MG_ThetaCM.clear();
  MG_X.clear();
  MG_Y.clear();
  MG_Z.clear();

  EnergyDoppler = -1000;
  EnergyAddBack = -1000;
  EnergyAddBackDoppler = -1000;
  highest_E = 0;

  DC_X = -1000;
  DC_Y = -1000;

  PlasticEnerM = 0;
  PlasticCounter = 0;
  for(int i = 0; i < 5; i++){
    PlasticEner[i] = -1000;
    PlasticEnerN[i] = 10;
    PlasticEnerTS[i] = 0;
  }

  for(int i = 0; i < 4; i++){
    EXO_ZDD[i] = -1000.;
    t_EXO_ZDD[i] = 0;
  }

  Ed = -1000.; 

  gd = 0;
  gf = 0;
  
  Si_E_d = -1000.; 
  CsI_E_d = -1000.; 
  
  Etot_d = -1000.; 
  ThetaLab_d = -1000.; 
  Ex_d = -1000.; 
  Ex_dc = -1000.; 

  Etot_p = -1000.; 
  ThetaLab_p = -1000.; 
  Ex_p = -1000.; 
  Ex_pc = -1000.; 

  IC_ZDD_sum = -1000.; 
  PL_center -1000.; 

  Beam_E = -1000.;
  //for(int i =0; i < ZDD->Get_m_Ionisation_Chamber_Z().size(); i++){
  //  ZDD_Corrected_IC_E[i] = 0;
  //}
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VAnalysis* Analysis::Construct(){
  return (NPL::VAnalysis*) new Analysis();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy{
  public:
    proxy(){
      NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
    }
};

proxy p;
}


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
    ZDD= (TZDDPhysics*) m_DetectorManager->GetDetector("ZDD");
    MG = (TMugastPhysics*)  m_DetectorManager -> GetDetector("Mugast");
    //Exogam = (TExogamPhysics*)  m_DetectorManager -> GetDetector("Exogam");
    TAC = (TTACPhysics*)  m_DetectorManager -> GetDetector("TAC");
    M2 = (TMust2Physics*)  m_DetectorManager -> GetDetector("M2Telescope");

    reaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
    OriginalBeamEnergy = reaction->GetBeamEnergy();
    OriginalBeamEnergync = reaction->GetBeamEnergy();

    string Path = "../../Inputs/EnergyLoss/";
    string TargetMaterial = m_DetectorManager->GetTargetMaterial();
    std::cout << " target " <<  TargetMaterial << "\n";
    TargetThickness = m_DetectorManager->GetTargetThickness();
    string beam=  NPL::ChangeNameToG4Standard(reaction->GetNucleus1()->GetName());
    string heavy_ejectile=  NPL::ChangeNameToG4Standard(reaction->GetNucleus4()->GetName());
    string light=NPL::ChangeNameToG4Standard(reaction->GetNucleus3()->GetName());



    Beam_Target = NPL::EnergyLoss(beam+"_"+TargetMaterial+".G4table","G4Table",100);
    //Heavy_Target = NPL::EnergyLoss(heavy_ejectile+"_"+TargetMaterial+".G4table","G4Table",100);
    //LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".G4table","G4Table",100 );
    HeavyMylar = NPL::EnergyLoss(beam+"_Mylar.G4table","G4Table",100);
    LightAl = NPL::EnergyLoss(light+"_Al.G4table","G4Table",100);
    LightSi = NPL::EnergyLoss(light+"_Si.G4table","G4Table",100);

    //ProtonSi = NPL::EnergyLoss(Path+ "proton_Si.G4table", "G4Table", 100);
    std::cout << " target " <<  TargetMaterial << "\n";
    Cal = CalibrationManager::getInstance();
    IsPhysics = NPOptionManager::getInstance()->GetInputPhysicalTreeOption();

    ReadConfigTS();
}

////////////////////////////////////////////////////////////////////////////////

bool Analysis::UnallocateBeforeBuild(){
    GATCONFMASTER.clear();
    GATCONFMASTERTS.clear();
    GATCONFMASTER = **GATCONFMASTER_;
    GATCONFMASTERTS = **GATCONFMASTERTS_;
    //return true;        //PS: comment this line out to let it flow to the next lines
    if(GATCONFMASTER.size()==1){
	// if(GATCONFMASTER[0]==0 || GATCONFMASTER[0]==1 || GATCONFMASTER[0]==2 || GATCONFMASTER[0]==16 || GATCONFMASTER[0]==32)
    //  if (GATCONFMASTER[0]==2)
    	// {
        	//SetRefTS();
            cout<<"TS is: "<<GATCONFMASTERTS[0]<<" ";

        	return true;
    	}
    	// else {return false;}
    	// }
    else
	return false;
	}
bool Analysis::UnallocateBeforeTreat(){

  //  if( MG->DSSD_E.size() == 1)

 //   {
        cout<<endl;
        return true;
 //   }
 //   else{
  //      return false;
  //  }

}

bool Analysis::FillOutputCondition(){
    //return bCATS;
    return true;
}

void Analysis::ReadConfigTS(){
    TSEvent->ReadConfigurationFile();
    ZDD->ReadConfigurationTS();
    TAC->ReadConfigurationTS();
    //Exogam->ReadConfigurationTS();
}

void Analysis::TreatEvent(){
    ///int iii=0;
    ReInit();
    ///iii++;

    TreatTAC(); //Originally after TreatZDD(): PS May 11, 2025
    TreatCATS();
    //TreatTAC();
    //TreatMugast();
    TreatZDD();
    


    if(bCATS){

      //TreatMugast();
      //TreatEXO();
      //TreatZDD();
      //TreatMugast2();
    }



}


void Analysis::TreatCATS(){
    //std::cout << " cats posY " << CATS->PositionOnTargetY  << "\n";
    
    

    if(CATS->PositionOnTargetX > -1000 && CATS->PositionOnTargetY > -1000){
        slopeCATSx=0;
        slopeCATSy=0;
        CATS1_X=0;
        CATS2_X=0;
        CATS1_Y=0;
        CATS2_Y=0;
        CATS1_X =  CATS->PositionX[0];
        CATS1_Y =  CATS->PositionY[0];
        CATS2_X =  CATS->PositionX[1];
        CATS2_Y =  CATS->PositionY[1];
        CATS2Y_CATS2X->Fill(CATS2_X, CATS2_Y);
        CATS1Y_CATS1X->Fill(CATS1_X, CATS1_Y);
    	BeamImpact = TVector3(-(CATS->PositionOnTargetX),-(CATS->PositionOnTargetY),0);
        //BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.7,-(CATS->PositionOnTargetY)+2.3,0); ///correctone cats3   ====>>>>> OZGE was using this: PS April 23, 2025
        //   BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.5,-(CATS->PositionOnTargetY)+3,0); ///correctone cats9
        //   BeamImpact = TVector3((CATS->PositionOnTargetX)-1.835,(CATS->PositionOnTargetY)-1.51,0); ///new one cast2
        //  BeamImpact = TVector3((CATS->PositionOnTargetX),(CATS->PositionOnTargetY),0); ///new one cats1
        // BeamImpact = TVector3(-(CATS->PositionOnTargetX),(CATS->PositionOnTargetY),0); ///new one cats6
        //  BeamImpact = TVector3((CATS->PositionOnTargetX)-1.21,(CATS->PositionOnTargetY)-4.35,0); ///new one cats4
        // BeamImpact = TVector3(-(CATS->PositionOnTargetX),(CATS->PositionOnTargetY)-4,0); ///new one cats5
        //BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.5,(CATS->PositionOnTargetY)-3,0); ///new one cats8 target centered
        //BeamImpact = TVector3(-(CATS->PositionOnTargetX)-0.5,(CATS->PositionOnTargetY)-1.3,0);//cats7
        BeamDirection = TVector3(CATS1_X - CATS2_X, CATS1_Y - CATS2_Y, -(CATS->PositionZ[0] - CATS->PositionZ[1]));
        
        slopeCATSx= (Position[1] - Position[0])/(CATS2_X - CATS1_X);
        slopeCATSy= (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);

        if(TAC_CATS_PL>0){
            for(int k= Position[0]; k<Position[5]; k+=10){
            Trajectory_XZ->Fill(k, CATS1_X + (k - Position[0])/slopeCATSx);
            Trajectory_YZ->Fill(k, CATS1_Y + (k - Position[0])/slopeCATSy);
            }
    
            PL_spot_const->Fill((CATS1_X + (Position[5] - Position[0])/slopeCATSx),(CATS1_Y + (Position[5] - Position[0])/slopeCATSy));
        }
        // std::cout << " BeamImpact " << BeamImpact.X() << "  " << BeamImpact.Y() <<  "	"<< BeamImpact.Z() << "\n";
        //  std::cout << " BeamDirection " << BeamDirection.X() << "  " << BeamDirection.Y() <<  "	"<< BeamDirection.Z() << std::endl;
        bCATS = true; /* cout<<"bcats returned true"<<endl; countPS_bcats++; cout<<"count ps_bcats: "<<countPS_bcats<<" "<<endl;*/
    }
    else {bCATS = false; /*cout<<"bcats returned false"<<endl;*/}
}
void Analysis::TreatTAC(){
   // TAC_TOF = false;
   norm_length=0.0;
    for(unsigned int i = 0; i < TAC->TAC_Time.size(); i++){
        if(TAC->TAC_Name[i].compare("TAC_CATS_PL") == 0)
        {
           // cout<<"the incoming cats_pl value is: "<<TAC->TAC_Time[i]<<endl;
            TAC_CATS_PL += (TAC->TAC_Time[i]*0.01306);
            TAC_CATS_PL_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_CATS2_CATS1") == 0)
        {
           // cout<<"the incoming c333 value is: "<<TAC->TAC_Time[i]<<endl;
            TAC_CATS2_CATS1 += (TAC->TAC_Time[i]*0.00635);
            TAC_CATS2_CATS1_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_MMG_CATS1") == 0)
        {
            TAC_MMG_CATS1 += (TAC->TAC_Time[i]*0.01327);
            TAC_MMG_CATS1_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_MMG_CATS2") == 0)
        {
            TAC_MMG_CATS2 += (TAC->TAC_Time[i]*0.01369);
            TAC_MMG_CATS2_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_MMG_EXOGAM") == 0)
        {
            TAC_MMG_EXOGAM += (TAC->TAC_Time[i]*0.01346);
            TAC_MMG_EXOGAM_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_CATS_EXOGAM") == 0)
        {
            TAC_CATS_EXOGAM += (TAC->TAC_Time[i]*0.01281);
            TAC_CATS_EXOGAM_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_CATS_HF") == 0)
        {
            TAC_CATS_HF += (TAC->TAC_Time[i]*0.0014);
            TAC_CATS_HF_TS.push_back(TAC->TAC_TS[i]);
        }
        else if (TAC->TAC_Name[i].compare("TAC_D4_CATS1") == 0) {
            //cout<<"the incoming gdfsgdf value is: "<<TAC->TAC_Time[i]<<endl;
            TAC_D4_CATS1 += (TAC->TAC_Time[i]*0.02554);
            TAC_D4_CATS1_TS.push_back(TAC->TAC_TS[i]);
           //TAC_D4_CATS1nc = TAC->TAC_Time[i];
           // if (TAC_D4_CATS1nc > 0) TAC_TOF = true;
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_1") == 0)
        {
            TAC_PL_1 += (TAC->TAC_Time[i])+0670.0-19395;
            TAC_PL_1_TS.push_back(TAC->TAC_TS[i]);
            norm_length=((TAC_PL_1-L_Bins[0])/perbin_value[0])-10;
            h2dPL->Fill(norm_length,5);
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_2") == 0)
        {
            TAC_PL_2 += (TAC->TAC_Time[i])+0105.0-19395;
            TAC_PL_2_TS.push_back(TAC->TAC_TS[i]);
            norm_length=((TAC_PL_2-L_Bins[1])/perbin_value[1])-10;
            h2dPL->Fill(norm_length,4);
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_3") == 0)
        {
            TAC_PL_3 += (TAC->TAC_Time[i])+0000.0-19395;
            TAC_PL_3_TS.push_back(TAC->TAC_TS[i]);
            norm_length=((TAC_PL_3-L_Bins[2])/perbin_value[2])-10;
            h2dPL->Fill(norm_length,3);
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_4") == 0)
        {
            TAC_PL_4 += (TAC->TAC_Time[i])+1217.0-19395;
            TAC_PL_4_TS.push_back(TAC->TAC_TS[i]);
            norm_length=((TAC_PL_4-L_Bins[3])/perbin_value[3])-10;
            h2dPL->Fill(norm_length,2);
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_5") == 0)
        {
            TAC_PL_5 += (TAC->TAC_Time[i])+0474.0-19395;
            TAC_PL_5_TS.push_back(TAC->TAC_TS[i]);
            norm_length=((TAC_PL_5-L_Bins[4])/perbin_value[4])-10;
            h2dPL->Fill(norm_length,1);
        }
        else if(TAC->TAC_Name[i].compare("TAC_DD6_DD4") == 0)
        {
            TAC_DD6_DD4 += (TAC->TAC_Time[i]*0.00749);
            TAC_DD6_DD4_TS.push_back(TAC->TAC_TS[i]);

        }
        else if(TAC->TAC_Name[i].compare("TAC_CATSD6_1_DD4") == 0)
        {
            TAC_CATSD6_1_DD4 += (TAC->TAC_Time[i]*0.00691);
            TAC_CATSD6_1_DD4_TS.push_back(TAC->TAC_TS[i]);
        }
    }
}


void Analysis::TreatZDD(){
    
    int IC_M = ZDD->IC_E.size();
    for (int i=0; i<IC_M;i++){
        if(ZDD->IC_Nbr[i]==0)      {IC_1.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==1) {IC_2.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==2) {IC_3.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==3) {IC_4.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==4) {IC_5.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==5) {IC_9.push_back(ZDD->IC_E[i]);}

    }

    //IC_ZDD_sum = ZDD->SUMofIC_PS;
    if (!(IC_1.empty()) && !(IC_2.empty()) && !(IC_3.empty()) && !(IC_4.empty()) && !(IC_5.empty())){
    IC_ZDD_sum =  IC_1[0] + IC_2[0] + IC_3[0] + IC_4[0] + IC_5[0];
    }

    for(int i; i<ZDD->PL_E.size() ; i++){
        if(ZDD->PL_Nbr[i]==0) PL_E1.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==1) PL_E2.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==2) PL_E3.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==3) PL_E4.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==4) PL_E5.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==5) PL_E6.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==6) PL_E7.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==7) PL_E8.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==8) PL_E9.push_back(ZDD->PL_E[i]);
        else if(ZDD->PL_Nbr[i]==9) PL_E10.push_back(ZDD->PL_E[i]);
        //PL_center= TMath::Sqrt(PL_E3[i]*PL_E8[i]);
        //PL_centerLog = TMath::Log(PL_E3[i]/PL_E8[i]);

    }
    for(int i; i<ZDD->DC_E.size() ; i++){
        if(ZDD->DC_Nbr[i]==0) DC_E1.push_back(ZDD->DC_E[i]);
        else if(ZDD->DC_Nbr[i]==1) DC_E2.push_back(ZDD->DC_E[i]);
        else if(ZDD->DC_Nbr[i]==2) DC_E3.push_back(ZDD->DC_E[i]);
        else if(ZDD->DC_Nbr[i]==3) DC_E4.push_back(ZDD->DC_E[i]);
    }

    for(int i; i<ZDD->EXO_E.size() ; i++){
        if(ZDD->EXO_Nbr[i]==0) EXO_E1.push_back(ZDD->EXO_E[i]);
        else if(ZDD->EXO_Nbr[i]==1) EXO_E2.push_back(ZDD->EXO_E[i]);
        else if(ZDD->EXO_Nbr[i]==2) EXO_E3.push_back(ZDD->EXO_E[i]);
        else if(ZDD->EXO_Nbr[i]==3) EXO_E4.push_back(ZDD->EXO_E[i]);
    }

}


// void Analysis::TreatEXO(){
//     int EXO_AB_size = Exogam->E_AB.size();
//     for(unsigned int countExo = 0 ; countExo < EXO_AB_size; countExo++){
//         // Doing Doppler correction only if one reaction occurs
//         if(Beta_dp.size() == 1){
//             EXO_Doppler_dp.push_back(Doppler_Correction(Exogam->Theta[countExo], Exogam->Phi[countExo], 0,0,Beta_dp[0],Exogam->E_AB[countExo]));

//         }
//     }
// }


void Analysis::InitOutputBranch() {
    RootOutput::getInstance()->GetTree()->Branch("GATCONF",&GATCONFMASTER);
    RootOutput::getInstance()->GetTree()->Branch("GATCONFTS",&GATCONFMASTERTS);
    //RootOutput::getInstance()->GetTree()->Branch("IC_ZDD_sum",&IC_ZDD_sum,"IC_ZDD_sum/I");
    //RootOutput::getInstance()->GetTree()->Branch("PL_center",&PL_center,"PL_center/D");
    RootOutput::getInstance()->GetTree()->Branch("gf",&gf,"gf/I");

    //////////////////////// MUST related branch ////////////////////////////////
    RootOutput::getInstance()->GetTree()->Branch("HitDirectionMG",&HitDirectionMG);

    RootOutput::getInstance()->GetTree()->Branch("BeamDirection",&BeamDirection);
    RootOutput::getInstance()->GetTree()->Branch("BeamImpact",&BeamImpact);
    RootOutput::getInstance()->GetTree()->Branch("BeamDirection0",&BeamDirection0);
    RootOutput::getInstance()->GetTree()->Branch("BeamImpact0",&BeamImpact0);
    RootOutput::getInstance()->GetTree()->Branch("MG_DetM",&MG_DetM,"MG_DetM/s");
    RootOutput::getInstance()->GetTree()->Branch("MG_Ex",&MG_Ex);
    RootOutput::getInstance()->GetTree()->Branch("MG_Ex2",&MG_Ex2);
    RootOutput::getInstance()->GetTree()->Branch("MG_Extof",&MG_Ex);
    RootOutput::getInstance()->GetTree()->Branch("MG_Exnocor",&MG_Exnocor);
    RootOutput::getInstance()->GetTree()->Branch("MG_Ex0nocor",&MG_Ex0nocor);
    RootOutput::getInstance()->GetTree()->Branch("MG_ELab",&MG_ELab);
    RootOutput::getInstance()->GetTree()->Branch("MG_ELab2",&MG_ELab2);
    RootOutput::getInstance()->GetTree()->Branch("MG_ELabraw",&MG_ELabraw);
    RootOutput::getInstance()->GetTree()->Branch("ELoss_Al",&ELoss_Al);
    RootOutput::getInstance()->GetTree()->Branch("ELoss",&ELoss);
    RootOutput::getInstance()->GetTree()->Branch("ELoss_Target",&ELoss_Target);
    RootOutput::getInstance()->GetTree()->Branch("MG_ThetaLab2",&MG_ThetaLab2);
    RootOutput::getInstance()->GetTree()->Branch("MG_PhiLab",&MG_PhiLab);
    RootOutput::getInstance()->GetTree()->Branch("MG_Phi0",&MG_Phi0);

    RootOutput::getInstance()->GetTree()->Branch("MG_ThetaCM",&MG_ThetaCM);
    RootOutput::getInstance()->GetTree()->Branch("MG_ThetaLab",&MG_ThetaLab);
    RootOutput::getInstance()->GetTree()->Branch("MG_ThetaCM2",&MG_ThetaCM2);
    RootOutput::getInstance()->GetTree()->Branch("MG_X",&MG_X);
    RootOutput::getInstance()->GetTree()->Branch("MG_Y",&MG_Y);
    RootOutput::getInstance()->GetTree()->Branch("MG_Z",&MG_Z);
    //////////////////////// Exogam related branch ////////////////////////////////
    RootOutput::getInstance()->GetTree()->Branch("EXO_Doppler_dp",&EXO_Doppler_dp);
    RootOutput::getInstance()->GetTree()->Branch("EXO_Doppler_pt",&EXO_Doppler_pt);
    RootOutput::getInstance()->GetTree()->Branch("EXO_Doppler_p3He",&EXO_Doppler_p3He);
    RootOutput::getInstance()->GetTree()->Branch("Beta_dp",&Beta_dp);
    RootOutput::getInstance()->GetTree()->Branch("Beta_p3He",&Beta_p3He);
    RootOutput::getInstance()->GetTree()->Branch("Beta_pt",&Beta_pt);

    RootOutput::getInstance()->GetTree()->Branch("TAC_CATS_PL",&TAC_CATS_PL);
    RootOutput::getInstance()->GetTree()->Branch("TAC_CATS_PL_TS",&TAC_CATS_PL_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_CATS2_CATS1",&TAC_CATS2_CATS1);
    RootOutput::getInstance()->GetTree()->Branch("TAC_CATS2_CATS1_TS",&TAC_CATS2_CATS1_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_MMG_CATS1",&TAC_MMG_CATS1);
    RootOutput::getInstance()->GetTree()->Branch("TAC_MMG_CATS1_TS",&TAC_MMG_CATS1_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_MMG_CATS2",&TAC_MMG_CATS2);
    RootOutput::getInstance()->GetTree()->Branch("TAC_MMG_CATS2_TS",&TAC_MMG_CATS2_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_MMG_EXOGAM",&TAC_MMG_EXOGAM);
    RootOutput::getInstance()->GetTree()->Branch("TAC_MMG_EXOGAM_TS",&TAC_MMG_EXOGAM_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_CATS_EXOGAM",&TAC_CATS_EXOGAM);
    RootOutput::getInstance()->GetTree()->Branch("TAC_CATS_EXOGAM_TS",&TAC_CATS_EXOGAM_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_CATS_HF",&TAC_CATS_HF);
    RootOutput::getInstance()->GetTree()->Branch("TAC_CATS_HF_TS",&TAC_CATS_HF_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_1",&TAC_PL_1);
    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_1_TS",&TAC_PL_1_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_2",&TAC_PL_2);
    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_2_TS",&TAC_PL_2_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_3",&TAC_PL_3);
    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_3_TS",&TAC_PL_3_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_4",&TAC_PL_4);
    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_4_TS",&TAC_PL_4_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_5",&TAC_PL_5);
    RootOutput::getInstance()->GetTree()->Branch("TAC_PL_5_TS",&TAC_PL_5_TS);


    RootOutput::getInstance()->GetTree()->Branch("TAC_DD6_DD4",&TAC_DD6_DD4);
    RootOutput::getInstance()->GetTree()->Branch("TAC_DD6_DD4_TS",&TAC_DD6_DD4_TS);


    RootOutput::getInstance()->GetTree()->Branch("TAC_D4_CATS1",&TAC_D4_CATS1);
    RootOutput::getInstance()->GetTree()->Branch("TAC_D4_CATS1_TS",&TAC_D4_CATS1_TS);

    RootOutput::getInstance()->GetTree()->Branch("TAC_CATSD6_1_DD4",&TAC_CATSD6_1_DD4);
    RootOutput::getInstance()->GetTree()->Branch("TAC_CATSD6_1_DD4_TS",&TAC_CATSD6_1_DD4_TS);

    RootOutput:: getInstance()->GetTree()->Branch("TOF",&TOF);
    RootOutput:: getInstance()->GetTree()->Branch("OriginalBeamEnergy",&OriginalBeamEnergy);
    RootOutput:: getInstance()->GetTree()->Branch("OriginalBeamEnergy2",&OriginalBeamEnergy2);
    RootOutput:: getInstance()->GetTree()->Branch("OriginalBeamEnergync",&OriginalBeamEnergync);

    RootOutput::getInstance()->GetTree()->Branch("PL_E1",&PL_E1);
    RootOutput::getInstance()->GetTree()->Branch("PL_E2",&PL_E2);
    RootOutput::getInstance()->GetTree()->Branch("PL_E3",&PL_E3);
    RootOutput::getInstance()->GetTree()->Branch("PL_E4",&PL_E4);
    RootOutput::getInstance()->GetTree()->Branch("PL_E5",&PL_E5);
    RootOutput::getInstance()->GetTree()->Branch("PL_E6",&PL_E6);
    RootOutput::getInstance()->GetTree()->Branch("PL_E7",&PL_E7);
    RootOutput::getInstance()->GetTree()->Branch("PL_E8",&PL_E8);
    RootOutput::getInstance()->GetTree()->Branch("PL_E9",&PL_E9);
    RootOutput::getInstance()->GetTree()->Branch("PL_E10",&PL_E10);

    RootOutput::getInstance()->GetTree()->Branch("IC_1",&IC_1);
    RootOutput::getInstance()->GetTree()->Branch("IC_2",&IC_2);
    RootOutput::getInstance()->GetTree()->Branch("IC_3",&IC_3);
    RootOutput::getInstance()->GetTree()->Branch("IC_4",&IC_4);
    RootOutput::getInstance()->GetTree()->Branch("IC_5",&IC_5);

    RootOutput::getInstance()->GetTree()->Branch("DC_1",&DC_E1);
    RootOutput::getInstance()->GetTree()->Branch("DC_2",&DC_E2);
    RootOutput::getInstance()->GetTree()->Branch("DC_3",&DC_E3);
    RootOutput::getInstance()->GetTree()->Branch("DC_4",&DC_E4);

    RootOutput::getInstance()->GetTree()->Branch("EXO_1",&EXO_E1);
    RootOutput::getInstance()->GetTree()->Branch("EXO_2",&EXO_E2);
    RootOutput::getInstance()->GetTree()->Branch("EXO_3",&EXO_E3);
    RootOutput::getInstance()->GetTree()->Branch("EXO_4",&EXO_E4);

    /********************try by ps start***************/
    // RootOutput::getInstance()->GetTree()->Branch("IC_1_arr",&IC_1_arr,"IC_1_arr/D");
    // RootOutput::getInstance()->GetTree()->Branch("IC_2_arr",&IC_2_arr,"IC_2_arr/D");
    // RootOutput::getInstance()->GetTree()->Branch("IC_3_arr",&IC_3_arr,"IC_3_arr/D");
    // RootOutput::getInstance()->GetTree()->Branch("IC_4_arr",&IC_4_arr,"IC_4_arr/D");
    // RootOutput::getInstance()->GetTree()->Branch("IC_5_arr",&IC_5_arr,"IC_5_arr/D");
    /******************** try by ps end ***************/

    RootOutput::getInstance()->GetTree()->Branch("IC_ZDD_sum",&IC_ZDD_sum);
    RootOutput::getInstance()->GetTree()->Branch("PL_center",&PL_center);
    RootOutput::getInstance()->GetTree()->Branch("PL_centerLog",&PL_centerLog);
}

void Analysis::UnallocateVariables(){
}

void Analysis::InitInputBranch(){

    if(!NPOptionManager::getInstance()->GetInputPhysicalTreeOption()){
        TTreeReader* inputTreeReader = RootInput::getInstance()->GetTreeReader();
        GATCONFMASTER_ = new TTreeReaderValue<vector<unsigned int>>(*inputTreeReader,"GATCONF");
        GATCONFMASTERTS_ = new TTreeReaderValue<vector<unsigned long long>>(*inputTreeReader,"GATCONFTS");

    }
}

////////////////////////////////////////////////////////////////////////////////

void Analysis::ReInit(){

    MG_DetM= 0;
    MG_Ex.clear();
    MG_Ex2.clear();
    MG_Extof.clear();
    MG_Ex0tof.clear();
    MG_Exnocor.clear();
    MG_Ex0nocor.clear();
    MG_ELab.clear();
    MG_ELab2.clear();
    MG_ELabraw.clear();
    ELoss_Al.clear();
    ELoss_Target.clear();
    ELoss.clear();
    MG_ThetaLab.clear();
    MG_PhiLab.clear();
    MG_Phi0.clear();

    MG_ThetaLab2.clear();
    MG_ThetaCM.clear();
    MG_ThetaCM2.clear();
    MG_X.clear();
    MG_Y.clear();
    MG_Z.clear();
    EXO_Doppler_p3He.clear();
    EXO_Doppler_pt.clear();
    EXO_Doppler_dp.clear();

    Beta_p3He.clear();
    Beta_pt.clear();
    Beta_dp.clear();

    TAC_CATS_PL = 0.0;
    TAC_CATS_PL_TS.clear();
    TAC_CATS2_CATS1 = 0.0;
    TAC_CATS2_CATS1_TS.clear();
    TAC_MMG_CATS1 = 0.0;
    TAC_MMG_CATS1_TS.clear();
    TAC_MMG_CATS2 = 0.0;
    TAC_MMG_CATS2_TS.clear();
    TAC_MMG_EXOGAM = 0.0;
    TAC_MMG_EXOGAM_TS.clear();
    TAC_CATS_EXOGAM = 0.0;
    TAC_CATS_EXOGAM_TS.clear();
    TAC_CATS_HF = 0.0;
    TAC_CATS_HF_TS.clear();
    TAC_PL_1 = 0.0;
    TAC_PL_1_TS.clear();
    TAC_PL_2 = 0.0;
    TAC_PL_2_TS.clear();
    TAC_PL_3 = 0.0;
    TAC_PL_3_TS.clear();
    TAC_PL_4 = 0.0;
    TAC_PL_4_TS.clear();
    TAC_PL_5 = 0.0;
    TAC_PL_5_TS.clear();
    TAC_D4_CATS1 = 0.0;
    TAC_D4_CATS1_TS.clear();
    TAC_DD6_DD4 = 0.0;
    TAC_DD6_DD4_TS.clear();
    TAC_CATSD6_1_DD4 = 0.0;
    TAC_CATSD6_1_DD4_TS.clear();
    TAC_D4_CATS1nc= -1000;
    TSEvent->ClearTimeStamps();
    IC_ZDD_sum = 0.0;
    PL_center= 0.0;
    PL_centerLog= 0.0;
    TOF= 0.0;
    velocity = 0.0;
    PL_E1.clear();
    PL_E2.clear();
    PL_E3.clear();
    PL_E4.clear();
    PL_E5.clear();
    PL_E6.clear();
    PL_E7.clear();
    PL_E8.clear();
    PL_E9.clear();
    PL_E10.clear();
    IC_1.clear();
    IC_2.clear();
    IC_3.clear();
    IC_4.clear();
    IC_5.clear();
    DC_E1.clear();
    DC_E2.clear();
    DC_E3.clear();
    DC_E4.clear();
    EXO_E1.clear();
    EXO_E2.clear();
    EXO_E3.clear();
    EXO_E4.clear();
    //norm_length=0;


}
void Analysis::SetRefTS(){
    ZDD->SetRefTS("GATCONFTS",GATCONFMASTERTS[0]);
    //cout<<"Debug by PS-------------->1"<<"\n \n \n"<<endl;
    TAC->SetRefTS("GATCONFTS",GATCONFMASTERTS[0]);
    //Exogam->SetRefTS("GATCONFTS",GATCONFMASTERTS[0]);
    //cout<<"setrefts() ended"<<endl;
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
    TFile *f = RootOutput::getInstance()->GetFile();
    f->cd();
    MASK1Y_MASK1X->Write();
    MASK2Y_MASK2X->Write();
    CATS1Y_CATS1X->Write();
    CATS2Y_CATS2X->Write();
    h2dPL->Write();
    Trajectory_XZ->Write();
    Trajectory_YZ->Write();
    PL_spot_const->Write();
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

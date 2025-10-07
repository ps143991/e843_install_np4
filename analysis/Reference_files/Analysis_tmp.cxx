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

    if (!ZDD) {
    std::cerr << "Error: ZDD detector not found!" << std::endl;
    return;
    }

    //ZDDr= (TZDDData*) m_DetectorManager->GetDetector("ZDD");
    //MG = (TMugastPhysics*)  m_DetectorManager -> GetDetector("Mugast");
    Exogam = (TExogamPhysics*)  m_DetectorManager -> GetDetector("Exogam");
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
    LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".G4table","G4Table",100 );
    HeavyMylar = NPL::EnergyLoss(beam+"_Mylar.G4table","G4Table",100);
    LightAl = NPL::EnergyLoss(light+"_Al.G4table","G4Table",100);
    LightSi = NPL::EnergyLoss(light+"_Si.G4table","G4Table",100);
    //
    ProtonSi = NPL::EnergyLoss(Path+ "proton_Si.G4table", "G4Table", 100);
    std::cout << " target " <<  TargetMaterial << "\n";
    Cal = CalibrationManager::getInstance();
    IsPhysics = NPOptionManager::getInstance()->GetInputPhysicalTreeOption();

  //  ReadConfigTS();
}

////////////////////////////////////////////////////////////////////////////////

bool Analysis::UnallocateBeforeBuild(){
    GATCONFMASTER.clear();
    GATCONFMASTERTS.clear();

    GATCONFMASTER = **GATCONFMASTER_;
    GATCONFMASTERTS = **GATCONFMASTERTS_;
        // Check if the vector is empty before accessing [0]
   // if (GATCONFMASTERTS.empty()) {
        //std::cerr << "WARNING: GATCONFMASTERTS is empty!" << std::endl;
     //   return false;
    //}

    // Debugging output
  //  std::cout << "DEBUG: The first value of GATCONFMASTERTS is: " << GATCONFMASTERTS[0] << std::endl;

    //if(GATCONFMASTER.size() == 1 && GATCONFMASTER[0] == 2)
     if(GATCONFMASTER.size() == 1 )
    {
     //   cout<<"DEBUG: The value of GATCONFMASTERTS_ is: "<<GATCONFMASTERTS[0]<<endl;
      //  SetRefTS();
        return true;
    }
    else
        return false;
}
bool Analysis::UnallocateBeforeTreat(){

  //  if( MG->DSSD_E.size() == 1)

 //   {
        return true;
 //   }
 //   else{
  //      return false;
  //  }

}

bool Analysis::FillOutputCondition(){
  //  return bCATS;
    return true;
}

void Analysis::ReadConfigTS(){
    TSEvent->ReadConfigurationFile();
    ZDD->ReadConfigurationTS();
    TAC->ReadConfigurationTS();
    Exogam->ReadConfigurationTS();
}

void Analysis::TreatEvent(){
    ///int iii=0;
    ReInit();
    ///iii++;
  //  cout<<"came here too"<<endl;
    TreatCATS();
    TreatTAC();


    //TreatMugast();
    //TreatZDD();
    //if(bCATS){

      //TreatMugast();
      //TreatMust2();
      TreatEXO();
      TreatZDD();
      //cout<<"bcats is true"<<endl;
      //TreatMugast2();
      //TreatTAC();
    //}

}

/* void Analysis::TreatCATS(){
    //std::cout << " cats posY " << CATS->PositionOnTargetY  << "\n";
    if(CATS->PositionOnTargetX > -1000 && CATS->PositionOnTargetY > -1000){
        cout<<"the position is >-1000"<<endl;
       // BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.7,-(CATS->PositionOnTargetY)+2.3,0); ///correctone cats3
        //   BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.5,-(CATS->PositionOnTargetY)+3,0); ///correctone cats9
        //   BeamImpact = TVector3((CATS->PositionOnTargetX)-1.835,(CATS->PositionOnTargetY)-1.51,0); ///new one cast2
         //  BeamImpact = TVector3((CATS->PositionOnTargetX),(CATS->PositionOnTargetY),0); ///new one cats1
         // BeamImpact = TVector3(-(CATS->PositionOnTargetX),(CATS->PositionOnTargetY),0); ///new one cats6
            //  BeamImpact = TVector3((CATS->PositionOnTargetX)-1.21,(CATS->PositionOnTargetY)-4.35,0); ///new one cats4
             // BeamImpact = TVector3(-(CATS->PositionOnTargetX),(CATS->PositionOnTargetY)-4,0); ///new one cats5
              BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.5,(CATS->PositionOnTargetY)-3,0); ///new one cats8 target centered
             //   BeamImpact = TVector3(-(CATS->PositionOnTargetX)-0.5,(CATS->PositionOnTargetY)-1.3,0);//cats7
        BeamDirection = TVector3(CATS->PositionX[0] - CATS->PositionX[1],CATS->PositionY[0] - CATS->PositionY[1],-(CATS->PositionZ[0] - CATS->PositionZ[1]));

     //   ax[0]=(Position[1] - Position[0])/(CATS->PositionX[1] - CATS->PositionX[0]);
     //   ay[0]=(Position[1] - Position[0])/(CATS->PositionY[1] - CATS->PositionY[0]);
 //for(int k= Position[0]; k<Position[2]; k++){
 //Trajectory_XZ->Fill(k, CATS->PositionX[0] + (k - Position[0])/ax[0]);
 //Trajectory_YZ->Fill(k, CATS->PositionY[0] + (k - Position[0])/ay[0]);
 // }

        // std::cout << " BeamImpact " << BeamImpact.X() << "  " << BeamImpact.Y() <<  "	"<< BeamImpact.Z() << "\n";
        //  std::cout << " BeamDirection " << BeamDirection.X() << "  " << BeamDirection.Y() <<  "	"<< BeamDirection.Z() << std::endl;
        bCATS = true;
    }
    else {bCATS = false; cout<<"bcats went false"<<endl;}

}*/

void Analysis::TreatCATS(){
    //std::cout << " cats posY " << CATS->PositionOnTargetY  << "\n";
    if(CATS->PositionOnTargetX > -1500 && CATS->PositionOnTargetY > -1500){
                 std::cout << " positionX " <<CATS->PositionOnTargetX << "  " << CATS->PositionOnTargetX << "\n";

       // BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.7,-(CATS->PositionOnTargetY)+2.3,0); ///correct one
                      BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.5,(CATS->PositionOnTargetY)-3,0); ///new one cats8 target centered

        //    BeamImpact = TVector3(-(CATS->PositionOnTargetX)-1.38,-(CATS->PositionOnTargetY)-4.2,0); ///new one
        BeamDirection = TVector3(CATS->PositionX[0] - CATS->PositionX[1],CATS->PositionY[0] - CATS->PositionY[1],-(CATS->PositionZ[0] - CATS->PositionZ[1]));

         std::cout << " BeamImpact " << BeamImpact.X() << "  " << BeamImpact.Y() <<  "	"<< BeamImpact.Z() << "\n";
          std::cout << " BeamDirection " << BeamDirection.X() << "  " << BeamDirection.Y() <<  "	"<< BeamDirection.Z() << std::endl;
        bCATS = true;
        //if((iii%1000)==0)cout<<"working fine so far!"<<endl;
    }
    else {bCATS = false; cout<<"bcats went false"<<endl;//if((iii%1000)==0)cout<<"bCATS returnin 0!"<<endl;
    }


}

void Analysis::TreatTAC(){
    cout<<"running treattac"<<endl;
    TAC_TOF = false;
    for(unsigned int i = 0; i < TAC->TAC_Time.size(); i++){
        if(TAC->TAC_Name[i].compare("TAC_CATS_PL") == 0)
        {
            TAC_CATS_PL += (TAC->TAC_Time[i]*0.01306);
            TAC_CATS_PL_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_CATS2_CATS1") == 0)
        {
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
            TAC_D4_CATS1 += (TAC->TAC_Time[i]*0.02554);
            TAC_D4_CATS1_TS.push_back(TAC->TAC_TS[i]);
            TAC_D4_CATS1nc = TAC->TAC_Time[i];
            if (TAC_D4_CATS1nc > 0) TAC_TOF = true;

        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_1") == 0)
        {
            TAC_PL_1 += (TAC->TAC_Time[i]);
            TAC_PL_1_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_2") == 0)
        {
            TAC_PL_2 += (TAC->TAC_Time[i]);
            TAC_PL_2_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_3") == 0)
        {
            TAC_PL_3 += (TAC->TAC_Time[i]);
            TAC_PL_3_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_4") == 0)
        {
            TAC_PL_4 += (TAC->TAC_Time[i]);
            TAC_PL_4_TS.push_back(TAC->TAC_TS[i]);
        }
        else if(TAC->TAC_Name[i].compare("TAC_PL_5") == 0)
        {
            TAC_PL_5 += (TAC->TAC_Time[i]);
            TAC_PL_5_TS.push_back(TAC->TAC_TS[i]);
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

/*
void Analysis::TreatMugast(){

    Time_0_Ref = 27594;          // Reference time constant
    Channel_Calib = 0.02554;     // Calibration factor
    //   Channel_Calib = -0.034473305;     // Calibration factor ozge

    D4_CATS1_L = 2147;           // Path length
    //  Time_0_Ref = 29850;          // Reference time constant new
    //    Time_0_Ref = 916.1264834;          // gain new calibration

    //  Channel_Calib = 0.02554;     // Calibration factor
    //  D4_CATS1_L = 2639;           // Path length correct
    // std::cout << "Time_0_Ref " << Time_0_Ref << std::endl;
    //  std::cout << "Channel_Calib " << Channel_Calib << std::endl;
    // std::cout << "D4_CATS1_L " << D4_CATS1_L << std::endl;


    for(unsigned int countMugast = 0 ; countMugast < MG->DSSD_E.size() ; countMugast++){
        MG_DetM++;
        thetalab_tmp = 0;
        philab_tmp =0;
        Energy = 0;
        Energycor = 0;
        Energy= MG->DSSD_E[countMugast];
        if (TAC_TOF == true) {

            // TOF= TAC_D4_CATS1nc*Channel_Calib+Time_0_Ref;
            // velocity = TOF/D4_CATS1_L/1e7;
            TOF = (Time_0_Ref - TAC_D4_CATS1nc) * Channel_Calib / D4_CATS1_L / 1e7;
            //std::cout << "EnerMGtof1 " << TOF*10e7 << std::endl;
            // std::cout << "EnerMGtof2 " << reaction->GetBeamEnergy() << std::endl;
            reaction->GetNucleus1()->SetTimeOfFlight(TOF);
            // reaction->GetNucleus1()->SetTimeOfFlight(velocity);
            reaction->SetBeamEnergy(reaction->GetNucleus1()->GetEnergy());
            reaction->initializePrecomputeVariable();
            OriginalBeamEnergync = reaction->GetBeamEnergy();
            if(OriginalBeamEnergync>800){
                //     std::cout << "EnerMGtof1 " << reaction->GetBeamEnergy() << std::endl;
                OriginalBeamEnergy = HeavyMylar.Slow( OriginalBeamEnergync ,4.5*micrometer , 0);
                //      std::cout << "EnerMGtof2 " << OriginalBeamEnergy<< std::endl;
                OriginalBeamEnergy = Beam_Target.Slow( OriginalBeamEnergy , TargetThickness*0.5, 0);


                reaction->SetBeamEnergy(OriginalBeamEnergy);
               // reaction->initializePrecomputeVariable();
              //  OriginalBeamEnergy  = reaction->GetBeamEnergy();
                //   std::cout << "EnerMGtof3 " << reaction->GetBeamEnergy() << std::endl;
                // Part 1 : Impact Angle
                HitDirectionMG = MG -> GetPositionOfInteraction(countMugast) - BeamImpact ;
                thetalab_tmp = HitDirectionMG.Angle(BeamDirection);
                philab_tmp = HitDirectionMG.Phi();
                MG_X.push_back(MG -> GetPositionOfInteraction(countMugast).X());
                MG_Y.push_back(MG -> GetPositionOfInteraction(countMugast).Y());
                MG_Z.push_back(MG -> GetPositionOfInteraction(countMugast).Z());
                ThetaMGSurface= HitDirectionMG.Angle(MG -> GetTelescopeNormal(countMugast) );
                ThetaNormalTargetMG = HitDirectionMG.Angle( TVector3(0,0,1) ) ;
cout<<MG->GetTelescopeNormal(countMugast).X()<< " X " << MG -> GetTelescopeNormal(countMugast).Y() << " Y " << MG -> GetTelescopeNormal(countMugast).Z() << " Z " << MG->TelescopeNumber[0] << endl;
                // Part 2 : Impact Energy

                Energycor = Energy;

                //std::cout << "EnerMG " << reaction->GetBeamEnergy() << std::endl;
                MG_ELabraw.push_back(Energy);

                MG_ThetaLab.push_back(thetalab_tmp / M_PI * 180.);
                MG_PhiLab.push_back(philab_tmp / M_PI * 180.);
                TLorentzVector PHeavy_dp = reaction->LorentzAfterReaction(Energy ,thetalab_tmp);
                Beta_dp.push_back(PHeavy_dp.Beta());

                Energycor = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaMGSurface); ///original
                // Energycor = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , 0); ///charlies code
                ELoss_Al.push_back(Energy-Energycor);

                // Target Correction
                double elab_tmp = Energycor;
                Energycor = LightTarget.EvaluateInitialEnergy( Energycor,TargetThickness*0.5, ThetaNormalTargetMG);
                MG_ELab.push_back(Energycor);
                ELoss_Target.push_back(elab_tmp-Energycor);
                ELoss.push_back(Energy-Energycor);

                //      // Part 3 : Excitation Energy Calculation

                MG_Ex.push_back(reaction->ReconstructRelativistic(Energycor, thetalab_tmp)); //charlies method
                MG_Exnocor.push_back(reaction->ReconstructRelativistic(Energy, thetalab_tmp)); //charlies method

                //      // Part 4 : Theta CM Calculation

                MG_ThetaCM.push_back(reaction->EnergyLabToThetaCM( MG_ELab[countMugast], MG_ThetaLab[countMugast])/deg); //original

            }



        }
    }// end loop for mugast
}
void Analysis::TreatMugast2(){
  double Channel_Calib2 = -0.034473305;     // Calibration factor ozge
  double  Time_0_Ref2 = 916.1264834;          // gain new calibration
  double  D4_CATS1_L2 = 2639;           // Path length correct
//  double  TOF2= TAC_D4_CATS1nc*Channel_Calib2+Time_0_Ref2;
 // velocity = TOF2/D4_CATS1_L2/1e7;
  for(unsigned int countMugast = 0 ; countMugast < MG->DSSD_E.size() ; countMugast++){
      thetalab_tmp2 = 0;
      Energy2 = 0;
      Energy2= MG->DSSD_E[countMugast];
      if (TAC_TOF == true) {

         double  TOF2= TAC_D4_CATS1nc*Channel_Calib2+Time_0_Ref2;
           velocity = TOF2/D4_CATS1_L2/1e7;
          //TOF = (Time_0_Ref - TAC_D4_CATS1nc) * Channel_Calib / D4_CATS1_L / 1e7;
          //std::cout << "EnerMGtof1 " << TOF*10e7 << std::endl;
          // std::cout << "EnerMGtof2 " << reaction->GetBeamEnergy() << std::endl;
         // reaction->GetNucleus1()->SetTimeOfFlight(TOF);
           reaction->GetNucleus1()->SetTimeOfFlight(velocity);
          reaction->SetBeamEnergy(reaction->GetNucleus1()->GetEnergy());
          reaction->initializePrecomputeVariable();
          OriginalBeamEnergy2 = reaction->GetBeamEnergy();
          if(OriginalBeamEnergy2>800){
              //     std::cout << "EnerMGtof1 " << reaction->GetBeamEnergy() << std::endl;
              OriginalBeamEnergy2 = HeavyMylar.Slow( OriginalBeamEnergy2 ,4.5*micrometer , 0);
              //      std::cout << "EnerMGtof2 " << OriginalBeamEnergy<< std::endl;
              OriginalBeamEnergy2 = Beam_Target.Slow( OriginalBeamEnergy2 , TargetThickness*0.5, 0);


              reaction->SetBeamEnergy(OriginalBeamEnergy2);
            //  reaction->initializePrecomputeVariable();
            //  OriginalBeamEnergy2  = reaction->GetBeamEnergy();
              //   std::cout << "EnerMGtof3 " << reaction->GetBeamEnergy() << std::endl;
              // Part 1 : Impact Angle
              HitDirectionMG2 = MG -> GetPositionOfInteraction(countMugast) - BeamImpact ;
              thetalab_tmp2 = HitDirectionMG2.Angle(BeamDirection);
              ThetaMGSurface2= HitDirectionMG2.Angle(MG -> GetTelescopeNormal(countMugast) );
              ThetaNormalTargetMG2 = HitDirectionMG2.Angle( TVector3(0,0,1) ) ;

              // Part 2 : Impact Energy

              //std::cout << "EnerMG " << reaction->GetBeamEnergy() << std::endl;

              MG_ThetaLab2.push_back(thetalab_tmp2 / M_PI * 180.);
              Energy2 = LightAl.EvaluateInitialEnergy( Energy2 ,0.4*micrometer , ThetaMGSurface2); ///original

              // Target Correction
              Energy2 = LightTarget.EvaluateInitialEnergy( Energy2,TargetThickness*0.5, ThetaNormalTargetMG2);
              MG_ELab2.push_back(Energy2);

              //      // Part 3 : Excitation Energy Calculation

              MG_Ex2.push_back(reaction->ReconstructRelativistic(Energy2, thetalab_tmp2)); //charlies method

              //      // Part 4 : Theta CM Calculation

              MG_ThetaCM2.push_back(reaction->EnergyLabToThetaCM( MG_ELab2[countMugast], MG_ThetaLab2[countMugast])/deg); //original

          }



      }
  }// end loop for mugast
}
*/

void Analysis::TreatZDD(){
    int IC_M = ZDD->IC_E.size();
    //int IC_1_count = 0, IC_2_count = 0, IC_3_count = 0, IC_4_count = 0, IC_5_count = 0;
    for (int i=0; i<IC_M;i++){
        if(ZDD->IC_Nbr[i]==0)      {IC_1.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==1) {IC_2.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==2) {IC_3.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==3) {IC_4.push_back(ZDD->IC_E[i]);}
        else if(ZDD->IC_Nbr[i]==4) {IC_5.push_back(ZDD->IC_E[i]);}
    }
    // for (int l = 0; l<IC_1_count; l++){IC_1_arr[l]=(ZDD->IC_E[l]);}
    // for (int l = 0; l<IC_2_count; l++){IC_2_arr[l]=(ZDD->IC_E[l+IC_1_count]);}
    // for (int l = 0; l<IC_3_count; l++){IC_3_arr[l]=(ZDD->IC_E[l+IC_1_count+IC_2_count]);}
    // for (int l = 0; l<IC_4_count; l++){IC_4_arr[l]=(ZDD->IC_E[l+IC_1_count+IC_2_count+IC_3_count]);}
    // for (int l = 0; l<IC_5_count; l++){IC_5_arr[l]=(ZDD->IC_E[l+IC_1_count+IC_2_count+IC_3_count+IC_4_count]);}

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
/*
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
*/
}
void Analysis::TreatEXO(){
    int EXO_AB_size = Exogam->E_AB.size();
    for(unsigned int countExo = 0 ; countExo < EXO_AB_size; countExo++){
        // Doing Doppler correction only if one reaction occurs
        if(Beta_dp.size() == 1){
            EXO_Doppler_dp.push_back(Doppler_Correction(Exogam->Theta[countExo], Exogam->Phi[countExo], 0,0,Beta_dp[0],Exogam->E_AB[countExo]));
        }
       // if(Beta_dp2.size() == 1){
          //  EXO_Doppler_dpL26.push_back(Doppler_Correction(Exogam->Theta[countExo], Exogam->Phi[countExo], 0,0,Beta_dp2[0],Exogam->E_AB[countExo]));

       // }
    }
}



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
    //TSEvent->ClearTimeStamps();
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

}
void Analysis::SetRefTS(){

    ZDD->SetRefTS("GATCONFTS",GATCONFMASTERTS[0]);
    TAC->SetRefTS("GATCONFTS",GATCONFMASTERTS[0]);
    Exogam->SetRefTS("GATCONFTS",GATCONFMASTERTS[0]);
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

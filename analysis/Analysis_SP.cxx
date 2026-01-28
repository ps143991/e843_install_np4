#include <iostream>
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
    //exogam = std::dynamic_pointer_cast<exogam::ExogamDetector>(app->GetDetector("exogam"));
    mugast = std::dynamic_pointer_cast<mugast::MugastDetector>(app->GetDetector("mugast"));


    
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
    // Calculated by H. Jacob fS36pdReaction = Reaction("36S(d,p)37S@2172MeV")rom data optimization
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

    // ------------ Reaction Initialization ------------

    reaction_dp = new nptool::Reaction("68Ni(d,p)69Ni@1224MeV");
    reaction_must2 = new nptool::Reaction("68Ni(d,p)69Ni@1224MeV");

    // (p,d) reaction for efficiency calculation
    //reaction_dp = new nptool::Reaction("68Ni(p,d)69Ni@2584MeV");
    //reaction_must2 = new nptool::Reaction("68Ni(p,d)69Ni@2584MeV");

    OriginalBeamEnergy = reaction_dp->GetBeamEnergy();
    // need to account for Eloss in mylar
    if(OriginalBeamEnergy==0){
      cout<<"Reaction not properly initialized, energy = 0"<< endl;
      return;
    }
    else{ 
      std::cout<<"Reaction created successfully "<< std::endl;
      reaction_dp->Print();
    }
    ReactionBeta = reaction_dp->GetEnergyImpulsionLab_1().Beta();
    std::cout<<"Beam beta: " << ReactionBeta << std::endl;
    // reaction_dp->ReadConfiguration("./Reactions/68Ni_dp_gs_18MeVu_np4.reaction");
    // reaction_dp->Read
    std::string light = ChangeNameToG4Standard(reaction_dp->GetNucleus3()->GetName(), false);
    std::string beam = ChangeNameToG4Standard(reaction_dp->GetNucleus1()->GetName(), false);
    std::string heavy = ChangeNameToG4Standard(reaction_dp->GetNucleus4()->GetName(), false);
    std::cout<<"Light ejectile: " << light << "\nBeam: " << beam << std::endl;

    string path = "./energy_loss/";
    string TargetMaterial_dp = "CD2";
    string TargetMaterial_pd = "CH2";

    cout<<"TargetMaterial " <<  TargetMaterial_dp << "\n";
    TargetThickness = 5*micrometer; // equivalent to 0.5 mg/cm2 for (d,p)
    //TargetThickness = 6.26*micrometer; // equivalent to 0.5 mg/cm2 for (d,p)
    //TargetThickness = 62.6*micrometer; // equivalent to 5 mg/cm2 for (p,d)
    string TargetInfo = TargetMaterial_dp + " " + to_string(TargetThickness/micrometer) + " micrometer";
    nptool::message("blue","","",TargetInfo,true);

    /* Energy Losses */
    Beam_Target =  new nptool::EnergyLoss(path+"Ni68_CD2.G4table", "G4Table", 100);
    Beam_Target =  new nptool::EnergyLoss(path+"Ni68_CD2.G4table", "G4Table", 100);
    //Beam_Target->Print(); // Dummy call to initialize the object properly
    //
    LightTarget = new nptool::EnergyLoss(path+light + "_" + TargetMaterial_dp + ".G4table", "G4Table", 100);
    HeavyMylar = new nptool::EnergyLoss(path+beam + "_Mylar.G4table", "G4Table", 100);
    LightAl = new nptool::EnergyLoss(path+light + "_Al.G4table", "G4Table", 100);
    LightSi = new nptool::EnergyLoss(path+light + "_Si.G4table", "G4Table", 100);

    
}
////////////////////////////////////////////////////////////////////////////////

void user_analysis::Analysis::TreatGATCONF(){
   GATCONFMASTER=*(gatconf->GenericRawBranch["GATCONF"]);
    // Sp I removed GATCONFMASTER==0 from good condition on 17 Dec 2025
    GATCONFMASTERTS=*(gatconf->GenericRawBranchTS["GATCONFTS"]);
    if ( GATCONFMASTER==1 || GATCONFMASTER==2 || GATCONFMASTER==16 || GATCONFMASTER==32){
      if(mugast->m_PhysicsData->DSSD_E.size()==1)
        decider = true;
      else decider = false;
    } else {
      decider = false;
    }//std::cout<<"GATCONFMASTER not good "<< GATCONFMASTER <<std::endl;
    // if(GATCONFMASTER.size()==1){
          //  decider = true;
       // }
}
/////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::ReadConfig() {
    // PreTreat called after BuildPhysicalEvent
    // Here we can apply thresholds and calibrations

  // get_config_file
  auto app = nptool::Application::GetApplication();
  if (app->HasFlag("--ts_cfg")) {
    auto arg = app->GetVectorArg("--ts_cfg");
    if (!arg.empty()) {
      std::cout << arg[0] << std::endl;
      m_config_file = arg[0];
    }
  }
    bool ReadingStatus = false;
  
    ifstream AnalysisConfigFile;
    AnalysisConfigFile.open(m_config_file.c_str()); 
    if(!AnalysisConfigFile.is_open()) {
      std::cerr << "Error opening config file: " << m_config_file << std::endl;
      return;
    }
    std::cout << " Loading user parameter for Analysis from " << m_config_file << std::endl;

    std::string LineBuffer, DataBuffer, whatToDo;
    while (std::getline(AnalysisConfigFile, LineBuffer)) {
      // Pick-up next line

    // search for "header"
    if (LineBuffer.compare(0, 10, "ConfigTS") == 0) ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus) {
      whatToDo = "";
      if (!(AnalysisConfigFile >> whatToDo)) {
        ReadingStatus = false;
        break;
      }
      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n');
      }

      else if (whatToDo == "ADD_TS TAC_CATS_PL") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        tac_CATS_PL.first = stod(DataBuffer);
        AnalysisConfigFile >> DataBuffer;
        tac_CATS_PL.second = stod(DataBuffer);
        cout << "  -> Loaded TAC_CATS_PL calibration: " << tac_CATS_PL.first << " " << tac_CATS_PL.second << endl;
      }
      
      else if (whatToDo == "ADD_TS TAC_MMG_CATS1") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        tac_MMG_CATS1.first = stod(DataBuffer);
        AnalysisConfigFile >> DataBuffer;
        tac_MMG_CATS1.second = stod(DataBuffer);
        cout << "  -> Loaded TAC_MMG_CATS1 calibration: " << tac_MMG_CATS1.first << " " << tac_MMG_CATS1.second << endl;
      }

      else if (whatToDo == "ADD_TS TAC_D4_CATS1") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        tac_D4_CATS1.first = stod(DataBuffer);
        AnalysisConfigFile >> DataBuffer;
        tac_D4_CATS1.second = stod(DataBuffer);
        cout << "  -> Loaded TAC_D4_CATS1 calibration: " << tac_D4_CATS1.first << " " << tac_D4_CATS1.second << endl;
      }

      else if (whatToDo == "ADD_TS TAC_MMG_EXOGAM") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        tac_MMG_EXOGAM.first = stod(DataBuffer);
        AnalysisConfigFile >> DataBuffer;
        tac_MMG_EXOGAM.second = stod(DataBuffer);
        cout << "  -> Loaded TAC_MMG_EXOGAM calibration: " << tac_MMG_EXOGAM.first << " " << tac_MMG_EXOGAM.second << endl;
      }

      else if (whatToDo == "ADD_TS TAC_CATS_HF") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        tac_CATS_HF.first = stod(DataBuffer);
        AnalysisConfigFile >> DataBuffer;
        tac_CATS_HF.second = stod(DataBuffer);
        cout << "  -> Loaded TAC_CATS_HF calibration: " << tac_CATS_HF.first << " " << tac_CATS_HF.second << endl;
      }

      else {
        ReadingStatus = false;
      }

    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::PreTreat() {
    // Apply thresholds and calibrations here
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
}

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis::TreatEvent() {
    //std::cout << "New Event" << std::endl;
    Clear();
    PreTreat();
    if(bMUGAST || bMUST2){
      TreatGATCONF();
      if(decider){
        TreatCATS();
        if(bCATS){
          //std::cout<<"------------ New Event ------------"<<std::endl;
          TreatTOF();
          if(cats_PL && TAC_TOF && mmg_TOF && bMUGAST)
            TreatMugast();
	        if(bMUST2){
            //std::cout<<"Debug: before TreatMust2 "<< std::endl;
	          TreatMust2();
          }
          //std::cout<<"-------------- END ------------------"<<std::endl;
        }
    }
    }
    m_keep_event = ((bMUGAST && decider && bCATS && cats_PL && TAC_TOF && mmg_TOF) || (decider && bCATS && bMUST2));
    m_total_event++;
    if (m_keep_event)
      m_kept_event++;
    /*if (m_total_event % 10000000 == 0) {
      std::cout << "Analysis kept/total: " << m_kept_event << "/" << m_total_event << std::endl;
    }*/
}

void user_analysis::Analysis::TreatTOF(){
  
  //for(unsigned int i = 0; i < tac->m_PhysicsData->TAC_Time.size(); i++){
  //    if (tac->m_PhysicsData->TAC_Name[i].compare("TAC_D4_CATS1") == 0) {
  //              TAC_D4_CATS1nc .push_back( tac->m_PhysicsData->TAC_Time[i]);
  //              //std::cout<<"Debug: TAC_D4_CATS1nc "<< TAC_D4_CATS1nc.at(0) << " channels" << std::endl;
  //              //std::cout<<" index i "<< i << std::endl;
  //              if (tac->m_PhysicsData->TAC_Time[i] > 0){
  //                TAC_TOF = true;
  //                //std::cout<<"Debug: TAC_TOF set to true "<< std::endl;
  //              }
  //    }
  //}
  for(unsigned int i = 0; i < tac->m_PhysicsData->TAC_Time.size(); i++){
    //std::cout<<"Debug: TAC_Name "
    //  << tac->m_PhysicsData->TAC_Name[i] << " Time "<< tac->m_PhysicsData->TAC_Time[i] << " index i "<< i << std::endl;  
    if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_CATS_PL") == 0)
      {
          // TAC_CATS_PL += (tac->m_PhysicsData->TAC_Time[i]*0.01276894);
          TAC_CATS_PL.push_back(tac->m_PhysicsData->TAC_Time[i]*0.01276894);
          if(tac->m_PhysicsData->TAC_Time[i]*0.01276894>300 && tac->m_PhysicsData->TAC_Time[i]*0.01276894<380){
            //std::cout<<"\nTAC_CATS_PL = "<<tac->m_PhysicsData->TAC_Time[i]<<std::endl;
            cats_PL = true;
          }
          // cout<<"works well"<<endl;
      }
      else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_MMG_CATS1") == 0)
      {
          // TAC_MMG_CATS1 += (tac->m_PhysicsData->TAC_Time[i]*0.01327);
          TAC_MMG_CATS1.push_back(tac->m_PhysicsData->TAC_Time[i]*0.01327);
          if(tac->m_PhysicsData->TAC_Time[i]>26500 && tac->m_PhysicsData->TAC_Time[i]<31000){
            mmg_TOF=true;
          //  std::cout<<"\nmmg_TOF = "<<tac->m_PhysicsData->TAC_Time[i]<<std::endl;
          }
      }
      else if (tac->m_PhysicsData->TAC_Name[i].compare("TAC_D4_CATS1") == 0) {
              TAC_D4_CATS1nc.push_back( tac->m_PhysicsData->TAC_Time[i]);
              if (tac->m_PhysicsData->TAC_Time[i] > 10000){
                TAC_TOF = true;
              //  std::cout<<"\nTAC_TOF = "<<tac->m_PhysicsData->TAC_Time[i]<<std::endl;
              }
      }

  }


}




void user_analysis::Analysis::TreatMugast(){      //From TreatMugast in 10March, not treatmugast2

  //double Channel_Calib = 0.02554;    // Time Calibration Coefficient ORIGINAL 0.02554 ns?/mm channel
  if(mugast->m_PhysicsData->DSSD_E.size()==1)
  if(mugast->m_PhysicsData->DSSD_E[0]>0.88){
    //std::cout<<"Debug: Mugast DSSD_E size "<< mugast->m_PhysicsData->DSSD_E.size() << std::endl;
  
  for(unsigned int countMugast = 0; countMugast < mugast->m_PhysicsData->DSSD_E.size(); countMugast++){
    MG_DetM++;
    thetalab_tmp = 0;
    philab_tmp =0;
    Energycor = 0;
    Energy= mugast->m_PhysicsData->DSSD_E[countMugast];
    //std::cout<<"Debug: the index countMugast "<< countMugast << " Energy " << Energy <<" MeV" << endl;
      

    // Calculate TOF
    if (TAC_TOF==true && mmg_TOF==true && cats_PL==true){
      // original TOF calculation 
      //TOF = (Time_0_Ref - TAC_D4_CATS1nc[0]) * Channel_Calib / D4_CATS1_L / 1e7;  // experiment
      // sp modif 17 Dec 2025
      TOF = (Time_0_Ref - TAC_D4_CATS1nc[0]) * Channel_Calib / 1e9;  // in [s] experiment
      //if (GATCONFMASTERTS>32650963337861) std::cout<<"\n******************\nDebug TS: "<<GATCONFMASTERTS<<std::endl;
      //std::cout<<"\n**********************\nDebug: TOF "<< TOF << " s" << std::endl;
      //std::cout<<"Debug: D4_CATS1_L "<< D4_CATS1_L << " mm" << std::endl;
      velocity = D4_CATS1_L / (TOF); // in mm/s
      //std::cout<<"Debug: velocity "<< velocity << std::endl;
      //std::cout<<"Debug: c_light "<< c_light << " mm/ns" << std::endl;
      beta = D4_CATS1_L / (TOF * c_light * 1e9); 
      //std::cout<<"Debug: beta "<< beta << std::endl;
      TOFLength = TOF / (D4_CATS1_L*1e-3); // in s/m
      //std::cout<<"TOF_length : "<<TOFLength<<std::endl;
      // SetTimeOfFlight asks for TOF/L in ns/mm or s/m 
      reaction_dp->GetNucleus1()->SetTimeOfFlight(TOFLength);
      reaction_dp->SetBeamEnergy(reaction_dp->GetNucleus1()->GetEnergy());
      OriginalBeamEnergy = reaction_dp->GetBeamEnergy();

      //std::cout<<"Debug: OriginalBeamEnergy "<< OriginalBeamEnergy << " MeV" << std::endl;
      if(OriginalBeamEnergy>800){
        OriginalBeamEnergy = HeavyMylar->Slow(OriginalBeamEnergy, mylar_thickness*micrometer, 0);
        OriginalBeamEnergy = Beam_Target->Slow(OriginalBeamEnergy, TargetThickness * 0.5, 0);
        reaction_dp->SetBeamEnergy(OriginalBeamEnergy);
        // Direction calculations
        HitDirectionMG = mugast->GetPositionOfInteraction(countMugast) - BeamImpact;
        thetalab_tmp = HitDirectionMG.Angle(BeamDirection);
        philab_tmp = HitDirectionMG.Phi();
        ThetaMGSurface = HitDirectionMG.Angle(mugast->GetTelescopeNormal(countMugast));
        ThetaNormalTargetMG = thetalab_tmp;
        //std::cout<<"\n------------ Mugast Interaction -----------"<<std::endl;
        //std::cout<<"Hit position mugast X "<< mugast->GetPositionOfInteraction(countMugast).x() << " Y "
        //  << mugast->GetPositionOfInteraction(countMugast).y() << " Z " << mugast->GetPositionOfInteraction(countMugast).z() << std::endl;
        //std::cout<<"Debug BeamImpact X "<< BeamImpact.x() << " Y "
        //  << BeamImpact.y() << " Z " << BeamImpact.z() << std::endl;
        //std::cout<<"Debug HitDirectionMG X "<< HitDirectionMG.x() << " Y "
        //  << HitDirectionMG.y() << " Z " << HitDirectionMG.z() << std::endl;
        //std::cout<<"Debug angle thetalab_tmp "<< thetalab_tmp*180./M_PI << " deg" << std::endl;
        //std::cout<<"Debug angle Phi "<<philab_tmp*180./M_PI<< " deg"<<std::endl;
        //std::cout<<"Debug angle ThetaMGSurface "<< ThetaMGSurface*180./M_PI << " deg" << std::endl;
        //std::cout<<"Debug angle ThetaNormalTargetMG "<< ThetaNormalTargetMG*180./M_PI << " deg" << std::endl;
        //std::cout<<"Normal to mugast detector "<< mugast->GetTelescopeNormal(countMugast).x() << " " 
        //  << mugast->GetTelescopeNormal(countMugast).y() << " " << mugast->GetTelescopeNormal(countMugast).z() << std::endl;

        // Part 1 : Store Raw Energy
        Energycor = Energy;
        MG_ELabraw.push_back(Energy);
        MG_PhiLab.push_back(HitDirectionMG.Phi()/M_PI*180.);
        MG_ThetaLab.push_back(HitDirectionMG.Theta() / M_PI * 180.);
        // Part 2 : Energy Loss Correction  
        // Aluminium Correction
        Energycor = LightAl->EvaluateInitialEnergy(Energy, al_thicknessMG*micrometer, ThetaMGSurface);
        ELoss_Al.push_back(Energy - Energycor);
        // Target Correction
        double elab_tmp = Energycor;
        Energycor = LightTarget->EvaluateInitialEnergy(Energycor, TargetThickness * 0.5, thetalab_tmp);
        MG_ELab.push_back(Energycor);
        ELoss_Target.push_back(elab_tmp - Energycor);
        ELoss.push_back(Energy - Energycor);
        // Part 3 : Excitation Energy Calculation
        MG_Ex.push_back(reaction_dp->ReconstructRelativistic(Energycor, thetalab_tmp,philab_tmp)); //charlies method
        //MG_Exnocor.push_back((reaction_dp->ReconstructRelativistic(Energy, thetalab_tmp))-(0.01992656*MG_ThetaLabcor-2.63549658)); //charlies method without correction
        // Part 4 : Get Heavy Recoil Beta for Doppler Correction
        TLorentzVector PHeavy_dp = reaction_dp->GetEnergyImpulsionLab_4();
        Beta_dp.push_back(PHeavy_dp.Beta());

        //std::cout<<"For MUGAST"<<std::endl;
        //std::cout << " BeamImpact " << BeamImpact.X() << "  " << BeamImpact.Y() <<  "	"<< BeamImpact.Z() << "\n";
        //std::cout << " BeamDirection " << BeamDirection.X() << "  " << BeamDirection.Y() <<  "	"<< BeamDirection.Z() << std::endl;
        //std::cout << " HitDirectionMG " << HitDirectionMG.X() << "  " << HitDirectionMG.Y() <<  "	"<< HitDirectionMG.Z() << std::endl;
        //std::cout<<"MG Ex : "<<MG_Ex[0]<<std::endl;
        //std::cout<<"MG ELab : "<<MG_ELab[0]<<std::endl;
        
        } 
      }
      //std::cout<<"MG_Ex SIZE : "<<MG_Ex.size()<<std::endl; 
    }
  }
}


////////////////////////////////////////////////////////////////////////////////

void user_analysis::Analysis::TreatCATS(){

    auto* catsPhysics = cats->m_PhysicsData;
    const bool hasCatsPositions = catsPhysics->PositionX.size() > 1 &&
                                  catsPhysics->PositionY.size() > 1 &&
                                  catsPhysics->PositionZ.size() > 1;
    if(catsPhysics->PositionOnTargetX > -35 && catsPhysics->PositionOnTargetX < 35 
        && catsPhysics->PositionOnTargetY > -35 && catsPhysics->PositionOnTargetY < 35 && hasCatsPositions){

        CATS1_XX=0;
        CATS2_XX=0;
        CATS1_YY=0;
        CATS2_YY=0;
        CATS1_XX =  catsPhysics->PositionX[0];
        CATS1_YY =  catsPhysics->PositionY[0];
        CATS2_XX =  catsPhysics->PositionX[1];
        CATS2_YY =  catsPhysics->PositionY[1];
    	  BeamImpact = TVector3(catsPhysics->PositionOnTargetX,catsPhysics->PositionOnTargetY,0);
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)+1.7,-(cats->m_PhysicsData->PositionOnTargetY)+2.3,0); ///correctone cats3   ====>>>>> OZGE was using this: PS April 23, 2025
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)+1.5,-(cats->m_PhysicsData->PositionOnTargetY)+3,0); ///correctone cats9
        //BeamImpact = TVector3((cats->m_PhysicsData->PositionOnTargetX)-1.835,(cats->m_PhysicsData->PositionOnTargetY)-1.51,0); ///new one cast2
        //BeamImpact = TVector3((cats->m_PhysicsData->PositionOnTargetX),(cats->m_PhysicsData->PositionOnTargetY),0); ///new one cats1
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX),(cats->m_PhysicsData->PositionOnTargetY),0); ///new one cats6
        //BeamImpact = TVector3((cats->m_PhysicsData->PositionOnTargetX)-1.21,(cats->m_PhysicsData->PositionOnTargetY)-4.35,0); ///new one cats4
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX),(cats->m_PhysicsData->PositionOnTargetY)-4,0); ///new one cats5
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)+1.5,(cats->m_PhysicsData->PositionOnTargetY)-3,0); ///new one cats8 target centered
        //BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)-0.5,(cats->m_PhysicsData->PositionOnTargetY)-1.3,0);//cats7
        BeamDirection = TVector3(CATS2_XX - CATS1_XX, CATS2_YY - CATS1_YY, -(cats->m_PhysicsData->PositionZ[0] - cats->m_PhysicsData->PositionZ[1]));
        
        //std::cout<<"For CATS"<<std::endl;
        //std::cout << " BeamImpact " << BeamImpact.X() << "  " << BeamImpact.Y() <<  "	"<< BeamImpact.Z() << "\n";
        //std::cout << " BeamDirection " << BeamDirection.X() << "  " << BeamDirection.Y() <<  "	"<< BeamDirection.Z() << std::endl;
        bCATS = true; /* cout<<"bcats returned true"<<endl; countPS_bcats++; cout<<"count ps_bcats: "<<countPS_bcats<<" "<<endl;*/
    }
    else {bCATS = false; /*cout<<"bcats returned false"<<endl;*/}
}



void user_analysis::Analysis::TreatMust2(){

  auto M2 = must2->m_PhysicsData;
  //auto Exo = exogam->m_PhysicsData;
  auto ZDD = zdd->m_PhysicsData;
  // auto tac = tac->m_PhysicsData;

  //Clear(); // don't do a clear here, otherwise the data filled by other detectors will be lost

  //Filter conditions

  if(M2->Si_E.size()!=1) return;
  if(M2->CsI_E.size()!=1) return;
  const int tel = M2->TelescopeNumber.size();
  const int x   = M2->Si_X[0];
  const int y   = M2->Si_Y[0];

  //std::cout <<" tel size= "<<M2->TelescopeNumber.size()<< " x=" << x << " y=" << y << std::endl;
  
  if (tel != 1) return;
  if (x < 1 || x > 128 || y < 1 || y > 128) return;
  
  // Get the position of the interaction in MUST2
  //TVector3 PosM2 {
  //  must2->GetStripPositionX(M2->TelescopeNumber[0], M2->Si_X[0],M2->Si_Y[0] ),
  //  must2->GetStripPositionY(M2->TelescopeNumber[0], M2->Si_X[0],M2->Si_Y[0] ),
  //  must2->GetStripPositionZ(M2->TelescopeNumber[0], M2->Si_X[0],M2->Si_Y[0] )
  //};
  TVector3 PosM2 = must2->GetPositionOfInteraction(0);
  M2X.push_back(PosM2.X());
  M2Y.push_back(PosM2.Y());
  M2Z.push_back(PosM2.Z());

  //std::cout << " PosM2 " << PosM2.X() << "  " << PosM2.Y() <<  "	"<< PosM2.Z() << std::endl;
  // Beam direction
  TVector3 BeamDirectionDefault(0.,0.,1.); // Beam on Z axis
  auto BeamDirectionXYZ  = BeamDirection;
  // Get the position of the interaction on TARGET
  TVector3 PosTargetDefault(0.,0.,0.); // Default position
  // Get the total energy deposited on MUST2: Si + CsI 
  ELab+=M2->Si_E[0];
  if(M2->CsI_E[0]>0){
    ELab+=M2->CsI_E[0];
  }
  else{
    ELab= -1000.;
  }

  double thetaMust2Surface = 0.;
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // ------------------------------------ Reaction reconstruction ------------------------------------
  auto ReconstructReaction = [&](Reaction& reaction, TVector3 PosOnTarget, TVector3 BeamDirection){
  
    //std::cout<<"Position on target: "<< PosOnTarget.X() << " " << PosOnTarget.Y() << " " << PosOnTarget.Z() << std::endl;
    //std::cout<<"Beam direction: "<< BeamDirection.X() << " " << BeamDirection.Y() << " " << BeamDirection.Z() << std::endl;
    
    TVector3 HitDirectionRec = PosM2 - PosOnTarget;
    double thetaLab = HitDirectionRec.Angle(BeamDirection);
    double ELabCorrected_tmp = -1000.;
    thetaMust2Surface= HitDirectionRec.Angle(must2->GetTelescopeNormal(0));
    ThetaLab = HitDirectionRec.Theta()*180./TMath::Pi(); // Convert to deg
    
    if(ELab > 0){
      // Energy loss in Al dead layer
      double EbeforeAl = LightAl->EvaluateInitialEnergy(ELab,al_thickness[M2->TelescopeNumber[0]]*micrometer,thetaMust2Surface);
      // Energy loss in CH2 target
      double Emidtarget = LightTarget->EvaluateInitialEnergy(EbeforeAl,0.5*TargetThickness,thetaLab);
      ELabCorrected_tmp = Emidtarget;
    }
    
    // Get the excitation energy
    Ex.push_back(reaction.ReconstructRelativistic(ELabCorrected_tmp, thetaLab));
    thetaRecoil.push_back(reaction.GetEnergyImpulsionLab_4().Vect().Theta()*180./TMath::Pi());
    phiRecoil.push_back(reaction.GetEnergyImpulsionLab_4().Vect().Phi());
    betaRecoil.push_back(reaction.GetEnergyImpulsionLab_4().Beta());
    ELabCorrected.push_back(ELabCorrected_tmp);
    thetaParticle.push_back(HitDirectionRec.Theta()*180./TMath::Pi());
  };
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  // --------------------------- Reaction reconstruction ---------------------------
  // SP
  OriginalBeamEnergy = reaction_must2->GetBeamEnergy();
  OriginalBeamEnergy = HeavyMylar->Slow(OriginalBeamEnergy, mylar_thickness*micrometer, 0);
  OriginalBeamEnergy = Beam_Target->Slow(OriginalBeamEnergy, TargetThickness * 0.5, 0);
  reaction_must2->SetBeamEnergy(OriginalBeamEnergy);
  // ------------------------
  ReconstructReaction(*reaction_must2, BeamImpact, BeamDirection); // Ex[0] 

  double thetaMust2SurfaceReal = thetaMust2Surface;
  //ReconstructReaction(*reaction_must2, PosTargetDefault , BeamDirectionDefault); // Ex[1]
  //ReconstructReaction(*reaction_must2, PosTargetDefault , BeamDirection); // Ex[2]
  //ReconstructReaction(*reaction_dp, BeamImpact, BeamDirectionDefault); // Ex[3]
  // Reconstruction of the E from the delta E in CsI
  CsI_EReconstructed = -M2->Si_E[0] + LightSi->EvaluateEnergyFromDeltaE(
    M2->Si_E[0],  // Delat E in Si
    si_thickness[M2->TelescopeNumber[0]]*um, // Thickness of Si at 0 degree
    thetaMust2Surface, // Particle angle
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

  
}
////////////////////////////////////////////////////////////////////////////////
/* void user_analysis::Analysis::TreatEvent() {


  auto M2 = must2->m_PhysicsData;
  auto cats = cats->m_PhysicsData;
  //auto Exo = exogam->m_PhysicsData;
  auto ZDD = zdd->m_PhysicsData;
  // auto tac = tac->m_PhysicsData;

  //Clear(); // don't do a clear here, otherwise the data filled by other detectors will be lost

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
  Tree->Branch("bMUST2", &bMUST2); // Flag indicating if MUST2 has valid data
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

  Tree->Branch("gf",&gf,"gf/I");
  Tree->Branch("HitDirectionMG",&HitDirectionMG);
  Tree->Branch("BeamImpact",&BeamImpact);
  Tree->Branch("BeamImpact0",&BeamImpact0);
  Tree->Branch("bMUGAST",&bMUGAST);
  Tree->Branch("MG_DetM",&MG_DetM,"MG_DetM/s");
  Tree->Branch("MG_Ex",&MG_Ex);
  Tree->Branch("MG_Ex2",&MG_Ex2);
  Tree->Branch("MG_Extof",&MG_Ex);
  Tree->Branch("MG_Exnocor",&MG_Exnocor);
  Tree->Branch("MG_Ex0nocor",&MG_Ex0nocor);
  Tree->Branch("MG_ELab",&MG_ELab);
  Tree->Branch("MG_ELab2",&MG_ELab2);
  Tree->Branch("MG_ELabraw",&MG_ELabraw);
  Tree->Branch("ELoss_Al",&ELoss_Al);
  Tree->Branch("ELoss",&ELoss);
  Tree->Branch("ELoss_Target",&ELoss_Target);
  Tree->Branch("MG_ThetaLab2",&MG_ThetaLab2);
  Tree->Branch("MG_PhiLab",&MG_PhiLab);
  Tree->Branch("MG_Phi0",&MG_Phi0);
  Tree->Branch("MG_ThetaCM",&MG_ThetaCM);
  Tree->Branch("MG_ThetaLab",&MG_ThetaLab);
  Tree->Branch("MG_ThetaCM2",&MG_ThetaCM2);
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
  decider = false;
  bCATS = false;
  bMUGAST = false;
  bMUST2 = false;
  m_keep_event = false;
  TAC_CATS_PL.clear();
  TAC_MMG_CATS1.clear();
  TAC_D4_CATS1nc.clear();
  TAC_TOF = false;
  mmg_TOF = false;
  cats_PL = false;
  decider = false;
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
  // TSEvent->ClearTimeStamps();
  TOF= 0.0;
  TOFLength= 0.0;
  beta = 0.0;
  velocity = 0.0;
  OriginalBeamEnergy = 0.0;
  OriginalBeamEnergy2 = 0.0;
  OriginalBeamEnergync = 0.0;

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
  std::cout << "End of Analysis" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserAnalysis> ConstructUserAnalysis() { return std::make_shared<user_analysis::Analysis>(); }
}

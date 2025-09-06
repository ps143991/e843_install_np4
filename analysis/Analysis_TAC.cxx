#include "Analysis_TAC.h"
#include "NPApplication.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_TAC::Init() {
  auto app = nptool::Application::GetApplication();

    tac = std::dynamic_pointer_cast<tac::TacDetector>(app->GetDetector("tac"));

}


////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_TAC::TreatEvent() {
    Clear();
    TreatTAC();
    BeamSpot_PL();
}

void user_analysis::Analysis_TAC::BeamSpot_PL() {
    for(unsigned int i = 0; i < tac->m_PhysicsData->TAC_Time.size(); i++){
        if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_3") == 0) {
            TAC_PL_3 += (tac->m_PhysicsData->TAC_Time[i]); 
            Double_t per_bin_value = (27062 - 18776)/20;                  // for 20 cm
            Double_t posX= ((TAC_PL_3 - 18776)/per_bin_value) - 10;       // to center about 0.
            if(abs(posX)<10) {Spot_Plastic_3 = posX;}
            else Spot_Plastic_3 = -10;
          }
    }
}

void user_analysis::Analysis_TAC::TreatTAC(){

//    // TAC_TOF = false;
//    norm_length=0.0;
//   //  counterTAC = 0;
    for(unsigned int i = 0; i < tac->m_PhysicsData->TAC_Time.size(); i++){
        if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_CATS_PL") == 0)
        {
            TAC_CATS_PL += (tac->m_PhysicsData->TAC_Time[i]*0.01276894);
            TAC_CATS_PL_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_CATS2_CATS1") == 0)
        {
            TAC_CATS2_CATS1 += (tac->m_PhysicsData->TAC_Time[i]*0.00635);
            TAC_CATS2_CATS1_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_MMG_CATS1") == 0)
        {
            TAC_MMG_CATS1 += (tac->m_PhysicsData->TAC_Time[i]*0.01327);
            TAC_MMG_CATS1_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_MMG_CATS2") == 0)
        {
            TAC_MMG_CATS2 += (tac->m_PhysicsData->TAC_Time[i]*0.01369);
            TAC_MMG_CATS2_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_MMG_EXOGAM") == 0)
        {
            TAC_MMG_EXOGAM += (tac->m_PhysicsData->TAC_Time[i]*0.01346);
            TAC_MMG_EXOGAM_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_CATS_EXOGAM") == 0)
        {
            TAC_CATS_EXOGAM += (tac->m_PhysicsData->TAC_Time[i]*0.01281);
            TAC_CATS_EXOGAM_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_CATS_HF") == 0)
        {
            TAC_CATS_HF += (tac->m_PhysicsData->TAC_Time[i]*0.0014);
            TAC_CATS_HF_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        }
        else if (tac->m_PhysicsData->TAC_Name[i].compare("TAC_D4_CATS1") == 0) {
            TAC_D4_CATS1 += (tac->m_PhysicsData->TAC_Time[i]/**0.02554*/);
            TAC_D4_CATS1_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
           //TAC_D4_CATS1nc = tac->m_PhysicsData->TAC_Time[i];
           // if (TAC_D4_CATS1nc > 0) TAC_TOF = true;
        }
        // else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_1") == 0)
        // {
        //     TAC_PL_1 += (tac->m_PhysicsData->TAC_Time[i])+0670.0-19395;
        //     TAC_PL_1_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        //     norm_length=((TAC_PL_1-L_Bins[0])/perbin_value[0])-10;
        //     h2dPL->Fill(norm_length,5);
        // }
        // else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_2") == 0)
        // {
        //     TAC_PL_2 += (tac->m_PhysicsData->TAC_Time[i])+0105.0-19395;
        //     TAC_PL_2_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        //     norm_length=((TAC_PL_2-L_Bins[1])/perbin_value[1])-10;
        //     h2dPL->Fill(norm_length,4);
        // }
        // else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_3") == 0)
        // {
        //     TAC_PL_3 += (tac->m_PhysicsData->TAC_Time[i])+0000.0-19395;
        //     TAC_PL_3_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        //     norm_length=((TAC_PL_3-L_Bins[2])/perbin_value[2])-10;
        //     h2dPL->Fill(norm_length,3);
        // }
        // else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_4") == 0)
        // {
        //     TAC_PL_4 += (tac->m_PhysicsData->TAC_Time[i])+1217.0-19395;
        //     TAC_PL_4_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        //     norm_length=((TAC_PL_4-L_Bins[3])/perbin_value[3])-10;
        //     h2dPL->Fill(norm_length,2);
        // }
        // else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_5") == 0)
        // {
        //     TAC_PL_5 += (tac->m_PhysicsData->TAC_Time[i])+0474.0-19395;
        //     TAC_PL_5_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        //     norm_length=((TAC_PL_5-L_Bins[4])/perbin_value[4])-10;
        //     h2dPL->Fill(norm_length,1);
        // } 
    //    PS: uncomment above for e869

        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_1") == 0)
        {
            TAC_PL_1 += (tac->m_PhysicsData->TAC_Time[i]) // +0670.0-19395
            ;
            TAC_PL_1_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
            // norm_length=((TAC_PL_1-L_Bins[0])/perbin_value[0])-10;
            // h2dPL->Fill(norm_length,5);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_2") == 0)
        {
            TAC_PL_2 += (tac->m_PhysicsData->TAC_Time[i])// +0105.0-19395
            ;
            TAC_PL_2_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
            // norm_length=((TAC_PL_2-L_Bins[1])/perbin_value[1])-10;
            // h2dPL->Fill(norm_length,4);
        }
        // else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_3") == 0)
        // {
        //     TAC_PL_3 += (tac->m_PhysicsData->TAC_Time[i])// +0000.0-19395
        //     ;
        //     TAC_PL_3_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        //     // norm_length=((TAC_PL_3-L_Bins[2])/perbin_value[2])-10;
        //     // h2dPL->Fill(norm_length,3);
        // }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_3") == 0)           /*FIXME: changing  the plastic 3 to normal to take into acc the plastic other than cosmics that passed through all plastsics */
        {
            TAC_PL_3 += (tac->m_PhysicsData->TAC_Time[i]);
            TAC_PL_3_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
            // cout<<"TAC_PL_3 value passing: "<<TAC_PL_3<<endl;
            // norm_length=((TAC_PL_3-L_Bins[2])/perbin_value[2])-10;
            // h2dPL->Fill(norm_length,3);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_4") == 0)
        {
            TAC_PL_4 += (tac->m_PhysicsData->TAC_Time[i]) //+1217.0-19395
            ;
            TAC_PL_4_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
            // norm_length=((TAC_PL_4-L_Bins[3])/perbin_value[3])-10;
            // h2dPL->Fill(norm_length,2);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_PL_5") == 0)
        {
            TAC_PL_5 += (tac->m_PhysicsData->TAC_Time[i])//+0474.0-19395
            ;
            TAC_PL_5_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
            // norm_length=((TAC_PL_5-L_Bins[4])/perbin_value[4])-10;
            // h2dPL->Fill(norm_length,1);
        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_DD6_DD4") == 0)
        {
            TAC_DD6_DD4 += (tac->m_PhysicsData->TAC_Time[i]*0.00749);
            TAC_DD6_DD4_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);

        }
        else if(tac->m_PhysicsData->TAC_Name[i].compare("TAC_CATSD6_1_DD4") == 0)
        {
            TAC_CATSD6_1_DD4 += (tac->m_PhysicsData->TAC_Time[i]*0.00691);
            TAC_CATSD6_1_DD4_TS.push_back(tac->m_PhysicsData->TAC_TS[i]);
        }
    }

} 


void user_analysis::Analysis_TAC::SetDataOutput(std::shared_ptr<nptool::VDataOutput> output) {
  auto Tree = std::dynamic_pointer_cast<nptool::RootOutput>(output)->GetTree();


  Tree->Branch("Spot_Plastic_3",&Spot_Plastic_3);

  Tree->Branch("TAC_CATS_PL",&TAC_CATS_PL);
  Tree->Branch("TAC_CATS_PL_TS",&TAC_CATS_PL_TS);
  Tree->Branch("TAC_CATS2_CATS1",&TAC_CATS2_CATS1);
  Tree->Branch("TAC_CATS2_CATS1_TS",&TAC_CATS2_CATS1_TS);
  Tree->Branch("TAC_MMG_CATS1",&TAC_MMG_CATS1);
  Tree->Branch("TAC_MMG_CATS1_TS",&TAC_MMG_CATS1_TS);
  Tree->Branch("TAC_MMG_CATS2",&TAC_MMG_CATS2);
  Tree->Branch("TAC_MMG_CATS2_TS",&TAC_MMG_CATS2_TS);
  Tree->Branch("TAC_MMG_EXOGAM",&TAC_MMG_EXOGAM);
  Tree->Branch("TAC_MMG_EXOGAM_TS",&TAC_MMG_EXOGAM_TS);
  Tree->Branch("TAC_CATS_EXOGAM",&TAC_CATS_EXOGAM);
  Tree->Branch("TAC_CATS_EXOGAM_TS",&TAC_CATS_EXOGAM_TS);
  Tree->Branch("TAC_CATS_HF",&TAC_CATS_HF);
  Tree->Branch("TAC_CATS_HF_TS",&TAC_CATS_HF_TS);
  Tree->Branch("TAC_PL_1",&TAC_PL_1);
  Tree->Branch("TAC_PL_1_TS",&TAC_PL_1_TS);
  Tree->Branch("TAC_PL_2",&TAC_PL_2);
  Tree->Branch("TAC_PL_2_TS",&TAC_PL_2_TS);
  Tree->Branch("TAC_PL_3",&TAC_PL_3);
  Tree->Branch("TAC_PL_3_TS",&TAC_PL_3_TS);
  Tree->Branch("TAC_PL_4",&TAC_PL_4);
  Tree->Branch("TAC_PL_4_TS",&TAC_PL_4_TS);
  Tree->Branch("TAC_PL_5",&TAC_PL_5);
  Tree->Branch("TAC_PL_5_TS",&TAC_PL_5_TS);
  Tree->Branch("TAC_DD6_DD4",&TAC_DD6_DD4);
  Tree->Branch("TAC_DD6_DD4_TS",&TAC_DD6_DD4_TS);
  Tree->Branch("TAC_D4_CATS1",&TAC_D4_CATS1);
  Tree->Branch("TAC_D4_CATS1_TS",&TAC_D4_CATS1_TS);
  Tree->Branch("TAC_CATSD6_1_DD4",&TAC_CATSD6_1_DD4);
  Tree->Branch("TAC_CATSD6_1_DD4_TS",&TAC_CATSD6_1_DD4_TS);
  
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_TAC::Clear() {
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


    Spot_Plastic_3 = 0;
}



////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_TAC::End() {}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserAnalysis> ConstructUserAnalysis() { return std::make_shared<user_analysis::Analysis_TAC>(); }
}

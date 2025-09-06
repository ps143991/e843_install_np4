#include "Analysis_ZDD.h"
#include "NPApplication.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_ZDD::Init() {
  auto app = nptool::Application::GetApplication();

    tac = std::dynamic_pointer_cast<tac::TacDetector>(app->GetDetector("tac"));
    zdd = std::dynamic_pointer_cast<zdd::ZddDetector>(app->GetDetector("zdd"));
}


////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_ZDD::TreatEvent() {
    Clear();
    TreatZDD();
    
}

void user_analysis::Analysis_ZDD::TreatZDD(){

    int IC_M = zdd->m_PhysicsData->IC_E.size();
    for (int i=0; i<IC_M;i++){
        //cout<<"IC_E_ "<<zdd->m_PhysicsData->IC_Nbr[i]<<" energy "<<zdd->m_PhysicsData->IC_E[i]<<endl;
        if(zdd->m_PhysicsData->IC_Nbr[i]==0)      {IC_1.push_back(zdd->m_PhysicsData->IC_E[i]);   /* if(TAC_CATS_PL>0 && zdd->m_PhysicsData->IC_E[i]>0)counterPS1++; */}
        else if(zdd->m_PhysicsData->IC_Nbr[i]==1) {IC_2.push_back(zdd->m_PhysicsData->IC_E[i]);   /* if(TAC_CATS_PL>0 && zdd->m_PhysicsData->IC_E[i]>0)counterPS2++; */}
        else if(zdd->m_PhysicsData->IC_Nbr[i]==2) {IC_3.push_back(zdd->m_PhysicsData->IC_E[i]);   /* if(TAC_CATS_PL>0 && zdd->m_PhysicsData->IC_E[i]>0)counterPS3++; */}
        else if(zdd->m_PhysicsData->IC_Nbr[i]==3) {IC_4.push_back(zdd->m_PhysicsData->IC_E[i]);}
        else if(zdd->m_PhysicsData->IC_Nbr[i]==4) {IC_5.push_back(zdd->m_PhysicsData->IC_E[i]);}
        else if(zdd->m_PhysicsData->IC_Nbr[i]==5) {IC_9.push_back(zdd->m_PhysicsData->IC_E[i]);}
        
    }
    //cout<<""<<endl;
    /*bool allZero = true;
    for (int lll = 0; lll < IC_1.size(); lll++) {
        if (IC_1[lll] == 0) {
            // Check if all remaining entries are also zero
            for (int mmm = lll + 1; mmm < IC_1.size(); mmm++) {
                if (IC_1[mmm] != 0) {
                    allZero = false;
                    break;
                }
            }
            if (allZero) {
                counterPS++;
                //std::cout << "All further entries of IC_1 are 0" << std::endl;
                
            }
            break; // stop checking after first zero
        }
    }*/

    //IC_ZDD_sum = zdd->m_PhysicsData->SUMofIC_PS;
    if (!(IC_1.empty()) && !(IC_2.empty()) && !(IC_3.empty()) && !(IC_4.empty()) && !(IC_5.empty())){
    IC_ZDD_sum =  IC_1[0] + IC_2[0] + IC_3[0] + IC_4[0] + IC_5[0];
    }

    for(int i=0; i<zdd->m_PhysicsData->PL_E.size() ; i++){
        if(zdd->m_PhysicsData->PL_Nbr[i]==0) PL_E1.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==1) PL_E2.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==2) PL_E3.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==3) PL_E4.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==4) PL_E5.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==5) PL_E6.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==6) PL_E7.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==7) PL_E8.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==8) PL_E9.push_back(zdd->m_PhysicsData->PL_E[i]);
        else if(zdd->m_PhysicsData->PL_Nbr[i]==9) PL_E10.push_back(zdd->m_PhysicsData->PL_E[i]);
        //PL_center= TMath::Sqrt(PL_E3[i]*PL_E8[i]);
        //PL_centerLog = TMath::Log(PL_E3[i]/PL_E8[i]);

    }
    for(int i=0; i<zdd->m_PhysicsData->DC_E.size() ; i++){
             if(zdd->m_PhysicsData->DC_Nbr[i]==0) {DC_E1.push_back(zdd->m_PhysicsData->DC_E[i]); DC_TS1.push_back(zdd->m_PhysicsData->DC_TS[i]);}
        else if(zdd->m_PhysicsData->DC_Nbr[i]==1) {DC_E2.push_back(zdd->m_PhysicsData->DC_E[i]); DC_TS2.push_back(zdd->m_PhysicsData->DC_TS[i]);}
        else if(zdd->m_PhysicsData->DC_Nbr[i]==2) {DC_E3.push_back(zdd->m_PhysicsData->DC_E[i]); DC_TS3.push_back(zdd->m_PhysicsData->DC_TS[i]);}
        else if(zdd->m_PhysicsData->DC_Nbr[i]==3) {DC_E4.push_back(zdd->m_PhysicsData->DC_E[i]); DC_TS4.push_back(zdd->m_PhysicsData->DC_TS[i]);}

    }

    for(int i=0; i<zdd->m_PhysicsData->Exo_E.size() ; i++){
        if(zdd->m_PhysicsData->Exo_Nbr[i]==0) EXO_E1.push_back(zdd->m_PhysicsData->Exo_E[i]);
        else if(zdd->m_PhysicsData->Exo_Nbr[i]==1) EXO_E2.push_back(zdd->m_PhysicsData->Exo_E[i]);
        else if(zdd->m_PhysicsData->Exo_Nbr[i]==2) EXO_E3.push_back(zdd->m_PhysicsData->Exo_E[i]);
        else if(zdd->m_PhysicsData->Exo_Nbr[i]==3) EXO_E4.push_back(zdd->m_PhysicsData->Exo_E[i]);
    }

}


void user_analysis::Analysis_ZDD::SetDataOutput(std::shared_ptr<nptool::VDataOutput> output) {
  auto Tree = std::dynamic_pointer_cast<nptool::RootOutput>(output)->GetTree();
  Tree->Branch("PL_E1",&PL_E1);
  Tree->Branch("PL_E2",&PL_E2);
  Tree->Branch("PL_E3",&PL_E3);
  Tree->Branch("PL_E4",&PL_E4);
  Tree->Branch("PL_E5",&PL_E5);
  Tree->Branch("PL_E6",&PL_E6);
  Tree->Branch("PL_E7",&PL_E7);
  Tree->Branch("PL_E8",&PL_E8);
  Tree->Branch("PL_E9",&PL_E9);
  Tree->Branch("PL_E10",&PL_E10);
  Tree->Branch("IC_1",&IC_1);
  Tree->Branch("IC_2",&IC_2);
  Tree->Branch("IC_3",&IC_3);
  Tree->Branch("IC_4",&IC_4);
  Tree->Branch("IC_5",&IC_5);
  Tree->Branch("DC_1",&DC_E1);
  Tree->Branch("DC_2",&DC_E2);
  Tree->Branch("DC_3",&DC_E3);
  Tree->Branch("DC_4",&DC_E4);
  Tree->Branch("DC_TS1",&DC_TS1);
  Tree->Branch("DC_TS2",&DC_TS2);
  Tree->Branch("DC_TS3",&DC_TS3);
  Tree->Branch("DC_TS4",&DC_TS4);
  Tree->Branch("EXO_1",&EXO_E1);
  Tree->Branch("EXO_2",&EXO_E2);
  Tree->Branch("EXO_3",&EXO_E3);
  Tree->Branch("EXO_4",&EXO_E4);
  Tree->Branch("IC_ZDD_sum",&IC_ZDD_sum);
  Tree->Branch("PL_center",&PL_center);
  Tree->Branch("PL_centerLog",&PL_centerLog);
  
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_ZDD::Clear() {
    IC_ZDD_sum = 0.0;
    PL_center= 0.0;
    PL_centerLog= 0.0;
    
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
    DC_TS1.clear();
    DC_TS2.clear();
    DC_TS3.clear();
    DC_TS4.clear();
    EXO_E1.clear();
    EXO_E2.clear();
    EXO_E3.clear();
    EXO_E4.clear();
    //norm_length=0;
}



////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_ZDD::End() {}
////////////////////////////////////////////////////////////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserAnalysis> ConstructUserAnalysis() { return std::make_shared<user_analysis::Analysis_ZDD>(); }
}

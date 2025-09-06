#include"Geometry_Clover_Exogam.h"
#include "TMust2Physics.h"
#include "TMugastPhysics.h"
#include"NPVAnalysis.h"
#include"TZDDPhysics.h"
#include"NPEnergyLoss.h"
#include"NPFunction.h"
#include"NPReaction.h"
#include"NPOptionManager.h"
#include"RootInput.h"
#include"RootOutput.h"
#include"TInitialConditions.h"
#include "TReactionConditions.h"
#include"NPParticle.h"
#include"NPBeam.h"
#include "TCutG.h"
#include<random>

void DrawTrajectory(){

  std::map<std::string, TChain*> Chains;
  Chains["Physics"] = new TChain("PhysicsTree"); 
 
  for(const auto &i:Chains){
    //i.second->Add("./NPreduced/r0517_3us_reduced.root");
    //i.second->Add("./NPreduced/r0518_3us_reduced.root");
    //i.second->Add("./NPreduced/r0515_reduced.root");
    i.second->Add("./r1_t1_negatifCATCorrection_L21TOF.root");
   // i.second->Add("./NPreduced/r0517_reduced.root");
   // i.second->Add("./NPreduced/r0518_reduced.root");
    // i.second->Add("./NPreduced/r0519_reduced.root");
   // i.second->Add("./NPreduced/r0520_reduced.root");
   // i.second->Add("./NPreduced/r0521_reduced.root");
  }

  TCATSPhysics* CATS = new TCATSPhysics();
  Chains["Physics"]->SetBranchAddress("CATS_", &CATS); 

TH2F *Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 4000, 6000, 10000, 1000, -499, 500 );
    TH2F *Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 4000, 6000, 10000, 1000, -499, 500 );
float ax[2], ay[2];
float Position[4]={6035.5,6532.5,7622.6,8883.5};

  unsigned int size = Chains["Physics"]->GetEntries();
  for(unsigned int i = 0 ; i < size ; i++){
    Chains["Physics"]->GetEntry(i);

 	if(CATS->PositionOnTargetX > -1000 && CATS->PositionOnTargetY > -1000){
       // BeamImpact = TVector3(-(CATS->PositionOnTargetX)+1.7,-(CATS->PositionOnTargetY)+2.3,0); ///correctone
        //    BeamImpact = TVector3(-(CATS->PositionOnTargetX)-1.38,-(CATS->PositionOnTargetY)-4.2,0); ///new one
      //  BeamDirection = TVector3(CATS->PositionX[0] - CATS->PositionX[1],CATS->PositionY[0] - CATS->PositionY[1],-(CATS->PositionZ[0] - CATS->PositionZ[1]));

        ax[0]=(Position[1] - Position[0])/(CATS->PositionX[1] - CATS->PositionX[0]);
        ay[0]=(Position[1] - Position[0])/(CATS->PositionY[1] - CATS->PositionY[0]);
 for(int k= Position[0]; k<Position[2]; k++){
 Trajectory_XZ->Fill(k, CATS->PositionX[0] + (k - Position[0])/ax[0]);
 Trajectory_YZ->Fill(k, CATS->PositionY[0] + (k - Position[0])/ay[0]);
  }

        // std::cout << " BeamImpact " << BeamImpact.X() << "  " << BeamImpact.Y() <<  "	"<< BeamImpact.Z() << "\n";
        //  std::cout << " BeamDirection " << BeamDirection.X() << "  " << BeamDirection.Y() <<  "	"<< BeamDirection.Z() << std::endl;
    
    }
  

    
    }
  }
 
  Trajectory_XZ->Draw("colz");
   //Trajectory_YZ->Draw("colz");
}

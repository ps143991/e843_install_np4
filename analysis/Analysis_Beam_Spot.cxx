#include "Analysis_Beam_Spot.h"
#include "NPApplication.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_Beam_SPOT::Init() {
  auto app = nptool::Application::GetApplication();

    tac = std::dynamic_pointer_cast<tac::TacDetector>(app->GetDetector("tac"));
    cats = std::dynamic_pointer_cast<cats::CatsDetector>(app->GetDetector("cats"));
    gatconf = std::dynamic_pointer_cast<ebye::EbyEDetector>(app->GetDetector("ebye"));
    zdd = std::dynamic_pointer_cast<zdd::ZddDetector>(app->GetDetector("zdd"));
    


    dl_dc_0       = new TH2D ("dl_dc_0","TS DIff. vs dl_x",1000, -1000, 1000, 400, -200, 200);
    dl_dc_1       = new TH2D ("dl_dc_1","TS DIff. vs dl_y",1000, -1000, 1000, 400, -200, 200);
    dl_dc_2       = new TH2D ("dl_dc_2","TS DIff. vs dl_x",1000, -1000, 1000, 400, -200, 200);
    dl_dc_3       = new TH2D ("dl_dc_3","TS DIff. vs dl_y",1000, -1000, 1000, 400, -200, 200);
    hdcxy00       = new TH2D ("hdcxy00","hdcxy00",1000, -1000, 1000, 4000, -2000, 2000);
    hdcxy01       = new TH2D ("hdcxy01","hdcxy01",1000, -1000, 1000, 4000, -2000, 2000);
    hdcxy10       = new TH2D ("hdcxy10","hdcxy10",1000, -1000, 1000, 4000, -2000, 2000);
    hdcxy11       = new TH2D ("hdcxy11","hdcxy11",1000, -1000, 1000, 4000, -2000, 2000);
    hdcxy         = new TH2D ("hdcxy","hdcxy",1000, -1000, 1000, 4000, -2000, 2000);
    hDC_reconst   = new TH2D ("hDC_reconst","hDC_reconst", 6400, -80, 80, 4000, -50, 50);

    test          = new TH2D ("test","Test",1000,-100,100,1000,-100,100);
    hDC0          = new TH1D("dc0","dc0",4000,0,16000);
    hDC1          = new TH1D("dc1","dc1",4000,0,16000);
    hDC2          = new TH1D("dc2","dc2",4000,0,16000);
    hDC3          = new TH1D("dc3","dc3",4000,0,16000);
    
    hDCpos        = new TH1D("hDCpos","hDCpos",400,-200,200);
    hgatconts     = new TH1D("hgatconts","hgatconts",300,0,3000);

    Trajectory_XZ = new TH2F("Trajectory_XZ", "Reconstruction of beam trajectory in detectors (X projection)", 3801, -1700, 2100, 1000, -499, 500 );
    Trajectory_YZ = new TH2F("Trajectory_YZ", "Reconstruction of beam trajectory in detectors (Y projection)", 3801, -1700, 2100, 1000, -499, 500 );
    hDC0ts        = new TH1D("dc0ts","dc0ts",1000,-400,600);        //VIEWING IN 10 NS TIMING WITHOUT DOWNBINNING
    hDC1ts        = new TH1D("dc1ts","dc1ts",1000,-400,600);
    hDC2ts        = new TH1D("dc2ts","dc2ts",1000,-400,600);
    hDC3ts        = new TH1D("dc3ts","dc3ts",1000,-400,600);

    hdcnum        = new TH1D("hdcnum","hdcnum",6,0,6);

    hdcnumx       = new TH1D("hdcnumx","hdcnumx",6,0,6);
    hdcnumy       = new TH1D("hdcnumy","hdcnumy",6,0,6);
    h2dPL         = new TH2F("Plastic_vs_lenght","Plastic_N vs lenght(20cm)",220,-1,21, 7,0,7);
    h2dPL_reconst = new TH2F("Plastic_vs_lenght_reconst","Plastic_N vs lenght_recons",800,-200,200, 1400,-350,350);
    hSpotPl       = new TH2F("hSpotPl","Plasticextrapolationspot",1000,-100,100,1000,-100,100);
    // h2d = new TH2F("h2d", "Beam Spot Analysis", 10,-5,5, 10,-5,5); //PS: histogram for beam spot analysis

    entriesPS = 400000;
}

void user_analysis::Analysis_Beam_SPOT::TreatGATCONF(){
   GATCONFMASTER=*(gatconf->GenericRawBranch["GATCONF"]);
   // if (GATCONFMASTER==1){
   if ( GATCONFMASTER==0 || GATCONFMASTER==1 || GATCONFMASTER==2 || GATCONFMASTER==16 || GATCONFMASTER==32){
    decider = true;
   // if (GATCONFMASTER==1 || GATCONFMASTER==2 || GATCONFMASTER==16 || GATCONFMASTER==32){

   // if(GATCONFMASTER.size()==1){
          //  decider = true;
       // }
   }

}



////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_Beam_SPOT::TreatEvent() {

    
      
  GATCONFMASTER= *(gatconf->GenericPhysicsBranch["GATCONF"]);
    
  GATCONFMASTERTS = *(gatconf->GenericPhysicsBranchTS["GATCONFTS"]);
  Clear();
  // if (static_cast<Long64_t>(h2d->GetEntries()) % 100000 == 0 && static_cast<Long64_t>(h2d->GetEntries()) <300001) h2d->Write("h2d",TObject::kOverwrite); //PS: fill histogram with dummy values for testing
  
  Further_Analysis();

  // if (static_cast<Long64_t>(h2d->GetEntries()) % 1000 == 0 && static_cast<Long64_t>(h2d->GetEntries()) <3001) h2d->Write();
  // if (h2d->GetEntries() ==1000){user_analysis::Analysis_Beam_SPOT::End();}

}


void user_analysis::Analysis_Beam_SPOT::Further_Analysis() {
    if (GATCONFMASTER == 32){
        
        if (cats->m_PhysicsData->PositionOnTargetY > -1000 && cats->m_PhysicsData->PositionOnTargetX > -1000) {

            targetX_byCATS = cats->m_PhysicsData->PositionOnTargetX;
            targetY_byCATS = cats->m_PhysicsData->PositionOnTargetY;
            
            hgatconts->Fill(GATCONFMASTERTS/1e7); //makes it in microseconds
            
            BeamDirection = TVector3(CATS1_X - CATS2_X,CATS1_Y - CATS2_Y,-(CATS1_Z - CATS2_Z));

            CATS1_X =  cats->m_PhysicsData->PositionX[0];
            CATS1_Y =  cats->m_PhysicsData->PositionY[0];
            CATS1_Z =  cats->m_PhysicsData->PositionZ[0];
            CATS2_X =  cats->m_PhysicsData->PositionX[1];
            CATS2_Y =  cats->m_PhysicsData->PositionY[1];
            CATS2_Z =  cats->m_PhysicsData->PositionZ[1];

            ay[0] = (Position[1] - Position[0])/(CATS2_Y - CATS1_Y);
            ax[0] = (Position[1] - Position[0])/(CATS2_X - CATS1_X);
            hSpotPl->Fill(CATS1_X + ( Position_ZDD[3]- Position[0])/ax[0],CATS1_Y + (Position_ZDD[3]- Position[0])/ay[0]); 

            for(int k= Position[0]; k<Position_ZDD[3]; k++){
                    Trajectory_XZ->Fill(k, CATS1_X + (k - Position[0])/ax[0]);
                    Trajectory_YZ->Fill(k, CATS1_Y + (k - Position[0])/ay[0]);
            }

            ////////////////////////////////////////////////////////Drift Chamber Part//////////////////////////////////////////

            dy_C1_DC = CATS1_Y + (Position_ZDD[0] - Position[0])/ay[0];
            dx_C1_DC = CATS1_X + (Position_ZDD[0] - Position[0])/ax[0];
            hDC_reconst->Fill(dx_C1_DC,dy_C1_DC); 


            auto sizeDC = zdd->m_PhysicsData->DC_E.size();    
            for (Int_t j =0; j<sizeDC;j++){

                
                if(zdd->m_PhysicsData->DC_E[j] > 0 ){                                                      //PS: can cause issues with multiple events #>4
                    hdcnum->Fill(zdd->m_PhysicsData->DC_Nbr[j]);
            
                    if(zdd->m_PhysicsData->DC_Nbr[j] == 2 ){
                        ix = 0;
                        //cout<<"ix1 " << ZDD->DC_Nbr[j] <<endl;
                        hDC2->Fill(zdd->m_PhysicsData->DC_E[j]);
                        // dt_x = (GATCONFMASTERTS-(zdd->m_PhysicsData->DC_TS[j]));
                        dt_x = static_cast<Long64_t>(GATCONFMASTERTS) - static_cast<Long64_t>(zdd->m_PhysicsData->DC_TS[j]);
                        // cout<<"for DC 2 GATCONFMASTERTS is: "<<GATCONFMASTERTS<<"and "<<"TS is "<<zdd->m_PhysicsData->DC_TS[j]<<" dt_x " << dt_x <<endl;
                        hDC2ts->Fill(dt_x);
                        hDCpos->Fill(dx_C1_DC);             // extrapolated position of DC
                        dl_dc_2->Fill(dt_x,dx_C1_DC);
                        dl_x1 = dt_x*dx_gain[0] + dx_off[0];
                        //cout<< "dt_x1 " << dt_x <<"dl_x1 " << dl_x1 <<endl;
                    }
                    else if(zdd->m_PhysicsData->DC_Nbr[j] == 3){
                        ix = 1;
                        // cout<<"ix2 " << ZDD->DC_Nbr[j] <<endl;
                        hDC3->Fill(zdd->m_PhysicsData->DC_E[j]);
                        // dt_x = (GATCONFMASTERTS-(zdd->m_PhysicsData->DC_TS[j]));
                        dt_x = static_cast<Long64_t>(GATCONFMASTERTS) - static_cast<Long64_t>(zdd->m_PhysicsData->DC_TS[j]);

                        // cout<<"for DC 3 "<<"dt_x " << dt_x <<endl;
                        
                        hDC3ts->Fill(dt_x);
                        dl_dc_3->Fill(dt_x,dx_C1_DC);
                        dl_x2 = dt_x*dx_gain[1] + dx_off[1];
                        //cout<< "dt_x2 " << dt_x <<"dl_x2 " << dl_x2 <<endl;
                    }
                    else if(zdd->m_PhysicsData->DC_Nbr[j] == 1 ){
                        iy = 0;
                        hDC1->Fill(zdd->m_PhysicsData->DC_E[j]);
                        // dt_y = -(GATCONFMASTERTS - (zdd->m_PhysicsData->DC_TS[j]));
                        dt_y = static_cast<Long64_t>(GATCONFMASTERTS) - static_cast<Long64_t>(zdd->m_PhysicsData->DC_TS[j]);

                        // cout<<"for DC 1 "<<"dt_y " << dt_x <<endl;

                        hDC1ts->Fill(dt_y);
                        dl_dc_1->Fill(dt_y,dy_C1_DC);
                        dl_y1 = dt_y*dy_gain[0] + dy_off[0];
                        //cout<< "dt_y1 " << dt_y <<"dl_y1 " << dl_y1 <<endl;
                    }
                    else if(zdd->m_PhysicsData->DC_Nbr[j] == 0){
                        iy = 1;
                        hDC0->Fill(zdd->m_PhysicsData->DC_E[j]);
                        // dt_y = -(GATCONFMASTERTS - (zdd->m_PhysicsData->DC_TS[j]));
                        dt_y = static_cast<Long64_t>(GATCONFMASTERTS) - static_cast<Long64_t>(zdd->m_PhysicsData->DC_TS[j]);
                        // cout<<"for DC 0 GATCONFMASTERTS is: "<<GATCONFMASTERTS<<"and "<<"TS is "<<zdd->m_PhysicsData->DC_TS[j]<<" dt_x " << dt_y <<endl;

                        // cout<<"for DC 0 "<<"dt_x " << dt_x <<endl;

                        hDC0ts->Fill(dt_y);
                        dl_dc_0->Fill(dt_y,dy_C1_DC);
                        dl_y2 = dt_y*dy_gain[1] + dy_off[1];
                        //cout<< "dt_y2 " << dt_y <<"dl_y2 " << dl_y2 <<endl;
                    }
                    hdcnumx->Fill(ix);
                    hdcnumy->Fill(iy);
                }
                
                // x_const  =
                





                if(ix==0 && iy==0)      hdcxy00->Fill(dt_x,dt_y);
                else if(ix==0 && iy==1) hdcxy01->Fill(dt_x,dt_y);
                else if(ix==1 && iy==0) hdcxy10->Fill(dt_x,dt_y);
                else if(ix==1 && iy==1) hdcxy11->Fill(dt_x,dt_y);
                if(ix >= 0 && iy >=0){

                    Double_t ran1 = rand() % 100000001;
                    Double_t ran2 = rand() % 100000001;
                    ran1 = ran1/100000000-0.5;
                    ran2 = ran2/100000000-0.5;
                    dt_x = dt_x + ran1;
                    dt_y = dt_y + ran2;
                    dl_x = dt_x*dx_gain[ix] + dx_off[ix];
                    dl_y = dt_y*dy_gain[iy] + dy_off[iy];
                    hdcxy->Fill(dl_x,dl_y);

                }
                // hdcxy->Fill(dl_x1+dl_x2,dl_y1+dl_y2);
                // dt_y = -(CATS_TS - ZDD->DC_TS[iy]);
                // dt_x = -(CATS_TS - ZDD->DC_TS[ix]);
                BeamImpact = TVector3(-(cats->m_PhysicsData->PositionOnTargetX)+1.7,-(cats->m_PhysicsData->PositionOnTargetY)+2.3,0);
                if(zdd->m_PhysicsData->DC_Nbr[j] == 2){
                    test->Fill(BeamImpact.X(),BeamImpact.Y());
                }
                if(zdd->m_PhysicsData->DC_Nbr[j] == 3){
                    //test->Fill(CATS1_X+30,CATS1_Y+30);
                }
                if(zdd->m_PhysicsData->DC_Nbr[j] == 1){
                    //  test->Fill(CATS1_X+30,CATS1_Y-30);
                }

                if(zdd->m_PhysicsData->DC_Nbr[j] == 0){
                    //test->Fill(CATS1_X-30,CATS1_Y-30);
                }
            }
                        
            dy_C1_PL = CATS1_Y + (Position_ZDD[3]-Position[0])/ay[0];
            dx_C1_PL = CATS1_X + (Position_ZDD[3]-Position[0])/ax[0];
        }
    }
}

void user_analysis::Analysis_Beam_SPOT::SetDataOutput(std::shared_ptr<nptool::VDataOutput> output) {
  auto Tree = std::dynamic_pointer_cast<nptool::RootOutput>(output)->GetTree();


  outFile = std::dynamic_pointer_cast<nptool::RootOutput>(output)->GetFile(); //PS

}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// void user_analysis::Analysis_Beam_SPOT::Clear() {
//       Spot_Plastic_3 = 0;
//       TAC_PL_3 = 0;
// }

void user_analysis::Analysis_Beam_SPOT::Clear() {
  // GATCONFMASTER = 0;
    dx_C12 = 999.;
    dy_C12 = 999.;
    CATS1_X = -1000.;
    CATS1_Y = -1000.;
    CATS1_Z = -1000.;
    CATS2_X = -1000.;
    CATS2_Y = -1000.;
    CATS2_Z = -1000.;


    dx_C1_DC = 999.;
    dx_C1_PL = 999.;
    dy_C1_DC = 999.;
    dy_C1_PL = 999.;
    targetY_byCATS = -1000.;
    targetX_byCATS = -1000.;

    ix = -1;
    iy = -1;
    dt_x  = -1.;
    dt_y  = -1.;
    dl_x  = -1.;
    dl_y  = -1.;
    dl_x1 = -1.;
    dl_y1 = -1.;
    dl_x2 = -1.;
    dl_y2 = -1.;
    dt_x1 = -1.;

}

////////////////////////////////////////////////////////////////////////////////
void user_analysis::Analysis_Beam_SPOT::End() {
  if(outFile){
      outFile->cd(); 
      dl_dc_0->Write("");
      dl_dc_0->Write("");
      dl_dc_1->Write("");
      dl_dc_2->Write("");
      dl_dc_3->Write("");
      hdcxy00->Write("");
      hdcxy10->Write("");
      hdcxy01->Write("");
      hdcxy11->Write("");
      hdcnum->Write("");
      hdcnumx->Write("");
      h2dPL->Write("");
      hdcnumy->Write("");
      h2dPL_reconst->Write("");
      hdcxy->Write("");
      hDC0->Write();
      hDC1->Write();
      hDC2->Write();
      hDC3->Write();
      Trajectory_XZ->Write();
      Trajectory_YZ->Write();
      hDC0ts->Write();
      hDC1ts->Write();
      hDC2ts->Write();
      hDC3ts->Write();
      hDCpos->Write();
      hgatconts->Write();
      hSpotPl->Write();
      hDC_reconst->Write();
  }
  else cout<<"PS: error in End() of Analysis_Beam_SPOT: h2d or outFile is null"<<endl; 
    gDirectory->cd(outFile->GetPath());
   
    TFile *outFile2 = new TFile("/home/sharmap/Workplace/np4/e843/data/analysed/Analysis_Beam_Spot_558.root", "RECREATE"); //PS: create a new file to store the histograms
    if (outFile2) {
      outFile2->cd();
      dl_dc_0->Write("");
      dl_dc_0->Write("");
      dl_dc_1->Write("");
      dl_dc_2->Write("");
      dl_dc_3->Write("");
      hdcxy00->Write("");
      hdcxy10->Write("");
      hdcxy01->Write("");
      hdcxy11->Write("");
      hdcnum->Write("");
      hdcnumx->Write("");
      h2dPL->Write("");
      hdcnumy->Write("");
      h2dPL_reconst->Write("");
      hdcxy->Write("");
      hDC0->Write();
      hDC1->Write();
      hDC2->Write();
      hDC3->Write();
      Trajectory_XZ->Write();
      Trajectory_YZ->Write();
      hDC0ts->Write();
      hDC1ts->Write();
      hDC2ts->Write();
      hDC3ts->Write();
      hDCpos->Write();
      hgatconts->Write();
      hSpotPl->Write();
      hDC_reconst->Write();
      outFile2->Close();
      delete outFile2; //PS: delete the file to free memory
    }
    outFile->cd();

}
/////////////////////////////////////////////////////   ///////////////////////////
extern "C" {
std::shared_ptr<nptool::VUserAnalysis> ConstructUserAnalysis() { return std::make_shared<user_analysis::Analysis_Beam_SPOT>(); }
}

// g4 headers
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4VTrajectory.hh"
#include "G4RegionStore.hh"

// nptool geant4
#include "NPG4RunAction.h"
//----------------------//

using namespace nptool::geant4;

RunAction *RunAction::m_RunAction = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::RunAction(): G4UserRunAction(){
  m_RunAction = this;
  // RootOutput::getInstance()->GetTree()->Branch("Run",&m_RunNumber);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::~RunAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::BeginOfRunAction(const G4Run* aRun){

    // cout << "Starting run " << aRun->GetRunID()+1;
    // Replug the branch in case it no longer exist (e.g. reloading geometry) 
    // RootOutput::getInstance()->GetTree()->SetBranchAddress("Run",&m_RunNumber);

    // Increment the run number to be stored in the tree
    // m_RunNumber = aRun->GetRunID()+1;
    
    //initialize event cumulative quantities
    m_EventCount = aRun->GetNumberOfEventToBeProcessed();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::EndOfRunAction(const G4Run* aRun){
    // Pass a line for nicer presentation when chainning event generator
    std::cout << std::endl;
    // // Force the tree to be saved at the end of the run
    // RootOutput::getInstance()->GetTree()->AutoSave("Overwrite SaveSelf");
    // // Write DEDX Tables
    // std::set<string> Particles;
    // const std::vector<const G4Event*>* events = aRun->GetEventVector();
    // unsigned int sizeE = events->size();
    // for(unsigned int e = 0 ; e < sizeE ; e++){
    //   TrajectoryVector* traj = (*events)[e]->GetTrajectoryContainer()->GetVector();
    //   unsigned int size = traj->size();

    //   for(unsigned int i = 0 ; i < size ; i++)
    //     Particles.insert( (*traj)[i]->GetParticleName());
    // }
    // MaterialManager::getInstance()->WriteDEDXTable(Particles,0,10*GeV); 
    // MaterialManager::getInstance()->WriteCrossSectionTable(Particles,0,19.9*MeV); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction *RunAction::GetInstance() { return m_RunAction; }



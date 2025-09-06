TChain* PhysicsTree;
TCutG* cutd;
TCutG* cutSi3;
TCutG* cutSi4;

void Chain() {
  PhysicsTree = new TChain("PhysicsTree");
  TFile* fcutd = new TFile("macroVal/cutd.root");  
  cutd = (TCutG*) fcutd->FindObjectAny("cutd"); 
  TFile* fcutSi3 = new TFile("macroVal/cutSi3.root");  
  cutSi3 = (TCutG*) fcutSi3->FindObjectAny("cutSi3"); 
  TFile* fcutSi4 = new TFile("macroVal/cutSi4.root");  
  cutSi4 = (TCutG*) fcutSi4->FindObjectAny("cutSi4"); 

  for (unsigned int i = 0; i < 5; i++) {
    PhysicsTree->Add(Form("output/analysis/run_012%i_0.root", i));
  }
}

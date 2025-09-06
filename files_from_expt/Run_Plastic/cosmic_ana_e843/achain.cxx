
void achain(){
  
  TChain*  chain = new TChain("AD");

//  // Tuning
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0500_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0500_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0500_002a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0501_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0502_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0503_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0504_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0505_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0506_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0507_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0508_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0509_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0509_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0510_000a.root");
//
//  // CH2 + 38 MeV/u
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0511_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0512_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0514_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0515_000a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0516_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0516_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0516_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0516_003a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0516_004a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0517_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0517_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0517_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0517_003a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0517_004a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0517_005a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0517_006a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0518_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0518_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0518_002a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0519_000a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0520_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0520_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0520_003a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0520_004a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0520_005a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0520_006a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0521_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0521_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0521_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0521_003a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0521_004a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0521_005a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0521_006a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0522_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0522_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0522_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0522_003a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0522_004a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0522_005a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0523_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0523_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0523_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0523_003a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0524_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0524_001a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0525_000a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0526_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0526_001a.root");
//
//  // Tuning 18 MeV/u
//  //  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0527_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0527_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0528_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0529_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0530_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0531_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0532_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0533_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0534_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0535_000a.root");
//
//  // CD2 + 18 MeV/u
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0536_000a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0537_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0537_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0537_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0537_003a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0538_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0538_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0538_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0538_003a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0538_004a.root");
//
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0539_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0539_001a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0539_002a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0539_003a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0539_004a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0539_005a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0539_006a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0539_007a.root");


//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0522_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0537_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0545_*a.root");

//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0555_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0555_024a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0555_025a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0556_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0557_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0558_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0559_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0560_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0561_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0562_*a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0563_*a.root");
  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0564_*a.root");

//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0564_000a.root");
//  chain->Add("/data/lise2023X/zdd/acquisition/RootA_e843/r0564_001a.root");

}

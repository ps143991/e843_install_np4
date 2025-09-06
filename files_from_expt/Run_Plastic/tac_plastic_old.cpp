#include <TH1F.h>
#include <TRoot.h>
TFile* file = TFile::Open("r0031_000a.root");
Double_t N_events; 	//number of events
TTree *ad = (TTree*)file->Get("AD");

Int_t bin_min, bin_max;
bin_max=28000;
bin_min=18000;
String_t name_series = "TAC_PL_cut_1";
String_t name_cut_tacs[5];
for (int i = 0; i<5; i++){
    name_cut_tacs[i]= name_series+i;
}

TH1F *hpl1, *hpl2, *hpl3, *hpl4, *hpl5;
ad->Draw("TAC_PL_1>>hpl1 (6400,1,64001)","","");
ad->Draw("TAC_PL_2>>hpl2 (6400,1,64001)","","");
ad->Draw("TAC_PL_3>>hpl3 (6400,1,64001)","","");
ad->Draw("TAC_PL_4>>hpl4 (6400,1,64001)","","");
ad->Draw("TAC_PL_5>>hpl5 (6400,1,64001)","","");

for (int j = 0; j<5; j++){
    TH1F *h[j] = new TH1F (name_cut_tacs[j],10000,18000,28000);
    



for (int i = 1; i<N_events+1; i++){

}
}

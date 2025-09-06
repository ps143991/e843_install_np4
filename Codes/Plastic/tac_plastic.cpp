

using namespace std;

int tac_plastic() {
    TChain *ad = new TChain("AD");
    ad->Add("r0031_000a.root");

    UShort_t TAC_PL_1, TAC_PL_2, TAC_PL_3, TAC_PL_4, TAC_PL_5;

    ad->SetBranchStatus("TAC_PL_1",true); ad->SetBranchAddress("TAC_PL_1",&TAC_PL_1);
    ad->SetBranchStatus("TAC_PL_2",true); ad->SetBranchAddress("TAC_PL_2",&TAC_PL_2);
    ad->SetBranchStatus("TAC_PL_3",true); ad->SetBranchAddress("TAC_PL_3",&TAC_PL_3);
    ad->SetBranchStatus("TAC_PL_4",true); ad->SetBranchAddress("TAC_PL_4",&TAC_PL_4);
    ad->SetBranchStatus("TAC_PL_5",true); ad->SetBranchAddress("TAC_PL_5",&TAC_PL_5);


    Int_t n_bins = 800;
    Double_t bin_min = 1;
    Double_t bin_max = 8001;

    TH1F* hpl[5];
	
    hpl[0] = new TH1F("hpl1", "TAC_PL_2 - 18710", n_bins, bin_min, bin_max);
    hpl[1] = new TH1F("hpl2", "TAC_PL_1 - 18210", n_bins, bin_min, bin_max);
    hpl[2] = new TH1F("hpl3", "TAC_PL_3 - 18710", n_bins, bin_min, bin_max);
    hpl[3] = new TH1F("hpl4", "TAC_PL_4 - 17810", n_bins, bin_min, bin_max);
    hpl[4] = new TH1F("hpl5", "TAC_PL_5 - 18710", n_bins, bin_min, bin_max);
    
    TH2F* h2d = new TH2F("h2d","j:h2d", 5, 0.5, 5.5, 20, 0, 20);
    
    
    TCanvas* c1 = new TCanvas("c1", "Canvas", 800, 600);
    
   
    ad->Draw("(TAC_PL_2-18710)/500>>hpl[0]", "TAC_PL_1>1 && TAC_PL_2>1 && TAC_PL_3>1 && TAC_PL_4>1 && TAC_PL_5>1", "");
    ad->Draw("(TAC_PL_1-18210)/500>>hpl[1]", "TAC_PL_1>1 && TAC_PL_2>1 && TAC_PL_3>1 && TAC_PL_4>1 && TAC_PL_5>1", "same");
    ad->Draw("(TAC_PL_3-18710)/500>>hpl[2]", "TAC_PL_1>1 && TAC_PL_2>1 && TAC_PL_3>1 && TAC_PL_4>1 && TAC_PL_5>1", "same");
    ad->Draw("(TAC_PL_4-17810)/500>>hpl[3]", "TAC_PL_1>1 && TAC_PL_2>1 && TAC_PL_3>1 && TAC_PL_4>1 && TAC_PL_5>1", "same");
    ad->Draw("(TAC_PL_5-18710)/500>>hpl[4]", "TAC_PL_1>1 && TAC_PL_2>1 && TAC_PL_3>1 && TAC_PL_4>1 && TAC_PL_5>1", "same");
    TCanvas* c2 = new TCanvas("c2", "Canvas_2", 800, 600);
    c2->cd();
    Double_t N_events = ad->GetEntries();
    Double_t processed_entries;
    for (Int_t i = 0; i<N_events; i++){
        processed_entries = 100.*i/N_events ;
        if (i%100000 == 0) cout << "\rEntry " << i << Form("---- Entry processed : %.2f%%",processed_entries) << flush;
        ad->GetEntry(i);

        if (TAC_PL_1>1 && TAC_PL_2>1 && TAC_PL_3>1 && TAC_PL_4>1 && TAC_PL_5>1){    
        h2d->Fill(1, (TAC_PL_1 - 18210)/500);
        h2d->Fill(2, (TAC_PL_2 - 18710)/500);
        h2d->Fill(3, (TAC_PL_3 - 18710)/500);
        h2d->Fill(4, (TAC_PL_4 - 17810)/500);
        h2d->Fill(5, (TAC_PL_5 - 18710)/500);
        }
        h2d->Draw("colz");

    }   
    return 0;
}

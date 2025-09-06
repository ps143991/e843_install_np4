#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TBranch.h>
#include <iostream>
#include <thread>
#include <TGraph.h>
#include <TCanvas.h>


using namespace std;
int trajectory_pl3(){
TFile *file1 = TFile::Open("r0031_000a.root");
TTree *ad = (TTree*)file1->Get("AD");

TFile *file2 = TFile::Open("r0031_000_ps2.root", "RECREATE");
TTree *ps = new TTree ("ps","all_pl_events");

UShort_t TAC_PL_var[5], PlasticRaw[10];
Long64_t PlasticRawTS[10];
ad->SetBranchAddress("TAC_PL_1", &TAC_PL_var[0]);
ad->SetBranchAddress("TAC_PL_2", &TAC_PL_var[1]);
ad->SetBranchAddress("TAC_PL_3", &TAC_PL_var[2]);
ad->SetBranchAddress("TAC_PL_4", &TAC_PL_var[3]);
ad->SetBranchAddress("TAC_PL_5", &TAC_PL_var[4]);
ad->SetBranchAddress("PlasticRaw",&PlasticRaw);
ad->SetBranchAddress("PlasticRawTS",&PlasticRawTS);

UShort_t TAC_PL_all_var[5];
ps->Branch("TAC_PL_1_all", &TAC_PL_all_var[0], "TAC_PL_1_all/s");
ps->Branch("TAC_PL_2_all", &TAC_PL_all_var[1], "TAC_PL_2_all/s");
ps->Branch("TAC_PL_3_all", &TAC_PL_all_var[2], "TAC_PL_3_all/s");
ps->Branch("TAC_PL_4_all", &TAC_PL_all_var[3], "TAC_PL_4_all/s");
ps->Branch("TAC_PL_5_all", &TAC_PL_all_var[4], "TAC_PL_5_all/s");

TH1F *htpl1all = new TH1F("htpl1all", "allEVENTpl1", 10000,0,10000);
TH1F *htpl2all = new TH1F("htpl2all", "allEVENTpl2", 10000,0,10000);
TH1F *htpl3all = new TH1F("htpl3all", "allEVENTpl3", 10000,0,10000);
TH1F *htpl4all = new TH1F("htpl4all", "allEVENTpl4", 10000,0,10000);
TH1F *htpl5all = new TH1F("htpl5all", "allEVENTpl5", 10000,0,10000);

TH1F *hEnrgypl1all = new TH1F("hEnrgypl1all", "allEVENTpl1", 500,7000,14000);
TH1F *hEnrgypl2all = new TH1F("hEnrgypl2all", "allEVENTpl2", 500,7000,14000);
TH1F *hEnrgypl3all = new TH1F("hEnrgypl3all", "allEVENTpl3", 500,7000,14000);
TH1F *hEnrgypl4all = new TH1F("hEnrgypl4all", "allEVENTpl4", 500,7000,14000);
TH1F *hEnrgypl5all = new TH1F("hEnrgypl5all", "allEVENTpl5", 500,7000,14000);

TH1F *hEnrgypl1left = new TH1F("hEnrgypl1left", "leftEVENTpl1", 500,7000,14000);
TH1F *hEnrgypl2left = new TH1F("hEnrgypl2left", "leftEVENTpl2", 500,7000,14000);
TH1F *hEnrgypl3left = new TH1F("hEnrgypl3left", "leftEVENTpl3", 500,7000,14000);
TH1F *hEnrgypl4left = new TH1F("hEnrgypl4left", "leftEVENTpl4", 500,7000,14000);
TH1F *hEnrgypl5left = new TH1F("hEnrgypl5left", "leftEVENTpl5", 500,7000,14000);
TH1F *hEnrgypl1right = new TH1F("hEnrgypl1right", "rightEVENTpl1", 500,7000,14000);
TH1F *hEnrgypl2right = new TH1F("hEnrgypl2right", "rightEVENTpl2", 500,7000,14000);
TH1F *hEnrgypl3right = new TH1F("hEnrgypl3right", "rightEVENTpl3", 500,7000,14000);
TH1F *hEnrgypl4right = new TH1F("hEnrgypl4right", "rightEVENTpl4", 500,7000,14000);
TH1F *hEnrgypl5right = new TH1F("hEnrgypl5right", "rightEVENTpl5", 500,7000,14000);



TH2F* h2d = new TH2F("h2d","j:htplall", 5, 0.5, 5.5, 20, 0, 20);
TH2F* h2d2[5];
h2d2[0] = new TH2F("h2d2(1)","plasticRAW1:htplall", 2000, 7000, 14000, 400, 0, 8000);
h2d2[1] = new TH2F("h2d2(2)","plasticRAW2:htplall", 2000, 7000, 14000, 400, 0, 8000);
h2d2[2] = new TH2F("h2d2(3)","plasticRAW3:htplall", 2000, 7000, 14000, 400, 0, 8000);
h2d2[3] = new TH2F("h2d2(4)","plasticRAW4:htplall", 2000, 7000, 14000, 400, 0, 8000);
h2d2[4] = new TH2F("h2d2(5)","plasticRAW5:htplall", 2000, 7000, 14000, 400, 0, 8000);
TCanvas *c = new TCanvas("c", "c", 800, 800);

Double_t old_L_Bins[5]={19617,19836,20106,19857,19395}; //lower bins of shifted spectra of TACs
Double_t L_Bins[5] = {222,441,711,462,0};               //lower bins subtracted by the lowest bin of all low bins of 5 plastics (the 5th one)
Double_t old_U_Bins[5] = {26541,26322,26052,26301,26763}; //upper bins of shifted spectra of TACs

Double_t y_value[5]={10.0,5.0,0.0,-5.0,-10.0};
Double_t perbin_value[5];
Double_t x_value[5];

for (int i=0; i<5;i++){
    perbin_value[i]=(old_U_Bins[i]-old_L_Bins[i])/20;
}

Int_t k=0;
Int_t m=0;

Double_t min_tac1=10000;
Double_t parameters[3][2240];       // [0]->intercept, [1]->Slope, [2]->ChiSq
Double_t lengths_at_points[5][2240]; //value of length in 5 plastics for each event evaluated from the fit
Double_t Deviation_x[5][2240];

Int_t para_index=0; //parameter index of parameters[][]

Long64_t Nevents= ad->GetEntries();
for (Long64_t i = 0; i<Nevents; i++){
    ad->GetEntry(i);
    if(TAC_PL_var[0]>1 && TAC_PL_var[1]>1 && TAC_PL_var[2]>1 && TAC_PL_var[3]>1 && TAC_PL_var[4]>1){
        TAC_PL_all_var[0]=((TAC_PL_var[0]+0670.0-19395));        //shifted all the peaks to the max centroid then cut off
        TAC_PL_all_var[1]=((TAC_PL_var[1]+0105.0-19395));        //the spectrum starting bin to the least of the 5 spectra
        TAC_PL_all_var[2]=((TAC_PL_var[2]+0000.0-19395));        //in terms of 3-sigma as in file "tac_gated.ods" column F
        TAC_PL_all_var[3]=((TAC_PL_var[3]+1217.0-19395));
        TAC_PL_all_var[4]=((TAC_PL_var[4]+0474.0-19395));

        if(TAC_PL_all_var[0]>=222 && TAC_PL_all_var[1]>=441 && TAC_PL_all_var[2]>=711 && TAC_PL_all_var[3]>=462 && TAC_PL_all_var[4]>=0
        && TAC_PL_all_var[0]<=7146 && TAC_PL_all_var[1]<=6927 && TAC_PL_all_var[2]<=6657 && TAC_PL_all_var[3]<=6906 && TAC_PL_all_var[4]<=7368)
        {



        ps->Fill();
        if (TAC_PL_all_var[3]<min_tac1){
        min_tac1= TAC_PL_all_var[3];}   //check to see which is the least tac entry

        htpl1all->Fill(TAC_PL_all_var[0]);
        htpl2all->Fill(TAC_PL_all_var[1]);
        htpl3all->Fill(TAC_PL_all_var[2]);
        htpl4all->Fill(TAC_PL_all_var[3]);
        htpl5all->Fill(TAC_PL_all_var[4]);
        hEnrgypl1all->Fill(sqrt(PlasticRaw[0]*PlasticRaw[5]));
        hEnrgypl2all->Fill(sqrt(PlasticRaw[1]*PlasticRaw[6]));
        hEnrgypl3all->Fill(sqrt(PlasticRaw[2]*PlasticRaw[7]));
        hEnrgypl4all->Fill(sqrt(PlasticRaw[3]*PlasticRaw[8]));
        hEnrgypl5all->Fill(sqrt(PlasticRaw[4]*PlasticRaw[9]));


        hEnrgypl1left ->Fill(PlasticRaw[0]);
        hEnrgypl2left ->Fill(PlasticRaw[1]);
        hEnrgypl3left ->Fill(PlasticRaw[2]);
        hEnrgypl4left ->Fill(PlasticRaw[3]);
        hEnrgypl5left ->Fill(PlasticRaw[4]);
        hEnrgypl1right->Fill(PlasticRaw[5]);
        hEnrgypl2right->Fill(PlasticRaw[6]);
        hEnrgypl3right->Fill(PlasticRaw[7]);
        hEnrgypl4right->Fill(PlasticRaw[8]);
        hEnrgypl5right->Fill(PlasticRaw[9]);


        Double_t norm_length[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
        for (int n = 0; n < 5; n++) {
            h2d->Fill(n, TAC_PL_all_var[n] / 330);
            h2d2[n]->Fill(sqrt(PlasticRaw[n] * PlasticRaw[n + 5]), TAC_PL_all_var[n]);
            norm_length[n] = ((TAC_PL_all_var[n] - L_Bins[n]) / perbin_value[n]);

            if (norm_length[n] >= 0 && norm_length[n] <= 20) {
                k++;
            }
        }

        TGraph *traj = new TGraph();
        TF1 *f1 = new TF1("f1", "pol1");
        f1->SetLineColor(kRed);
        f1->SetLineWidth(2);

        traj->SetMarkerStyle(20);
        traj->SetMarkerSize(1);
        traj->SetMarkerColor(1);

        for (int i = 0; i < 5; i++) {
            if (norm_length[i] >= 0 && norm_length[i] <= 20) {  // Only use valid points
                traj->SetPoint(i, y_value[i], norm_length[i]);
            } else {
                cout << "Skipping invalid point: norm_length[" << i << "] = " << norm_length[i] << endl;
            }
        }

        // Set axis ranges for better visualization
        traj->GetXaxis()->SetLimits(-12, 12);  // Adjust X-axis
        traj->GetYaxis()->SetRangeUser(0,20);

        if (traj->GetN() == 5) {  // Check if there are enough points to fit
            traj->Fit("f1");
            traj->Draw("AP");
            f1->Draw("same");
        } else {
            cout << "Not enough points for fitting." << endl;
        }


        Double_t intercept = f1->GetParameter(0);
        Double_t slope = f1->GetParameter(1);
        Double_t ChiSq = f1->GetChisquare();
        Double_t Eval_value[5];

        if(intercept && slope){
            parameters[0][para_index]=intercept;
            parameters[1][para_index]=slope;
            parameters[2][para_index]=ChiSq;

            for (Int_t u=0; u<5; u++){
                Eval_value[u] = f1->Eval(y_value[u]);
                Deviation_x[u][para_index]=Eval_value[u]-norm_length[u];
            }
            cout<<"Deviation in the 3rd plastic is: "<<Deviation_x[3][para_index]<<endl;
            para_index++;
        }



        c->Update();
        //c->WaitPrimitive();
        // TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);


        }

    }
}


TH1F *hDeviation[6];
hDeviation[0] = new TH1F("hDeviation1", "Deviation Distribution_PL1;Deviation;Occurrences", 100, -10, 10);
hDeviation[1] = new TH1F("hDeviation2", "Deviation Distribution_PL2;Deviation;Occurrences", 100, -10, 10);
hDeviation[2] = new TH1F("hDeviation3", "Deviation Distribution_PL3;Deviation;Occurrences", 100, -10, 10);
hDeviation[3] = new TH1F("hDeviation4", "Deviation Distribution_PL4;Deviation;Occurrences", 100, -10, 10);
hDeviation[4] = new TH1F("hDeviation5", "Deviation Distribution_PL5;Deviation;Occurrences", 100, -10, 10);
hDeviation[5] = new TH1F("hDeviationSUM", "Deviation Distribution_PL_all;Deviation;Occurrences", 100, -10, 10);
for (Int_t index = 0; index < para_index; index++) {
    for (Int_t u = 0; u < 5; u++) { // Loop through all Deviation_x[u][index] values
        hDeviation[u]->Fill(Deviation_x[u][index]);
    }
}
hDeviation[5]->Add(hDeviation[0]);
hDeviation[5]->Add(hDeviation[1]);
hDeviation[5]->Add(hDeviation[2]);
hDeviation[5]->Add(hDeviation[3]);
hDeviation[5]->Add(hDeviation[4]);
TCanvas *c3 = new TCanvas("c3", "Deviation Distribution", 800, 600);
c3->cd();
hDeviation[0]->SetLineColor(1);
hDeviation[1]->SetLineColor(2);
hDeviation[2]->SetLineColor(3);
hDeviation[3]->SetLineColor(4);
hDeviation[4]->SetLineColor(5);
hDeviation[0]->Draw();
hDeviation[1]->Draw("same");
hDeviation[2]->Draw("same");
hDeviation[3]->Draw("same");
hDeviation[4]->Draw("same");
hDeviation[0]->Write();
hDeviation[1]->Write();
hDeviation[2]->Write();
hDeviation[3]->Write();
hDeviation[4]->Write();
hDeviation[5]->Write();
c3->Update();

TCanvas *c4 = new TCanvas("c4", "Deviation Distribution-PL1_with_FIT", 800, 600);
c4->cd();
hDeviation[2]->Draw();
c4->Update();
cout<<"the parameters are: "<<endl;
for (int i = 0; i<15; i++){
    cout<<parameters[0][i]<<"   "<<parameters[1][i]<<endl;

}

cout<<"the total entries with norm length inside plastic: "<<k/5.0<<endl;
cout<<"the total entries with norm length outside plastic: "<<m<<endl;
cout<<"the min_tac is:  "<<min_tac1<<endl;
h2d->Write();
h2d2[0]->Write();
h2d2[1]->Write();
h2d2[2]->Write();
h2d2[3]->Write();
h2d2[4]->Write();
ps->Write();
htpl1all->Write();
htpl2all->Write();
htpl3all->Write();
htpl4all->Write();
htpl5all->Write();
// c->cd();
// h2d2[0]->Draw();
// h2d2[1]->Draw("same");
// h2d2[2]->Draw("same");
// h2d2[3]->Draw("same");
// h2d2[4]->Draw("same");
c->Update();
hEnrgypl1all->Write();
hEnrgypl2all->Write();
hEnrgypl3all->Write();
hEnrgypl4all->Write();
hEnrgypl5all->Write();

hEnrgypl1left -> Write();
hEnrgypl2left -> Write();
hEnrgypl3left -> Write();
hEnrgypl4left -> Write();
hEnrgypl5left -> Write();
hEnrgypl1right-> Write();
hEnrgypl2right-> Write();
hEnrgypl3right-> Write();
hEnrgypl4right-> Write();
hEnrgypl5right-> Write();
// TCanvas *c2 = new TCanvas("c2","c2",800,600);
// c2->cd();

// htpl1all->Draw();
// htpl2all->Draw("same");
// htpl3all->Draw("same");
// htpl4all->Draw("same");
// htpl5all->Draw("same");

/*
ps->Draw("TAC_PL_1_all>>h6(500,16000,26000)","","");
ps->Draw("TAC_PL_2_all>>h7(500,16000,26000)","","same");
ps->Draw("TAC_PL_3_all>>h8(500,16000,26000)","","same");
ps->Draw("TAC_PL_4_all>>h9(500,16000,26000)","","same");
ps->Draw("TAC_PL_5_all>>h10(500,16000,26000)","","same");
*/

/*
ps->Draw("TAC_PL_1_all>>h6(500,0,10000)","","");
ps->Draw("TAC_PL_2_all>>h7(500,0,10000)","","same");
ps->Draw("TAC_PL_3_all>>h8(500,0,10000)","","same");
ps->Draw("TAC_PL_4_all>>h9(500,0,10000)","","same");
ps->Draw("TAC_PL_5_all>>ha(500,0,10000)","","same");
*/

/*
ps->Draw("TAC_PL_1_all/1.164>>h6(50,18947,25871)","","");
ps->Draw("TAC_PL_2_all/1.091>>h7(50,19731,26217)","","same");
ps->Draw("TAC_PL_3_all/1.000>>h8(50,20106,26052)","","same");
ps->Draw("TAC_PL_4_all/1.084>>h9(50,18640,25084)","","same");
ps->Draw("TAC_PL_5_all/1.239>>ha(50,18921,26289)","","same");
*/

/*
ps->Draw("((TAC_PL_1_all+0670.0))>>h1(50,16000,26000)","","");
ps->Draw("((TAC_PL_2_all+0105.0))>>h2(50,16000,26000)","","same");
ps->Draw("((TAC_PL_3_all+0000.0))>>h3(50,16000,26000)","","same");
ps->Draw("((TAC_PL_4_all+1217.0))>>h4(50,16000,26000)","","same");
ps->Draw("((TAC_PL_5_all+0474.0))>>h5(50,16000,26000)","","same");
*/

        // htpl1all->Fill((TAC_PL1all_var+0670.0)/1.164);
        // htpl2all->Fill((TAC_PL2all_var+0105.0)/1.091);
        // htpl3all->Fill((TAC_PL3all_var+0000.0)/1.000);
        // htpl4all->Fill((TAC_PL4all_var+1217.0)/1.084);
        // htpl5all->Fill((TAC_PL5all_var+0474.0)/1.239);

//file1->Close();
//file2->Close();
return 0;
}

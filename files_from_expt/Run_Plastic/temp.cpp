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
#include <TGraphErrors.h>
#include <TProfile.h>
#include <TSystem.h>

using namespace std;

void fit_ridge_slices5(TH2F* hplot5) {
  auto cs = new TCanvas("c","",900,500);
  hplot5->Draw("colz");
  hplot5->FitSlicesY(nullptr, 1, -1, 0, "QNR");
  auto mu = (TH1D*)gDirectory->Get(Form("%s_1", hplot5->GetName()));
  TGraphErrors gr(mu);
  double xmin = -10.0;  // lower bound of fit
  double xmax = 10.0;  // upper bound of fit
  gr.Fit("pol1", "", "", xmin, xmax);
  auto f = gr.GetFunction("pol1");
  f->SetLineColor(kRed); f->SetLineWidth(3);
  f->Draw("same");

//   cs->Update();
}

void fit_ridge_profile(TH2* hplot4){
  auto c = new TCanvas("c","",900,500);
  hplot4->Draw("colz");
  auto p = hplot4->ProfileX("p", 1, -1, "s");
  p->SetMarkerStyle(20); p->SetMarkerSize(0.6);
  p->Draw("P same");
  TF1 f("f","pol1", -7.0, 7.0);
  p->Fit(&f,"");
  f.SetLineColor(kRed); f.SetLineWidth(3);
  f.Draw("same");

  c->Update();
}



int temp(){
TFile *file1 = TFile::Open("r0031_000a.root");
TTree *ad = (TTree*)file1->Get("AD");

TFile *file2 = TFile::Open("test.root", "RECREATE");
TTree *ps = new TTree ("ps","all_pl_events");

Double_t cutoff_charge[10]= {8332,8660,8290,8537,7815,8020,7842,7810,8779,8205};
Double_t Qgain[10] = {3.3415,1.0,0.96738,2.27838,1.08208,1.58414,1.01826,1.784784,1.191078,1.89953};
  Double_t Qgm[10] = {};

    Double_t off_XQr[5]={0.001124,-0.017847,-0.091954,0.0390684,-0.00772665};
  Double_t cff_XQr[5]={0.105755,0.100234,0.0997495,0.0911443,0.118792};


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

TH1F *hEnrgypl1left    = new TH1F("hEnrgypl1left", "leftEVENTpl1", 500,0,15000);
TH1F *hEnrgypl2left    = new TH1F("hEnrgypl2left", "leftEVENTpl2", 500,0,15000);
TH1F *hEnrgypl3left    = new TH1F("hEnrgypl3left", "leftEVENTpl3", 500,0,15000);
TH1F *hEnrgypl4left    = new TH1F("hEnrgypl4left", "leftEVENTpl4", 500,0,15000);
TH1F *hEnrgypl5left    = new TH1F("hEnrgypl5left", "leftEVENTpl5", 500,0,15000);
TH1F *hEnrgypl1right = new TH1F("hEnrgypl1right", "rightEVENTpl1", 500,0,15000);
TH1F *hEnrgypl2right = new TH1F("hEnrgypl2right", "rightEVENTpl2", 500,0,15000);
TH1F *hEnrgypl3right = new TH1F("hEnrgypl3right", "rightEVENTpl3", 500,0,15000);
TH1F *hEnrgypl4right = new TH1F("hEnrgypl4right", "rightEVENTpl4", 500,0,15000);
TH1F *hEnrgypl5right = new TH1F("hEnrgypl5right", "rightEVENTpl5", 500,0,15000);

TH1F *hlogErEl1 = new TH1F("hlogErEl1", "logEr_El_PL1", 500,-5,5);
TH1F *hlogErEl2 = new TH1F("hlogErEl2", "logEr_El_PL2", 500,-5,5);
TH1F *hlogErEl3 = new TH1F("hlogErEl3", "logEr_El_PL3", 500,-5,5);
TH1F *hlogErEl4 = new TH1F("hlogErEl4", "logEr_El_PL4", 500,-5,5);
TH1F *hlogErEl5 = new TH1F("hlogErEl5", "logEr_El_PL5", 500,-5,5);


TH2F* hplot1 = new TH2F("hplot1","norm_length:Qr1", 440, -11, 11, 500, -2, 2);
TH2F* hplot2 = new TH2F("hplot2","norm_length:Qr2", 440, -11, 11, 500, -2, 2);
TH2F* hplot3 = new TH2F("hplot3","norm_length:Qr3", 440, -11, 11, 500, -2, 2);
TH2F* hplot4 = new TH2F("hplot4","norm_length:Qr4", 440, -11, 11, 500, -2, 2);
TH2F* hplot5 = new TH2F("hplot5","norm_length:Qr5", 440, -11, 11, 500, -2, 2);




TH2F* h2d = new TH2F("h2d","j:htplall", 5, 0.5, 5.5, 20, 0, 20);
TH2F* h2d3 = new TH2F("h2d3","j:htplallenergy", 10, 0.5, 10.5, 500, 0, 15000);

TH2F* h2d2[5];
h2d2[0] = new TH2F("h2d2(1)","plasticRAW1:htplall", 2000, 7000, 14000, 400, 0, 8000);
h2d2[1] = new TH2F("h2d2(2)","plasticRAW2:htplall", 2000, 7000, 14000, 400, 0, 8000);
h2d2[2] = new TH2F("h2d2(3)","plasticRAW3:htplall", 2000, 7000, 14000, 400, 0, 8000);
h2d2[3] = new TH2F("h2d2(4)","plasticRAW4:htplall", 2000, 7000, 14000, 400, 0, 8000);
h2d2[4] = new TH2F("h2d2(5)","plasticRAW5:htplall", 2000, 7000, 14000, 400, 0, 8000);
TCanvas *ccc = new TCanvas("ccc", "ccc", 800, 800);
ccc->Divide(1,3);

Double_t old_L_Bins[5]={19617,19836,20106,19857,19395}; //lower bins of shifted spectra of TACs
Double_t L_Bins[5] = {222,441,711,462,0};               //lower bins subtracted by the lowest bin of all low bins of 5 plastics (the 5th one)
Double_t old_U_Bins[5] = {26541,26322,26052,26301,26763}; //upper bins of shifted spectra of TACs

Double_t upperLOGvalue[5]= {1.1042,0.9799,1.0464,1.0851,1.3079}; //upper limit of log value for each plastic
Double_t lowerLOGvalue[5]= {-1.1398,-1.0397,-1.1748,-1.0323,-1.2943}; //LOWER limit of log value for each plastic
Double_t perbin_value_energy[5];

Double_t y_value[5]={8.0,4.0,0.0,-4.0,-8.0};

Double_t perbin_value[5];
Double_t x_value[5];

Double_t Qr[5];

for (int i=0; i<5;i++){
    perbin_value[i]=(old_U_Bins[i]-old_L_Bins[i])/20;
    perbin_value_energy[i]=(upperLOGvalue[i]-lowerLOGvalue[i])/20;
}

Int_t k=0;
Int_t m=0;

Double_t min_tac1=10000;
Double_t parameters[3][2240];       // [0]->intercept, [1]->Slope, [2]->ChiSq
Double_t parameters_en[3][2240];
Double_t parameters_combined[3][2240];

Double_t lengths_at_points[5][2240]; //value of length in 5 plastics for each event evaluated from the fit
Double_t lengths_at_points_en[5][2240];
Double_t lengths_at_points_combined[5][2240];
Double_t Deviation_x[5][2240];
Double_t Deviation_x_en[5][2240];
Double_t Deviation_x_combined[5][2240];
Int_t para_index=0; //parameter index of parameters[][]
Int_t para_index_en=0;
Int_t para_index_combined=0;


Long64_t Nevents= ad->GetEntries();
for (Long64_t i = 0; i<Nevents; i++){
    ad->GetEntry(i);
//////////////////////////////////////////////////////////////////////////////////time///////////////////////////////////////////////////////////////////////////////////

    if(TAC_PL_var[0]>1 && TAC_PL_var[1]>1 && TAC_PL_var[2]>1 && TAC_PL_var[3]>1 && TAC_PL_var[4]>1){
        TAC_PL_all_var[0]=((TAC_PL_var[0]+0670.0-19395));        //shifted all the peaks to the max centroid then cut off
        TAC_PL_all_var[1]=((TAC_PL_var[1]+0105.0-19395));        //the spectrum starting bin to the least of the 5 spectra
        TAC_PL_all_var[2]=((TAC_PL_var[2]+0000.0-19395));        //in terms of 3-sigma as in file "tac_gated.ods" column F
        TAC_PL_all_var[3]=((TAC_PL_var[3]+1217.0-19395));
        TAC_PL_all_var[4]=((TAC_PL_var[4]+0474.0-19395));

        if(TAC_PL_all_var[0]>=222 && TAC_PL_all_var[1]>=441 && TAC_PL_all_var[2]>=711 && TAC_PL_all_var[3]>=462 && TAC_PL_all_var[4]>=0
        && TAC_PL_all_var[0]<=7146 && TAC_PL_all_var[1]<=6927 && TAC_PL_all_var[2]<=6657 && TAC_PL_all_var[3]<=6906 && TAC_PL_all_var[4]<=7368){
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

            Double_t norm_length[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
            for (int n = 0; n < 5; n++) {
                h2d->Fill(n, TAC_PL_all_var[n] / 330);
                h2d2[n]->Fill(sqrt(PlasticRaw[n] * PlasticRaw[n + 5]), TAC_PL_all_var[n]);
                norm_length[n] = ((TAC_PL_all_var[n] - L_Bins[n]) / perbin_value[n]);
                cout<<"the norm length ### for plastic "<<n+1<<" is: "<<norm_length[n]<<endl;
                if (norm_length[n] >= 0 && norm_length[n] <= 20) {
                    k++;
                }
            }
//////////////////////////////////////////////////////////////////////////////////energy///////////////////////////////////////////////////////////////////////////////////

            for (int n = 0; n < 10; n++){
                Qgm[n] = (PlasticRaw[n] - cutoff_charge[n]) * Qgain[n];
            }

            hEnrgypl1left ->Fill(Qgm[0]); 
            hEnrgypl2left ->Fill(Qgm[1]);
            hEnrgypl3left ->Fill(Qgm[2]);
            hEnrgypl4left ->Fill(Qgm[3]);
            hEnrgypl5left ->Fill(Qgm[4]);
            hEnrgypl1right->Fill(Qgm[5]);
            hEnrgypl2right->Fill(Qgm[6]);
            hEnrgypl3right->Fill(Qgm[7]);
            hEnrgypl4right->Fill(Qgm[8]);
            hEnrgypl5right->Fill(Qgm[9]);

            for(int jj=0; jj<10; jj++){
                h2d3->Fill(jj+1, Qgm[jj]);
            }

                    //filling log (qr/ql) against norm length for each plastic 
            for (int j = 0; j < 5; j++) {
                Qr[j] = log(Qgm[j] / Qgm[j + 5]);
            }
            hlogErEl1 -> Fill(Qr[0]);
            hlogErEl2 -> Fill(Qr[1]);
            hlogErEl3 -> Fill(Qr[2]);
            hlogErEl4 -> Fill(Qr[3]);
            hlogErEl5 -> Fill(Qr[4]);

            hplot1 -> Fill(norm_length[0]-10, Qr[0]); 
            hplot2 -> Fill(norm_length[1]-10, Qr[1]);
            hplot3 -> Fill(norm_length[2]-10, Qr[2]);
            hplot4 -> Fill(norm_length[3]-10, Qr[3]);
            hplot5 -> Fill(norm_length[4]-10, Qr[4]);

            Double_t norm_length2[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
            for(int ll = 0; ll<5; ll++){
                norm_length2[ll] = (Qr[ll]-lowerLOGvalue[ll])/perbin_value_energy[ll];
                cout<<"the norm length *** for plastic "<<ll+1<<" is: "<<norm_length2[ll]<<endl;
            }


            /////////////////////////////////////////////////////////////////////////////////////////////energy///////////////////////////////////////////////////////////////////////////
            ccc->cd(1);

            TGraph *traj_en = new TGraph();
            TF1 *f1_en = new TF1("f1_en", "pol1");
            f1_en->SetLineColor(kRed);
            f1_en->SetLineWidth(2);

            traj_en->SetMarkerStyle(20);
            traj_en->SetMarkerSize(1);
            traj_en->SetMarkerColor(1);
            for (int ij = 0; ij < 5; ij++) {
                if (norm_length2[ij] >= 0 && norm_length2[ij] <= 20) {  // Only use valid points
                    traj_en->SetPoint(ij, y_value[ij], norm_length2[ij]);
                } else {
                    cout << "Skipping invalid point: norm_length[" << ij << "] = " << norm_length2[ij] << endl;
                }
            }

                    // Set axis ranges for better visualization
            traj_en->GetXaxis()->SetLimits(-12, 12);  // Adjust X-axis
            traj_en->GetYaxis()->SetRangeUser(0,20);

            if (traj_en->GetN() == 5) {  // Check if there are enough points to fit
                traj_en->Fit("f1_en","Q");
                traj_en->Draw("AP");
                f1_en->Draw("same");
            } else {
                cout << "Not enough points for fitting." << endl;
            }

            Double_t intercept_en = f1_en->GetParameter(0);
            Double_t slope_en = f1_en->GetParameter(1);
            Double_t ChiSq_en = f1_en->GetChisquare();
            Double_t Eval_value_en[5];

            if(intercept_en && slope_en){
                parameters_en[0][para_index_en]=intercept_en;
                parameters_en[1][para_index_en]=slope_en;
                parameters_en[2][para_index_en]=ChiSq_en;

                for (Int_t u=0; u<5; u++){
                    Eval_value_en[u] = f1_en->Eval(y_value[u]);
                    Deviation_x_en[u][para_index_en]=Eval_value_en[u]-norm_length2[u];
                }
                // cout<<"Deviation in the 3rd plastic is: "<<Deviation_x[3][para_index]<<endl;
                para_index_en++;
            }

            /////////////////////////////////////////////////////////////////////////////////////////////time///////////////////////////////////////////////////////////////////////////

            ccc->cd(2);

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
                traj->Fit("f1","Q");
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
                // cout<<"Deviation in the 3rd plastic is: "<<Deviation_x[3][para_index]<<endl;
                para_index++;
            }



            ccc->Update();
            //c->WaitPrimitive();
            // TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);

//////////////////////////////////////////////////////////////////////////////////combined///////////////////////////////////////////////////////////////////////////////////
// TCanvas *c2_combined = new TCanvas("c2_combined", "Combined Trajectory", 800, 600);

ccc->cd(3);
// double sigma_t = 0.65; // cm
// double sigma_e = 0.75; // cm
// double w_t = 1.0/(sigma_t*sigma_t);
// double w_e = 1.0/(sigma_e*sigma_e);
bool all_valid = true;
for(int i = 0; i < 5; i++) {
    if(!(norm_length[i] >= 0 && norm_length[i] <= 20 && norm_length2[i] >= 0 && norm_length2[i] <= 20)) {
        all_valid = false;
        break;
    }
}

if(all_valid){
// for(int iii = 0; iii < 5; iii++) {
//     double x_comb = (norm_length[iii]*w_t + norm_length2[iii]*w_e) / (w_t + w_e);
// }
            // TGraph *traj_combined = new TGraph();
            // TF1 *f1_combined = new TF1("f1_combined", "pol1");
            // f1_combined->SetLineColor(kRed);
            // f1_combined->SetLineWidth(2);

            // traj_combined->SetMarkerStyle(20);
            // traj_combined->SetMarkerSize(1);
            // traj_combined->SetMarkerColor(1);

            // for (int i = 0; i < 5; i++) {
            //     if (norm_length[i] >= 0 && norm_length[i] <= 20) {  // Only use valid points
            //         traj_combined->SetPoint(i, y_value[i], (norm_length[i]*w_t + norm_length2[i]*w_e) / (w_t + w_e));
            //     } else {
            //         cout << "Skipping invalid point: norm_length[" << i << "] = " << norm_length[i] << endl;
            //     }
            // }

            // // Set axis ranges for better visualization
            // traj_combined->GetXaxis()->SetLimits(-12, 12);  // Adjust X-axis
            // traj_combined->GetYaxis()->SetRangeUser(0,20);

            // if (traj_combined->GetN() == 5) {  // Check if there are enough points to fit
            //     traj_combined->Fit("f1_combined","Q");
            //     traj_combined->Draw("AP");
            //     f1_combined->Draw("same");
            // } else {
            //     cout << "Not enough points for fitting." << endl;
            // }

            // Double_t intercept_combined = f1_combined->GetParameter(0);
            // Double_t slope_combined = f1_combined->GetParameter(1);
            // Double_t ChiSq_combined = f1_combined->GetChisquare();
            // Double_t Eval_value_combined[5];

            // if(intercept_combined && slope_combined){
            //     parameters_combined[0][para_index_combined]=intercept_combined;
            //     parameters_combined[1][para_index_combined]=slope_combined;
            //     parameters_combined[2][para_index_combined]=ChiSq_combined;

            //     for (Int_t u=0; u<5; u++){
            //         Eval_value_combined[u] = f1_combined->Eval(y_value[u]);
            //         Deviation_x_combined[u][para_index]=Eval_value_combined[u]-((norm_length[u]*w_t + norm_length2[u]*w_e) / (w_t + w_e));
            //     }
            //     // cout<<"Deviation in the 3rd plastic is: "<<Deviation_x[3][para_index]<<endl;
            //     para_index_combined++;
            // }


///////////////////////////////////////////////////////////overlayed//////////////////////////////////////
ccc->cd(3);
            traj->SetMarkerStyle(20);
traj->SetMarkerSize(1);
traj->SetMarkerColor(kBlue); // Time: blue
traj->SetLineColor(kBlue);

traj_en->SetMarkerStyle(21);
traj_en->SetMarkerSize(1);
traj_en->SetMarkerColor(kRed); // Energy: red
traj_en->SetLineColor(kRed);

// Draw both graphs
traj->Draw("AP");        // Draw axes and points for time
traj_en->Draw("P SAME"); // Overlay energy points
f1->SetLineColor(kBlue);
f1->Draw("same");
f1_en->SetLineColor(kRed);
f1_en->Draw("same");
            ccc->Update();
gSystem->Exec("sleep 1"); // 

        }
    }

    }
}
//////////////////////////////////////////////////////////////////////////////////combined///////////////////////////////////////////////////////////////////////////////////

TH1F *hDeviation_combined[6];
hDeviation_combined[0] = new TH1F("hDeviation1_combined", "Deviation Distribution_PL1_combined;Deviation;Occurrences", 100, -10, 10);
hDeviation_combined[1] = new TH1F("hDeviation2_combined", "Deviation Distribution_PL2_combined;Deviation;Occurrences", 100, -10, 10);
hDeviation_combined[2] = new TH1F("hDeviation3_combined", "Deviation Distribution_PL3_combined;Deviation;Occurrences", 100, -10, 10);
hDeviation_combined[3] = new TH1F("hDeviation4_combined", "Deviation Distribution_PL4_combined;Deviation;Occurrences", 100, -10, 10);
hDeviation_combined[4] = new TH1F("hDeviation5_combined", "Deviation Distribution_PL5_combined;Deviation;Occurrences", 100, -10, 10);
hDeviation_combined[5] = new TH1F("hDeviationSUM_combined", "Deviation Distribution_PL_all_combined;Deviation;Occurrences", 100, -10, 10);
for (Int_t index = 0; index < para_index_combined; index++) {
    for (Int_t u = 0; u < 5; u++) { // Loop through all Deviation_x[u][index] values
        hDeviation_combined[u]->Fill(Deviation_x_combined[u][index]);
    }
}
hDeviation_combined[5]->Add(hDeviation_combined[0]);
hDeviation_combined[5]->Add(hDeviation_combined[1]);
hDeviation_combined[5]->Add(hDeviation_combined[2]);
hDeviation_combined[5]->Add(hDeviation_combined[3]);
hDeviation_combined[5]->Add(hDeviation_combined[4]);
TCanvas *c3_combined = new TCanvas("c3_combined", "Deviation Distribution_en", 800, 600);
c3_combined->cd();
hDeviation_combined[0]->SetLineColor(1);
hDeviation_combined[1]->SetLineColor(2);
hDeviation_combined[2]->SetLineColor(3);
hDeviation_combined[3]->SetLineColor(4);
hDeviation_combined[4]->SetLineColor(5);
hDeviation_combined[0]->Draw();
hDeviation_combined[1]->Draw("same");
hDeviation_combined[2]->Draw("same");
hDeviation_combined[3]->Draw("same");
hDeviation_combined[4]->Draw("same");
hDeviation_combined[0]->Write();
hDeviation_combined[1]->Write();
hDeviation_combined[2]->Write();
hDeviation_combined[3]->Write();
hDeviation_combined[4]->Write();
hDeviation_combined[5]->Write();
c3_combined->Update();

TCanvas *c4_combined = new TCanvas("c4_combined", "Deviation Distribution-PL1_with_FIT_combined", 800, 600);
c4_combined->cd();
hDeviation_combined[2]->Draw();
c4_combined->Update();
//////////////////////////////////////////////////////////////////////////////////energy///////////////////////////////////////////////////////////////////////////////////

TH1F *hDeviation_en[6];
hDeviation_en[0] = new TH1F("hDeviation1_en", "Deviation Distribution_PL1_en;Deviation;Occurrences", 100, -10, 10);
hDeviation_en[1] = new TH1F("hDeviation2_en", "Deviation Distribution_PL2_en;Deviation;Occurrences", 100, -10, 10);
hDeviation_en[2] = new TH1F("hDeviation3_en", "Deviation Distribution_PL3_en;Deviation;Occurrences", 100, -10, 10);
hDeviation_en[3] = new TH1F("hDeviation4_en", "Deviation Distribution_PL4_en;Deviation;Occurrences", 100, -10, 10);
hDeviation_en[4] = new TH1F("hDeviation5_en", "Deviation Distribution_PL5_en;Deviation;Occurrences", 100, -10, 10);
hDeviation_en[5] = new TH1F("hDeviationSUM_en", "Deviation Distribution_PL_all_en;Deviation;Occurrences", 100, -10, 10);
for (Int_t index = 0; index < para_index_en; index++) {
    for (Int_t u = 0; u < 5; u++) { // Loop through all Deviation_x[u][index] values
        hDeviation_en[u]->Fill(Deviation_x_en[u][index]);
    }
}
hDeviation_en[5]->Add(hDeviation_en[0]);
hDeviation_en[5]->Add(hDeviation_en[1]);
hDeviation_en[5]->Add(hDeviation_en[2]);
hDeviation_en[5]->Add(hDeviation_en[3]);
hDeviation_en[5]->Add(hDeviation_en[4]);
TCanvas *c3_en = new TCanvas("c3_en", "Deviation Distribution_en", 800, 600);
c3_en->cd();
hDeviation_en[0]->SetLineColor(1);
hDeviation_en[1]->SetLineColor(2);
hDeviation_en[2]->SetLineColor(3);
hDeviation_en[3]->SetLineColor(4);
hDeviation_en[4]->SetLineColor(5);
hDeviation_en[0]->Draw();
hDeviation_en[1]->Draw("same");
hDeviation_en[2]->Draw("same");
hDeviation_en[3]->Draw("same");
hDeviation_en[4]->Draw("same");
hDeviation_en[0]->Write();
hDeviation_en[1]->Write();
hDeviation_en[2]->Write();
hDeviation_en[3]->Write();
hDeviation_en[4]->Write();
hDeviation_en[5]->Write();
c3_en->Update();

TCanvas *c4_en = new TCanvas("c4_en", "Deviation Distribution-PL1_with_FIT_en", 800, 600);
c4_en->cd();
hDeviation_en[2]->Draw();
c4_en->Update();

//////////////////////////////////////////////////////////////////////////////////time///////////////////////////////////////////////////////////////////////////////////

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

ccc->Update();
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

h2d3->Write();

hlogErEl1->Write();
hlogErEl2->Write();
hlogErEl3->Write();
hlogErEl4->Write();
hlogErEl5->Write();

hplot1->Write();
hplot2->Write();
hplot3->Write();
hplot4->Write();
hplot5->Write();

return 0;
}


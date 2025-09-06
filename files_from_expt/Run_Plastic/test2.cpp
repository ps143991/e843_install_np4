#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TBranch.h>
#include <iostream>
#include <thread>

using namespace std;
int test2(){
    Double_t norm_length[5]={2,3,4,5,6.4};
    Double_t y_value[5] = {1.5, 3, 4.5, 6, 7.9};
    TCanvas *c1 = new TCanvas();
    c1->cd();
    
    TGraph *traj = new TGraph();
    TF1 *f1 = new TF1("f1", "pol1");
    f1->SetLineColor(kRed);
    f1->SetLineWidth(2);

    traj->SetMarkerStyle(20);
    traj->SetMarkerSize(1);
    traj->SetMarkerColor(1);
    for (int j = 0; j<3; j++){
    for (int i = 0; i < 5; i++) {
            traj->SetPoint(i, norm_length[i]+j, y_value[i]+j);
            cout<<"point set "<<i<<endl;
    }
    if (traj->GetN() == 5) {  // Check if there are enough points to fit
        traj->Fit("f1");
        traj->Draw("AP");
        f1->Draw("same");
    } else {
        cout << "Not enough points for fitting." << endl;
    }
    c1->Update();
    c1->WaitPrimitive();
    }
    // Set axis ranges for better visualization


    



    
   
    
    return 0;
}  


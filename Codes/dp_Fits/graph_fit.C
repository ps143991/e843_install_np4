#include <iostream>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TMultiGraph.h>

void graph_fit(){
    std::cout << "graph fit" << std::endl;
    int n = 9;
    double x[9] = {153,147,141,135,133,129,125,119,113};
    double y_0[9] = {90.900153,80.882468,158.877774,164.139436,268.120033,359.998909,306.224829,203.536543,67.79231};
    double y_2[9] = {199.66916,209.06861,208.151349,153.569794,124.542209,83.207147,129.066964,150.58478,161.867656};

    TGraphErrors* g_0 = new TGraphErrors(n, x, y_0);
    TGraphErrors* g_2 = new TGraphErrors(n, x, y_2);
    auto mg = new TMultiGraph();
    
    mg->Add(g_0);
    mg->Add(g_2);
    mg->SetTitle("Graph with error bars for 0 MeV; Theta lab; Cross section (arb. unit)");
    TCanvas* c1 = new TCanvas("c1","Graph with error bars",1200,800);
    c1->cd();
    g_0->SetMarkerStyle(21);
    g_0->SetMarkerColor(kRed);
    g_0->SetLineColor(kRed);
    g_0->SetLineWidth(2);   g_0->SetLineStyle(1);


    g_2->SetMarkerStyle(22);
    g_2->SetMarkerColor(kBlue);
    g_2->SetLineColor(kBlue);
    g_2->SetLineWidth(2);    g_2->SetLineStyle(2);

    mg->Draw("AP");

    //new pol3 fit
    TF1* fit_0 = new TF1("fit_0", "pol3", 110, 160);
    fit_0->SetLineColor(kRed);
    fit_0->SetLineWidth(2);
    g_0->Fit(fit_0, "R");
    fit_0->Draw("same");



}
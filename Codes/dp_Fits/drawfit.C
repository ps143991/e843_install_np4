#include "drawfit.h"
#include "response_fitPS.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TLegend.h"
#include <cstdio>

void drawfit(TF1* whole) {
    // Drawing background
    TF1* bcf = new TF1("bcf", left_skewed_landau, -2, 5, 3);
    bcf->SetParameters(whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    bcf->SetLineColor(kRed+1);
    bcf->Draw("same");

    // Drawing fit functions from baseline
    TF1* peak1f = new TF1("peak1f", resp1, -1, 2, 2);
    peak1f->SetParameters(whole->GetParameter(0), whole->GetParameter(1));
    peak1f->SetLineColor(kYellow);
    peak1f->SetFillColor(kYellow);
    peak1f->SetFillStyle(3003);
    //peak1f->Draw("same");

    TF1* peak2f = new TF1("peak2f", resp2, -0, 10, 2);
    peak2f->SetParameters(whole->GetParameter(2), whole->GetParameter(3));
    peak2f->SetLineColor(kGreen);
    peak2f->SetFillColor(kGreen);
    peak2f->SetFillStyle(3003);
    //peak2f->Draw("same");

    TF1* peak3f = new TF1("peak3f", resp3, -0, 10, 2);
    peak3f->SetParameters(whole->GetParameter(4), whole->GetParameter(5));
    peak3f->SetLineColor(kBlue);
    peak3f->SetFillColor(kBlue);
    peak3f->SetFillStyle(3003);
    //peak3f->Draw("same");

    TF1* peak4f = new TF1("peak4f", resp4, -0, 10, 2);
    peak4f->SetParameters(whole->GetParameter(6), whole->GetParameter(7));
    peak4f->SetLineColor(kOrange+6);
    peak4f->SetFillColor(kOrange+6);
    peak4f->SetFillStyle(3003);
    //peak4f->Draw("same");

    TF1* peak5f = new TF1("peak5f", resp5, -0, 10, 2);
    peak5f->SetParameters(whole->GetParameter(8), whole->GetParameter(9));
    peak5f->SetLineColor(kViolet+3);
    peak5f->SetFillColor(kViolet+3);
    peak5f->SetFillStyle(3003);
    //peak5f->Draw("same");

    TF1* peak6f = new TF1("peak6f", resp6, -0, 10, 2);
    peak6f->SetParameters(whole->GetParameter(10), whole->GetParameter(11));
    peak6f->SetLineColor(kCyan+1);
    peak6f->SetFillColor(kCyan+1);
    peak6f->SetFillStyle(3003);
    //peak6f->Draw("same");

    // Drawing fit functions on background
    TF1* peak1ft = new TF1("peak1ft", P1_with_landau, -1, 10, 5);
    peak1ft->SetParameters(whole->GetParameter(0), whole->GetParameter(1), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak1ft->SetLineColor(kMagenta);
    peak1ft->SetFillColor(kMagenta);
    peak1ft->SetFillStyle(0);
    peak1ft->Draw("same");

    TF1* peak2ft = new TF1("peak2ft", P1_with_landau, -1, 10, 5);
    peak2ft->SetParameters(whole->GetParameter(2), whole->GetParameter(3), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak2ft->SetLineColor(kBlack);
    peak2ft->SetFillColor(kBlack);
    peak2ft->SetFillStyle(0);
    peak2ft->Draw("same");

    TF1* peak3ft = new TF1("peak3ft", P1_with_landau, -1, 10, 5);
    peak3ft->SetParameters(whole->GetParameter(4), whole->GetParameter(5), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak3ft->SetLineColor(kBlack);
    peak3ft->SetFillColor(kBlack);
    peak3ft->SetFillStyle(0);
    peak3ft->Draw("same");

    TF1* peak4ft = new TF1("peak4ft", P1_with_landau, -1, 10, 5);
    peak4ft->SetParameters(whole->GetParameter(6), whole->GetParameter(7), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak4ft->SetLineColor(kBlack);
    peak4ft->SetFillColor(kBlack);
    peak4ft->SetFillStyle(0);
    peak4ft->Draw("same");

    TF1* peak5ft = new TF1("peak5ft", P1_with_landau, -1, 10, 5);
    peak5ft->SetParameters(whole->GetParameter(8), whole->GetParameter(9), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak5ft->SetLineColor(kBlack);
    peak5ft->SetFillColor(kBlack);
    peak5ft->SetFillStyle(0);
    peak5ft->Draw("same");

    TF1* peak6ft = new TF1("peak6ft", P1_with_landau, -1, 10, 5);
    peak6ft->SetParameters(whole->GetParameter(10), whole->GetParameter(11), whole->GetParameter(12), whole->GetParameter(13), whole->GetParameter(14));
    peak6ft->SetLineColor(kBlack);
    peak6ft->SetFillColor(kBlack);
    peak6ft->SetFillStyle(0);
    peak6ft->Draw("same");
}

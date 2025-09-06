#ifndef RESPONSE_FITPS_H
#define RESPONSE_FITPS_H

#include "TMath.h"
#include "TH1F.h"
#include "TF1.h"

// Global histogram pointers

Double_t fit_rangeMIN = -2;
Double_t fit_rangeMAX = 5;


extern TH1F* peak1g;
extern TH1F* peak2g;
extern TH1F* peak3g;
extern TH1F* peak4g;
extern TH1F* peak5g;
extern TH1F* peak6g;
extern TH1F* bc;
double resp1(double *x, double *par)
{
    return peak1g->Interpolate(x[0] - par[1]) * par[0] / 10000.;
}

double resp2(double *x, double *par)
{
    return peak2g->Interpolate(x[0] - par[1]) * par[0] / 10000.;
}

double resp3(double *x, double *par)
{
    return peak3g->Interpolate(x[0] - par[1]) * par[0] / 10000.;
}

double resp4(double *x, double *par)
{
    return peak4g->Interpolate(x[0] - par[1]) * par[0] / 10000.;
}

double resp5(double *x, double *par)
{
    return peak5g->Interpolate(x[0] - par[1]) * par[0] / 10000.;
}
double resp6(double *x, double *par)
{
    return peak6g->Interpolate(x[0] - par[1]) * par[0] / 10000.;
}
double left_skewed_landau(double *x, double *par)
{
    double mirrored_x = 2*par[1] - x[0];
    return par[0] * TMath::Landau(mirrored_x, par[1], par[2]);
}
double doubexp(double* x, double* par){
             return resp1(x, par)
                  + resp2(x, par + 2)
                  + resp3(x, par + 4)
                  + resp4(x, par + 6)
                  + resp5(x, par + 8)
                  + resp6(x, par + 10)
                  + left_skewed_landau(x, par+12);
}

double allgaus_plus_pol4(double* x, double* par) {
    // par[0-2]: gaus amplitude, mean, sigma
    // par[18-21]: pol3 coefficients
    double gaus1 = par[5] * exp(-0.5 *  pow((x[0] - par[6]) /  par[7], 2));
    double gaus2 = par[8] * exp(-0.5 *  pow((x[0] - par[9]) /  par[10], 2));
    double gaus3 = par[11] * exp(-0.5 * pow((x[0] - par[12]) / par[13], 2));
    double gaus4 = par[14] * exp(-0.5 * pow((x[0] - par[15]) / par[16], 2));
    double gaus5 = par[17] * exp(-0.5 * pow((x[0] - par[18]) / par[19], 2));
    double gaus6 = par[20] * exp(-0.5 * pow((x[0] - par[21]) / par[22], 2));
    double gaus7 = par[23] * exp(-0.5 * pow((x[0] - par[24]) / par[25], 2));
    double gaus8 = par[26] * exp(-0.5 * pow((x[0] - par[27]) / par[28], 2));
    double gaus9 = par[29] * exp(-0.5 * pow((x[0] - par[30]) / par[31], 2));
    double gaus10= par[32] * exp(-0.5 * pow((x[0] - par[33]) / par[34], 2));
    double gaus11= par[35] * exp(-0.5 * pow((x[0] - par[36]) / par[37], 2));
    double gaus12= par[38] * exp(-0.5 * pow((x[0] - par[39]) / par[40], 2));
    // double gaus13= par[41] * exp(-0.5 * pow((x[0] - par[42]) / par[43], 2));
    // double gaus14= par[44] * exp(-0.5 * pow((x[0] - par[45]) / par[46], 2));

    double mypol4 = (par[0] + par[1] * x[0] + par[2] * x[0] * x[0] + par[3] * x[0] * x[0] * x[0])*par[4];   //par[4] is overall scale factor for pol3

    return  mypol4 + gaus1 + gaus2 + gaus3 + gaus4 + gaus5 + gaus6 + gaus7 /* + gaus8 + gaus9 + gaus10 + gaus11 + gaus12  */;// + gaus13 + gaus14;
}   



double allgaus_plus_left_skewed_landau(double* x, double* par) {
    // par[0-2]: gaus amplitude, mean, sigma
    // par[3-5]: left_skewed_landau amplitude, MPV, sigma
    double gaus1 = par[0] * exp(-0.5 * pow((x[0] - par[1]) / par[2], 2));
    double gaus2 = par[3] * exp(-0.5 * pow((x[0] - par[4]) / par[5], 2));
    double gaus3 = par[6] * exp(-0.5 * pow((x[0] - par[7]) / par[8], 2));
    double gaus4 = par[9] * exp(-0.5 * pow((x[0] - par[10]) / par[11], 2));
    double gaus5 = par[12] * exp(-0.5 * pow((x[0] - par[13]) / par[14], 2));
    double gaus6 = par[15] * exp(-0.5 * pow((x[0] - par[16]) / par[17], 2));
    return gaus1 + gaus2 + gaus3 + gaus4 + gaus5 + gaus6 + par[18] * TMath::Landau(2*par[19] - x[0], par[19], par[20]);
}

/* double gaus_plus_left_skewed_landau(double* x, double* par) {
    // par[0-2]: gaus amplitude, mean, sigma
    // par[3-5]: left_skewed_landau amplitude, MPV, sigma
    double gaus = par[0] * exp(-0.5 * pow((x[0] - par[1]) / par[2], 2));
    double landau = left_skewed_landau(x, par + 3);
    return gaus + landau;
} */

double gaus_plus_left_skewed_landau(double* x, double* par) {
    double gaus = par[0] * exp(-0.5 * pow((x[0] - par[1]) / par[2], 2));
    double landau = left_skewed_landau(x, par + 3);
    return gaus + (par[3] * TMath::Landau(2*par[4] - x[0], par[4], par[5]));
}

double gaus_plus_pol4(double* x, double* par) {
    double gaus = par[0] * exp(-0.5 * pow((x[0] - par[1]) / par[2], 2));
    double mypol4 = (par[0+3] + par[1+3] * x[0] + par[2+3] * x[0] * x[0] + par[3+3] * x[0] * x[0] * x[0])*par[4+3];
    return gaus + mypol4;
}

// TF1 *fCarbonScaled = new TF1("fCarbonScaled","[4]*( [0] + [1]*x + [2]*x*x + [3]*x*x*x )",-5, -1);

double pol3_C(double* x, double* par) {
    return (par[0] + par[1] * x[0] + par[2] * x[0] * x[0] + par[3] * x[0] * x[0] * x[0])*par[4];
}


TF1* peak1ft = new TF1("peak1ft", [](double* x, double* par) {
        return (peak1g->Interpolate(x[0] - par[1]) * par[0] / 10000.) 
                + (par[2] * TMath::Landau(2*par[3] - x[0], par[3], par[4]));}, fit_rangeMIN, fit_rangeMAX, 5);

TF1* peak2ft = new TF1("peak2ft", [](double* x, double* par) {
        return (peak2g->Interpolate(x[0] - par[1]) * par[0] / 10000.) 
                + (par[2] * TMath::Landau(2*par[3] - x[0], par[3], par[4]));}, fit_rangeMIN, fit_rangeMAX, 5);


TF1* peak3ft = new TF1("peak3ft", [](double* x, double* par) {
        return (peak3g->Interpolate(x[0] - par[1]) * par[0] / 10000.) 
                + (par[2] * TMath::Landau(2*par[3] - x[0], par[3], par[4]));}, fit_rangeMIN, fit_rangeMAX, 5);

TF1* peak4ft = new TF1("peak4ft", [](double* x, double* par) {
        return (peak4g->Interpolate(x[0] - par[1]) * par[0] / 10000.) 
                + (par[2] * TMath::Landau(2*par[3] - x[0], par[3], par[4]));}, fit_rangeMIN, fit_rangeMAX, 5);

TF1* peak5ft = new TF1("peak5ft", [](double* x, double* par) {
        return (peak5g->Interpolate(x[0] - par[1]) * par[0] / 10000.) 
                + (par[2] * TMath::Landau(2*par[3] - x[0], par[3], par[4]));}, fit_rangeMIN, fit_rangeMAX, 5);

TF1* peak6ft = new TF1("peak6ft", [](double* x, double* par) {
        return (peak6g->Interpolate(x[0] - par[1]) * par[0] / 10000.) 
                + (par[2] * TMath::Landau(2*par[3] - x[0], par[3], par[4]));}, fit_rangeMIN, fit_rangeMAX, 5);

#endif // RESPONSE_FITSIMPLE2_H


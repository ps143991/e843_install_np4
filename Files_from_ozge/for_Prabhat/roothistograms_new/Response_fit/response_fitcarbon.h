#ifndef RESPONSE_FITCARBON_H
#define RESPONSE_FITCARBON_H

#include "TMath.h"
#include "TH1F.h"

// Global histogram pointers
extern TH1F* peak1g;
extern TH1F* peak2g;
extern TH1F* peak3g;
extern TH1F* peak4g;
extern TH1F* peak5g;
extern TH1F* peak6g;
extern TH1F* peak7g;
extern TH1F* peak8g;
extern TH1F* carbon;

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
double resp7(double *x, double *par)
{
    return peak7g->Interpolate(x[0] - par[1]) * par[0] / 10000.;
}
double resp8(double *x, double *par)
{
    return peak8g->Interpolate(x[0] - par[1]) * par[0] / 10000.;
}
double Res_carbon(double *x, double *par)
{
    return carbon->Interpolate(x[0] - par[1]) * par[0];
}
// Combined model: 8 responses + carbon background
double doubexp(double* x, double* par)
{
    return resp1(x, par)
            + resp2(x, par + 2)
            + resp3(x, par + 4)
            + resp4(x, par + 6)
            + resp5(x, par + 8)
            + resp6(x, par + 10)
            + resp7(x, par + 12)
            + resp8(x, par + 14)+Res_carbon(x, par + 16);
}

#endif // RESPONSE_FITCARBON_H


#ifndef RESPONSE_FITSIMPLE2_H
#define RESPONSE_FITSIMPLE2_H

#include "TMath.h"
#include "TH1F.h"

// Global histogram pointers
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
double landau(double *x, double *par)
{
    return par[0] * TMath::Landau(x[0], par[1], par[2], true);
}
double doubexp(double* x, double* par)
{
    return resp1(x, par)
                  + resp2(x, par + 2)
                  + resp3(x, par + 4)
                  + resp4(x, par + 6)
                  + resp5(x, par + 8)+ resp6(x, par + 10);//+landau(x, par+12);
    
    /*
   double signal = resp1(x, par)
                  + resp2(x, par + 2)
                  + resp3(x, par + 4)
                  + resp4(x, par + 6)
                  + resp5(x, par + 8)+ resp6(x, par + 10);
    double background = par[12] * TMath::Landau(x[0], par[13], par[14], true);
    return signal + background;*/
}

#endif // RESPONSE_FITSIMPLE2_H


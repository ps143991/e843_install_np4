#ifndef RESPONSE_fitsimple_H
#define RESPONSE_fitsimple_H

#include "TMath.h"
#include "TH1F.h"

// Assume peak1g etc are declared as global pointers
extern TH1F* peak1g;
extern TH1F* peak2g;
extern TH1F* peak3g;
extern TH1F* peak4g;

double doubexp(double* x, double* par)
{
    return peak1g->Interpolate(x[0] - par[1]) * par[0]/10000. +
           peak2g->Interpolate(x[0] - par[3]) * par[2]/10000. +
           peak3g->Interpolate(x[0] - par[5]) * par[4]/10000. +
           peak4g->Interpolate(x[0] - par[7]) * par[6]/10000.;
}

#endif // RESPONSE_DOUBEXP_H


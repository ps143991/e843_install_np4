#include <iostream>
#include <TF1.h>
#include <TMath.h>
#include <TGraph.h>
#include <cmath>
using namespace std;
double function1(double a, double b, double c, double d, double e, double f, double g)
{
    double Ta,Tb,ma,mb,mx,my,theta,Q;
    double EnergyTot=Ta+Q;
    Ta=a; mx=b; ma=c;mb=d;my=e;theta=f;Q=g;
    double cos_theta=cos(theta);
    // LAB FRAME ENERGY
    //Tb = pow((((sqrt(ma*mb*Ta)*cos_theta)+sqrt((ma*mb*Ta*pow(cos_theta,2))+(my+mb)*((my*Q)+(my-ma)*Ta)))/(my+mb)),2);

    // CM FRAME ENERGY
    //Tb = (mb/(my+mb))*(Q+(1-(ma/(my+mb)))*Ta);

    //


    double A,B,C,D,Energy3, Energy4;


    A=ma*my*(Ta/EnergyTot)/((ma+mx)/(mb+my));
    B=ma*mb*(Ta/EnergyTot)/((ma+mx)/(mb+my));
    C=mx*mb*(1+((ma*(abs(Q)))/mx*EnergyTot))/((ma+mx)/(mb+my));
    D=mx*my*(1+((ma*(abs(Q)))/mx*EnergyTot))/((ma+mx)/(mb+my));

    double sin2_theta =pow(sin(theta),2);
    Tb=EnergyTot*B*(pow((cos_theta+sqrt((D/B)-sin2_theta)),2));

    cout<<Ta<<" "<<ma<<" "<<mb<<" "<<my<<" "<<theta<<" "<<Q<<"    "<<Tb<<"| "<<EnergyTot<<endl;
    cout<<sin2_theta<<"  "<<cos_theta<<" "<<A<<" "<<B<<" "<<C<<" "<<D<<endl;
    return Tb;
}

int reaction_dynamics(){
    double Ta,Tb,ma,mb,mx,my,theta,Q;
    double cos_theta = cos(theta);
    Ta=553;
    ma=11.043909565;
    mb=2.0135;
    mx=3.016029;
    my=10.0523997;
    Q=-10.260;
    //Tb=5;
    auto graph = new TGraph;

    for (int i=0; i<40; i++) {
        if(function1(Ta,mx,ma,mb,my,i,Q)>=0){
        graph->AddPoint(i*3.1415/180, function1(Ta,mx,ma,mb,my,i*3.1415/180,Q));}}
    graph->Draw();
    return 0;
}

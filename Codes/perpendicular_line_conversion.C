#include <iostream>
#include "TF1.h"
#include "TSystem.h"
// Suppose you fitted ProfileY and got:
// X = a + b * Y

using namespace std;

void perpendicular_line_conversion() {
    // Given fit parameters

double C = -0.0130252;   // intercept from fit
double m = 0.00822122;   // slope from fit

// Compute equivalent relation Y = A + B * X
double m_new = 1.0 / m;         
double C_new = -C / m;

cout << "ProfileY fit: X = " << C << " + " << m << "*Y" << std::endl;
cout << "Equivalent (normal form): Y = " << C_new << " + " << m_new << "*X" << std::endl;

TF1 *line_perp = new TF1("line_perp", Form("%f + %f*x", C, m_new), -100, 100);

gPad->cd();
line_perp->SetLineColor(kRed);
line_perp->SetLineWidth(2);
line_perp->Draw("same");
}
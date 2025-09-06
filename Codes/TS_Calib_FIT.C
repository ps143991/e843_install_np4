#include <iostream>
#include <fstream>
#include <vector>
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"

void TS_Calib_FIT() {
    // Input file name
    const char* filename = "TS_Calib_pts.txt"; // Replace with your file name

    // Vectors to store the data
    std::vector<double> x, y;

    // Open the input file
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // Read the file line by line
    double xval, yval;
    while (infile >> xval >> yval) {
        x.push_back(xval);
        y.push_back(yval);
    }
    infile.close();

    // Check if data was read
    if (x.empty() || y.empty()) {
        std::cerr << "Error: No data read from file!" << std::endl;
        return;
    }

    // Create a TGraph to hold the data
    TGraph* graph = new TGraph(x.size(), x.data(), y.data());
    graph->SetTitle("Linear Fit;X-axis;Y-axis");
    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(kBlue);

    // Create a canvas
    TCanvas* canvas = new TCanvas("canvas", "Linear Fit", 800, 600);
    graph->Draw("AP");

    // Define a linear fit function (pol1)
    TF1* fitFunc = new TF1("fitFunc", "pol1", x.front(), x.back());
    fitFunc->SetLineColor(kRed);

    // Perform the fit
    graph->Fit(fitFunc, ""); // "Q" option suppresses fit messages

    // Draw the fit result
    canvas->Update();

    // Print the fit parameters
    std::cout << "Fit Results: " << std::endl;
    std::cout << "Slope (p1): " << fitFunc->GetParameter(1) << std::endl;
    std::cout << "Intercept (p0): " << fitFunc->GetParameter(0) << std::endl;
}

#include <TSystemDirectory.h>
#include <TList.h>
#include <TSystemFile.h>
#include <TString.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <TPaveText.h>
#include <iostream>

using namespace std;

void save_GATCONFTS_spectra_to_image(const char* folderPath = "./") {
    // Get list of files in the directory
    TSystemDirectory dir(folderPath, folderPath);
    TList *files = dir.GetListOfFiles();
    if (!files) {
        std::cerr << "Error: No files found in directory!" << std::endl;
        return;
    }

    TIter next(files);
    TSystemFile *file;
    TString fileName;

    while ((file = (TSystemFile*)next())) {
        fileName = file->GetName();

        // Only process files that match "NPRaw*"
        if (!file->IsDirectory() && fileName.BeginsWith("NPRaw")) {
            TString filePath = TString::Format("%s/%s", folderPath, fileName.Data());
            std::cout << "Processing: " << filePath << std::endl;

            // Open the ROOT file
            TFile *f = TFile::Open(filePath);
            if (!f || f->IsZombie()) {
                std::cerr << "Error opening file: " << filePath << std::endl;
                continue;
            }

            // Get the tree
            TTree *tree = (TTree*)f->Get("RawTree");
            if (!tree) {
                std::cerr << "Error: RawTree not found in " << filePath << std::endl;
                f->Close();
                continue;
            }



 // Create a temporary histogram with a broad bin range
TH1F *tempHist = new TH1F("tempHist", fileName.Data(), 10000, 0, 40000);

// Fill the histogram
tree->Draw("GATCONFTS/1E9>>tempHist", "", "goff");  // "goff" avoids drawing to canvas

// Find the actual data range using bin content
int firstBin = tempHist->FindFirstBinAbove(0); // First non-empty bin
int lastBin = tempHist->FindLastBinAbove(0);   // Last non-empty bin

// Convert to actual min/max values
double minVal = tempHist->GetXaxis()->GetBinLowEdge(firstBin);
double maxVal = tempHist->GetXaxis()->GetBinUpEdge(lastBin);

if (minVal == maxVal) {  // Avoid zero-bin histograms
    minVal -= 1;
    maxVal += 1;
}

// Define number of bins (adaptive binning)
//int numBins = std::min(200, std::max(50, (int)(maxVal - minVal) / 10));
int numBins = (maxVal-minVal)*50;
// Now create the final histogram with the correct binning
TH1F *hist = new TH1F("hist", fileName.Data(), numBins, minVal-(minVal/50), maxVal+(maxVal/50));

// Fill again with optimized binning
tree->Draw("GATCONFTS/1E9>>hist", "", "");

// Cleanup
delete tempHist;  // No longer needed


            // Create canvas
            TCanvas *canvas = new TCanvas("canvas", "Histogram", 800, 600);

            // Draw histogram
            hist->Draw();

            // Add a label with filename
            TPaveText *label = new TPaveText(0.7, 0.75, 0.9, 0.85, "NDC");
            label->AddText(fileName.Data());
            label->SetFillColor(0);
            label->Draw();

            // Save image
            TString imgName = fileName;
            imgName.ReplaceAll(".root", ".jpg");  // Change extension to jpg
            canvas->SaveAs(imgName.Data());

            // Cleanup
            delete hist;
            delete canvas;
            f->Close();
            delete f;
        }
    }
}

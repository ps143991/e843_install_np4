// File: grouped_circles.C
#include <vector>
#include <cmath>
#include "TCanvas.h"
#include "TEllipse.h"
#include "TLine.h"
#include "TLegend.h"
#include "TText.h"

struct Circle {
  double x, y;   // center
  double r;      // radius
};
struct CircleGroup {
  std::vector<Circle> cs;
  // Draw all (as circles)
  void Draw(int lineColor=kBlue, int lineWidth=2, int fillStyle=0) const {
    for (auto& c: cs){
      auto *e = new TEllipse(c.x, c.y, c.r, c.r);
      e->SetFillStyle(fillStyle);
      e->SetLineColor(lineColor);
      e->SetLineWidth(lineWidth);
      e->Draw("same");
    }
  }

};

// Demo
/* void target_mask(){
//   TCanvas *c = new TCanvas("c","Grouped circles", 700, 700);

    gPad->cd();
//   c->DrawFrame(-20, -20, 20, 20, "Grouped circles;X;Y");

  // Build a group: 1 big circle with 3 inner circles
  CircleGroup G;
  G.cs.push_back({-2.4,-2.1,13}); // outer
  G.cs.push_back({-3.6,8.8,0.2});   // top left
  G.cs.push_back({-2.4,-2.1,0.2});  // central
  G.cs.push_back({-1.5,-10.1,0.2}); // bottop right
  G.cs.push_back({-7.37, -2.6471, 0.2}); // left middle
  G.cs.push_back({2.57, -1.5528, 0.2});  // right middle
  G.cs.push_back({-12.34, -3.194, 0.2}); // far left
  G.cs.push_back({7.54, -1.006, 0.2});   // far right
  G.cs.push_back({-2.947, 2.87, 0.2});    // top middle
  G.cs.push_back({-2.071, -5.082, 0.2});   // bottom middle
  G.cs.push_back({0.63114, 0.10651, 0.2});    // identifier hole
//   G.cs.push_back({ 0, 0, 7});
//   G.cs.push_back({-3, 2, 2});
//   G.cs.push_back({ 4,-1, 1.2});

  // Draw original
  G.Draw(kBlack, 2);

//   c->Update();
} */

  Double_t holes_Target_X[11] = {-2.4, -3.6, -2.4, -1.5, -7.37, 2.57, -12.34, 7.54, -2.947, -2.071, -0.62586};
  Double_t holes_Target_Y[11] = {-2.1, 8.8, -2.1, -10.1, -2.6471, -1.5528, -3.194, -1.006, 2.87, -5.082, 0.102821};

  enum { OUTER=0, TOP_LEFT, CENTER, BOTTOM_RIGHT, LEFT_MIDDLE, RIGHT_MIDDLE, FAR_LEFT, FAR_RIGHT, TOP_MIDDLE, BOTTOM_MIDDLE, IDENTIFIER_HOLE };

void target_mask(){
//   TCanvas *c = new TCanvas("c","Grouped circles", 700, 700);

    gPad->cd();
//   c->DrawFrame(-20, -20, 20, 20, "Grouped circles;X;Y");

  // Build a group: 1 big circle with 3 inner circles
  CircleGroup G;

  G.cs.push_back({holes_Target_X[0], holes_Target_Y[0], 13});     // outer
  G.cs.push_back({holes_Target_X[1], holes_Target_Y[1], 0.2});    // top left
  G.cs.push_back({holes_Target_X[2], holes_Target_Y[2], 0.2});    // central
  G.cs.push_back({holes_Target_X[3], holes_Target_Y[3], 0.2});    // bottop right
  G.cs.push_back({holes_Target_X[4], holes_Target_Y[4], 0.2});    // left middle
  G.cs.push_back({holes_Target_X[5], holes_Target_Y[5], 0.2});    // right middle
  G.cs.push_back({holes_Target_X[6], holes_Target_Y[6], 0.2});    // far left
  G.cs.push_back({holes_Target_X[7], holes_Target_Y[7], 0.2});    // far right
  G.cs.push_back({holes_Target_X[8], holes_Target_Y[8], 0.2});    // top middle
  G.cs.push_back({holes_Target_X[9], holes_Target_Y[9], 0.2});    // bottom middle
  G.cs.push_back({holes_Target_X[10], holes_Target_Y[10], 0.2});  // identifier hole
//   G.cs.push_back({ 0, 0, 7});
//   G.cs.push_back({-3, 2, 2});
//   G.cs.push_back({ 4,-1, 1.2});

  // Draw original
  G.Draw(kRed, 5);

//   c->Update();
}
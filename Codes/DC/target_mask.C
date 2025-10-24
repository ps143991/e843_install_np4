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

  // --- transforms (all in-place) ---
  void Translate(double dx, double dy){
    for (auto& c: cs){ c.x += dx; c.y += dy; }
  }

  // Rotate by angle (radians) about point (cx,cy)
  void Rotate(double angle, double cx=0, double cy=0){
    const double ca = std::cos(angle), sa = std::sin(angle);
    for (auto& c: cs){
      double x = c.x - cx, y = c.y - cy;
      double xr =  ca*x - sa*y;
      double yr =  sa*x + ca*y;
      c.x = xr + cx; c.y = yr + cy;
    }
  }

  // Reflect across a line through origin making angle 'phi' from +x axis
  // (phi=0: reflect across x-axis; phi=pi/2: reflect across y-axis)
  void Reflect(double phi){
    const double c = std::cos(2*phi), s = std::sin(2*phi);
    for (auto& c0: cs){
      double x = c0.x, y = c0.y;
      c0.x =  c*x + s*y;
      c0.y =  s*x - c*y;
    }
  }

  // Invert all circles w.r.t. inversion circle center (cx,cy) radius R.
  // Circle not through center -> circle; circle through center -> line.
  // Returns pair of (circles, lines). Lines are given as (ax,ay,bx,by) long segments.
  std::pair<std::vector<Circle>, std::vector<std::array<double,4>>>
  Invert(double cx, double cy, double R, double drawSpan=1e3) const {
    std::vector<Circle> outC;
    std::vector<std::array<double,4>> outL;

    for (auto c: cs){
      double dx = c.x - cx, dy = c.y - cy;
      double d2 = dx*dx + dy*dy;
      double dd = std::sqrt(d2);
      // if |d - r| ~ 0 -> line
      if (std::abs(dd - c.r) < 1e-12){
        // Line is the image: its equation is the image of circle through center.
        // The line is orthogonal to vector (dx,dy) and located at distance R^2/(2r) from center along that direction.
        // Safer approach: compute line n·(x - x0) = 0 with normal along (dx,dy)
        // Distance from inversion center to line:
        double dist = (R*R) / (2*c.r);
        double nx = dx/dd, ny = dy/dd; // unit normal
        double x0 = cx + nx*dist;
        double y0 = cy + ny*dist;
        // Build a long visible segment perpendicular to normal (i.e. along tangent direction)
        double tx = -ny, ty = nx;
        outL.push_back({ x0 - tx*drawSpan, y0 - ty*drawSpan,
                         x0 + tx*drawSpan, y0 + ty*drawSpan });
      } else {
        // Circle maps to circle
        double denom = (d2 - c.r*c.r);
        double factor = (R*R)/denom;
        Circle ci;
        ci.x = cx + factor*dx;
        ci.y = cy + factor*dy;
        ci.r = std::abs(factor)*c.r;
        outC.push_back(ci);
      }
    }
    return {outC, outL};
  }

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

Double_t dist(Double_t x1, Double_t y1, Double_t x2, Double_t y2){
    return sqrt( (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) );
}

// Demo
void target_mask(){
  TCanvas *c = new TCanvas("c","Grouped circles", 700, 700);
  c->DrawFrame(-20, -20, 20, 20, "Grouped circles;X;Y");

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

//   TLine *l1 = new TLine(-2.4,-2.1,-3.6,8.8);
//     l1->SetLineColor(kGray+2); l1->SetLineStyle(2); l1->Draw("same");
//   TLine *l2 = new TLine(-2.4,-2.1,-1.5,-10.1);
//     l2->SetLineColor(kGray+2); l2->SetLineStyle(2); l2->Draw("same");

//     TText *t1 = new TText(-3.6,8.8,Form("d=%.3f", dist(-2.4,-2.1,-3.6,8.8)));
//     t1->SetTextSize(0.03); t1->SetTextColor(kGray+2); t1->Draw("same");
//     TText *t2 = new TText(-1.5,-10.1,Form("d=%.3f", dist(-2.4,-2.1,-1.5,-10.1)));
//     t2->SetTextSize(0.03); t2->SetTextColor(kGray+2); t2->Draw("same");


  // Make a copy and apply transforms as a single unit
  CircleGroup G2 = G;
  G2.Translate(6, 4);
  G2.Rotate(M_PI/6.0, 6, 4);   // rotate about its current "group center" (6,4)
  G2.Reflect(0);               // reflect across x-axis
//   G2.Draw(kRed+1, 2);

  // Inversion of original group about circle centered at origin with R=8
  auto inv = G.Invert(0,0,8);
  // Draw inversion circle (reference)
  TEllipse *invCirc = new TEllipse(0,0,8,8);
//   invCirc->SetLineStyle(2); invCirc->SetLineColor(kGray+2); invCirc->Draw("same");

  // Draw inverted circles
  for (auto& c3 : inv.first){
    auto *e = new TEllipse(c3.x, c3.y, c3.r, c3.r);
    e->SetLineColor(kGreen+2);
    e->SetLineWidth(2);
    e->SetFillStyle(0);
    // e->Draw("same");
  }
  // Draw lines (images of circles through inversion center)
  for (auto& L : inv.second){
    auto *ln = new TLine(L[0], L[1], L[2], L[3]);
    ln->SetLineColor(kGreen+2);
    ln->SetLineWidth(2);
    // ln->Draw("same");
  }

  // legend
  auto *leg = new TLegend(0.62,0.72,0.88,0.88);
  leg->AddEntry((TObject*)0,"Black: original group","");
  leg->AddEntry((TObject*)0,"Red: translated+rotated+reflected","");
  leg->AddEntry(invCirc,"Inversion reference (R=8)","l");
  leg->AddEntry((TObject*)0,"Green: inversion of original","");
  leg->Draw();

  c->Update();
}

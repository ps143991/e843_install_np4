
#ifndef EXOGAMSIMULATEDGEOMETRY_H
#define EXOGAMSIMULATEDGEOMETRY_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "NPFunction.h"

namespace exogam {
  

class ExogamSimulatedGeometry 
{
  public:

    ExogamSimulatedGeometry(){};
    ~ExogamSimulatedGeometry(){};

    // simulations performed with target at 0,0,0 and exogam shifter back 100mm
    TVector3 TargetPosition{0.0,0.0,0.0};
    TVector3 BeamDirection{0.0,0.0,1.0};

    // struct to hold relationship btwn energy and angle
    // currently linear, might need to change?
    struct LinearEquation {
      double slope = 0.0;
      double intercept = 0.0;
      double evaluate(double energy) const {
        return slope * (energy/1000.) + intercept;
      }
    };
  
    // struct to hold both equations for each segment
    struct SegmentAngles {
      LinearEquation thetaEq;
      LinearEquation phiEq;
    };

    struct SegmentXYZPos {
      LinearEquation xEq;
      LinearEquation yEq;
      LinearEquation zEq;
    };

    std::vector<SegmentAngles> segment_angles{16 * 4 * 4};
    std::vector<SegmentXYZPos> segment_xyzpos{16 * 4 * 4};

    bool LoadTextFile(const std::string& filename) {
      std::ifstream txt(filename);
      if (!txt.is_open()) {
	nptool::message("red","exogam","ExogamSimulatedGeometry",
                        "Failed to load simulated angles for doppler correction!");
        return false;
      }

      std::string line;
      int i = 0;
      while (std::getline(txt, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        int flange, crystal, segment;
        double st, it, sp, ip, sx, ix, sy, iy, sz, iz;
        //if (!(ss >> flange >> crystal >> segment >> st >> it >> sp >> ip)) {
        if (!(ss >> flange >> crystal >> segment 
		 >> st >> it >> sp >> ip
		 >> sx >> ix >> sy >> iy >> sz >> iz)) {
          std::cout << " BAD LINE: " << line << std::endl;
          continue;
        }
 
        int idx = getIndex(flange, crystal, segment);
        segment_angles[idx] = {{st, it}, {sp, ip}};
        segment_xyzpos[idx] = {{sx, ix}, {sy, iy}, {sz, iz}};
      }
      return true;
    }

    void PrintAngleData(double energy){
      std::cout << " -------- ANGLES FOR ENERGY " << energy << " --------" << std::endl;
      for(int f = 0; f<16; f++){
        for(int c = 0; c<4; c++){
          for(int s = 0; s<4; s++){
	    std::cout << f << " " << c << " " << s << "  ||  "
		      << GetTheta(f, c, s, energy) << " "
		      << GetPhi(f, c, s, energy) << " "
		      << std::endl;
	  }
	}
      }
    }

    void PrintXYZData(double energy){
      std::cout << " -------- XYZ FOR ENERGY " << energy << " --------" << std::endl;
      for(int f = 0; f<16; f++){
        for(int c = 0; c<4; c++){
          for(int s = 0; s<4; s++){
	    std::cout << f << " " << c << " " << s << "  ||  "
		      << GetX(f, c, s, energy) << " "
		      << GetY(f, c, s, energy) << " "
		      << GetZ(f, c, s, energy) << " "
		      << std::endl;
	  }
	}
      }
    }

    double GetTheta_Rad(int flange, int crystal, int segment, double energy) const {
      return GetTheta(flange,crystal,segment,energy) * M_PI / 180.0;
    }
  
    double GetPhi_Rad(int flange, int crystal, int segment, double energy) const {
      return GetPhi(flange,crystal,segment,energy) * M_PI / 180.0;
    }

    double GetTheta(int flange, int crystal, int segment, double energy) const {
      return segment_angles[getIndex(flange, crystal, segment)].thetaEq.evaluate(energy);
    }
  
    double GetPhi(int flange, int crystal, int segment, double energy) const {
      return segment_angles[getIndex(flange, crystal, segment)].phiEq.evaluate(energy);
    }

    double GetX(int flange, int crystal, int segment, double energy) const {
      return segment_xyzpos[getIndex(flange, crystal, segment)].xEq.evaluate(energy);
    }

    double GetY(int flange, int crystal, int segment, double energy) const {
      return segment_xyzpos[getIndex(flange, crystal, segment)].yEq.evaluate(energy);
    }

    double GetZ(int flange, int crystal, int segment, double energy) const {
      return segment_xyzpos[getIndex(flange, crystal, segment)].zEq.evaluate(energy);
    }
 
  private:
    int getIndex(int flange, int crystal, int segment) const {
      return flange * 16 + crystal * 4 + segment;
    }
  };

} // namespace exogam

#endif

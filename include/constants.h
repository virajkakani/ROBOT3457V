#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "math_funcs.h"

constexpr double OMEGA_THRESHOLD = 10; // Threshold for turning speed (percent)
constexpr double VELOCITY_THRESHOLD = 1; // Threshold for velocity (percent)
constexpr double VERTICAL_THRESHOLD = 1; // Threshold for vertical movement (percent)
constexpr int circle_prec = 12; // Precision for circle calculations (number of segments to approximate a circle)
constexpr double TOLERANCE = 1e-5;

const geo::point START = geo::point(2.75,2);

const geo::point GOALCOLOR1(1,2); // Edit these to ur needs
const geo::point GOALCOLOR2(2,1); // Edit these to ur needs

const geo::point GOALYELLOW1(-1,2); // Edit these to ur needs
const geo::point GOALYELLOW4(2,-1); // Edit these to ur needs
const geo::point GOALCENTER(0,0); // Edit these to ur needs

const geo::circle OPPGOAL1(geo::point(-1,-2), 0.75); // Edit these to ur needs
const geo::circle OPPGOAL2(geo::point(-2,-1), 0.75); // Edit these to ur needs

const geo::lineseg Toggle2(geo::point(-0.25,2),geo::point(0.25,2)); // Edit these to ur needs
const geo::lineseg Toggle3(geo::point(2,0.25),geo::point(2,-0.25)); // Edit these to ur needs

const geo::point PICKUPVERT3(2,2); // Edit these to ur needs
const geo::point PICKUPVERT4(1,1); // Edit these to ur needs
const geo::point PICKUPVERT9(1,0); // Edit these to ur needs
const geo::point PICKUPVERT11(0,1); // Edit these to ur needs

const std::vector<geo::shape> All = {
    geo::circle(PICKUPVERT3, 0.75),
    geo::circle(PICKUPVERT4, 0.75), geo::circle(PICKUPVERT9, 0.75), geo::circle(PICKUPVERT11, 0.75),
    geo::circle(GOALCOLOR1, 0.75), geo::circle(GOALCOLOR2, 0.75),
    geo::circle(GOALYELLOW1, 0.75),
    geo::circle(GOALYELLOW4, 0.75)
    };

#endif // CONSTANTS_H
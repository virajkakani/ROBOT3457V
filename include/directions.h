#ifndef DIRECTIONS_H
#define DIRECTIONS_H

#include "vex.h"
#include "math_funcs.h"
#include "constants.h"


struct Directions{
    bool is_straight = 0;
        double dist;
        geo::shape Shape;
    Directions();
    Directions(const Directions& d);
};

struct DirectionLedger{
    double general_angle_heading;
    std::vector<Directions> dirs;

    DirectionLedger(const geo::point& starting_point, const std::pair<double, double> &dir);
};

void turn(vex::smartdrive& d, vex::inertial& InertialSensor, const double& degrees);
vex::turnType operator!(vex::turnType T);
void go_circle(vex::smartdrive& d, const geo::circle& c, std::pair<double,double>& dir, geo::point& pos, double angle_heading);
void go_rectangle(const vex::smartdrive& d, const geo::rectangle_par& r, geo::point& pos, double dist, double angle_heading);
void go_dir(vex::smartdrive& d, vex::inertial& ine, geo::point& pos, std::pair<double,double>& dir, double angle_heading);
void hit_Toggle(vex::smartdrive& d, vex::inertial& iner, geo::point& pos, const bool& y_axis, bool& blue);


#endif // DIRECTIONS_H
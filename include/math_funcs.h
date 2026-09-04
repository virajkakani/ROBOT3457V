#ifndef MATH_FUNCS_H
#define MATH_FUNCS_H  

#include <cmath>
#include <vector>
#include <cstddef>
#include <utility>
#include "Err_Types.h"


namespace geo{
    struct point;
    struct lin;
    struct lineseg;
    struct shape;
    struct rectangle_par;
    struct circle;
    struct shape;

    // The constant π in math 
    extern const double _PI;

    // The corigin in the coordinate plane
    extern const point __origin;

    // The square function
    double sq(double k);

    // Finds the distance from the origin with x coordinate i, and y coordinate j
    double magn(double i, double j);

    // The angle in the polar coordinates of a point (i,j)
    double angle(double i, double j);


    // Leaves m unchanged when quotient is zero.
    TypeandERR<void> mod_eq(double &m, unsigned int quotient);

    // A class for points based on rectangular coordinates
    struct point{
        double x_coord = 0;
        double y_coord = 0;

        point();
        point(const double& x, const double& y);
        point(const point& other);
        TypeandERR<bool> operator==(const point& p) const;
        bool in_rect(const rectangle_par& rect) const;
        bool in_circle(const circle& c) const;
        bool on_circle(const circle& c) const;
        // Right (counter-clockwise) is true, left (clockwise) is false (assuming you are facing the circle)
        TypeandERR<bool> circdir(const circle& c, const double& angle) const;
        TypeandERR<bool> on_seg(const lineseg& p) const;
    };

    struct lineseg{
        point point1 = point(0,0);
        point point2  = point(0,1);

        lineseg();
        lineseg(const lineseg& l);
        lineseg(const point& p1, const point& p2);
        TypeandERR<double> slp() const;
        TypeandERR<double> y_int() const;
        TypeandERR<bool> operator==(const lineseg& l) const;
        TypeandERR<double> len() const;
        TypeandERR<point> intersection(const lineseg& l) const;
        TypeandERR<point> intersection(const lin& l) const;
        bool intersects(const shape& s);
        TypeandERR<double> perp_slp() const;
        bool is_vertical() const;
        bool is_horizontal() const;

    };

    struct lin{
        bool is_vertical;
        union{
            struct{
                double slope;
                double y_inter;
            };
            double x_inter;
        };
        lin();
        lin(const double& x_i);
        lin(const point& p1, const point& p2);
        lin(const lineseg& l);
        lin(const lin& l);
        lin(const double& slp, const double& y_inter);
        static TypeandERR<lin> lin_x_y(const double& x, const double& y);
        bool operator==(const lin& other) const;

        // Safer alternative to calling the attribute " lin::slope" directly
        TypeandERR<double> slp() const;

        // Safer alternative to calling the attribute " lin::y_inter" directly
        TypeandERR<double> y_int() const;

        // Safer alternatives to calling the attribute x_inter directly
        TypeandERR<double> x_int() const;

        TypeandERR<point> intersection(const lin& l) const;
        TypeandERR<std::pair<point, point>> intersection(const circle& c) const;
        TypeandERR<std::pair<point, point>> intersection(const rectangle_par& c) const;
        TypeandERR<std::pair<point, point>> intersection(const shape& c) const;
        TypeandERR<double>  y_coord_at_x(const double& x_coord) const;
        double ang() const;
    };

    struct rectangle_par{
        point bottom_left = point(0,0);
        double y_para = 1;
        double x_para = 1;

        rectangle_par();
        rectangle_par(const double& len, const double& wid);
        rectangle_par(const point& bl, const double& y_p_len, const double& x_p_len);
        rectangle_par(const rectangle_par& other);
    };

    struct circle{
        point center = point(0,0);
        double radius = 1;

        circle();
        circle(const point& c, const double& r);
        circle(const circle& other);
        TypeandERR<double> len_min_arc(const point& p1, const point& p2) const;
    };

    struct shape {
        bool type; // false = rect, true = circle
        circle cir;
        rectangle_par rect;
        shape();    
        shape(const circle& c);
        shape(const rectangle_par& r);
    };

    // The distance between two points on the x-y coordinate plane: p1 and p2
    double __dist(point p1, point p2);

    // The angle that the  lin p1 p2  makes with the x- axis
    double __angle_from_horizontal(point p1,point p2);

    // The minimum of a set of numbers. Empty input returns nums.size().
    TypeandERR<std::size_t> min_ind(const std::vector<double>& nums);

    // Empty input returns NaN.
    TypeandERR<double> min_invec(const std::vector<double>& nums);

    double distance_to_lineseg(const point& p, const lineseg& l);

    // Finds the directions to the closest point (distance, angle from horizontal) from a set of points
    TypeandERR<std::pair<double, double>> closest_point_dir(const point &p1, const std::vector<point> &other_pts);

    // Finds the direction to the closest point in a  lin segment to a given point
    std::pair<double, double> closest_point_inseg_dir(const point &p, const lineseg&  l);

    double dist_from_line(const point& p, const lin& l);

    double dist_from_rect(const point& p, const rectangle_par& rect);

    double dist_from_circle(const point& p, const circle& c);

    double line_len_incircle(const lin& l, const circle& c);
    
    std::vector<size_t> intersects_shape(const point& start, const std::pair<double, double>& dir, const std::vector<shape>& v);

}
#endif // MATH_FUNCS_H
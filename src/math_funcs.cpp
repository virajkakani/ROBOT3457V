#include "math_funcs.h"

#include <iostream>
#include <algorithm>
#include <cmath>
namespace geo{

    // The constant π in math 
    const double _PI = 3.1415926535897932384626433832795;
    const point __origin = point(0,0);

    bool is_between(double a, double b, double c){
        if (a > c){
            std::swap(a, c);
        }
        return (a <= b) && (b <= c);
    }


    // The square function (k^2)
    double sq(double k) {
        return k * k;
    }

    // Finds the distance from the origin with x, y
    double magn(double x, double y) {
        return std::sqrt(sq(x) + sq(y));
    }

    // The angle in the polar coordinates of a point (x,y)
    double angle(double x, double y) {
        return std::atan2(y, x) * 180.0 / _PI;
    }

    TypeandERR<void> mod_eq(double &m, unsigned int quotient) {
        if (quotient == 0) {
            inv_arg("The quotient cannot be zero");
        }
        m = std::fmod(m, static_cast<double>(quotient));
        okay;
    }

    // The distance between two points on the x-y coordinate plane: p1 and p2
    double __dist(point p1, point p2) {
        return magn(p2.x_coord - p1.x_coord, p2.y_coord - p1.y_coord);
    }

    // The angle that the line p1 p2  makes with the x- axis
    double __angle_from_horizontal(point p1, point p2) {
        return angle(p2.x_coord - p1.x_coord, p2.y_coord - p1.y_coord);
    }


    // shape member fns

    shape::shape() = default;
    shape::shape(const circle& c){
        cir = c;
        type = true;
    }

    shape::shape(const rectangle_par& r){
        rect = r;
        type = false;
    }



    // point member functions

    point::point() = default;
    point::point(const double& x, const double& y) : x_coord(x), y_coord(y) {}
    point::point(const point& other) : x_coord(other.x_coord), y_coord(other.y_coord){};
    TypeandERR<bool> point::operator==(const point& p) const{
        return (p.x_coord==x_coord) && (p.y_coord==y_coord);
    }

    bool point::in_rect(const rectangle_par& rect) const{
        int x_min = rect.bottom_left.x_coord;
        int x_max = rect.bottom_left.x_coord + rect.x_para;
        int y_min = rect.bottom_left.y_coord;
        int y_max = rect.bottom_left.y_coord + rect.y_para;
        return is_between(x_min, x_coord, x_max) && is_between(y_min, y_coord, y_max);
    }

    TypeandERR<bool> point::on_seg(const lineseg& l) const{
        if ((*this == l.point1) || (*this == l.point2)){
            return true;
        }
        if ((l.point1 == l.point2)){
            inv_arg("The two points cannot be the same");
        }
        if (l.is_vertical()){
            return (x_coord == l.point1.x_coord) && is_between(l.point1.y_coord, y_coord, l.point2.y_coord);
        }
        if (l.is_horizontal()){
            return (y_coord == l.point1.y_coord) && is_between(l.point1.x_coord, x_coord, l.point2.x_coord);
        }
        return ((l.point1.x_coord - x_coord)/(l.point2.x_coord - x_coord) == (l.point1.y_coord - y_coord)/(l.point2.y_coord - y_coord)) && 
        is_between(l.point1.x_coord, x_coord, l.point2.x_coord) &&
        is_between(l.point1.y_coord, y_coord, l.point2.y_coord);
    }

    bool point::in_circle(const circle& c) const{
        return __dist(*this, c.center) <= c.radius;
    }

    bool point::on_circle(const circle& c) const{
        return __dist(*this, c.center) == c.radius;
    }

    TypeandERR<bool> point::circdir(const circle& c, const double& angle_heading) const{
        if (!on_circle(c)){
        inv_arg("Point must be on circle!");
        }
        double sl = tan(angle_heading);
        lin _l;
        _l =(std::isinf(sl)) ? lin(x_coord) : lin(sl, y_coord - x_coord * sl);
        point other = (_l.intersection(c)->first == *this) ? _l.intersection(c)->second : _l.intersection(c)->first;
        lineseg l(*this, other);
        if (c.center.on_seg(l)){
            return true;
        }
        if (l.is_horizontal()){
            bool ans = (x_coord > other.x_coord) && (y_coord > c.center.y_coord);
            return ans;
        }
        if (l.is_vertical()){
            bool ans = (y_coord < other.y_coord) && (x_coord > c.center.x_coord);
            return ans;
        }
        if (l.slp() > 0){
            bool ans = (y_coord < other.y_coord) && (c.center.y_coord > lin(l).y_coord_at_x(c.center.x_coord));
            return ans;
        }
        if (l.slp() < 0){
            bool ans = !(y_coord < other.y_coord) && (c.center.y_coord > lin(l).y_coord_at_x(c.center.x_coord));
            return ans;
        }

        return false;
    }


    // lineseg member functions

    lineseg::lineseg() = default;
    lineseg::lineseg(const point& p1, const point& p2){
        if (p1 == p2){
            std::cerr << "The two points cannot be the same" << std::endl;;
        }
        lineseg l;
        point1 = p1;
        point2 = p2;
    }

    lineseg::lineseg(const lineseg& l) : point1(l.point1), point2(l.point2) {}

    TypeandERR<double> lineseg::slp() const{
        auto res = (point1.y_coord - point2.y_coord)/(point1.x_coord - point2.x_coord);
        if (std::isinf(res)){
            inv_arg("Lines which are vertical have undefined slope");
        }
        return res;
    }

    TypeandERR<double> lineseg::y_int() const{
        return point1.y_coord - (slp() * point1.x_coord);
    }

    TypeandERR<bool> lineseg::operator==(const lineseg& l) const{
        return ((point1 == l.point1) && (point2 == l.point2)) || ((point2 == l.point1) && (point1 == l.point2));
    }

    TypeandERR<double> lineseg::len() const{
        return __dist(point1, point2);
    }

    TypeandERR<point> lineseg::intersection(const lineseg& l) const{
        lin L(*this);
        lin _l(l);

        auto p = L.intersection(l);

        if (p->on_seg(l) && p->on_seg(*this)){
            return p;
        }
        else{
            range_err("They don't intersect since the segments never meet");
        }
        
    }
    TypeandERR<point> lineseg::intersection(const lin& l) const{
        lin L(*this);

        auto p = L.intersection(l);

        if (p->on_seg(*this)){
            return p;
        }
        else{
            range_err("They don't intersect since the segments never meet");
        }
        
    }

    TypeandERR<double> lineseg::perp_slp() const{
        if (slp() == 0){
            inv_arg("The perpendicular slope is undefined");
        }
        if (slp().is_err()){
            return 0;
        }
        return -1/slp();
    }

    bool lineseg::is_vertical() const{
        return point1.x_coord == point2.x_coord;
    }

    bool lineseg::is_horizontal() const{
        return point1.y_coord == point2.y_coord;
    }

    bool lineseg::intersects(const shape& s){
        auto intersec = lin(*this).intersection(s);
        return intersec.is_err() && (intersec->first.on_seg(*this) || intersec->second.on_seg(*this));
    }














    // lin member functions

    lin::lin() : is_vertical(false), slope(0), y_inter(0) {}

    lin::lin(const point& p1, const point& p2){
        if (p1 == p2){
            std::cerr << "The two points cannot be the same" << std::endl;
        }
        if (p1.x_coord == p2.x_coord){
            x_inter = p1.x_coord;
            is_vertical = true;
        } else {
            auto L = lineseg(p1,p2);
            slope = L.slp();
            y_inter = L.y_int();
            is_vertical = false;
        }
    }

    lin::lin(const double& x_i){
        is_vertical = true;
        x_inter = x_i;
    }

    lin::lin(const lineseg& l){
        if (l.point1.x_coord == l.point2.x_coord){
            is_vertical = true;
            x_inter = l.point1.x_coord;
        } else{
            is_vertical = false;
            slope = l.slp();
            y_inter = l.y_int();
        }
    }

    lin::lin(const lin& l){
        if (l.is_vertical){
            is_vertical = l.is_vertical;
            x_inter = l.x_inter;
        } else{
            is_vertical = l.is_vertical;
            slope = l.slope;
            y_inter = l.y_inter;
        }
    }

    lin::lin(const double& slp, const double& y_inter) : is_vertical(false), slope(slp), y_inter(y_inter) {}



    TypeandERR<lin> lin::lin_x_y(const double& x, const double& y){
        if ((x == 0) && (y != 0)){
            auto x_in = 0;
            return lin(x_in);
        } else if ((x == 0) && (y == 0)){
            inv_arg("Line cannot be determined!");
        } else{
            auto slp = -y/x;
            auto y_in = y;
            return lin(slp, y_in);
        }
    }

    bool lin::operator==(const lin& other) const{
        if (is_vertical == other.is_vertical){
            if (is_vertical){
                return x_inter == other.x_inter;
            }
            return (slope == other.slope) && (y_inter == other.y_inter);
        }

        return false;
    }

    TypeandERR<double> lin::slp() const{
        if (is_vertical){
            inv_arg("Slope does not exist!!");
        }
        return slope;
    }

    TypeandERR<double> lin::y_int() const{
        if (is_vertical){
            inv_arg("Y intercept does not exist or is undefined!!");
        }
        return y_inter;
    }

    TypeandERR<double> lin::x_int() const{
        if (is_vertical){
            return x_inter;
        } else if (slope == 0){
            inv_arg("X intercept does not exist or is undefined!!");
        }
        return - y_inter / slope;
    }

    TypeandERR<point> lin::intersection(const lin& l) const{
        if (!is_vertical){
            if(l.slope == slope){
                range_err("Lines do not intersect since parallel!");
            }
            if (l.is_vertical){
                auto x = l.x_inter;
                auto y = slope * x + y_inter;
                return point(x,y);
            }
            auto x = (y_inter-l.y_inter)/(l.slope-slope);
            auto y = slope * x + y_inter;
            return point(x,y);
        } else if (l.is_vertical){
            range_err("Lines do not intersect since parallel!");
        }

        auto x = x_inter;
        auto y = l.slope * x + l.y_inter;
        return point(x,y);
    }

    TypeandERR<std::pair<point, point>> lin::intersection(const circle& c) const{
        if (dist_from_line(c.center, *this) > c.radius){
            range_err("Line does not intersect with the circle!");
        }
        if (is_vertical){
            auto x = x_inter;
            auto y1 = sqrt(sq(c.radius) - sq(x)) + c.center.y_coord;
            auto y2 = -sqrt(sq(c.radius) - sq(x)) + c.center.y_coord;
            return std::make_pair(point(x,y1), point(x,y2));
        }
        if (slope == 0){
            auto y = y_inter;
            auto x1 = sqrt(sq(c.radius) - sq(y)) + c.center.x_coord;
            auto x2 = -sqrt(sq(c.radius) - sq(y)) + c.center.x_coord;
            return std::make_pair(point(x1,y), point(x2,y));
        }
        auto a = slope;
        auto b = y_inter;
        auto r = c.radius;
        auto x1 = (-a*b+sqrt(sq(a*b) - (sq(a)-1) * (sq(b) - sq(r))))/(sq(a)-1) + c.center.x_coord;
        auto x2 = (-a*b-sqrt(sq(a*b) - (sq(a)-1) * (sq(b) - sq(r))))/(sq(a)-1) + c.center.x_coord;
        auto y1 = a * x1 + b + c.center.y_coord;
        auto y2 = a * x2 + b + c.center.y_coord;
        return std::make_pair(point(x1,y1), point(x2,y2));
    }

    TypeandERR<std::pair<point, point>> lin::intersection(const rectangle_par& c) const{
        std::vector<lineseg> l = {lineseg(c.bottom_left, point(c.bottom_left.x_coord + c.x_para,  c.bottom_left.y_coord)), 
                                lineseg(c.bottom_left, point(c.bottom_left.x_coord,  c.bottom_left.y_coord+ c.y_para)),
                                lineseg(point(c.bottom_left.x_coord,  c.bottom_left.y_coord+ c.y_para), point(c.bottom_left.x_coord + c.x_para,  c.bottom_left.y_coord+ c.y_para)),
                                lineseg(point(c.bottom_left.x_coord + c.x_para,  c.bottom_left.y_coord), point(c.bottom_left.x_coord + c.x_para,  c.bottom_left.y_coord+ c.y_para))
                                };
        std::pair<point, point> ans;
        uint8_t ans_filled = 0;
        for (size_t i = 0; i <= 3; i++){
            if (ans_filled == 2){
                return ans;
            }
            if (!intersection(l[i]).is_err()){
                if (ans.first == intersection(l[i])){
                    continue;
                }
                else if (ans_filled == 1){
                    ans.second = intersection(l[i]);
                } else{
                    ans.first = intersection(l[i]);
                }
                ans_filled++;
            }
            else if (lin(l[i]) == *this){
                range_err("The line is tangent to the rectangle");
            }
        }
        range_err("The line does not intersect the rectangle");
    }

    TypeandERR<std::pair<point, point>> lin::intersection(const shape& c) const{
        if (c.type){
            return intersection(c.cir);
        }
            return intersection(c.rect);
    }


    double lin::ang() const{
        if (is_vertical){
            return 90;
        }
        return angle(1,slope);
    }

    TypeandERR<double> lin::y_coord_at_x(const double& x_coord) const{
        if (is_vertical){
            range_err("The line is not a function");
        }
        return slope * x_coord + y_inter;
    }



    // rectangle_par member functions
    rectangle_par::rectangle_par() = default;
    rectangle_par::rectangle_par(const double& len, const double& wid) : bottom_left(point(0,0)), y_para(std::abs(len)), x_para(std::abs(wid)) {}
    rectangle_par::rectangle_par(const point& bl, const double& y_p_len, const double& x_p_len) : bottom_left(bl), y_para(std::abs(y_p_len)), x_para(std::abs(x_p_len)) {}
    rectangle_par::rectangle_par(const rectangle_par& other) : bottom_left(other.bottom_left), y_para(other.y_para), x_para(other.x_para) {}




    // circle member functions
    circle::circle() = default;
    circle::circle(const point& c, const double& r) : center(c), radius(r) {}
    circle::circle(const circle& other) : center(other.center), radius(other.radius) {}

    TypeandERR<double> circle::len_min_arc(const point& p1, const point& p2) const{
        if (!((p1.on_circle(*this)) || (p2.on_circle(*this)))){
            range_err("Point(s) are not on circle");
        }
        double angle = asin(__dist(p1,p2)/(2*radius));
        return radius * angle;
    }



    TypeandERR<std::size_t> min_ind(const std::vector<double>& nums) {
        if (nums.empty()) {
            inv_arg("The list of numbers cannot be empty");
        }

        return static_cast<std::size_t>(std::min_element(nums.begin(), nums.end()) - nums.begin());
    }

    TypeandERR<double> min_invec(const std::vector<double>& nums) {
        if (nums.empty()) {
            inv_arg("The list of numbers cannot be empty");
        }

        return *std::min_element(nums.begin(), nums.end());
    }

    TypeandERR<std::pair<double, double>> closest_point_dir(const point &p1, const std::vector<point> &other_pts) {
        if (other_pts.empty()) {
            inv_arg("Enter your points!");
        }

        std::vector<double> distances;
        distances.reserve(other_pts.size());
        for (std::size_t i = 0; i < other_pts.size(); ++i) {
            distances.push_back(__dist(p1, other_pts[i]));
        }

        const std::size_t closest_index = min_ind(distances);
        return std::make_pair(distances[closest_index],__angle_from_horizontal(p1, other_pts[closest_index]));
    }

    std::pair<double,double> direction(const point &p1, const point &p2) {
        return std::make_pair(__dist(p1, p2), __angle_from_horizontal(p1, p2));
    }


    std::pair<double, double> closest_point_inseg_dir(const point &p, const lineseg& l){
        // Find slope - intercept form
        TypeandERR<double> pslp = l.perp_slp();
        lin perp_line;
        if(pslp.is_err()){
            perp_line = lin(p.x_coord);
        }
        else{
            double y_i = p.y_coord - (pslp * p.x_coord);
            perp_line = lin(pslp, y_i);
        }
        TypeandERR<point> intersection_point = l.intersection(perp_line);
        if(intersection_point.is_err()){
            return closest_point_dir(p, {l.point1, l.point2});
        }
        return direction(p, intersection_point);
    }

    double distance_to_lineseg(const point& p, const lineseg& l){
        if (p.on_seg(l)){
            return 0;
        }
        else{
            auto perp_sl = l.perp_slp();
            lin perp_line;
            if(perp_sl.is_err()){
                perp_line = lin(p.x_coord);
            }
            else{
                double y_i = p.y_coord - (perp_sl * p.x_coord);
                perp_line = lin(perp_sl, y_i);
            }
            TypeandERR<point> intersection_point = l.intersection(perp_line);
            if(intersection_point.is_err()){
                return std::min(__dist(p, l.point1), __dist(p, l.point2));
            }
            return __dist(p, intersection_point);
        }
    }

    double dist_from_line(const point& p, const lin& l){
        if (l.is_vertical){
            return std::abs(p.x_coord - l.x_inter);
        }
        if (l.slope == 0){
            return std::abs(p.y_coord - l.y_inter);
        }
        double perp_sl = -1/l.slope;
        double y_i = p.y_coord - (perp_sl * p.x_coord);
        lin perp_line(perp_sl, y_i);
        TypeandERR<point> intersection_point = l.intersection(perp_line);
        if(intersection_point.is_err()){
            return __dist(p, intersection_point);
        }
        return __dist(p, intersection_point);
    }


    double dist_from_rect(const point& p, const rectangle_par& rect){
        if (p.in_rect(rect)){
            return 0;
        }
        point p1 = rect.bottom_left;
        point p2 = point(rect.bottom_left.x_coord + rect.x_para, rect.bottom_left.y_coord);
        point p3 = point(rect.bottom_left.x_coord, rect.bottom_left.y_coord + rect.y_para);
        point p4 = point(rect.bottom_left.x_coord + rect.x_para, rect.bottom_left.y_coord + rect.y_para);
        std::vector<lineseg> edges = {lineseg(p1, p2), lineseg(p1, p3), lineseg(p2, p4), lineseg(p3, p4)};
        std::vector<double> distances;
        for (const auto& edge : edges) {
            distances.push_back(distance_to_lineseg(p, edge));
        }
        return *std::min_element(distances.begin(), distances.end());
    }

    double dist_from_circle(const point& p, const circle& c){
        double distance_to_center = __dist(p, c.center);
        if (distance_to_center <= c.radius) {
            return 0;
        } else {
            return distance_to_center - c.radius;
        }
    }

    double line_len_incircle(const lin& l, const circle& c) {
        auto intersection_points = l.intersection(c);
        if (intersection_points.is_err()) {
            return 0; // No intersection, so length is 0
        }
        const auto& p1 = intersection_points.get_val().first;
        const auto& p2 = intersection_points.get_val().second;
        return __dist(p1, p2);
    }

    std::vector<size_t> intersects_shape(const point& start, const std::pair<double, double>& dir, const std::vector<shape>& v){
        lineseg l;
        point other_point(start.x_coord + dir.first * cos(dir.second/180 * M_PI), start.y_coord + dir.first * sin(dir.second/180 * M_PI));
        l = lineseg(start, other_point);
        std::vector<size_t> ints;
        for (size_t i = 0; i < v.size(); i++){
            if (l.intersects(v[i])){
                ints.push_back(i);
            }
        }
        lin line(l);
        std::vector<double> dists;
        for (auto i : ints){
            auto a = line.intersection(v[i]);
            std::vector<point> avals = {a->first ,a->second};
            dists.push_back(std::pair<double,double>(closest_point_dir(start, avals)).first);
        }
        const auto a = dists;
        std::stable_sort(dists.begin(), dists.end());
        std::vector<size_t> sorted_vec;
        for (auto num : dists){
            auto it = std::find(a.begin(), a.end(), num);
            long long index = std::distance(a.begin(), it);
            sorted_vec.push_back(ints[index]);
        }
        return sorted_vec;
    }
}

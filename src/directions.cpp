#include "directions.h"
using namespace vex;
using namespace geo;

Directions::Directions() = default;
Directions::Directions(const Directions& d){
    is_straight = d.is_straight;
    if (is_straight){
        dist = d.dist;
    } else {
        Shape = d.Shape;
    }
}

void turn(smartdrive& d, inertial& inertial_sensor, const double& deg) {
    inertial_sensor.resetRotation();
    auto new_deg = deg;
    mod_eq(new_deg, 360);
    new_deg -= 180;
    double prop_const = fabs(new_deg/90);
    double err = deg;
    while (fabs(err) > fabs(prop_const/2)){
        err = deg - inertial_sensor.rotation(degrees);
        double omega = err * prop_const;
        d.setTurnVelocity(omega, percent);
        if (omega > 100){
            d.setTurnVelocity(100, percent);
        }
        if (new_deg < 0){
            d.turn(right);
        }
        else {
            d.turn(left);
        }
    }
}


DirectionLedger::DirectionLedger(const point& starting_point, const std::pair<double, double>& dir){
    general_angle_heading = dir.second;
    lineseg l;
    point other_point(starting_point.x_coord + dir.first * cos(dir.second/180 * M_PI), starting_point.y_coord + dir.first * sin(dir.second/180 * M_PI));
    l = lineseg(starting_point, other_point);
    lin li(l);
    auto a = intersects_shape(starting_point, dir, All);
    std::vector<double> dists;
    std::vector<shape> shapes;
    size_t i = 0;
    point c_point = starting_point;
    for (auto ind : a){
        if(i == a.size() - 1){
            dists.push_back(__dist(c_point, other_point));
            break;
        }
        auto shap = All[ind];
        shapes.push_back(shap);
        auto inter = li.intersection(shap);
        std::vector<point> vec = {inter->first, inter->second};
        auto direc = closest_point_dir(c_point, vec);
        dists.push_back(direc->first);
        point closer(starting_point.x_coord+(direc->first)*cos(direc->second),starting_point.y_coord+(direc->first)*sin(direc->second));
        c_point = (closer == inter->second) ? inter->first : inter->second;
        i++;
    }
    for (size_t i = 0; i < 2 * dists.size()-1; i++){
        if (i % 2 == 1){
            auto shap = shapes[(i-1)/2];
            if (shap.type){
                Directions cir;
                cir.is_straight = false;
                cir.Shape = shap;
                dirs.push_back(cir);
                continue;
            }
            Directions rect;
            rect.is_straight = false;
            rect.Shape = shap;
            dirs.push_back(rect);
            continue;
        }
        Directions s;
        s.is_straight = true;
        s.dist = dists[i/2];
        dirs.push_back(s);
    }
    
}

turnType operator!(vex::turnType T){
    return (T == left) ? right : left;
}

void go_circle(smartdrive& d, inertial& ine, const circle& c, std::pair<double,double>& dir, point& pos, double angle_heading){
    double angle = 360.0/(2*circle_prec);
    double seg_len = 2*c.radius*sin(_PI/(2*circle_prec)) * 24;
    lin lin_dir = lin(tan(dir.second), pos.y_coord - tan(dir.second) * pos.x_coord);
    if (lin_dir.intersection(c).is_err()){
        turn(d, ine, dir.second - angle_heading);
        d.setDriveVelocity(100, percent);
        d.driveFor(forward, dir.first * 24, distanceUnits::in);
        d.stop();
        pos = point(pos.x_coord + dir.first*std::cos(dir.second), pos.y_coord + dir.first*std::sin(dir.second));
        return;
    }
    std::vector<point> a = {lin_dir.intersection(c)->first,lin_dir.intersection(c)->second};
    turn(d, ine, dir.second - angle_heading);
    auto k = a[min_ind({__dist(pos, lin_dir.intersection(c)->first), __dist(pos,lin_dir.intersection(c)->second)})];
    auto npos = closest_point_dir(pos, a);
    auto dis = npos->first;
    d.setDriveVelocity(60, percent);
    d.driveFor(forward, dis * 24, distanceUnits::in);
    pos = k;
    auto new_pos = (lin_dir.intersection(c)->first == pos) ? lin_dir.intersection(c)->second : lin_dir.intersection(c)->first;
    auto starting_turn_angle = (180 + angle)/2;
    turn(d, ine, starting_turn_angle);
    auto curr_pos = new_pos;
    for (int i = 0; i < floor(6 * c.len_min_arc(pos, new_pos)/( _PI * c.radius)) - 1; i++){
        d.driveFor(forward, seg_len, distanceUnits::in);
        turn(d, ine, angle);
    }
    d.driveFor(forward ,seg_len, distanceUnits::in);
    turn(d, ine, starting_turn_angle);


    dir = std::make_pair(dir.first - line_len_incircle(lin_dir, c), dir.second);
    pos = new_pos;
    angle_heading = dir.second;

}


void go_dir(smartdrive& d, inertial& ine, point& pos, std::pair<double,double>& dir, double angle_heading){
    // Set velocity
    d.setDriveVelocity(60, pct);
    turn(d, ine, dir.second - angle_heading);
    DirectionLedger path(pos, dir);
    for (auto direction : path.dirs){
        if (direction.is_straight){
            d.setDriveVelocity(60, pct);
            d.driveFor(direction.dist * 24, distanceUnits::in);
            pos = point(pos.x_coord + direction.dist * cos(dir.second), pos.y_coord + direction.dist * sin(dir.second));
            continue;
        }
        else if (direction.Shape.type){
            auto circ = direction.Shape.cir;
            go_circle(d,ine,circ,dir,pos,dir.second);
            // go_circle already modifies pos
            continue;
        }
    }
    angle_heading = dir.second;
}

void hit_Toggle(smartdrive& d, inertial& iner, point& pos, const bool& y_axis, bool& blue){
    uint8_t num = (blue)+1;
    for (uint8_t i = 1; i <= num; i++){
        d.setDriveVelocity(100, pct);
        while(iner.acceleration(xaxis) < 1.2){
            d.drive(reverse);
        } 
        d.stop(hold);

        point curr_pos(3, pos.y_coord);
        auto dist = 3 - pos.x_coord;
        if (y_axis){
            curr_pos = point(pos.x_coord, 3);
            dist = 3 - pos.y_coord;
        }
        d.setDriveVelocity(60, pct);
        d.driveFor(forward, dist, distanceUnits::in);
    }
    
}
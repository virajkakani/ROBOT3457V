#include "Err_Types.h"
#include "math_funcs.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

const double EPSILON = 1e-9;
int groups_run = 0;
int groups_failed = 0;

#define CHECK(condition)                                                     \
    do {                                                                     \
        if (!(condition)) {                                                  \
            throw std::runtime_error("check failed: " #condition);          \
        }                                                                    \
    } while (false)

bool close_to(double actual, double expected) {
    return std::fabs(actual - expected) <= EPSILON;
}

template <typename T>
void check_value(TypeandERR<T> result, const T& expected) {
    CHECK(!result.is_err());
    CHECK(*result == expected);
}

void check_double(TypeandERR<double> result, double expected) {
    CHECK(!result.is_err());
    CHECK(close_to(*result, expected));
}

template <typename T>
void check_error(TypeandERR<T> result,
                 ERRTYPE expected_type,
                 const std::string& expected_message) {
    CHECK(result.is_err());
    const ERROR error = result.get_err();
    CHECK(error.err == expected_type);
    CHECK(error.message == expected_message);
}

void run_test(const char* name, void (*test)()) {
    ++groups_run;
    std::cout << "[ RUN      ] " << name << std::endl;
    try {
        test();
        std::cout << "[       OK ] " << name << std::endl;
    } catch (const std::exception& error) {
        ++groups_failed;
        std::cout << "[  FAILED  ] " << name << ": " << error.what()
                  << std::endl;
    } catch (...) {
        ++groups_failed;
        std::cout << "[  FAILED  ] " << name << ": unknown failure"
                  << std::endl;
    }
}

// Err_Types tests

void test_error_constructors_and_comparisons() {
    const ERROR ok;
    CHECK(ok.err == ERRTYPE::OK);
    CHECK(ok.message.empty());

    const ERROR first(ERRTYPE::IA, "bad argument");
    const ERROR same_type(ERRTYPE::IA, "different message");
    const ERROR different(ERRTYPE::RE, "range");
    CHECK(!(first != same_type));
    CHECK(first != different);
    CHECK(!(first != ERRTYPE::IA));
    CHECK(first != ERRTYPE::RE);
}

void test_error_factories() {
    const ERROR errors[] = {
        INVALID_ARGUMENT("ia"), OUT_OF_RANGE("oor"), DOMAIN_ERROR("de"),
        OVERFLOW_ERROR("oe"), UNDERFLOW_ERROR("ue"), RANGE_ERROR("re"),
        SYSTEM_ERROR("se")};
    const ERRTYPE types[] = {
        ERRTYPE::IA, ERRTYPE::OOR, ERRTYPE::DE, ERRTYPE::OE,
        ERRTYPE::UE, ERRTYPE::RE, ERRTYPE::SE};
    const char* messages[] = {"ia", "oor", "de", "oe", "ue", "re", "se"};

    for (std::size_t i = 0; i < 7; ++i) {
        CHECK(errors[i].err == types[i]);
        CHECK(errors[i].message == messages[i]);
    }
}

void test_error_display() {
    std::ostringstream output;
    std::streambuf* original = std::cout.rdbuf(output.rdbuf());
    ERROR(ERRTYPE::IA, "bad argument").show_err();
    std::cout.rdbuf(original);
    CHECK(output.str() == "INVALID_ARGUMENT: bad argument\n");
}

void test_type_and_error_value() {
    TypeandERR<int> result(42);
    CHECK(!result.is_err());
    CHECK(*result == 42);
    CHECK(static_cast<int>(result) == 42);

    TypeandERR<std::pair<int, int> > pair_result(std::make_pair(3, 4));
    CHECK(pair_result->first == 3);
    CHECK(pair_result->second == 4);

    TypeandERR<int> copied(result);
    CHECK(!copied.is_err());
    CHECK(*copied == 42);
}

void test_type_and_error_failure() {
    TypeandERR<int> result(INVALID_ARGUMENT("bad value"));
    check_error(result, ERRTYPE::IA, "bad value");

    TypeandERR<void> success;
    CHECK(!success.is_err());
    TypeandERR<void> failure(RANGE_ERROR("no result"));
    check_error(failure, ERRTYPE::RE, "no result");
}

// math_funcs tests

void test_scalar_math() {
    CHECK(close_to(sq(0.0), 0.0));
    CHECK(close_to(sq(-4.0), 16.0));
    CHECK(close_to(sq(0.5), 0.25));
    CHECK(close_to(magn(0.0, 0.0), 0.0));
    CHECK(close_to(magn(3.0, 4.0), 5.0));
    CHECK(close_to(magn(-5.0, 12.0), 13.0));
    CHECK(close_to(angle(1.0, 0.0), 0.0));
    CHECK(close_to(angle(0.0, 1.0), 90.0));
    CHECK(close_to(angle(-1.0, 0.0), 180.0));
    CHECK(close_to(angle(0.0, -1.0), -90.0));
}

void test_modulo() {
    double positive = 370.0;
    TypeandERR<void> success = mod_eq(positive, 360);
    CHECK(!success.is_err());
    CHECK(close_to(positive, 10.0));

    double negative = -10.5;
    CHECK(!mod_eq(negative, 4).is_err());
    CHECK(close_to(negative, -2.5));

    double unchanged = 12.0;
    check_error(mod_eq(unchanged, 0), ERRTYPE::IA,
                "The quotient cannot be zero");
    CHECK(close_to(unchanged, 12.0));
}

void test_points_and_rectangles() {
    const point origin;
    CHECK(close_to(origin.x_coord, 0.0));
    CHECK(close_to(origin.y_coord, 0.0));
    check_value(origin == point(0.0, 0.0), true);
    check_value(origin == point(0.0, 1.0), false);
    CHECK(close_to(__dist(origin, point(3.0, 4.0)), 5.0));
    CHECK(close_to(__angle_from_horizontal(origin, point(1.0, 1.0)), 45.0));

    const rectangle_par rectangle(point(1.0, 2.0), 4.0, 6.0);
    CHECK(point(1.0, 2.0).in_rect(rectangle));
    CHECK(point(7.0, 6.0).in_rect(rectangle));
    CHECK(point(3.0, 4.0).in_rect(rectangle));
    CHECK(!point(7.1, 4.0).in_rect(rectangle));

    const rectangle_par copy(rectangle);
    CHECK(close_to(copy.bottom_left.x_coord, 1.0));
    CHECK(close_to(copy.y_para, 4.0));
    CHECK(close_to(copy.x_para, 6.0));
}

void test_segment_properties() {
    const lineseg segment(point(1.0, 3.0), point(4.0, 9.0));
    check_double(segment.slp(), 2.0);
    check_double(segment.y_int(), 1.0);
    check_double(segment.len(), std::sqrt(45.0));
    check_double(segment.perp_slp(), -0.5);
    check_value(segment.on_seg(point(2.0, 5.0)), true);
    check_value(segment.on_seg(point(5.0, 11.0)), false);
    check_value(segment.on_seg(point(2.0, 6.0)), false);

    const lineseg reversed(point(4.0, 9.0), point(1.0, 3.0));
    check_value(segment == reversed, true);
}

void test_vertical_and_horizontal_segments() {
    const lineseg vertical(point(2.0, 0.0), point(2.0, 5.0));
    check_error(vertical.slp(), ERRTYPE::IA,
                "Lines which are vertical have undefined slope");
    check_value(vertical.on_seg(point(2.0, 3.0)), true);

    const lineseg horizontal(point(0.0, 3.0), point(5.0, 3.0));
    check_double(horizontal.slp(), 0.0);
    check_error(horizontal.perp_slp(), ERRTYPE::IA,
                "The perpendicular slope is undefined");
    check_value(horizontal.on_seg(point(3.0, 3.0)), true);
}

void test_lines() {
    const lin diagonal(2.0, 1.0);
    check_double(diagonal.slp(), 2.0);
    check_double(diagonal.y_int(), 1.0);
    check_double(diagonal.x_int(), -0.5);

    const lin vertical(4.0);
    check_double(vertical.x_int(), 4.0);
    check_error(vertical.slp(), ERRTYPE::IA, "Slope does not exist!!");
    check_error(vertical.y_int(), ERRTYPE::IA,
                "Y intercept does not exist or is undefined!!");

    const lin horizontal(0.0, 3.0);
    check_error(horizontal.x_int(), ERRTYPE::IA,
                "X intercept does not exist or is undefined!!");

    TypeandERR<lin> invalid = lin::lin_x_y(0.0, 0.0);
    check_error(invalid, ERRTYPE::IA, "Line cannot be determined!");
}

void test_line_intersections() {
    const lin rising(1.0, 0.0);
    const lin falling(-1.0, 2.0);
    TypeandERR<point> crossing = rising.intersection(falling);
    CHECK(!crossing.is_err());
    CHECK(close_to(crossing->x_coord, 1.0));
    CHECK(close_to(crossing->y_coord, 1.0));

    const lin vertical(1.0);
    TypeandERR<point> vertical_crossing = vertical.intersection(rising);
    CHECK(!vertical_crossing.is_err());
    CHECK(close_to(vertical_crossing->x_coord, 1.0));
    CHECK(close_to(vertical_crossing->y_coord, 1.0));

    check_error(rising.intersection(lin(1.0, 5.0)), ERRTYPE::RE,
                "Lines do not intersect since parallel!");
}

void test_segment_intersections() {
    const lineseg first(point(0.0, 0.0), point(4.0, 4.0));
    const lineseg second(point(0.0, 4.0), point(4.0, 0.0));
    TypeandERR<point> crossing = first.intersection(second);
    CHECK(!crossing.is_err());
    CHECK(close_to(crossing->x_coord, 2.0));
    CHECK(close_to(crossing->y_coord, 2.0));

    const lineseg outside(point(5.0, 0.0), point(5.0, 4.0));
    check_error(first.intersection(outside), ERRTYPE::RE,
                "They don't intersect since the segments never meet");
}

void test_minimum_functions() {
    const std::vector<double> values{8.0, -2.0, 4.0, -2.0};
    check_value(min_ind(values), static_cast<std::size_t>(1));
    check_double(min_invec(values), -2.0);
    check_value(min_ind(std::vector<double>(1, 7.5)),
                static_cast<std::size_t>(0));

    const std::vector<double> empty;
    check_error(min_ind(empty), ERRTYPE::IA,
                "The list of numbers cannot be empty");
    check_error(min_invec(empty), ERRTYPE::IA,
                "The list of numbers cannot be empty");
}

void test_closest_point_functions() {
    const point origin(0.0, 0.0);
    const std::vector<point> points{
        point(10.0, 0.0), point(3.0, 4.0), point(-8.0, 0.0)};
    TypeandERR<std::pair<double, double> > closest =
        closest_point_dir(origin, points);
    CHECK(!closest.is_err());
    CHECK(close_to(closest->first, 5.0));
    CHECK(close_to(closest->second, 53.13010235415598));

    check_error(closest_point_dir(origin, std::vector<point>()), ERRTYPE::IA,
                "Enter your points!");

    const lineseg segment(point(0.0, 0.0), point(10.0, 0.0));
    const std::pair<double, double> direction =
        closest_point_inseg_dir(point(5.0, 3.0), segment);
    CHECK(close_to(direction.first, 3.0));
    CHECK(close_to(direction.second, -90.0));
    CHECK(close_to(distance_to_lineseg(point(5.0, 3.0), segment), 3.0));
    CHECK(close_to(distance_to_lineseg(point(5.0, 0.0), segment), 0.0));
}

}  // namespace

int main() {
    std::cout << "Running all library tests..." << std::endl;

    run_test("ERROR constructors and comparisons",
             test_error_constructors_and_comparisons);
    run_test("ERROR factory functions", test_error_factories);
    run_test("ERROR display", test_error_display);
    run_test("TypeandERR values", test_type_and_error_value);
    run_test("TypeandERR errors", test_type_and_error_failure);
    run_test("scalar math", test_scalar_math);
    run_test("modulo", test_modulo);
    run_test("points and rectangles", test_points_and_rectangles);
    run_test("segment properties", test_segment_properties);
    run_test("vertical and horizontal segments",
             test_vertical_and_horizontal_segments);
    run_test("lines", test_lines);
    run_test("line intersections", test_line_intersections);
    run_test("segment intersections", test_segment_intersections);
    run_test("minimum functions", test_minimum_functions);
    run_test("closest-point functions", test_closest_point_functions);

    std::cout << groups_run - groups_failed << "/" << groups_run
              << " test groups passed." << std::endl;
    return groups_failed == 0 ? 0 : 1;
}

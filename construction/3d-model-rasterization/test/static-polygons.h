#include <string>
#include <vector>
#include "Point.h"
#include "LineSegment.h"
#include "Rectangle.h"

extern const std::vector<LineSegment2D> six_by_six_square;
extern const std::vector<LineSegment2D> three_by_three_square;
extern const std::vector<LineSegment2D> big_polygon_1;
extern const std::vector<LineSegment2D> inner_polygon_1;
extern const std::vector<LineSegment2D> six_by_six_square_with_points_on_y_axis;
extern const std::vector<LineSegment2D> test_6_polygon_1;
extern const std::vector<LineSegment2D> test_6_polygon_2;
extern const std::vector<LineSegment2D> test_6_polygon_3;
extern const std::vector<LineSegment2D> test_7_polygon_1;

extern const std::vector<LineSegment2D> poly_union_1_1;
extern const std::vector<LineSegment2D> poly_union_1_2;

std::vector<LineSegment2D> makePolygonFromPoints(const std::vector<Point2D> points);

std::vector<LineSegment2D> makeSquare(const Rectangle &rect);

void printPolygonBounds(const std::vector<std::vector<LineSegment2D>> &polygons);

std::string polygonsToString(const std::vector<std::vector<LineSegment2D>> &polygons, bool shortened = false);

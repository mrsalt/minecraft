#include "static-polygons.h"
#include "string_format.h"
#include <iostream>

using namespace std;

const vector<LineSegment2D> six_by_six_square = makeSquare({{-3, -3}, {3, 3}});
const vector<LineSegment2D> three_by_three_square = makeSquare({{-1.5, -1.5}, {1.5, 1.5}});
const vector<LineSegment2D> big_polygon_1 = makePolygonFromPoints({{68, 284}, {109, 147}, {212, 66}, {355, 101}, {289, 255}, {257, 392}, {386, 335}, {443, 223}, {510, 125}, {625, 32}, {800, 74}, {820, 248}, {686, 372}, {470, 494}, {316, 556}, {118, 448}});
const vector<LineSegment2D> inner_polygon_1 = makePolygonFromPoints({{553, 262}, {654, 200}, {683, 287}, {572, 380}, {502, 373}});
const vector<LineSegment2D> six_by_six_square_with_points_on_y_axis = makePolygonFromPoints({{-3, -3}, {-3, 0}, {-3, 3}, {3, 3}, {3, 0}, {3, -3}});
const vector<LineSegment2D> test_6_polygon_1 = makePolygonFromPoints({{83, 97}, {132, 20}, {185, 94}, {133, 156}});
const vector<LineSegment2D> test_6_polygon_2 = makePolygonFromPoints({{42, 182}, {68, 139}, {103, 181}, {75, 223}});
const vector<LineSegment2D> test_6_polygon_3 = makePolygonFromPoints({{164, 189}, {191, 143}, {227, 187}, {199, 228}});
const vector<LineSegment2D> test_7_polygon_1 = makePolygonFromPoints({{118, 172}, {74, 114}, {132, 20}, {194, 108}, {149, 172}, {133, 156}});

const vector<LineSegment2D> poly_union_1_1 = makePolygonFromPoints({{86, 207}, {144, 99}, {244, 154}, {182, 247}});
const vector<LineSegment2D> poly_union_1_2 = makePolygonFromPoints({{368, 161}, {282, 241}, {199, 174}, {271, 94}});

vector<LineSegment2D> makePolygonFromPoints(const vector<Point2D> points)
{
    vector<LineSegment2D> polygon;
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        polygon.push_back({points[i], points[i + 1]});
    }
    polygon.push_back({points.back(), points.front()});
    return polygon;
}

vector<LineSegment2D> makeSquare(const Rectangle &rect)
{
    return makePolygonFromPoints({{rect.min.x, rect.min.y},
                                  {rect.min.x, rect.max.y},
                                  {rect.max.y, rect.max.y},
                                  {rect.max.y, rect.min.y}});
}

void printPolygonBounds(const vector<vector<LineSegment2D>> &polygons)
{
    int i = 0;
    for (auto &shape : polygons)
    {
        Point2D min = shape.front().first;
        Point2D max = shape.front().first;
        for (auto &segment : shape)
        {
            if (segment.first.x < min.x)
                min.x = segment.first.x;
            else if (segment.first.x > max.x)
                max.x = segment.first.x;
            if (segment.first.y < min.y)
                min.y = segment.first.y;
            else if (segment.first.y > max.y)
                max.y = segment.first.y;
        }
        cout << "Polygon #" << i << ", Segments: " << shape.size() << ", Bounds: " << min << " to " << max << " (" << max.x - min.x << " x " << max.y - min.y << ")" << endl;
        cout << "    ";
        bool first = true;
        for (auto &segment : shape)
        {
            if (!first)
                cout << ", ";
            cout << segment.first;
            first = false;
        }
        cout << endl;
        i++;
    }
}

string polygonsToString(const vector<vector<LineSegment2D>> &polygons, bool shortened)
{
    string ret;
    int c = 0;
    for (auto &poly : polygons)
    {
        ret += "\n";
        ret += format("Polygon %d containing %d segments%s\n", c++, poly.size(), shortened ? "" : ":");
        if (!shortened)
        {
            for (size_t i = 0; i < poly.size(); i++)
            {
                auto &seg = poly[i];
                ret += format("  %zd: ", i) + seg.first.toString() + "-" + seg.second.toString() + "\n";
            }
        }
    }
    return ret;
}
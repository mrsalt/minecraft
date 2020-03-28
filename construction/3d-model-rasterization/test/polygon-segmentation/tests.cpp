#include "gtest/gtest.h"
#include "PolygonSegmentation.h"
#include "cairo-util.h"

using namespace std;

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

TEST(PolygonSegmentation, Test1)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makeSquare({{-3, -3}, {3, 3}}));
    LineSegment2D slice1{{-5, 0}, {5, 0}};
    LineSegment2D slice2{{0, -5}, {0, 5}};

    drawPolygonsToFile("Test1-0.svg", polygons);
    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test1-1.svg", polygons);
    slicePolygons(slice2, polygons);
    drawPolygonsToFile("Test1-2.svg", polygons);
}

TEST(PolygonSegmentation, Test2)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makeSquare({{-3, -3}, {3, 3}}));
    polygons.push_back(makeSquare({{-1.5, -1.5}, {1.5, 1.5}}));
    LineSegment2D slice1{{-5, 0}, {5, 0}};
    LineSegment2D slice2{{0, -5}, {0, 5}};

    drawPolygonsToFile("Test2-0.svg", polygons);
    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test2-1.svg", polygons);
    slicePolygons(slice2, polygons);
    drawPolygonsToFile("Test2-2.svg", polygons);
}

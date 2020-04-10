#include "gtest/gtest.h"
#include "PolygonSegmentation.h"
#include "cairo-util.h"
#include "string_format.h"

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

void printPolygonBounds(const vector<vector<LineSegment2D>> & polygons)
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
        for (auto& segment : shape)
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
TEST(PolygonSegmentation, Test3)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makePolygonFromPoints({ {68, 284}, { 109,147 }, { 212,66 }, { 355,101 }, { 289,255 }, { 257,392 }, { 386,335 }, { 443,223 }, { 510,125 }, { 625,32 }, { 800,74 }, { 820,248 }, { 686,372 }, { 470,494 }, { 316,556 }, { 118,448 } }));
    polygons.push_back(makePolygonFromPoints({ { 553,262 }, { 654,200 }, { 683,287 }, { 572,380 }, { 502,373 } }));
    drawPolygonsToFile("Test3-0.svg", polygons);

    for (double y = 50; y < 600; y += 50)
    {
        LineSegment2D slice1{ {0, y}, {900, y} };
        slicePolygons(slice1, polygons);
        drawPolygonsToFile(format("Test3-y-%d.svg", (int)y).c_str(), polygons);
        //cout << "After slice " << slice1 << endl;
        //printPolygonBounds(polygons);
        //cout << endl;
    }

    for (double x = 50; x < 1000; x += 50)
    {
        LineSegment2D slice1{ {x, 0}, {x, 800} };
        slicePolygons(slice1, polygons);
        drawPolygonsToFile(format("Test3-x-%d.svg", (int)x).c_str(), polygons);
        //cout << "After slice " << slice1 << endl;
        //printPolygonBounds(polygons);
        //cout << endl;
    }
}

TEST(PolygonSegmentation, Test4)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makePolygonFromPoints({{-3, -3}, {-3, 0}, {-3, 3}, {3, 3}, {3, 0}, {3, -3}}));

    LineSegment2D slice1{{-5, 0}, {5, 0}};

    drawPolygonsToFile("Test4-0.svg", polygons);
    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test4-1.svg", polygons);
}
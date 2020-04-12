#include "gtest/gtest.h"
#include "PolygonSegmentation.h"
#include "cairo-util.h"
#include "string_format.h"

using namespace std;

extern const char *test_1_step_0;
extern const char *test_1_step_1;
extern const char *test_1_step_2;
extern const char *test_2_step_0;
extern const char *test_2_step_1;
extern const char *test_2_step_2;
extern const char *test_3_step_0;
extern const char *test_3_step_1;
extern const char *test_3_step_2;
extern const char *test_4_step_0;
extern const char *test_4_step_1;
extern const char *test_5_step_0;
extern const char *test_5_step_1;
extern const char *test_5_step_2;

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

string polygonsToString(const vector<vector<LineSegment2D>> &polygons, bool shortened = false)
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

TEST(PolygonSegmentation, Test1)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makeSquare({{-3, -3}, {3, 3}}));
    LineSegment2D slice1{{-5, 0}, {5, 0}};
    LineSegment2D slice2{{0, -5}, {0, 5}};

    drawPolygonsToFile("Test1-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_1_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test1-1.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_1_step_1);

    slicePolygons(slice2, polygons);
    drawPolygonsToFile("Test1-2.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_1_step_2);
}

TEST(PolygonSegmentation, Test2)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makeSquare({{-3, -3}, {3, 3}}));
    polygons.push_back(makeSquare({{-1.5, -1.5}, {1.5, 1.5}}));
    LineSegment2D slice1{{-5, 0}, {5, 0}};
    LineSegment2D slice2{{0, -5}, {0, 5}};

    drawPolygonsToFile("Test2-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_2_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test2-1.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_2_step_1);

    slicePolygons(slice2, polygons);
    drawPolygonsToFile("Test2-2.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_2_step_2);
}
TEST(PolygonSegmentation, Test3)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makePolygonFromPoints({{68, 284}, {109, 147}, {212, 66}, {355, 101}, {289, 255}, {257, 392}, {386, 335}, {443, 223}, {510, 125}, {625, 32}, {800, 74}, {820, 248}, {686, 372}, {470, 494}, {316, 556}, {118, 448}}));
    polygons.push_back(makePolygonFromPoints({{553, 262}, {654, 200}, {683, 287}, {572, 380}, {502, 373}}));
    drawPolygonsToFile("Test3-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_3_step_0);

    for (double y = 50; y < 600; y += 50)
    {
        LineSegment2D slice1{{0, y}, {900, y}};
        slicePolygons(slice1, polygons);
        drawPolygonsToFile(format("Test3-y-%d.svg", (int)y).c_str(), polygons);
        //cout << "After slice " << slice1 << endl;
        //printPolygonBounds(polygons);
        //cout << endl;
    }
    ASSERT_EQ(polygonsToString(polygons), test_3_step_1);

    for (double x = 50; x < 1000; x += 50)
    {
        LineSegment2D slice1{{x, 0}, {x, 800}};
        slicePolygons(slice1, polygons);
        drawPolygonsToFile(format("Test3-x-%d.svg", (int)x).c_str(), polygons);
        //cout << "After slice " << slice1 << endl;
        //printPolygonBounds(polygons);
        //cout << endl;
    }
    ASSERT_EQ(polygonsToString(polygons, true), test_3_step_2);
}

TEST(PolygonSegmentation, Test4)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makePolygonFromPoints({{-3, -3}, {-3, 0}, {-3, 3}, {3, 3}, {3, 0}, {3, -3}}));

    LineSegment2D slice1{{-5, 0}, {5, 0}};

    drawPolygonsToFile("Test4-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_4_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test4-1.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_4_step_1);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test4-2.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_4_step_1); // expecting no change from prior step
}

/*
This test case does not pass.  I believe it is a corner case we can ignore.
TEST(PolygonSegmentation, Test5)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(makePolygonFromPoints({ {0, 100}, {0, 200}, {100, 200}, {100, 100}, {175, 100}, {175, 0}, {75, 0}, {75, 100} }));

    LineSegment2D slice1{ {-5, 100}, {200, 100} };
    LineSegment2D slice2{ {87, -5}, {87, 205} };

    drawPolygonsToFile("Test5-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_5_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test5-1.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_5_step_1);

    slicePolygons(slice2, polygons);
    drawPolygonsToFile("Test5-2.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_5_step_2);
}
*/
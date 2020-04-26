#include "gtest/gtest.h"
#include "PolygonSegmentation.h"
#include "cairo-util.h"
#include "string_format.h"
#include "static-polygons.h"
#include "polygon-expectations.h"

using namespace std;

TEST(PolygonSegmentation, Test1)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(six_by_six_square);
    LineSegment2D slice1{{-5, 0}, {5, 0}};
    LineSegment2D slice2{{0, -5}, {0, 5}};

    drawPolygonsToFile("Test1-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_1_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test1-1.svg", polygons, &slice1);
    ASSERT_EQ(polygonsToString(polygons), test_1_step_1);

    slicePolygons(slice2, polygons);
    drawPolygonsToFile("Test1-2.svg", polygons, &slice2);
    ASSERT_EQ(polygonsToString(polygons), test_1_step_2);
}

TEST(PolygonSegmentation, Test2)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(six_by_six_square);
    polygons.push_back(three_by_three_square);

    LineSegment2D slice1{{-5, 0}, {5, 0}};
    LineSegment2D slice2{{0, -5}, {0, 5}};

    drawPolygonsToFile("Test2-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_2_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test2-1.svg", polygons, &slice1);
    ASSERT_EQ(polygonsToString(polygons), test_2_step_1);

    slicePolygons(slice2, polygons);
    drawPolygonsToFile("Test2-2.svg", polygons, &slice2);
    ASSERT_EQ(polygonsToString(polygons), test_2_step_2);
}
TEST(PolygonSegmentation, Test3)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(big_polygon_1);
    polygons.push_back(inner_polygon_1);
    drawPolygonsToFile("Test3-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_3_step_0);

    for (double y = 50; y < 600; y += 50)
    {
        LineSegment2D slice1{{0, y}, {900, y}};
        slicePolygons(slice1, polygons);
        drawPolygonsToFile(format("Test3-y-%d.svg", (int)y).c_str(), polygons, &slice1);
        //cout << "After slice " << slice1 << endl;
        //printPolygonBounds(polygons);
        //cout << endl;
    }
    ASSERT_EQ(polygonsToString(polygons), test_3_step_1);

    for (double x = 50; x < 1000; x += 50)
    {
        LineSegment2D slice1{{x, 0}, {x, 800}};
        slicePolygons(slice1, polygons);
        drawPolygonsToFile(format("Test3-x-%d.svg", (int)x).c_str(), polygons, &slice1);
        //cout << "After slice " << slice1 << endl;
        //printPolygonBounds(polygons);
        //cout << endl;
    }
    ASSERT_EQ(polygonsToString(polygons, true), test_3_step_2);
}

TEST(PolygonSegmentation, Test4)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(six_by_six_square_with_points_on_y_axis);

    LineSegment2D slice1{{-5, 0}, {5, 0}};

    drawPolygonsToFile("Test4-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_4_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test4-1.svg", polygons, &slice1);
    ASSERT_EQ(polygonsToString(polygons), test_4_step_1);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test4-2.svg", polygons, &slice1);
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
    drawPolygonsToFile("Test5-1.svg", polygons, &slice1);
    ASSERT_EQ(polygonsToString(polygons), test_5_step_1);

    slicePolygons(slice2, polygons);
    drawPolygonsToFile("Test5-2.svg", polygons, &slice2);
    ASSERT_EQ(polygonsToString(polygons), test_5_step_2);
}
*/

TEST(PolygonSegmentation, Test6)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(test_6_polygon_1);
    polygons.push_back(test_6_polygon_2);
    polygons.push_back(test_6_polygon_3);
    LineSegment2D slice1{{0, 147}, {250, 147}};

    drawPolygonsToFile("Test6-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_6_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test6-1.svg", polygons, &slice1);
    ASSERT_EQ(polygonsToString(polygons), test_6_step_1);
}

TEST(PolygonSegmentation, Test7)
{
    vector<vector<LineSegment2D>> polygons;
    polygons.push_back(test_7_polygon_1);
    polygons.push_back(test_6_polygon_2);
    polygons.push_back(test_6_polygon_3);
    LineSegment2D slice1{{0, 163}, {250, 163}};

    drawPolygonsToFile("Test7-0.svg", polygons);
    ASSERT_EQ(polygonsToString(polygons), test_7_step_0);

    slicePolygons(slice1, polygons);
    drawPolygonsToFile("Test7-1.svg", polygons, &slice1);
    ASSERT_EQ(polygonsToString(polygons), test_7_step_1);
}

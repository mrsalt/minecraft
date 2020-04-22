#include "gtest/gtest.h"
#include "PolygonMath.h"
#include "static-polygons.h"

using namespace std;

vector<LineSegment2D> reversed(const vector<LineSegment2D> &poly)
{
    vector<LineSegment2D> ret;
    for (auto it = poly.rbegin(); it != poly.rend(); it++)
    {
        ret.push_back(!*it);
    }
    return ret;
}

Direction normalize(Direction dir)
{
    if (Y_AXIS_INCREASES_DOWNWARD)
        return dir;
    return dir == Direction::CLOCKWISE ? Direction::COUNTER_CLOCKWISE : Direction::CLOCKWISE;
}

TEST(PolygonMath, DirectionTest1)
{
    // assertions are based on assumption that Y_AXIS_INCREASES_DOWNWARD.  If we compile it differently,
    // the normalize method will reverse the expected direction.
    ASSERT_EQ(PolygonMath::calculateDirection(six_by_six_square), normalize(Direction::COUNTER_CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(six_by_six_square)), normalize(Direction::CLOCKWISE));

    ASSERT_EQ(PolygonMath::calculateDirection(three_by_three_square), normalize(Direction::COUNTER_CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(three_by_three_square)), normalize(Direction::CLOCKWISE));

    ASSERT_EQ(PolygonMath::calculateDirection(big_polygon_1), normalize(Direction::CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(big_polygon_1)), normalize(Direction::COUNTER_CLOCKWISE));

    ASSERT_EQ(PolygonMath::calculateDirection(inner_polygon_1), normalize(Direction::CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(inner_polygon_1)), normalize(Direction::COUNTER_CLOCKWISE));

    ASSERT_EQ(PolygonMath::calculateDirection(six_by_six_square_with_points_on_y_axis), normalize(Direction::COUNTER_CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(six_by_six_square_with_points_on_y_axis)), normalize(Direction::CLOCKWISE));

    ASSERT_EQ(PolygonMath::calculateDirection(test_6_polygon_1), normalize(Direction::CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(test_6_polygon_1)), normalize(Direction::COUNTER_CLOCKWISE));

    ASSERT_EQ(PolygonMath::calculateDirection(test_6_polygon_2), normalize(Direction::CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(test_6_polygon_2)), normalize(Direction::COUNTER_CLOCKWISE));

    ASSERT_EQ(PolygonMath::calculateDirection(test_6_polygon_3), normalize(Direction::CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(test_6_polygon_3)), normalize(Direction::COUNTER_CLOCKWISE));

    ASSERT_EQ(PolygonMath::calculateDirection(test_7_polygon_1), normalize(Direction::CLOCKWISE));
    ASSERT_EQ(PolygonMath::calculateDirection(reversed(test_7_polygon_1)), normalize(Direction::COUNTER_CLOCKWISE));
}

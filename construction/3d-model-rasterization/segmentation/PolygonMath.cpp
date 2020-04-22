#include "PolygonMath.h"
#define _USE_MATH_DEFINES
#include <math.h>
//#include <iostream>
//#include "string_format.h"

using namespace std;

static double radiansToDegrees(double rad)
{
    return rad / M_PI * 180;
}

// great candidate to become part of Polygon2D
Direction PolygonMath::calculateDirection(const std::vector<LineSegment2D> &poly)
{
    assert(poly.front().first.isReallyCloseTo(poly.back().second));

    Point2D prevPoint = poly.back().second;
    double totalTheta = 0;
    auto calcTheta = [](const LineSegment2D &segment) {
        double xDelta = segment.second.x - segment.first.x;
        double yDelta = segment.second.y - segment.first.y;
        return atan2(yDelta, xDelta);
    };

    double thetaPrev = calcTheta(poly.back());
    //cout << "  back = " << poly.back() << ", angle(back) = " << radiansToDegrees(thetaPrev) << endl;
    for (size_t i = 0; i < poly.size(); i++)
    {
        auto &segment = poly[i];
        assert(segment.first == prevPoint || segment.first.isReallyCloseTo(prevPoint));
        double theta = calcTheta(segment);
        double thetaD = theta - thetaPrev;
        // keep thetaD in the range from -pi to pi
        if (thetaD > M_PI)
            thetaD -= 2.0 * M_PI;
        else if (thetaD < -M_PI)
            thetaD += 2.0 * M_PI;

        totalTheta += thetaD;
        //cout << format("  segment #%2d = %52s, angle(segment) = %3d, delta = %3d, total = %3d\n",
        //    i, segment.toString().c_str(), (int)radiansToDegrees(theta), (int)radiansToDegrees(thetaD), (int)radiansToDegrees(totalTheta));
        thetaPrev = theta;
        prevPoint = segment.second;
    }

    double diffFrom2Pi = abs(totalTheta) - 2.0 * M_PI;
    assert(diffFrom2Pi < M_PI / 100.0);

    // Whether the direction is clockwise or counterclockwise depends on the direction
    // of the y axis.  We're using a Y axis increasing in a downward direction.
    // See also PolygonSegmentation.h, pointingRightOfSlice.  I believe these are the only
    // two places in code that depend on this.  Search for references to Y_AXIS_INCREASES_DOWNWARD.

    if (Y_AXIS_INCREASES_DOWNWARD)
        return totalTheta < 0 ? Direction::COUNTER_CLOCKWISE : Direction::CLOCKWISE;
    else
        return totalTheta < 0 ? Direction::CLOCKWISE : Direction::COUNTER_CLOCKWISE;
}

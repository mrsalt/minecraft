#pragma once

#include "PolygonMath.h"

class PolygonUnion : protected PolygonMath
{
public:
    PolygonUnion(const std::vector<std::vector<LineSegment2D>> &polygons);

    bool polygonsAreIntersecting() { return !intersections().empty(); }

    std::vector<std::vector<LineSegment2D>> combinePolygons();

private:
    void findIntersectingSegments(const std::vector<LineSegment2D>& a, const std::vector<LineSegment2D>& b);
};
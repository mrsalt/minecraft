#pragma once

#include "PolygonMath.h"

void combinePolygons(std::vector<std::vector<LineSegment2D>> &polygons);

class PolygonUnion : protected PolygonMath
{
public:
    PolygonUnion(const std::vector<std::vector<LineSegment2D>> &polygons);

    bool polygonsAreIntersecting() { return !intersections().empty(); }

    std::vector<LineSegment2D> combinePolygons();

    bool findIntersectingSegments(const std::vector<LineSegment2D> &a, const std::vector<LineSegment2D> &b);

private:
};
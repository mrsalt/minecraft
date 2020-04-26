#pragma once

#include "PolygonMath.h"

void combinePolygons(std::vector<std::vector<LineSegment2D>> &polygons);

class PolygonUnion : protected PolygonMath
{
public:
    PolygonUnion(const std::vector<std::vector<LineSegment2D>> &polygons);

    bool polygonsAreIntersecting() { return !intersections().empty(); }

    virtual void addIntersection(const LineSegment2D *segment, Point2D location) override;

    std::vector<LineSegment2D> combinePolygons();

    bool findIntersectingSegments(const std::vector<LineSegment2D> &a, const std::vector<LineSegment2D> &b);

    struct Comparator
    {
        bool operator()(const std::shared_ptr<PolygonUnion> &lhs, const std::shared_ptr<PolygonUnion> &rhs) const;
    };

private:
    std::map<Point2D, std::vector<const LineSegment2D *>> intersection_points;
};
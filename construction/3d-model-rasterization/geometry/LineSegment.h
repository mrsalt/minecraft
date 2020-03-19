#pragma once
#include "Point.h"
#include "Surface.h"
#include <algorithm>
#include <ostream>
#include <vector>

class ModelBuilder;

struct LineSegment : std::pair<Point *, Point *>
{
    LineSegment();
    LineSegment(Point *a, Point *b);
    operator bool() const;
    static LineSegment create(Point *a, Point *b, PointComparisonMethod lessThan);
    void print(std::ostream &out, const Point *first, bool printCoordinates = false) const;

    SurfaceInfo *surfaceInfo{nullptr};
};

void print(std::ostream &out, const std::vector<LineSegment> &list, const ModelBuilder &source);

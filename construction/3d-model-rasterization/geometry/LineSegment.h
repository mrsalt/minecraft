#pragma once
#include "Point.h"
#include "Rectangle.h"
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

struct LineSegment2D
{
    LineSegment2D(const Point2D &first, const Point2D &second, SurfaceInfo *surface = nullptr)
        : first{first}, second{second}, bounds(first, second), surface{surface}
    {
    }

    Point2D first;
    Point2D second;
    Rectangle bounds;
    SurfaceInfo *surface;

    bool intersects(const LineSegment2D &segment, Point2D &intersection) const;

    bool operator<(const LineSegment2D &rhs) const
    {
        if (first < rhs.first)
            return true;
        if (second < rhs.second)
            return true;
        return false;
    }

    bool operator==(const LineSegment2D &rhs) const
    {
        return first == rhs.first && second == rhs.second;
    }

    LineSegment2D operator!() const
    {
        return {second, first, surface};
    }

    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &out, const LineSegment2D &);
};

void print(std::ostream &out, const std::vector<LineSegment> &list, const ModelBuilder &source);

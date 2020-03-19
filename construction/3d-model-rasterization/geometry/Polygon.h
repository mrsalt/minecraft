#pragma once
#include "Point.h"
#include "Surface.h"

#include <string>
#include <vector>

class ModelBuilder;

struct Polygon
{
    std::string toString(const ModelBuilder &source) const;

    SurfaceInfo *surface;
    std::vector<Point *> vertices;
};

struct PolygonBounds
{
    PolygonBounds(const Polygon *polygon);
    std::string toString(const ModelBuilder &source) const;

    const Polygon *polygon;
    Point min;
    Point max;
};

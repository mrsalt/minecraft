#pragma once
#include "ModelBuilder.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

struct PolygonBounds
{
    PolygonBounds(const Polygon *polygon);
    std::string toString(const ModelBuilder &source) const;

    const Polygon *polygon;
    Point min;
    Point max;
};

typedef bool (*PointComparisonMethod)(const Point &a, const Point &b);
typedef bool (*PolygonComparisonMethod)(const PolygonBounds &a, const PolygonBounds &b);
typedef bool (*PolygonPlaneComparisonMethod)(const PolygonBounds &a, double p);

struct SpanningPair : std::pair<Point *, Point *>
{
    SpanningPair();
    SpanningPair(Point *a, Point *b);
    operator bool() const;
    static SpanningPair create(Point *a, Point *b, PointComparisonMethod lessThan);
    void print(std::ostream &out, const Point *first, bool printCoordinates = false) const;

    SurfaceInfo *surfaceInfo{nullptr};
};

class ModelGenerator
{
public:
    void setVerticalScale(double height) { this->modelHeight = height; }
    void generate(const ModelBuilder &source);

private:
    template <typename TMember>
    void slicePolygonsAlongAxis(
        std::vector<PolygonBounds> &polyBounds,
        const double layerDist,
        const int layerCount,
        const ModelBuilder &source,
        const ModelBuilder::Statistics &stats,
        TMember member,
        TMember _2D_xaxis,
        TMember _3D_yaxis,
        PolygonComparisonMethod polyOrderingMethod,
        PointComparisonMethod pointOrderingMethod);
    template <typename TMember>
    std::set<std::vector<SpanningPair>> placePolygonsInLayer(
        std::set<const Polygon *> &unplacedPolygons,
        const double layerPosition,
        const ModelBuilder &source,
        TMember member,
        PointComparisonMethod pointOrderingMethod);

private:
    double modelHeight{0.0};

    ModelBuilder cross_model;
};
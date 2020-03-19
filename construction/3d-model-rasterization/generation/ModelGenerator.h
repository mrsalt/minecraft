#pragma once
#include "LineSegment.h"
#include "ModelBuilder.h"
#include "Polygon.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

typedef bool (*PolygonComparisonMethod)(const PolygonBounds &a, const PolygonBounds &b);
typedef bool (*PolygonPlaneComparisonMethod)(const PolygonBounds &a, double p);

class ModelGenerator
{
public:
    ModelGenerator(const ModelBuilder & source, double height);
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
    std::set<std::vector<LineSegment>> placePolygonsInLayer(
        std::set<const Polygon *> &unplacedPolygons,
        const double layerPosition,
        const ModelBuilder &source,
        TMember member,
        PointComparisonMethod pointOrderingMethod);

private:
    double modelHeight{0.0};

    ModelBuilder cross_model;
};
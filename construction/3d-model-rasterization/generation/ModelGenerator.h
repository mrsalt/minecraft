#pragma once
#include "LineSegment.h"
#include "ModelBuilder.h"
#include "Polygon.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

typedef bool (*PolygonComparisonMethod)(const PolygonBounds &a, const PolygonBounds &b);
typedef bool (*PolygonPlaneComparisonMethod)(const PolygonBounds &a, double p);

class model_error : public std::runtime_error
{
public:
    model_error(const std::string &message)
        : runtime_error(message)
    {
    }
};

class ModelGenerator
{
public:
    ModelGenerator(const ModelBuilder &source, double height);
    void generate(); // throws a model_error if there are problems.

private:
    template <typename TMember>
    void slicePolygonsAlongAxis(
        std::vector<PolygonBounds> &polyBounds,
        const int layerCount,
        TMember member,
        TMember _2D_xaxis,
        TMember _2D_yaxis,
        PolygonComparisonMethod polyOrderingMethod,
        PointComparisonMethod pointOrderingMethod);
    template <typename TMember>
    std::vector<std::vector<LineSegment>> placePolygonsInLayer(
        const std::set<const PolygonBounds *> &polygonsToPlace,
        const double layerPosition,
        TMember member,
        PointComparisonMethod pointOrderingMethod);
    void slicePolygons(const LineSegment2D &slice, std::vector<std::vector<LineSegment2D>> &polygons);

private:
    const ModelBuilder &source;
    const ModelBuilder::Statistics stats;
    const double layerDist;

    ModelBuilder cross_model;
};
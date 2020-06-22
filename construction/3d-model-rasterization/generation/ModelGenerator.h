#pragma once
#include "CubeModel.h"
#include "LineSegment.h"
#include "ModelBuilder.h"
#include "Polygon.h"

#include <algorithm>
#include <iostream>
#include <map>
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
    template <typename TMember, typename CMember>
    void slicePolygonsAlongAxis(
        std::vector<PolygonBounds> &polyBounds,
        const int layerCount,
        TMember member,
        TMember _2D_xaxis,
        TMember _2D_yaxis,
        CMember cubeMember_xAxis,
        CMember cubeMember_yAxis,
        PolygonComparisonMethod polyOrderingMethod,
        PointComparisonMethod pointOrderingMethod,
        const Color &color);

    template <typename TMember>
    std::vector<std::vector<LineSegment>> placePolygonsInLayer(
        const std::set<const PolygonBounds *> &polygonsToPlace,
        const double layerPosition,
        TMember member,
        PointComparisonMethod pointOrderingMethod);
    IntSize calculateCubeDimensions();

    template <typename TMember>
    void insert_cross_model_polygons(
        const std::vector<std::vector<LineSegment2D>> &shapes_2D,
        TMember& member,
        TMember &_2D_xaxis,
        TMember &_2D_yaxis,
        double &p,
        const Color &color);

    template <typename TMember, typename CMember>
    void segmentPolygons(const LineSegment2D& slice, size_t steps, std::vector<std::vector<LineSegment2D>>& polygons, TMember& member, TMember& _2D_xaxis, TMember& _2D_yaxis, CMember& cubeMember, double layerConstValue);

private:
    const ModelBuilder &source;
    const ModelBuilder::Statistics stats;
    const double layerDist;
    CubeModel cubeModel;

    // These 3 members are only used if we're outputting a 'cross' model (cross section 3D model)
    ModelBuilder cross_model;
    std::vector<std::pair<std::vector<int>, Color>> cross_model_polygons;
    std::map<Point, int> point_map;
};
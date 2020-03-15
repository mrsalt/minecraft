#pragma once
#include "ModelBuilder.h"

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
        PolygonComparisonMethod polyOrderingMethod,
        PointComparisonMethod pointOrderingMethod);

private:
    double modelHeight{0.0};
};
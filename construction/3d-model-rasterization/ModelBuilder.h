#pragma once
#include "model-readers\ModelReader.h"
#include <vector>

class SurfaceInfo
{
};

class SolidColorSurface : public SurfaceInfo
{
public:
    int r;
    int g;
    int b;
};

struct Point
{
    double x;
    double y;
    double z;
};

struct Polygon
{
    SurfaceInfo *surface;
    std::vector<Point *> vertices;
};

class ModelBuilder : public ModelConsumer
{
public:
    virtual void verticeRead(double x, double y, double z)
    {
        points.push_back({x, y, z});
    }

    virtual void polygonRead(int vertices[], int countVertices)
    {
        Polygon p;
        for (int n = 0; n < countVertices; n++)
            p.vertices.push_back(points.data() + vertices[n]);
        p.surface = currentSurfaceInfo;
        polygons.push_back(p);
    }

    virtual void setCurrentColor(int r, int g, int b)
    {
        SolidColorSurface *s = new SolidColorSurface();
        s->r = r;
        s->g = g;
        s->b = b;
        currentSurfaceInfo = s;
        surfaceInfos.push_back(s);
    }

    std::vector<Point> points;
    std::vector<Polygon> polygons;
    std::vector<SurfaceInfo *> surfaceInfos;
    SurfaceInfo *currentSurfaceInfo = nullptr;
};
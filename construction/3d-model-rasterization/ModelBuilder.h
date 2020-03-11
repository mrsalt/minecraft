#pragma once
#include "model-readers\ModelReader.h"
#include <ostream>
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

struct Size;
struct Point
{
    double x;
    double y;
    double z;
    Size operator-(const Point &rhs);
    friend std::ostream &operator<<(std::ostream &out, const Point &);
};

struct Size
{
    double width;
    double height;
    double depth;
    friend std::ostream &operator<<(std::ostream &out, const Size &);
};

Size Point::operator-(const Point &rhs)
{
    return {x - rhs.x, y - rhs.y, z - rhs.z};
}

std::ostream &operator<<(std::ostream &out, const Point &p)
{
    out << "(x: " << p.x << ", y: " << p.y << ", z: " << p.z << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Size &s)
{
    out << "(width: " << s.width << ", height: " << s.height << ", depth: " << s.depth << ")";
    return out;
}

struct Polygon
{
    SurfaceInfo *surface;
    std::vector<Point *> vertices;
};

class ModelBuilder : public ModelConsumer
{
public:
    struct Statistics
    {
        Point min;
        Point max;
        Size extent;
        size_t count_vertices;
        size_t count_polygons;
        friend std::ostream &operator<<(std::ostream &out, const Statistics &);
    };

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

    Statistics getStatistics()
    {
        Statistics stats;
        stats.count_vertices = points.size();
        stats.count_polygons = polygons.size();
        if (stats.count_vertices == 0) return stats;
        stats.min = stats.max = points[0];
        for (const Point &p : points)
        {
            if (p.x < stats.min.x)
                stats.min.x = p.x;
            if (p.y < stats.min.y)
                stats.min.y = p.y;
            if (p.z < stats.min.z)
                stats.min.z = p.z;
            if (p.x > stats.max.x)
                stats.max.x = p.x;
            if (p.y > stats.max.y)
                stats.max.y = p.y;
            if (p.z > stats.max.z)
                stats.max.z = p.z;
        }
        stats.extent = stats.max - stats.min;
        return stats;
    }

    std::vector<Point> points;
    std::vector<Polygon> polygons;
    std::vector<SurfaceInfo *> surfaceInfos;
    SurfaceInfo *currentSurfaceInfo = nullptr;
};

std::ostream &operator<<(std::ostream &out, const ModelBuilder::Statistics &stats)
{
    out << "Model Statistics:" << std::endl;
    out << "  Count of vertices: " << stats.count_vertices << std::endl;
    out << "  Count of polygons: " << stats.count_polygons << std::endl;
    if (stats.count_vertices == 0) return out;
    out << "  Min: " << stats.min << std::endl;
    out << "  Max: " << stats.max << std::endl;
    out << "  Size: " << stats.extent << std::endl;
    return out;
}
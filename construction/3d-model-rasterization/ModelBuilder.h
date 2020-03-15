#pragma once
#include "model-readers\ModelReader.h"
#include <ostream>
#include <set>
#include <vector>

class SurfaceInfo
{
};

class SolidColorSurface : public SurfaceInfo
{
public:
    SolidColorSurface(int r, int g, int b)
        : r{r}, g{g}, b{b}
    {
    }

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
    Point &operator+=(const Point &rhs);
    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &out, const Point &);
};

struct Size
{
    double width;
    double height;
    double depth;
    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &out, const Size &);
};

class ModelBuilder;

struct Polygon
{
    std::string toString(const ModelBuilder &source) const;

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
        size_t count_vertices{0};
        size_t count_polygons{0};
        void addPoints(const std::vector<Point *> &);
        void addPoints(const std::vector<Point> &);
        void addPolygons(const std::vector<Polygon> &);

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
        {
            p.vertices.push_back(points.data() + vertices[n]);
        }
        p.surface = currentSurfaceInfo;
        polygons.push_back(p);
        int polygonIndex = (int)polygons.size() - 1;

        adjacent_polygons.resize(points.size());
        for (int n = 0; n < countVertices; n++)
        {
            int verticeIndex = vertices[n];
            adjacent_polygons.at(verticeIndex).push_back(polygonIndex);
        }
    }

    virtual void setCurrentColor(int r, int g, int b)
    {
        SolidColorSurface *s = new SolidColorSurface(r, g, b);
        currentSurfaceInfo = s;
        surfaceInfos.push_back(s);
    }

    Statistics getStatistics() const
    {
        Statistics stats;
        stats.addPoints(points);
        stats.addPolygons(polygons);
        return stats;
    }

    std::set<const Polygon *> getPolygonsAdjacentTo(const Polygon *currentPiece) const
    {
        std::set<const Polygon *> adjacentPolygons;
        for (Point *point : currentPiece->vertices)
        {
            size_t verticeIndice = point - points.data();
            const std::vector<int> &polygonsAdjacentToPoint = adjacent_polygons.at(verticeIndice);
            for (int index : polygonsAdjacentToPoint)
            {
                const Polygon *poly = polygons.data() + index;
                if (poly == currentPiece)
                    continue;
                adjacentPolygons.insert(poly);
            }
        }
        return adjacentPolygons;
    }

    std::vector<Point> points;
    std::vector<Polygon> polygons;
    std::vector<std::vector<int>> adjacent_polygons; // these are indexed by vertice index
    std::vector<SurfaceInfo *> surfaceInfos;
    SurfaceInfo *currentSurfaceInfo = nullptr;
};
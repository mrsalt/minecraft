#include "ModelGenerator.h"
#include "string_format.h"
#include <vector>

using namespace std;

string Polygon::toString(const ModelBuilder &source) const
{
    return format("Polygon #%d", this - source.polygons.data());
}

ostream &operator<<(ostream &out, const ModelBuilder::Statistics &stats)
{
    out << "Model Statistics:" << endl;
    out << "  Count of vertices: " << stats.count_vertices << endl;
    out << "  Count of polygons: " << stats.count_polygons << endl;
    if (stats.count_vertices == 0)
        return out;
    out << "  Min: " << stats.min << endl;
    out << "  Max: " << stats.max << endl;
    out << "  Size: " << stats.extent << endl;
    return out;
}

void ModelBuilder::Statistics::addPoints(const vector<Point *> &points)
{
    if (count_vertices == 0)
    {
        min = max = *points[0];
    }
    count_vertices += points.size();
    for (const Point *ptr : points)
    {
        const Point &p = *ptr;
        if (p.x < min.x)
            min.x = p.x;
        else if (p.x > max.x)
            max.x = p.x;
        if (p.y < min.y)
            min.y = p.y;
        else if (p.y > max.y)
            max.y = p.y;
        if (p.z < min.z)
            min.z = p.z;
        else if (p.z > max.z)
            max.z = p.z;
    }
    extent = Size(max - min);
}

void ModelBuilder::Statistics::addPoints(const vector<Point> &points)
{
    if (count_vertices == 0)
    {
        min = max = points[0];
    }
    count_vertices += points.size();
    for (const Point &p : points)
    {
        if (p.x < min.x)
            min.x = p.x;
        else if (p.x > max.x)
            max.x = p.x;
        if (p.y < min.y)
            min.y = p.y;
        else if (p.y > max.y)
            max.y = p.y;
        if (p.z < min.z)
            min.z = p.z;
        else if (p.z > max.z)
            max.z = p.z;
    }
    extent = Size(max - min);
}

void ModelBuilder::Statistics::addPolygons(const vector<Polygon> &polygons)
{
    count_polygons += polygons.size();
}

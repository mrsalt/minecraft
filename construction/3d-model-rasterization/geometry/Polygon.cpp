#include "Polygon.h"
#include "string_format.h"

using namespace std;

PolygonBounds::PolygonBounds(const Polygon *polygon)
    : polygon(polygon)
{
    min = max = *polygon->vertices[0];
    for (size_t i = polygon->vertices.size() - 1; i > 0; i--)
    {
        Point &p = *polygon->vertices[i];
        if (p.x < min.x)
            min.x = p.x;
        if (p.x > max.x)
            max.x = p.x;
        if (p.y < min.y)
            min.y = p.y;
        if (p.y > max.y)
            max.y = p.y;
        if (p.z < min.z)
            min.z = p.z;
        if (p.z > max.z)
            max.z = p.z;
    }
}

string PolygonBounds::toString(const ModelBuilder &source) const
{
    return format("%s, bounds: %s - %s", polygon->toString(source).c_str(), min.toString().c_str(), max.toString().c_str());
}
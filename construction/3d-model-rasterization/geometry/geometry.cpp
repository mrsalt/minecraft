#include "geometry.h"

double dist(const Point &a, const Point &b)
{
    double dx = (a.x - b.x);
    double dy = (a.y - b.y);
    double dz = (a.z - b.z);
    return sqrt(dx * dx + dy * dy + dz * dz);
}

Point center(const Polygon *p)
{
    Point a{0, 0, 0};
    for (auto &v : p->vertices)
        a += *v;
    a.x /= (double)p->vertices.size();
    a.y /= (double)p->vertices.size();
    a.z /= (double)p->vertices.size();
    return a;
}

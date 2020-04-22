#include "PolygonUnion.h"

using namespace std;

// TODO: search for all getBounds methods and remove them once a Polygon2D class exists
static Rectangle getBounds(const vector<LineSegment2D>& polygon)
{
    Rectangle bounds = polygon.front().bounds;
    for (const auto& segment : polygon)
    {
        bounds += segment.bounds;
    }
    return bounds;
}

PolygonUnion::PolygonUnion(const vector<vector<LineSegment2D>> &polygons)
: PolygonMath(polygons)
{
    //TODO: create a 'Polygon2D' that knows it's bounds.  Replace all vector<LineSegment2D> with Polygon2D.
    vector<Rectangle> bounds;
    for (auto& p : polygons)
        bounds.push_back(getBounds(p));

    // build up a list of line segments which intersect each other.
    for (size_t i = 0; i < polygons.size(); i++)
    {
        for (size_t j = i + 1; j < polygons.size(); j++)
        {
            if (bounds[i].overlaps(bounds[j]))
                findIntersectingSegments(polygons[i], polygons[j]);
        }
    }
}

void PolygonUnion::findIntersectingSegments(const vector<LineSegment2D> &a, const vector<LineSegment2D> &b)
{
    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = i + 1; j < b.size(); j++)
        {
            bool online;
            Point2D intersection;
            if (a[i].intersects(b[j], intersection, online))
            {
                intersecting.push_back({ true, intersection, &a[i] });
                set_intersecting.insert(&a[i]);
                intersecting.push_back({ true, intersection, &b[i] });
                set_intersecting.insert(&b[i]);
            }
        }
    }
}

vector<vector<LineSegment2D>> PolygonUnion::combinePolygons()
{
    return vector<vector<LineSegment2D>>();
}

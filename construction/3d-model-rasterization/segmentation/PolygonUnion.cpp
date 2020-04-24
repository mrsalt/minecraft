#include "PolygonUnion.h"
#include <map>

using namespace std;

// TODO: search for all getBounds methods and remove them once a Polygon2D class exists
static Rectangle getBounds(const vector<LineSegment2D> &polygon)
{
    Rectangle bounds = polygon.front().bounds;
    for (const auto &segment : polygon)
    {
        bounds += segment.bounds;
    }
    return bounds;
}

PolygonUnion::PolygonUnion(const vector<vector<LineSegment2D>> &polygons)
    : PolygonMath(polygons)
{
}

bool PolygonUnion::findIntersectingSegments(const vector<LineSegment2D> &a, const vector<LineSegment2D> &b)
{
    bool intersection_found = false;
    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = 0; j < b.size(); j++)
        {
            bool online;
            Point2D intersection;
            if (a[i].intersects(b[j], intersection, online))
            {
                intersecting.push_back({true, intersection, &a[i]});
                set_intersecting.insert(&a[i]);
                intersecting.push_back({true, intersection, &b[j]});
                set_intersecting.insert(&b[j]);
                intersection_found = true;
            }
        }
    }
    return intersection_found;
}

vector<LineSegment2D> PolygonUnion::combinePolygons()
{
    // PolygonUnion knows about a bunch of polygons, which can comprise N overlapping (or non-overlapping) polygons.
    // A PolygonUnion is a set of a single group of polygons that are all overlapping with each other.
    set<const vector<LineSegment2D> *> overlappingPolygons;
    for (auto lineSegment : set_intersecting)
    {
        auto poly = polygonOwningSegment(lineSegment);
        overlappingPolygons.insert(poly);
    }

    assert(overlappingPolygons.size() > 1);

    map<const vector<LineSegment2D> *, Direction> polygonDirection;
    const LineSegment2D *leftMost = &(*overlappingPolygons.begin())->front();
    for (auto poly : overlappingPolygons)
    {
        polygonDirection.insert({poly, calculateDirection(*poly)});
        for (auto &segment : *poly)
        {
            if (segment < *leftMost)
            {
                leftMost = &segment;
            }
        }
    }

    const LineSegment2D *currentSegment = leftMost;
    const LineSegment2D *endAt = currentSegment;
    bool reachedEnd = false;
    vector<LineSegment2D> ret;
    Direction dir = polygonDirection[polygonOwningSegment(currentSegment)];
    while (true)
    {
        // We want to iterate in the same direction (clockwise) around each polygon.
        // If the polygon's natural direction is counterclockwise, iterating counterclockwise
        // around it will actually be clockwise.
        DividingSegment &div = iterate(currentSegment, dir, [&ret, &reachedEnd, endAt](const LineSegment2D *segment) {
            if (reachedEnd)
                return;
            ret.push_back(*segment);
            if (segment == endAt)
                reachedEnd = true;
        });
        if (reachedEnd)
            break;
        ret.push_back({dir == Direction::CLOCKWISE ? div.line->first : div.line->second, div.intersection_point, div.line->surface});

        // find another dividing segment that shares this same intersecting point.
        bool found = false;
        for (auto &divSegment : intersecting)
        {
            if (divSegment.intersection_point == div.intersection_point && &divSegment != &div)
            {
                currentSegment = divSegment.line;
                dir = polygonDirection[polygonOwningSegment(currentSegment)];
                ret.push_back({div.intersection_point, dir == Direction::CLOCKWISE ? divSegment.line->second : divSegment.line->first, divSegment.line->surface});
                found = true;
                break;
            }
        }
        if (!found)
            throw runtime_error("Logic error -- cannot find matching intersection");
    }
    return ret;
}

void combinePolygons(vector<std::vector<LineSegment2D>> &polygons)
{
    //TODO: create a 'Polygon2D' that knows it's bounds.  Replace all vector<LineSegment2D> with Polygon2D.
    vector<Rectangle> bounds;
    for (auto &p : polygons)
        bounds.push_back(getBounds(p));

    // build up a list of sets of polygons which intersect each other.
    vector<shared_ptr<PolygonUnion>> polygonSetMap(polygons.size());
    set<shared_ptr<PolygonUnion>> polygonSets;

    for (size_t i = 0; i < polygons.size(); i++)
    {
        for (size_t j = i + 1; j < polygons.size(); j++)
        {
            if (bounds[i].overlaps(bounds[j]))
            {
                shared_ptr<PolygonUnion> poly_union;
                bool new_union = false;
                if (polygonSetMap[i])
                    poly_union = polygonSetMap[i];
                else if (polygonSetMap[j])
                    poly_union = polygonSetMap[j];
                else
                {
                    poly_union = make_shared<PolygonUnion>(polygons);
                    new_union = true;
                }
                if (poly_union->findIntersectingSegments(polygons[i], polygons[j]))
                {
                    if (new_union)
                        polygonSets.insert(poly_union);
                    if (!polygonSetMap[i])
                        polygonSetMap[i] = poly_union;
                    else
                        assert(polygonSetMap[i] == poly_union);
                    if (!polygonSetMap[j])
                        polygonSetMap[j] = poly_union;
                    else
                        assert(polygonSetMap[j] == poly_union);
                }
            }
        }
    }

    if (polygonSets.empty())
        return; // no work to do

    vector<std::vector<LineSegment2D>> new_polygons;

    // Add polygons which are not part of any union (not
    // overlapping with any other polygon) into the return set.
    for (size_t i = 0; i < polygonSetMap.size(); i++)
    {
        if (!polygonSetMap[i])
        {
            new_polygons.push_back(polygons[i]);
        }
    }

    // polygonSets are a group of disjoint sets of polygons.
    for (auto overlapping : polygonSets)
    {
        new_polygons.push_back(overlapping->combinePolygons());
    }

    polygons = new_polygons;
}
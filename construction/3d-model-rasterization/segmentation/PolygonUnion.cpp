#include "PolygonUnion.h"
#include <set>

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
                addIntersection(&a[i], intersection);
                addIntersection(&b[j], intersection);
                intersection_found = true;
            }
        }
    }
    return intersection_found;
}

void PolygonUnion::addIntersection(const LineSegment2D *segment, Point2D location)
{
    auto p = intersection_points.insert({location, {}});
    p.first->second.push_back(segment);
    PolygonMath::addIntersection(segment, location);
}

vector<LineSegment2D> PolygonUnion::combinePolygons()
{
    updateIntersectingSetPointers();

    // PolygonUnion knows about a bunch of polygons, which can comprise N overlapping (or non-overlapping) polygons.
    // A PolygonUnion is a set of a single group of polygons that are all overlapping with each other.
    set<const vector<LineSegment2D> *> overlappingPolygons;
    for (auto it : set_intersecting)
    {
        auto poly = polygonOwningSegment(it.first);
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

    bool reachedEnd = false;
    vector<LineSegment2D> ret;
    const LineSegment2D *currentSegment = leftMost;
    Direction dir = polygonDirection[polygonOwningSegment(currentSegment)];
    auto it = getIterator<RingIterator<LineSegment2D>>(currentSegment);

    LineSegment2D segment = currentSegment->normalize(dir);
    Point2D lastPoint = segment.first;
    const Point2D endAt = lastPoint;
    while (true)
    {
        // We want to iterate in the same direction (clockwise) around each polygon.
        // If the polygon's natural direction is counterclockwise, iterating counterclockwise
        // around it will actually be clockwise.

        const LineSegment2D *nextSegment = nullptr;
        auto &v = set_intersecting[currentSegment];
        Point2D nextPoint = segment.second;

        if (!v.empty())
        {
            // get the next intersection between lastPoint and segment.second, if there is one.

            double maxDistance = 0;
            double distanceToLastPoint = lastPoint.relativeDistance(segment.second);
            for (auto &divSegment : v)
            {
                double relDistance = divSegment->intersection_point.relativeDistance(segment.second);
                if (relDistance > maxDistance && relDistance < distanceToLastPoint)
                {
                    maxDistance = relDistance;
                    nextPoint = divSegment->intersection_point;
                    for (auto s : intersection_points[nextPoint])
                    {
                        if (s != divSegment->line)
                            nextSegment = s;
                    }
                }
            }
        }

        ret.push_back({lastPoint, nextPoint, currentSegment->surface});
        lastPoint = nextPoint;
        if (lastPoint == endAt)
            break;

        if (nextSegment)
        {
            currentSegment = nextSegment;
            dir = polygonDirection[polygonOwningSegment(currentSegment)];
            it = getIterator<RingIterator<LineSegment2D>>(currentSegment);
        }
        else
        {
            dir == Direction::CLOCKWISE ? it++ : it--;
            currentSegment = &(*it);
        }
        segment = currentSegment->normalize(dir);
    }
    return ret;
}

bool PolygonUnion::Comparator::operator()(const shared_ptr<PolygonUnion> &lhs, const shared_ptr<PolygonUnion> &rhs) const
{
    size_t lSize = lhs->polygons.size();
    size_t rSize = rhs->polygons.size();
    if (lSize != rSize)
        return lSize < rSize;
    lSize = lhs->intersecting.size();
    rSize = rhs->intersecting.size();
    if (lSize != rSize)
        return lSize < rSize;
    for (size_t i = 0; i < lhs->polygons.size(); i++)
    {
        lSize = lhs->polygons[i].size();
        rSize = rhs->polygons[i].size();
        if (lSize != rSize)
            return lSize < rSize;
    }
    return false;
}

void findOverlappingPolygons(const vector<vector<LineSegment2D>> &polygons, const vector<Rectangle> &bounds, vector<shared_ptr<PolygonUnion>> &polygonSetMap, set<shared_ptr<PolygonUnion>, PolygonUnion::Comparator> &polygonSets)
{
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
}

void combinePolygons(vector<std::vector<LineSegment2D>> &polygons)
{
    //TODO: create a 'Polygon2D' that knows it's bounds.  Replace all vector<LineSegment2D> with Polygon2D.
    vector<Rectangle> bounds;
    for (auto &p : polygons)
        bounds.push_back(getBounds(p));

    // build up a list of sets of polygons which intersect each other.
    vector<shared_ptr<PolygonUnion>> polygonSetMap(polygons.size());
    set<shared_ptr<PolygonUnion>, PolygonUnion::Comparator> polygonSets;

    findOverlappingPolygons(polygons, bounds, polygonSetMap, polygonSets);

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
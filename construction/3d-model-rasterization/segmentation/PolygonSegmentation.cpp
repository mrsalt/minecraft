#include "PolygonSegmentation.h"
#include "string_format.h"
#include "cairo-util.h"
#include <set>

using namespace std;

#define SPECIAL_POLYGON_DEBUG 0
#if SPECIAL_POLYGON_DEBUG
static int polyCounter = 0;
static vector<LineSegment2D> *lastPoly = nullptr;
bool drawPolysToFile = false;
#endif

static Rectangle getBounds(const vector<LineSegment2D> &polygon)
{
    Rectangle bounds = polygon.front().bounds;
    for (const auto &segment : polygon)
    {
        bounds += segment.bounds;
    }
    return bounds;
}

size_t determineStartingSegment(const vector<LineSegment2D> &shape, const LineSegment2D &slice)
{
    size_t nextBest = 0;
    for (size_t i = 0; i < shape.size(); i++)
    {
        auto &segment = shape[i];
        Point2D intersection;
        bool online;
        if (segment.intersects(slice, intersection, online))
        {
            if (segment.first != intersection && segment.second != intersection)
            {
                nextBest = i;
            }
        }
        else
        {
            return i;
        }
    }
    return nextBest;
}

bool segmentsAreContiguous(const vector<vector<LineSegment2D>> &polygons)
{
    for (size_t i = 0; i < polygons.size(); i++)
    {
        auto &shape = polygons[i];
        Point2D previous = shape.back().second;
        for (size_t j = 0; j < shape.size(); j++)
        {
            auto &segment = shape[j];
            bool isReallyCloseToPrevious = segment.first.isReallyCloseTo(previous);
            assert(isReallyCloseToPrevious);
            if (!isReallyCloseToPrevious)
                return false;
            previous = segment.second;
        }
    }
    return true;
}

SliceData::SliceData(const LineSegment2D &slice, const vector<vector<LineSegment2D>> &polygons)
    : PolygonMath(polygons), slice{slice}, horizontal_slice{slice.first.y == slice.second.y}, vertical_slice{slice.first.x == slice.second.x}
{
    // in the future if necessary slice could be at any angle.  just have to use vector math
    // to determine if a point is left or right of the line.
    if (!(horizontal_slice ^ vertical_slice))
        throw runtime_error("Logic error -- expect slice to be totally vertical or horizontal");

    assert(segmentsAreContiguous(polygons));

    // Step 1: determine all line segments intersected by the 'slicing' line
    for (auto &shape : polygons)
    {
        vector<const LineSegment2D *> segmentsOnSlice;
        size_t startAt = determineStartingSegment(shape, slice);
        for (size_t i = 0; i < shape.size(); i++)
        {
            auto &segment = shape[(startAt + i) % shape.size()];
            Point2D intersection;
            bool online = false;
            if (segment.intersects(slice, intersection, online))
            {
                if (segment.first == intersection || segment.second == intersection)
                {
                    segmentsOnSlice.push_back(&segment);
                }
                else
                {
                    if (!segmentsOnSlice.empty())
                        handleSegmentsOnSlice(segmentsOnSlice);
                    addIntersection(&segment, intersection);
                }
            }
            else if (segmentsOnSlice.size() > 1)
            {
                handleSegmentsOnSlice(segmentsOnSlice);
            }
        }
        if (!segmentsOnSlice.empty())
            handleSegmentsOnSlice(segmentsOnSlice);
    }

    //if (intersecting.size() % 2 != 0)
    //    throw runtime_error("Logic error -- count of intersecting line segments must be even (or our slicing line did not make it all the way through a polygon!)");
    //This assertion is valid only if no line segments have a line end on the slice.

    if (!intersecting.empty())
    {
        std::stable_sort(intersecting.begin(), intersecting.end(), [this](const DividingSegment &a, const DividingSegment &b) {
            return this->slice.first.relativeDistance(a.intersection_point) < this->slice.first.relativeDistance(b.intersection_point);
        });

        // We must account for the possibility that two different polygons share an edge.
        // in this case, the edge that comes first is the one belonging to the
        // polygon that includes the immediately preceding intersecting line.
        for (size_t i = 2; i < intersecting.size() - 1; i++)
        {
            if (intersecting[i].intersection_point.isReallyCloseTo(intersecting[i - 1].intersection_point))
            {
                if (polygonOwningSegment(intersecting[i].line) == polygonOwningSegment(intersecting[i - 2].line) &&
                    polygonOwningSegment(intersecting[i].line) != polygonOwningSegment(intersecting[i - 1].line))
                    swap(intersecting[i], intersecting[i - 1]);
            }
        }
        bool is_even = true;
        Point2D prevIntersection;
        size_t prevPolygon;
        for (size_t i = 0; i < intersecting.size(); i++)
        {
            auto &ds = intersecting[i];
            size_t currentPolygon = polygonOwningSegment(ds.line) - polygons.data();
            if (i > 0)
            {
                // If we happen to intersect right on a point in an existing polygon, we'll get two dividing segments.
                // We don't want to toggle on the second one (toggling on line 93 reverses the toggle done on line 96).
                bool sameLine = currentPolygon == prevPolygon && ds.intersection_point.isReallyCloseTo(prevIntersection);
                if (sameLine)
                    is_even = !is_even;
            }
            ds.is_even = is_even;
            is_even = !is_even;
            prevIntersection = ds.intersection_point;
            prevPolygon = currentPolygon;
        }
        updateIntersectingSetPointers();
    }

#if SPECIAL_POLYGON_DEBUG
    if (drawPolysToFile && intersecting.size())
    {
        vector<LineSegment2D> intersectingSegments;
        int i = 0;
        for (auto &ds : intersecting)
        {
            intersectingSegments.push_back(*ds.line);
            size_t p = polygonOwningSegment(ds.line) - polygons.data();
            Rectangle bounds = getBounds(polygons[p]);
            cout << i++ << ": " << ds.intersection_point << ", " << *ds.line << " owned by " << p << " (bounds: " << bounds.min << "-" << bounds.max << ")" << endl;
        }
        ::drawPolygonToFile("interesecting-segments.svg", intersectingSegments, &slice);
    }
#endif
}

void SliceData::handleSegmentsOnSlice(vector<const LineSegment2D *> &segmentsOnSlice)
{
    assert(segmentsOnSlice.size() > 1);

    while (true)
    {
        // if the segments don't cross the slice, but merely run along it for some number of
        // segments, we can ignore them completely.
        bool compareFirstToSecond = onSlice(segmentsOnSlice.front()->second);
        if (compareFirstToSecond)
        {
            if (isLeftOfSlice(segmentsOnSlice.front()->first) == isLeftOfSlice(segmentsOnSlice.back()->second))
                break;
        }
        else
        {
            assert(onSlice(segmentsOnSlice.front()->first));
            if (isLeftOfSlice(segmentsOnSlice.front()->second) == isLeftOfSlice(segmentsOnSlice.back()->first))
                break;
        }
        if (compareFirstToSecond) // means that second has been tested and it IS on the slice
            addIntersection(segmentsOnSlice.front(), segmentsOnSlice.front()->second);
        else
            addIntersection(segmentsOnSlice.front(), segmentsOnSlice.front()->first);
        if (onSlice(segmentsOnSlice.back()->second))
            addIntersection(segmentsOnSlice.back(), segmentsOnSlice.back()->second);
        else
            addIntersection(segmentsOnSlice.back(), segmentsOnSlice.back()->first);
        break;
    }
    segmentsOnSlice.clear();
}

void addSegment(const LineSegment2D &segment, vector<LineSegment2D> &new_polygon)
{
    if (segment.first == segment.second)
        return;
    new_polygon.push_back(segment);
#if SPECIAL_POLYGON_DEBUG
    if (&new_polygon != lastPoly)
    {
        lastPoly = &new_polygon;
        polyCounter++;
    }

    if (new_polygon.size() > 300)
        throw exception("max polygon size reached");

    if (drawPolysToFile)
        drawPolygonToFile(format("Polygon-%d-size-%zd.svg", polyCounter, new_polygon.size()), new_polygon);
#endif
    if (new_polygon.size() > 1)
    {
        assert(new_polygon.back().first == new_polygon[new_polygon.size() - 2].second);
    }
}

DividingSegment &SliceData::buildNewSegment(const DividingSegment &first, const Direction direction, vector<LineSegment2D> &new_polygon, const DividingSegment &initial)
{
    addSegment({first.intersection_point,
                direction == Direction::CLOCKWISE ? first.line->second : first.line->first,
                first.line->surface},
               new_polygon);

    DividingSegment &s1 = iterate(first.line, direction, [&new_polygon, direction](const LineSegment2D *segment) {
        addSegment(direction == Direction::CLOCKWISE ? *segment : !*segment, new_polygon);
    });

    addSegment({direction == Direction::CLOCKWISE ? s1.line->first : s1.line->second,
                s1.intersection_point,
                s1.line->surface},
               new_polygon);

    if (&initial == &s1) // we're done!
        return s1;

    size_t index = &s1 - intersecting.data();
    int d = s1.is_even ? 1 : -1;
    bool stopAt = !s1.is_even;
    while (intersecting[index].is_even != stopAt)
    {
        index += d;
        assert(index >= 0 && index < intersecting.size());
    }

    DividingSegment &s2 = *(intersecting.data() + index);
    addSegment({s1.intersection_point, s2.intersection_point, s1.line->surface}, new_polygon);

    return s2;
}

void eraseFromPolygonset(const std::vector<LineSegment2D> *poly, set<const vector<LineSegment2D> *> &nonIntersectingPolygons)
{
    auto it = nonIntersectingPolygons.find(poly);
    if (it != nonIntersectingPolygons.end())
        nonIntersectingPolygons.erase(it);
}

vector<vector<LineSegment2D>> SliceData::segmentPolygons()
{
    // Step 2:
    // For each intersecting segment, from left to right, begin iterating forward (clockwise) from that segment.
    // If the segment is odd numbered, iterate backwards (counter-clockwise).

    // Step 2a:
    // Mark this segment as traversed or assigned, so that we do not traverse it again.

    // When a line segment is reached that is in the set of intersecting segments, if the index of that point is
    // odd, make a connecting segment back to the previous intersecting segment, otherwise make a connecting
    // segment to the next intersecting segment.

    // If the next/previous segment is the same as the one that we started with, we have completed a polygon.
    // Go back to the beginning of step 2 and continue with the next unassigned line segment.

    // If it is not the same as the segment we began with (for the current polygon), iteration direction is now
    // clockwise for odd segments and counter-clockwise for even segments.  Go back to Step 2a.

    vector<vector<LineSegment2D>> new_polygon_list;
    vector<bool> assigned(intersecting.size(), false);
    set<const vector<LineSegment2D> *> nonIntersectingPolygons;
    for_each(polygons.begin(), polygons.end(), [&nonIntersectingPolygons](auto &poly) { nonIntersectingPolygons.insert(&poly); });

    for (size_t i = 0; i < intersecting.size(); i++)
    {
        DividingSegment *current = &intersecting[i];

        if (!assigned[i])
        {
            Direction direction = naturalDirection(current);
            assigned[i] = true;
            auto currentPoly = polygonOwningSegment(current->line);
            eraseFromPolygonset(currentPoly, nonIntersectingPolygons);
            vector<LineSegment2D> new_polygon;
            while (true)
            {
                DividingSegment &next = buildNewSegment(*current, direction, new_polygon, intersecting[i]);
                if (&next == &intersecting[i] || intersecting[i].intersection_point.isReallyCloseTo(next.intersection_point))
                    break;
                auto nextPoly = polygonOwningSegment(next.line);
                if (nextPoly != currentPoly)
                {
                    eraseFromPolygonset(nextPoly, nonIntersectingPolygons);
                    direction = naturalDirection(&next);
                    currentPoly = nextPoly;
                }
                current = &next;
                size_t index = current - intersecting.data();
                // This assertion doesn't fire until test case 4, which is an unusual case.
                // assert(!assigned[index]);
                assigned[index] = true;
            }
            // This check isn't necessary until test case 4, which is unusual.
            if (new_polygon.size() > 2)
                new_polygon_list.push_back(new_polygon);
#if SPECIAL_POLYGON_DEBUG
            if (drawPolysToFile)
                drawPolygonsToFile(format("intersecting-%d.svg", i), new_polygon_list);
#endif
        }
    }
    for (auto &poly : nonIntersectingPolygons)
    {
        new_polygon_list.push_back(*poly);
    }
    return new_polygon_list;
}

void slicePolygons(const LineSegment2D &slice, vector<vector<LineSegment2D>> &polygons)
{
    SliceData segmenter(slice, polygons);

    if (!segmenter.polygonsAreIntersectedBySlice())
        return; // nothing to do

    polygons = segmenter.segmentPolygons();
}
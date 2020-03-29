#include "PolygonSegmentation.h"

using namespace std;

SliceData::SliceData(const LineSegment2D &slice, const vector<vector<LineSegment2D>> &polygons)
    : polygons{polygons}, slice{slice}, horizontal_slice{slice.first.y == slice.second.y}, vertical_slice{slice.first.x == slice.second.x}
{
    // in the future if necessary slice could be at any angle.  just have to use vector math
    // to determine if a point is left or right of the line.
    if (!(horizontal_slice ^ vertical_slice))
        throw runtime_error("Logic error -- expect slice to be totally vertical or horizontal");

    // Step 1: determine all line segments intersected by the 'slicing' line
    for (auto &shape : polygons)
    {
        for (auto &segment : shape)
        {
            Point2D intersection;
            if (segment.intersects(slice, intersection))
            {
                intersecting.push_back({&segment, intersection});
                set_intersecting.insert(&segment);
            }
        }
    }
    if (intersecting.size() % 2 != 0)
        throw runtime_error("Logic error -- count of intersecting line segments must be even (or our slicing line did not make it all the way through a polygon!)");
    if (!intersecting.empty())
    {
        std::sort(intersecting.begin(), intersecting.end(), [this](const DividingSegment &a, const DividingSegment &b) {
            return this->slice.first.relativeDistance(a.intersection_point) < this->slice.first.relativeDistance(b.intersection_point);
        });
        // We must account for the possibility that polygons share an edge.
        // in this case, the edge that comes first is the one belonging to the
        // polygon that includes the immediately preceding intersecting line.
        for (size_t i = 2; i < intersecting.size() - 1; i++)
        {
            if (intersecting[i].intersection_point == intersecting[i - 1].intersection_point)
            {
                if (polygonOwningSegment(intersecting[i].line) == polygonOwningSegment(intersecting[i - 2].line))
                    swap(intersecting[i], intersecting[i - 1]);
            }
        }
    }
}

void addSegment(const LineSegment2D &segment, vector<LineSegment2D> &new_polygon)
{
    if (segment.first == segment.second)
        return;
    new_polygon.push_back(segment);
    if (new_polygon.size() > 1)
    {
        assert(new_polygon.back().first == new_polygon[new_polygon.size() - 2].second);
    }
}

DividingSegment &SliceData::buildNewSegment(const DividingSegment &first, const Direction direction, vector<LineSegment2D> &new_polygon)
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

    size_t index = &s1 - intersecting.data();
    if (index % 2 == 1)
        index--;
    else
        index++;

    assert(index >= 0 && index < intersecting.size());

    DividingSegment &s2 = *(intersecting.data() + index);
    addSegment({s1.intersection_point, s2.intersection_point, s1.line->surface}, new_polygon);

    return s2;
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
            auto it = nonIntersectingPolygons.find(currentPoly);
            if (it != nonIntersectingPolygons.end())
                nonIntersectingPolygons.erase(it);
            vector<LineSegment2D> new_polygon;
            while (true)
            {
                DividingSegment &next = buildNewSegment(*current, direction, new_polygon);
                if (&next == &intersecting[i])
                    break;
                auto nextPoly = polygonOwningSegment(next.line);
                if (nextPoly != currentPoly)
                {
                    reverse(direction);
                    currentPoly = nextPoly;
                }
                current = &next;
                size_t index = current - intersecting.data();
                assert(!assigned[index]);
                assigned[index] = true;
            }
            new_polygon_list.push_back(new_polygon);
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

    if (segmenter.intersections().empty())
        return; // nothing to do

    polygons = segmenter.segmentPolygons();
}
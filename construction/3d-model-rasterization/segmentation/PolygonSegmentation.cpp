#include "PolygonSegmentation.h"

using namespace std;

Segmenter::Segmenter(const LineSegment2D &slice, vector<vector<LineSegment2D>> &polygons)
    : slice{slice}, polygons{polygons}, horizontal_slice{slice.first.y == slice.second.y}, vertical_slice{slice.first.x == slice.second.x}
{
    // in the future if necessary slice could be at any angle.  just have to use vector math
    // to determine if a point is left or right of the line.
    if (!(horizontal_slice ^ vertical_slice))
        throw runtime_error("Logic error -- expect slice to be totally vertical or horizontal");
}

size_t Segmenter::findIntersections()
{
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
    return intersecting.size();
}

DividingSegment &Segmenter::buildNewSegment(DividingSegment &first, Direction direction, vector<LineSegment2D> &new_polygon)
{
    new_polygon.push_back({first.intersection_point,
                           direction == Direction::CLOCKWISE ? first.line->second : first.line->first,
                           first.line->surface});
    if (new_polygon.size() > 1)
    {
        assert(new_polygon.back().first == new_polygon[new_polygon.size() - 2].second);
    }

    DividingSegment &s1 = iterate(first.line, direction, [&new_polygon, direction](LineSegment2D *segment) {
        new_polygon.push_back(direction == Direction::CLOCKWISE ? *segment : !*segment);
        assert(new_polygon.back().first == new_polygon[new_polygon.size() - 2].second);
    });

    new_polygon.push_back({direction == Direction::CLOCKWISE ? s1.line->first : s1.line->second,
                           s1.intersection_point,
                           s1.line->surface});
    assert(new_polygon.back().first == new_polygon[new_polygon.size() - 2].second);

    size_t index = &s1 - intersecting.data();
    if (index % 2 == 1)
        index--;
    else
        index++;

    assert(index >= 0 && index < intersecting.size());

    DividingSegment &s2 = *(intersecting.data() + index);
    new_polygon.push_back({s1.intersection_point, s2.intersection_point, s1.line->surface});
    assert(new_polygon.back().first == new_polygon[new_polygon.size() - 2].second);

    return s2;
}

vector<vector<LineSegment2D>> Segmenter::segmentPolygons()
{
    // Step 2: Starting with the first intersecting segment, begin iterating forward (clockwise)
    // from the first intersecting segment.
    // When a line segment is reached that is in the set of intersecting polygons, store a pointer
    // to the line segment in the “second” slot.
    // The segment that is reached will store a pointer back to the current segment in its “first” slot.
    // Step 3: Continuing with the current segment, begin iterating backwards (counter-clockwise) from
    // the first intersecting segment.
    // When a line segment is reached that is in the set of intersecting polygons, store a pointer
    // to the line segment in the “first” slot.
    // The segment that is reached will store a pointer back to the current segment in its “second” slot.
    // Step 4: Repeat steps 2 and 3 for all the intersecting (red) line segments.  If the “first” or
    // “second” pointers are already set, iterating that direction can be skipped.

    // Step 5:
    // In this step new polygons will be made from existing polygons.
    // Connections will be made between all pairs of consecutive numbers.
    // Step 5a:
    // A pair of line segments will be created for each new joining line segment.  (shown in purple)
    // I’ve drawn them with separation, in reality, there will be no separation between them.  They will have identical start and end coordinates.
    // Step 5b:
    // For each new joining line segment, iteration will be done until that purple line segment is reached.  At this point a new polygon will be complete.
    // If a purple line segment is used in order to reach the originating line segment, it will be considered “used” and will be skipped.
    vector<vector<LineSegment2D>> new_polygon_list;

    for (size_t i = 0; i < intersecting.size(); i++)
    {
        DividingSegment *current = &intersecting[i];

        if (!current->assigned)
        {
            Direction direction = naturalDirection(current);
            current->assigned = true;
            vector<LineSegment2D> new_polygon;
            while (true)
            {
                DividingSegment &next = buildNewSegment(*current, direction, new_polygon);
                if (&next == &intersecting[i])
                    break;
                direction = naturalDirection(current);
                reverse(direction);
                current = &next;
                assert(!current->assigned);
                current->assigned = true;
            }
            new_polygon_list.push_back(new_polygon);
        }
    }
    return new_polygon_list;
}

void slicePolygons(const LineSegment2D &slice, vector<vector<LineSegment2D>> &polygons)
{
    cout << "slicing polygons along line " << slice << endl;

    // Step 1: determine all line segments intersected by the 'slicing' line
    Segmenter segmenter(slice, polygons);

    if (segmenter.findIntersections() == 0)
        return; // nothing to do

    polygons = segmenter.segmentPolygons();
}
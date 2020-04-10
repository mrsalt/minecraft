#pragma once

#include "LineSegment.h"
#include "RingIterator.h"
#include <vector>
#include <assert.h>
#include <iostream>
#include <functional>
#include <set>

void slicePolygons(const LineSegment2D &slice, std::vector<std::vector<LineSegment2D>> &polygons);

struct DividingSegment
{
    bool is_even; // If ALL dividing segments spanned the slice (see spansSlice), this
                  // would alternate true/false always.  However, the universe of possibilities
                  // allows for a line segment to begin or end on a dividing slice, which allows
                  // two dividing segments in a row to be even or odd.
    Point2D intersection_point;
    const LineSegment2D* line;

    bool spansSlice() const
    {
        return line->first != intersection_point && line->second != intersection_point;
    }
};

enum class Direction
{
    CLOCKWISE = 0,
    COUNTER_CLOCKWISE = 1
};

class SliceData
{
public:
    SliceData(const LineSegment2D &slice, const std::vector<std::vector<LineSegment2D>> &polygons);

    std::vector<std::vector<LineSegment2D>> segmentPolygons();
    const std::vector<DividingSegment> &intersections() { return intersecting; }

public:
    const LineSegment2D slice;
    const bool horizontal_slice;
    const bool vertical_slice;

private:
    const std::vector<std::vector<LineSegment2D>> &polygons;
    std::set<const LineSegment2D *> set_intersecting;
    std::vector<DividingSegment> intersecting;

private:
    void handleSegmentsOnSlice(std::vector<const LineSegment2D*> &segmentsOnSlice);

    template <class I>
    I getIterator(const LineSegment2D *segment)
    {
        for (auto &shape : const_cast<vector<vector<LineSegment2D>> &>(polygons))
        {
            if (segment >= shape.data() && segment < shape.data() + shape.size())
            {
                size_t position = segment - shape.data();
                return I(shape, position);
            }
        }
        throw std::runtime_error("Logic error -- no polygon found containing segment");
    }

    bool onSlice(const Point2D& p) const
    {
        if (horizontal_slice)
        {
            return (p.y == slice.first.y);
        }
        else if (vertical_slice)
        {
            return (p.x == slice.first.x);
        }
        throw std::runtime_error("Logic error -- only horizontal/vertical slice support built in.");
    }

    bool isLeftOfSlice(const Point2D &p) const
    {
        // assumptions: slice is either horizontal or vertical
        if (horizontal_slice)
        {
            assert(p.y != slice.first.y);
            return p.y > slice.first.y;
        }
        else if (vertical_slice)
        {
            assert(p.x != slice.first.x);
            return p.x < slice.first.x;
        }
        throw std::runtime_error("Logic error -- only horizontal/vertical slice support built in.");
    }

    bool pointingRightOfSlice(const LineSegment2D *line) const
    {
        // assumptions: slice is either horizontal or vertical
        if (horizontal_slice)
        {
            return line->second.y > line->first.y;
        }
        else if (vertical_slice)
        {
            return line->second.x > line->first.x;
        }
        throw std::runtime_error("Logic error -- only horizontal/vertical slice support built in.");
    }

    Direction naturalDirection(DividingSegment *current) const
    {
        if (current->is_even)
            return pointingRightOfSlice(current->line) ? Direction::COUNTER_CLOCKWISE : Direction::CLOCKWISE;
        else
            return pointingRightOfSlice(current->line) ? Direction::CLOCKWISE : Direction::COUNTER_CLOCKWISE;
    }

    static void reverse(Direction &direction)
    {
        if (direction == Direction::CLOCKWISE)
            direction = Direction::COUNTER_CLOCKWISE;
        else
            direction = Direction::CLOCKWISE;
    }

    // This method iterates over the polygon until an intersecting line segment is reached, or the initial line segment is reached.
    DividingSegment &iterate(
        const LineSegment2D *start,
        const Direction direction,
        const std::function<void(const LineSegment2D *)> &callback = std::function<void(const LineSegment2D *)>() // invoked when the line segment is NOT an intersecting segment.
    )
    {
        if (direction == Direction::CLOCKWISE)
            return iterate_helper<RingIterator<LineSegment2D>>(start, callback);
        else
            return iterate_helper<ReverseRingIterator<LineSegment2D>>(start, callback);
    }

    template <class I>
    DividingSegment &iterate_helper(
        const LineSegment2D *start,
        const std::function<void(const LineSegment2D *)> &callback)
    {
        auto it = getIterator<I>(start);
        auto begin = it;
        it++;
        while (it != begin)
        {
            auto it_intersecting = set_intersecting.find(&(*it));
            if (it_intersecting != set_intersecting.end())
            {
                const LineSegment2D *ls = &(*it);
                for (auto &ds : intersecting)
                {
                    if (ds.line == ls)
                        return ds;
                }
                throw std::runtime_error("Logic error -- found segment in set_intersecting but failed to find corresponding DividingSegment");
            }
            else if (callback)
            {
                callback(&(*it));
            }
            it++;
        }
        throw std::runtime_error("Logic error -- failed to find DividingSegment");
    }

    const std::vector<LineSegment2D> *polygonOwningSegment(const LineSegment2D *segment) const
    {
        for (auto &shape : polygons)
        {
            if (&(shape.front()) <= segment && segment <= &(shape.back()))
                return &shape;
        }
        throw std::runtime_error("Logic error -- cannot locate polygon owning line segment");
    }

    DividingSegment &buildNewSegment(const DividingSegment &first, const Direction direction, std::vector<LineSegment2D> &new_polygon, const DividingSegment& initial);
};

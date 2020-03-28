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
    LineSegment2D *line;
    Point2D intersection_point;
    bool assigned{false};
};

enum class Direction
{
    CLOCKWISE = 0,
    COUNTER_CLOCKWISE = 1
};

class Segmenter
{
public:
    Segmenter(const LineSegment2D &slice, std::vector<std::vector<LineSegment2D>> &polygons);
    size_t findIntersections();
    std::vector<std::vector<LineSegment2D>> segmentPolygons();

private:
    template <class I>
    I getIterator(LineSegment2D *segment)
    {
        for (auto &shape : polygons)
        {
            if (segment >= shape.data() && segment < shape.data() + shape.size())
            {
                size_t position = segment - shape.data();
                return I(shape, position);
            }
        }
        throw std::runtime_error("Logic error -- no polygon found containing segment");
    }

    bool isPointLeftOfSlice(const Point2D &p) const
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

    Direction naturalDirection(DividingSegment *current) const
    {
        size_t index = current - intersecting.data();
        if (index % 2 == 0)
            return isPointLeftOfSlice(current->line->first) ? Direction::COUNTER_CLOCKWISE : Direction::CLOCKWISE;
        else
            return isPointLeftOfSlice(current->line->first) ? Direction::CLOCKWISE : Direction::COUNTER_CLOCKWISE;
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
        LineSegment2D *start,
        Direction direction,
        const std::function<void(LineSegment2D *)> &callback = std::function<void(LineSegment2D *)>() // invoked when the line segment is NOT an intersecting segment.
    )
    {
        if (direction == Direction::CLOCKWISE)
            return iterate_helper<RingIterator<LineSegment2D>>(start, callback);
        else
            return iterate_helper<ReverseRingIterator<LineSegment2D>>(start, callback);
    }

    template <class I>
    DividingSegment &iterate_helper(
        LineSegment2D *start,
        const std::function<void(LineSegment2D *)> &callback)
    {
        auto it = getIterator<I>(start);
        auto begin = it;
        it++;
        while (it != begin)
        {
            auto it_intersecting = set_intersecting.find(&(*it));
            if (it_intersecting != set_intersecting.end())
            {
                LineSegment2D *ls = &(*it);
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

    const std::vector<LineSegment2D> *polygonOwningSegment(LineSegment2D *segment) const
    {
        for (auto &shape : polygons)
        {
            if (&(shape.front()) <= segment && segment <= &(shape.back()))
                return &shape;
        }
        throw std::runtime_error("Logic error -- cannot locate polygon owning line segment");
    }

    DividingSegment &buildNewSegment(DividingSegment &first, Direction direction, std::vector<LineSegment2D> &new_polygon);

private:
    const LineSegment2D &slice;
    std::vector<std::vector<LineSegment2D>> &polygons;
    const bool horizontal_slice;
    const bool vertical_slice;
    std::set<LineSegment2D *> set_intersecting;
    std::vector<DividingSegment> intersecting;
};

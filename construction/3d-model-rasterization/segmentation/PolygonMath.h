#pragma once

#include "LineSegment.h"
#include "RingIterator.h"
#include <vector>
#include <assert.h>
#include <iostream>
#include <functional>
#include <map>

struct DividingSegment
{
    bool is_even; // If ALL dividing segments spanned the slice (see spansSlice), this
                  // would alternate true/false always.  However, the universe of possibilities
                  // allows for a line segment to begin or end on a dividing slice, which allows
                  // two dividing segments in a row to be even or odd.
    Point2D intersection_point;
    const LineSegment2D *line;

    bool spansSlice() const
    {
        return line->first != intersection_point && line->second != intersection_point;
    }
};

const bool Y_AXIS_INCREASES_DOWNWARD = true; // true for computer graphics, false for standard cartesian coordinate system.

// This is a base class for polygon division and combining operations.
// Perhaps 'PolygonMath' isn't the best name.

class PolygonMath
{
public:
    PolygonMath(const std::vector<std::vector<LineSegment2D>> &polygons)
        : polygons(polygons)
    {
    }

    static Direction calculateDirection(const std::vector<LineSegment2D> &poly); // great candidate to become part of Polygon2D

    const std::vector<DividingSegment> &intersections() { return intersecting; }

protected:
    const std::vector<std::vector<LineSegment2D>> &polygons;
    std::map<const LineSegment2D *, std::vector<DividingSegment *>> set_intersecting;
    std::vector<DividingSegment> intersecting;

    virtual void addIntersection(const LineSegment2D *segment, Point2D location)
    {
        intersecting.push_back({true, location, segment}); // is even/odd will be determined later
        set_intersecting.insert({segment, {}});
    }

    // call this method after sorting intersecting.
    void updateIntersectingSetPointers()
    {
        for (auto &pair : set_intersecting)
        {
            assert(pair.second.empty());
        }
        for (auto &intersection : intersecting)
        {
            auto &v = set_intersecting[intersection.line];
            v.push_back(&intersection);
        }
    }

protected:
    template <class I>
    I getIterator(const LineSegment2D *segment)
    {
        for (auto &shape : const_cast<std::vector<std::vector<LineSegment2D>> &>(polygons))
        {
            if (segment >= shape.data() && segment < shape.data() + shape.size())
            {
                size_t position = segment - shape.data();
                return I(shape, position);
            }
        }
        throw std::runtime_error("Logic error -- no polygon found containing segment");
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
        while (true)
        {
            const LineSegment2D *ls = &(*it);
            auto it_intersecting = set_intersecting.find(ls);
            if (it_intersecting != set_intersecting.end())
            {
                auto &vec = it_intersecting->second;
                if (vec.size() == 0)
                    throw std::runtime_error("Logic error -- found segment in set_intersecting but failed to find corresponding DividingSegment");
                else if (vec.size() > 1)
                    throw std::runtime_error("Logic error -- found segment in set_intersecting but failed to find exactly 1 corresponding DividingSegment");
                return *vec[0];
            }
            else if (callback)
            {
                callback(ls);
            }
            if (it == begin)
                throw std::runtime_error("Logic error -- failed to find DividingSegment");
            it++;
        }
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
};

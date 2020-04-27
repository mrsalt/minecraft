#pragma once

#include "PolygonMath.h"

void slicePolygons(const LineSegment2D &slice, std::vector<std::vector<LineSegment2D>> &polygons);

class SliceData : public PolygonMath
{
public:
    SliceData(const LineSegment2D &slice, const std::vector<std::vector<LineSegment2D>> &polygons);

    bool polygonsAreIntersectedBySlice() { return !intersections().empty(); }

    std::vector<std::vector<LineSegment2D>> segmentPolygons();

public:
    const LineSegment2D slice;
    const bool horizontal_slice;
    const bool vertical_slice;

private:
    void handleSegmentsOnSlice(std::vector<const LineSegment2D *> &segmentsOnSlice);

    bool onSlice(const Point2D &p) const
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
            if (Y_AXIS_INCREASES_DOWNWARD)
                return line->second.y > line->first.y;
            else
                return line->second.y < line->first.y;
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

    DividingSegment &buildNewSegment(const DividingSegment &first, const Direction direction, std::vector<LineSegment2D> &new_polygon, const DividingSegment &initial);
};

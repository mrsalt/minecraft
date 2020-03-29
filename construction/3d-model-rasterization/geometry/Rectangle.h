#pragma once
#include "Point.h"
#include <algorithm>

class Rectangle
{
public:
    Rectangle()
        : min{ 0.0,0.0 }
        , max{ 0.0,0.0 }
    {
    }

    Rectangle(const Point2D &a, const Point2D &b)
        : min{std::min<double>(a.x, b.x), std::min<double>(a.y, b.y)}, max{std::max<double>(a.x, b.x), std::max<double>(a.y, b.y)}
    {
    }

    bool overlaps(const Rectangle &other) const
    {
        if (other.max.x < min.x || other.min.x >= max.x)
            return false;
        if (other.max.y < min.y || other.min.y >= max.y)
            return false;
        return true;
    }

    Point2D min;
    Point2D max;
};
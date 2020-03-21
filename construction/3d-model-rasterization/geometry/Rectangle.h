#pragma once
#include "Point.h"
#include <algorithm>

class Rectangle
{
public:
    Rectangle(const Point2D & a, const Point2D & b)
    : min{std::min<double>(a.x, b.x), std::min<double>(a.y, b.y)}
    , max{std::max<double>(a.x, b.x), std::max<double>(a.y, b.y)}
    {
    }

    const Point2D min;
    const Point2D max;

};
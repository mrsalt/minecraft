#pragma once
#include "Point.h"

struct IntSize
{
    IntSize(size_t width, size_t height, size_t depth)
        : width(width)
        , height(height)
        , depth(depth)
    {
    }

    IntSize(Point p)
        : width((size_t)p.x)
        , height((size_t)p.y)
        , depth((size_t)p.z)
    {
    }

    size_t width;
    size_t height;
    size_t depth;

    bool operator!=(const IntSize& rhs) const
    {
        return width != rhs.width ||
            height != rhs.height ||
            depth != rhs.depth;
    }

    IntSize& operator += (const IntSize& rhs)
    {
        width += rhs.width;
        height += rhs.height;
        depth += rhs.depth;
        return *this;
    }
};

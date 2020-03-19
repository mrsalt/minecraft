#pragma once
#include "Point.h"

#include <ostream>
#include <string>

struct Size
{
    Size()
        : width(0), height(0), depth(0)
    {
    }

    explicit Size(const Point &p)
        : width(p.x), height(p.y), depth(p.z)
    {
    }

    double width{0};
    double height{0};
    double depth{0};
    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &out, const Size &);
};

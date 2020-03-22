#pragma once
#include <ostream>
#include <string>

struct Point
{
    double x;
    double y;
    double z;
    Point operator-(const Point &rhs) const;
    Point operator+(const Point &rhs) const;
    Point operator*(double scalar) const;
    Point &operator+=(const Point &rhs);
    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &out, const Point &);
};

struct Point2D
{
    double x;
    double y;
    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &out, const Point2D &);
};

typedef bool (*PointComparisonMethod)(const Point &a, const Point &b);

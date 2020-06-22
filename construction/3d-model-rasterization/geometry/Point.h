#pragma once
#include <ostream>
#include <string>
#include <assert.h>

struct Point
{
    double x;
    double y;
    double z;
    Point operator-(const Point &rhs) const;
    Point operator+(const Point &rhs) const;
    Point operator*(double scalar) const;
    Point operator/(double scalar) const;
    Point &operator+=(const Point &rhs);

    bool operator<(const Point &rhs) const
    {
        if (abs(rhs.x - x) < FLT_EPSILON)
        {
            if (abs(rhs.y - y) < FLT_EPSILON)
            {
                if (abs(rhs.z - z) < FLT_EPSILON)
                {
                    return false;
                }
                return z < rhs.z;
            }
            return y < rhs.y;
        }
        return x < rhs.x;
    }

    // not the same as actual distance because no square root is done
    double relativeDistance(const Point &other) const
    {
        double xDelta = other.x - x;
        double yDelta = other.y - y;
        double zDelta = other.z - z;
        return xDelta * xDelta + yDelta * yDelta + zDelta * zDelta;
    }

    bool isReallyCloseTo(const Point &other) const
    {
        return relativeDistance(other) <= FLT_EPSILON;
    }

    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &out, const Point &);
};

struct Point2D
{
    double x;
    double y;
    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &out, const Point2D &);

    // not the same as actual distance because no square root is done
    double relativeDistance(const Point2D &other) const
    {
        double xDelta = other.x - x;
        double yDelta = other.y - y;
        return xDelta * xDelta + yDelta * yDelta;
    }

    bool isReallyCloseTo(const Point2D &other) const
    {
        return relativeDistance(other) <= FLT_EPSILON * FLT_EPSILON;
    }

    bool operator<(const Point2D &rhs) const
    {
        if (x < rhs.x)
            return true;
        if (x > rhs.x)
            return false;
        if (y < rhs.y)
            return true;
        if (y > rhs.y)
            return false;
        return false;
    }

    bool operator>(const Point2D& rhs) const
    {
        if (x > rhs.x)
            return true;
        if (x < rhs.x)
            return false;
        if (y > rhs.y)
            return true;
        if (y < rhs.y)
            return false;
        return false;
    }

    bool operator==(const Point2D &rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const Point2D &rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }

    Point2D &operator+=(const Point2D &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Point2D operator+(const Point2D& rhs) const
    {
        return { x + rhs.x, y + rhs.y };
    }

    Point2D operator-(const Point2D& rhs) const
    {
        return { x - rhs.x, y - rhs.y };
    }

    Point2D operator/(const double scale) const
    {
        return { x / scale, y / scale };
    }

    Point2D operator/(const Point2D &rhs) const
    {
        double nx, ny;
        if (abs(rhs.x) > FLT_EPSILON)
            nx = x / rhs.x;
        else if (abs(x) < FLT_EPSILON)
            nx = 0;
        else
            throw std::runtime_error("Logic error -- if rhs.x == 0 expecting x to also be 0");
        if (abs(rhs.y) > FLT_EPSILON)
            ny = y / rhs.y;
        else if (abs(y) < FLT_EPSILON)
            ny = 0;
        else
            throw std::runtime_error("Logic error -- if rhs.y == 0 expecting y to also be 0");
        return { nx, ny };
    }

    Point2D operator*(double scalar) const
    {
        return { x * scalar, y * scalar };
    }

    double scalarDistanceFromOrigin() const
    {
        if (x == 0.)
            return y;
        if (y == 0.)
            return x;
        return sqrt(relativeDistance({ 0.,0. }));
    }

};

typedef bool (*PointComparisonMethod)(const Point &a, const Point &b);

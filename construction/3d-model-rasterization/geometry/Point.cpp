#include "Point.h"
#include "string_format.h"

using namespace std;

Point Point::operator-(const Point &rhs) const
{
    return {x - rhs.x, y - rhs.y, z - rhs.z};
}

Point Point::operator+(const Point &rhs) const
{
    return {x + rhs.x, y + rhs.y, z + rhs.z};
}

Point Point::operator*(double scalar) const
{
    return {x * scalar, y * scalar, z * scalar};
}

Point &Point::operator+=(const Point &rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

string Point::toString() const
{
    return format("(x: %.3f, y: %.3f, z: %.3f)", x, y, z);
}

ostream &operator<<(ostream &out, const Point &p)
{
    out << p.toString();
    return out;
}

string Point2D::toString() const
{
    return format("(x: %.3f, y: %.3f)", x, y);
}

ostream &operator<<(std::ostream &out, const Point2D &p)
{
    out << p.toString();
    return out;
}

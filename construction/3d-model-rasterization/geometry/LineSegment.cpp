#include "LineSegment.h"
#include "ModelBuilder.h"
#include "string_format.h"
#include <iostream>

using namespace std;

LineSegment::LineSegment()
    : pair<Point *, Point *>{nullptr, nullptr}
{
}

LineSegment::LineSegment(Point *a, Point *b)
    : pair<Point *, Point *>{a, b}
{
}

LineSegment::operator bool() const
{
    return first != nullptr && second != nullptr;
}

LineSegment LineSegment::create(Point *a, Point *b, PointComparisonMethod lessThan)
{
    if (lessThan(*a, *b))
        return LineSegment(a, b);
    else
        return LineSegment(b, a);
}

void LineSegment::print(ostream &out, const Point *first, bool printCoordinates) const
{
    out << "#" << (this->first - first) + 1 << " - #" << (this->second - first) + 1;
    if (printCoordinates)
        out << " " << *this->first << ", " << *this->second;
}

void print(ostream &out, const vector<LineSegment> &list, const ModelBuilder &source)
{
    out << "list @" << &list << '\n';
    for (auto &sp : list)
    {
        out << "  ";
        sp.print(cout, source.points.data());
        out << '\n';
    }
}

string LineSegment2D::toString() const
{
    // not printing bounds or surface
    return format("%s - %s", first.toString().c_str(), second.toString().c_str());
}

ostream &operator<<(ostream &out, const LineSegment2D &ls)
{
    out << ls.toString();
    return out;
}

// Credit for this goes to Gavin:
// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect?noredirect=1&lq=1
//   It is based on an algorithm in Andre LeMothe's "Tricks of the Windows Game Programming Gurus". It's not dissimilar
//   to some of the algorithm's in other answers (e.g. Gareth's). LeMothe then uses Cramer's Rule (don't ask me) to solve
//   the equations themselves.

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
bool get_line_intersection(const LineSegment2D &a, const LineSegment2D &b, Point2D &intersection, bool &online)
{
    double s1_x = a.second.x - a.first.x;
    double s1_y = a.second.y - a.first.y;
    double s2_x = b.second.x - b.first.x;
    double s2_y = b.second.y - b.first.y;

    double divisor = (-s2_x * s1_y + s1_x * s2_y);
    if (divisor == 0)
    {
        online = true;
        return true;
    }

    double s = (-s1_y * (a.first.x - b.first.x) + s1_x * (a.first.y - b.first.y)) / divisor;
    double t =  (s2_x * (a.first.y - b.first.y) - s2_y * (a.first.x - b.first.x)) / divisor;

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        intersection.x = a.first.x + (t * s1_x);
        intersection.y = a.first.y + (t * s1_y);
        return true;
    }

    return false; // No collision
}

bool LineSegment2D::intersects(const LineSegment2D &segment, Point2D &intersection, bool &online) const
{
    if (!bounds.overlaps(segment.bounds))
        return false;
    return get_line_intersection(*this, segment, intersection, online);
}

#include "LineSegment.h"
#include "ModelBuilder.h"
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
    out << "list @" << &list << endl;
    for (auto &sp : list)
    {
        out << "  ";
        sp.print(cout, source.points.data());
        out << endl;
    }
}

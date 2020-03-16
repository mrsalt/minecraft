#pragma once
#include <string>
#include <Color.h>

struct Size;
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

class ModelConsumer
{
public:
    virtual void verticeRead(const Point &) = 0;
    virtual void polygonRead(int vertices[], int countVertices) = 0;
    virtual void setCurrentColor(const Color &) = 0;
};

class ModelReader
{
public:
    virtual bool parse(const std::string &, ModelConsumer &) = 0;
};
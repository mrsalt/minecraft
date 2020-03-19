#pragma once
#include <string>
#include <Color.h>
#include <Point.h>



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
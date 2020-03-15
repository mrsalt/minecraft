#pragma once
#include <string>

class ModelConsumer
{
public:
    virtual void verticeRead(double x, double y, double z) = 0;
    virtual void polygonRead(int vertices[], int countVertices) = 0;
    virtual void setCurrentColor(int r, int g, int b) = 0;
};

class ModelReader
{
public:
    virtual bool parse(const std::string &, ModelConsumer &) = 0;
};
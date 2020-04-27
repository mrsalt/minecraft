#pragma once
#include "IntSize.h"
#include "ModelReader.h"
#include <vector>

class CubeData
{
public:
    bool interior{false}; // denotes if the min position of this cube is inside the model or exterior
    std::vector<double> x_intersections; // locations along x axis where model surface intersects
    std::vector<double> y_intersections; // locations along y axis where model surface intersects
    std::vector<double> z_intersections; // locations along z axis where model surface intersects
};

class CubeModel
{
public:
    CubeModel(const IntSize &dimensions)
        : size(dimensions)
        , cubes_per_layer{(size.width + 1) * (size.depth + 1)}
        , cubes_per_row{size.width + 1}
    {
        size_t vertices = cubes_per_layer * size.height;
        data = new CubeData*[vertices];
        memset(data, 0, sizeof(CubeData*) * vertices);
    }

    inline CubeData * cubeAt(const IntSize & p) { return data[p.height * cubes_per_layer + p.depth * cubes_per_row + p.width]; }
    bool isEmpty(const IntSize & p) { return cubeAt(p) == nullptr; }

private:
    const IntSize size;
    const size_t cubes_per_layer;
    const size_t cubes_per_row;
    CubeData ** data;
};
#pragma once
#include "IntSize.h"
#include "ModelReader.h"
#include <vector>
#include <functional>

class CubeData
{
public:
    uint8_t interior{0}; // denotes if the min position of this cube is inside the model or exterior

    struct Extent
    {
        uint8_t start{0};
        uint8_t end{0};

        void update(double min, double max)
        {
            start = (uint8_t)(min * UINT8_MAX);
            end = (uint8_t)(max * UINT8_MAX);
        }
    };

    Extent x;
    Extent y;
    Extent z;
};

class CubeModel
{
public:
    CubeModel(const IntSize &dimensions)
        : size(dimensions)
        , cubes_per_layer{(size.width + 1) * (size.depth + 1)}
        , cubes_per_row{size.width + 1}
        , non_empty_cubes{ 0 }
    {
        size_t vertices = cubes_per_layer * (size.height + 1);
        data = new CubeData*[vertices];
        memset(data, 0, sizeof(CubeData*) * vertices);
    }

    ~CubeModel()
    {
        size_t vertices = cubes_per_layer * (size.height + 1);
        for (size_t i = 0; i < vertices; i++) {
            CubeData* e = data[i];
            if (e) delete e;
        }
        delete[] data;
    }

    inline size_t offset(const IntSize& p) {
        assert(p.height >= 0 && p.height <= size.height);
        assert(p.width >= 0 && p.width <= size.width);
        assert(p.depth >= 0 && p.depth <= size.depth);
        return p.height* cubes_per_layer + p.depth * cubes_per_row + p.width;
    }

    inline CubeData * cubeAt(const IntSize & p) {
        return data[offset(p)];
    }

    inline const CubeData* nextCubeAlongXAxis(const CubeData** c)
    {
        return *(c + 1);
    }

    inline const CubeData* nextCubeAlongYAxis(const CubeData** c)
    {
        return *(c + cubes_per_layer);
    }

    inline const CubeData* nextCubeAlongZAxis(const CubeData** c)
    {
        return *(c + cubes_per_row);
    }

    bool isEmpty(const IntSize & p) {
        return cubeAt(p) == nullptr;
    }

    CubeData& get(const IntSize& p) {
        size_t o = offset(p);
        CubeData* e = data[o];
        if (e) return *e;
        data[o] = new CubeData();
        non_empty_cubes++;
        return *data[o];
    }

    IntSize getSize() { return size; }

    void iterate(const std::function<void(const IntSize & start, const IntSize & end, const IntSize & increment, const CubeData** firstCube)>& callback);

private:
    const IntSize size;
    const size_t cubes_per_layer;
    const size_t cubes_per_row;
    CubeData ** data;
    size_t non_empty_cubes;
};
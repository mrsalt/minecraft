#include "CubeModel.h"

using namespace std;

void CubeModel::iterate(const function<void(const IntSize& start, const IntSize& end, const IntSize& increment, const CubeData** firstCube)>& callback)
{
    IntSize increment{ 1, 0, 0 };
    for (size_t y = 0; y < size.height; y++) {
        for (size_t z = 0; z < size.depth; z++) {
            IntSize start(0, y, z);
            IntSize end(size.width, y, z);
            const CubeData** ptr = (const CubeData**)data + offset(start);
            callback(start, end, increment, ptr);
        }
    }
}

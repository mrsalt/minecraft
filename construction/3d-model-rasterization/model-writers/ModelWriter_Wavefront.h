#pragma once
#include <string>
#include <fstream>
#include "ModelBuilder.h"

class ModelWriter_Wavefront
{
public:
    ModelWriter_Wavefront(std::string output_dir, std::string file_name);
    void write(const std::vector<Point> & points, const std::vector<std::pair<SolidColorSurface, std::vector<const Polygon*>>> & surfaces);
    void write(const ModelBuilder & model);

private:
    std::string file_name;
    std::ofstream out_obj;
    std::ofstream out_mtl;
    int surface_count{0};
    size_t points_written{0};
};
#include "ModelWriter_Wavefront.h"
#include "string_format.h"
#include <iostream>

using namespace std;

ModelWriter_Wavefront::ModelWriter_Wavefront(std::string output_dir, std::string file_name)
: file_name(file_name)
, out_obj(format("%s/%s.obj", output_dir.c_str(), file_name.c_str()))
, out_mtl(format("%s/%s.mtl", output_dir.c_str(), file_name.c_str()))
{
}

void ModelWriter_Wavefront::write(const vector<Point> & points, const vector<pair<SolidColorSurface, vector<const Polygon*>>> & surfaces)
{
    if (!out_obj)
    {
        cerr << "Failed to open " << file_name << ".obj" << endl;
        return;
    }
    if (points_written == 0)
    {
        out_obj << "#  Wavefront 3d Model Format\n";
        out_obj << '\n';
        out_obj << "mtllib ./" << file_name << ".mtl\n";
    }
    out_obj << '\n';
    out_obj << "g\n";
    for (auto & p : points)
    {
        out_obj << "v " << p.x << " " << p.y << " " << p.z << '\n';
    }
    out_obj << '\n';
    for (auto & pair : surfaces)
    {
        out_obj << "g region_" << surface_count << '\n';
        out_obj << "usemtl color_" << surface_count << '\n';
        out_obj << "s " << (surface_count + 1) << '\n';

        out_mtl << "newmtl color_" << surface_count << '\n';
        out_mtl << "Ka " << (double)(pair.first.r / 255.0) << " " << (double)(pair.first.g / 255.0) << " " << (double)(pair.first.b / 255.0) << '\n';
        out_mtl << "Kd " << (double)(pair.first.r / 255.0) << " " << (double)(pair.first.g / 255.0) << " " << (double)(pair.first.b / 255.0) << '\n';
        out_mtl << '\n';

        for (auto & poly : pair.second)
        {
            out_obj << "f";
            for (auto & vertex : poly->vertices)
            {
                out_obj << " " << (vertex - points.data()) + 1 + points_written;
            }
            out_obj << '\n';
        }

        surface_count++;
    }
    points_written += points.size();
}
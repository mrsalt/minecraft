#include "Arguments.h"
#include "debug.h"
#include "geometry.h"
#include "ModelWriter_Wavefront.h"
#include <iostream>

using namespace std;

extern const Arguments &args;

void write_debug_model(
    const string &filename,
    const ModelBuilder &source,
    const set<const Polygon *> &unplacedPolygons,
    const Polygon *firstPiece,
    const vector<const Polygon *> placed,
    const Polygon *currentPiece,
    const Polygon *closestPiece,
    double p)
{
    // let's write a 3d model to help visualize the state of things...
    SolidColorSurface gray({100, 100, 100});  // completed sections
    SolidColorSurface blue({50, 50, 150});    // head of current section
    SolidColorSurface white({255, 255, 255}); // current section, middle pieces
    SolidColorSurface green({50, 150, 50});   // current polygon
    SolidColorSurface red({150, 50, 50});     // unplaced polygons
    SolidColorSurface purple({128, 0, 128});  // closest piece

    vector<const Polygon *> vector_unplaced_poly;
    for (auto &up : unplacedPolygons)
    {
        if (up != closestPiece && dist(center(up), center(currentPiece)) < 1.0)
            vector_unplaced_poly.push_back(up);
    }
    vector<pair<SolidColorSurface, vector<const Polygon *>>> debug_surfaces;
    //debug_surfaces.push_back({gray, debug_completed_surfaces});
    debug_surfaces.push_back({blue, {firstPiece}});
    debug_surfaces.push_back({white, {++placed.begin(), placed.end()}});
    debug_surfaces.push_back({green, {currentPiece}});
    debug_surfaces.push_back({red, vector_unplaced_poly});
    if (closestPiece)
        debug_surfaces.push_back({purple, {closestPiece}});

    cout << "Writing 3D model debug.obj to visualize polygons." << endl;
    ModelWriter_Wavefront model_writer(args.output_directory, filename);
    model_writer.write(source.points, debug_surfaces);

    // add rectangle that shows the plane we're trying to intersect.
    ModelBuilder::Statistics stats;
    for (auto &pair : debug_surfaces)
    {
        for (auto &poly : pair.second)
        {
            stats.addPoints(poly->vertices);
        }
    }
    vector<Point> plane_points = {
        {p, stats.min.y, stats.min.z},
        {p, stats.min.y, stats.max.z},
        {p, stats.max.y, stats.max.z},
        {p, stats.max.y, stats.min.z}};
    Polygon plane;
    plane.vertices.push_back(&plane_points[0]);
    plane.vertices.push_back(&plane_points[1]);
    plane.vertices.push_back(&plane_points[2]);
    plane.vertices.push_back(&plane_points[3]);
    vector<pair<SolidColorSurface, vector<const Polygon *>>> debug_plane;
    debug_plane.push_back({gray, {&plane}});
    model_writer.write(plane_points, debug_plane);
}

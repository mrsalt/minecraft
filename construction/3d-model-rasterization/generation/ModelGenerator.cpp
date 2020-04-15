#include "Arguments.h"
#include "debug.h"
#include "ModelGenerator.h"
#include "ModelWriter_Wavefront.h"
#include "PolygonSegmentation.h"
#include "string_format.h"
#include "cairo-util.h"

#include <assert.h>
#include <map>
#include <set>

using namespace std;

extern const Arguments &args;

pair<bool, LineSegment> areAdjacent(const Polygon &first, const Polygon &second, PointComparisonMethod aPrecedesB)
{
    Point *common_vertices[2];
    size_t count_common = 0;
    for (auto p1 : first.vertices)
    {
        for (auto p2 : second.vertices)
        {
            if (p1 == p2)
            {
                assert(count_common < 2);
                common_vertices[count_common++] = p1;
            }
        }
    }
    if (count_common == 2)
    {
        return {true, LineSegment::create(common_vertices[0], common_vertices[1], aPrecedesB)};
    }
    else
    {
        return {false, LineSegment()};
    }
}

template <typename TMember>
Point projectLineOntoPlane(const Point &pMin, const Point &pMax, double planeValue, TMember member)
{
    //https://www.youtube.com/watch?v=qVvvy5hsQwk
    Point pVec = pMax - pMin;
    double t = (planeValue - pMin.*member) / pVec.*member;
    return pMin + pVec * t;
}

ModelGenerator::ModelGenerator(const ModelBuilder &source, double model_height)
    : source{source}, stats{source.getStatistics()}, layerDist{stats.extent.height / model_height}
{
}

// Give a list of groups of 3D line segments (polygons) that intersect a plane, determine
// where those line segments intersect with the plane.  Basically convert these 3D line segments
// (edges of 3D polygons) to the points these edges cross a plane and assemble these points
// into 2D line segments (polygons).
template <typename TMember>
vector<vector<LineSegment2D>> intersectSurfacesWithPlane(vector<vector<LineSegment>> &shapes_by_lines, double &p, TMember &member, TMember &_2D_xaxis, TMember &_2D_yaxis)
{
    vector<vector<LineSegment2D>> shapes_2D;
    for (auto &list : shapes_by_lines)
    {
        vector<LineSegment2D> points_in_shape;
        assert(list.size() > 2);
        const LineSegment &last = list.back();

        Point pPlane = projectLineOntoPlane(*last.first, *last.second, p, member);
        // initialize the first 'previousPoint' to be the ending point.
        Point2D previousPoint{pPlane.*_2D_xaxis, pPlane.*_2D_yaxis};

        //print(cout, list, source);
        for (const auto &line : list)
        {
            pPlane = projectLineOntoPlane(*line.first, *line.second, p, member);
            assert(abs(pPlane.*member - p) < 0.000001);
            Point2D point{pPlane.*_2D_xaxis, pPlane.*_2D_yaxis};
            LineSegment2D segment(previousPoint, point, line.surfaceInfo);
            points_in_shape.push_back(segment);
            previousPoint = point;
        }
        assert(points_in_shape.size() > 2);
        shapes_2D.push_back(points_in_shape);
    }
    return shapes_2D;
}

template <typename TMember>
void ModelGenerator::slicePolygonsAlongAxis(
    vector<PolygonBounds> &polyBounds,
    const int layerCount,
    TMember member,
    TMember _2D_xaxis,
    TMember _2D_yaxis,
    PolygonComparisonMethod polyOrderingMethod,
    PointComparisonMethod pointOrderingMethod,
    const Color &color)
{
    std::sort(polyBounds.begin(), polyBounds.end(), polyOrderingMethod);

    const char *axisTitle = (member == &Point::x ? "x" : member == &Point::y ? "y" : member == &Point::z ? "z" : "?");
    cout << "For axis " << axisTitle << " there are " << layerCount << " layers to generate." << endl;

    set<const PolygonBounds *> activePolygons;
    int nextPoly = 0;
    for (int l = 0; l <= layerCount; l++)
    {
        double p = stats.min.*member + l * layerDist;

        // look at activePolygons and see which ones are no longer intersecting the plane:
        vector<const PolygonBounds *> toRemove;
        for (const PolygonBounds *poly : activePolygons)
            if (poly->max.*member < p)
                toRemove.push_back(poly);
        for (const PolygonBounds *poly : toRemove)
            activePolygons.erase(poly);

        // see which polys are now intersecting the plane and should be added to activePolygons:
        while (nextPoly < polyBounds.size())
        {
            PolygonBounds &poly = polyBounds[nextPoly];
            const Point &min = poly.min;
            const Point &max = poly.max;
            if (min.*member > p)
                break;
            if (min.*member <= p && max.*member > p)
                activePolygons.insert(&poly);
            nextPoly++;
        }

        cout << "  at layer #" << l << " (position: " << p << ") there are " << activePolygons.size() << " polygons to place." << endl;
        if (activePolygons.size())
        {
            vector<vector<LineSegment>> shapes_by_lines = placePolygonsInLayer(activePolygons, p, member, pointOrderingMethod);
            cout << "    " << shapes_by_lines.size() << " separate shapes identified (cross section areas)." << endl;
            auto shapes_2D = intersectSurfacesWithPlane(shapes_by_lines, p, member, _2D_xaxis, _2D_yaxis);
            size_t slices;
            slices = (size_t)round((stats.max.*_2D_yaxis - stats.min.*_2D_yaxis) / layerDist) + 1;
            for (size_t n = 0; n < slices; n++)
            {
                Point2D p1{stats.min.*_2D_xaxis, stats.min.*_2D_yaxis + n * layerDist};
                Point2D p2{stats.max.*_2D_xaxis, stats.min.*_2D_yaxis + n * layerDist};
                LineSegment2D slice{p1, p2};
                //drawPolygonsToFile(format("%s-slice-%03d.svg", axisTitle, (int)l), shapes_2D, &slice);
                slicePolygons(slice, shapes_2D);
            }
            slices = (size_t)round((stats.max.*_2D_xaxis - stats.min.*_2D_xaxis) / layerDist) + 1;
            for (size_t n = 0; n < slices; n++)
            {
                Point2D p1{stats.min.*_2D_xaxis + n * layerDist, stats.min.*_2D_yaxis};
                Point2D p2{stats.min.*_2D_xaxis + n * layerDist, stats.max.*_2D_yaxis};
                LineSegment2D slice{p1, p2};
                //drawPolygonsToFile(format("%s-slice-%03d.svg", axisTitle, (int)l), shapes_2D, &slice);
                slicePolygons(slice, shapes_2D);
            }

            if (args.output_cross_model)
            {
                insert_cross_model_polygons(shapes_2D, _2D_xaxis, _2D_yaxis, member, p, color);
            }
        }
    }
}

template <typename TMember>
void ModelGenerator::insert_cross_model_polygons(const vector<vector<LineSegment2D>> &shapes_2D, TMember &_2D_xaxis, TMember &_2D_yaxis, TMember &member, double &p, const Color &color)
{
    for (auto &shape : shapes_2D)
    {
        vector<int> cross_model_polygon;
        for (const LineSegment2D &segment : shape)
        {
            Point point;
            point.*_2D_xaxis = segment.first.x;
            point.*_2D_yaxis = segment.first.y;
            point.*member = p;
            auto it = point_map.find(point);
            int index;
            if (it == point_map.end())
            {
                index = (int)cross_model.points.size();
                point_map.insert({point, index});
                cross_model.verticeRead(point);
            }
            else
            {
                index = it->second;
            }
            cross_model_polygon.push_back(index);
        }
        cross_model_polygons.push_back({cross_model_polygon, color});
    }
}

template <typename TMember>
vector<vector<LineSegment>> ModelGenerator::placePolygonsInLayer(
    const set<const PolygonBounds *> &toPlace,
    const double layerPosition,
    TMember member,
    PointComparisonMethod pointOrderingMethod)
{
    // PUZZLE ALGORITHM
    // Create a set of unplaced polygons, which is a copy of active polygons.
    // Think of these like floating puzzle pieces.
    // 1. Take one unplaced polygon and remove it from the set of unplaced polygons.
    //    Also, note it as the 'first' and 'current' polygon in the current set of
    //    adjacent polygons.
    // 2. Next, consider all other unplaced polygons to find any that border the
    //    'current' polygon.
    //    (Bordering polygons share adjacent vertices).
    // 3. When an adjacent polygon is found, take the vertices that are shared, create
    //    a LineSegment, and add that LineSegment to a vector representing the current
    //    set of adjacent polygons.  Remove that adjacent polygon from the set of
    //    unplaced polygons.  Consider the adjacent polygon as the 'current' polygon
    //    and go back to step 2.
    // 4. When no unplaced polygon can be matched, check and see if the current polygon
    //    borders the 'first' polygon.  Add those shared vertices (spanning pair) to
    //    the vector.  Add this vector to the set of shapes for this layer and go back
    //    to step 1.
    set<const Polygon *> unplacedPolygons;
    for (const auto &poly : toPlace)
        unplacedPolygons.insert(poly->polygon);

    vector<vector<LineSegment>> shapes;
    vector<const Polygon *> debug_completed_surfaces;

    while (unplacedPolygons.size())
    {
        assert(unplacedPolygons.size() > 2); // it takes at least 3 unplaced polygons to make a shape.
        auto it = unplacedPolygons.begin();
        const Polygon *firstPiece = *it;
        unplacedPolygons.erase(it);
        vector<LineSegment> orderedPairs;
        vector<const Polygon *> placed;
        placed.push_back(firstPiece);

        try
        {
            const Polygon *currentPiece = firstPiece;
            const Polygon *closestPiece = nullptr;
            bool firstMatched = false;
            while (!firstMatched)
            {
                //cout << format("Trying to match %s", currentPiece->toString(source).c_str()) << endl;
                LineSegment common_edge;
                for (const Polygon *adjacentPolygon : source.getPolygonsAdjacentTo(currentPiece))
                {
                    auto it = unplacedPolygons.find(adjacentPolygon);
                    //cout << format("  checking if %s is in unplaced polygon list", adjacentPolygon->toString(source).c_str()) << endl;
                    if (it == unplacedPolygons.end())
                        continue;
                    //cout << format("  %s IS in unplaced polygon list", adjacentPolygon->toString(source).c_str()) << endl;
                    auto pair = areAdjacent(*currentPiece, *adjacentPolygon, pointOrderingMethod);
                    if (pair.first)
                    {
                        //cout << format("  found adjacent polygon %s", adjacentPolygon->toString(source).c_str()) << endl;
                        closestPiece = adjacentPolygon;
                        // common edge spans the plane:
                        if (pair.second.first->*member <= layerPosition &&
                            pair.second.second->*member > layerPosition)
                        {
                            common_edge = pair.second;
                            common_edge.surfaceInfo = currentPiece->surface;
                            currentPiece = adjacentPolygon;
                            placed.push_back(currentPiece);
                            unplacedPolygons.erase(it);
                            break;
                        }
                    }
                }
                if (!common_edge)
                {
                    assert(currentPiece != firstPiece);
                    auto ret = areAdjacent(*currentPiece, *firstPiece, pointOrderingMethod);
                    if (!ret.first)
                    {
                        write_debug_model("debug", source, unplacedPolygons, firstPiece, placed, currentPiece, closestPiece, layerPosition);
                        throw model_error(format("Unable to find match for polygon %s", currentPiece->toString(source).c_str()));
                    }
                    //static int file_count = 0;
                    //set<const Polygon*> emptySet;
                    //write_debug_model(format("debug-%.3f-%d", layerPosition, file_count++), source, emptySet, firstPiece, placed, currentPiece, closestPiece, layerPosition);
                    assert(ret.first);
                    firstMatched = true;
                    common_edge = ret.second;
                    common_edge.surfaceInfo = currentPiece->surface;
                }
                orderedPairs.push_back(common_edge);
            }
        }
        catch (const model_error &e)
        {
            cerr << e.what() << '\n';
            throw e;
        }
        shapes.push_back(orderedPairs);
        debug_completed_surfaces.insert(debug_completed_surfaces.end(), placed.begin(), placed.end());
    }
    return shapes;
}

void ModelGenerator::generate()
{
    if (stats.count_vertices == 0 || stats.count_polygons == 0)
    {
        throw model_error(format("Error: invalid model.  vertices and polygons must be > 0.  (vertice count: %u, polygon count: %u)", stats.count_vertices, stats.count_polygons));
    }
    if (args.show_stats)
    {
        cout << stats;
    }

    vector<PolygonBounds> polyBounds;
    for (const auto &polygon : source.polygons)
    {
        polyBounds.push_back(PolygonBounds(&polygon));
    }

    slicePolygonsAlongAxis(
        polyBounds, (int)(stats.extent.width / layerDist),
        &Point::x,
        &Point::z,
        &Point::y,
        [](const PolygonBounds &a, const PolygonBounds &b) {
            return a.min.x < b.min.x;
        },
        [](const Point &a, const Point &b) {
            return a.x < b.x;
        },
        {255, 100, 100});
    slicePolygonsAlongAxis(
        polyBounds, (int)(stats.extent.height / layerDist),
        &Point::y,
        &Point::x,
        &Point::z,
        [](const PolygonBounds &a, const PolygonBounds &b) {
            return a.min.y < b.min.y;
        },
        [](const Point &a, const Point &b) {
            return a.y < b.y;
        },
        {100, 255, 100});
    slicePolygonsAlongAxis(
        polyBounds, (int)(stats.extent.depth / layerDist),
        &Point::z,
        &Point::x,
        &Point::y,
        [](const PolygonBounds &a, const PolygonBounds &b) {
            return a.min.z < b.min.z;
        },
        [](const Point &a, const Point &b) {
            return a.z < b.z;
        },
        {100, 100, 255});

    if (args.output_cross_model)
    {
        for (auto &poly : cross_model_polygons)
        {
            cross_model.setCurrentColor(poly.second);
            cross_model.polygonRead(poly.first.data(), (int)poly.first.size());
        }
        ModelWriter_Wavefront writer(args.output_directory, "cross-model");
        writer.write(cross_model);
    }
}
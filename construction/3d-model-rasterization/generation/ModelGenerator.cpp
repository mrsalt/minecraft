#include "debug.h"
#include "ModelGenerator.h"
#include "ModelWriter_Wavefront.h"
#include "Arguments.h"
#include "string_format.h"

#include <assert.h>
#include <map>
#include <stdexcept>

using namespace std;

extern const Arguments &args;

class model_error : public runtime_error
{
public:
    model_error(const string &message)
        : runtime_error(message)
    {
    }
};

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
    : modelHeight{model_height}
{
}

template <typename TMember>
void ModelGenerator::slicePolygonsAlongAxis(
    vector<PolygonBounds> &polyBounds,
    const double layerDist,
    const int layerCount,
    const ModelBuilder &source,
    const ModelBuilder::Statistics &stats,
    TMember member,
    TMember _2D_xaxis,
    TMember _3D_yaxis,
    PolygonComparisonMethod polyOrderingMethod,
    PointComparisonMethod pointOrderingMethod)
{
    std::sort(polyBounds.begin(), polyBounds.end(), polyOrderingMethod);

    cout << "For axis " << (member == &Point::x ? "x" : member == &Point::y ? "y" : member == &Point::z ? "z" : "?")
         << " there are " << layerCount << " layers to generate." << endl;

    set<PolygonBounds *> activePolygons;
    int nextPoly = 0;
    for (int l = 0; l <= layerCount; l++)
    {
        double p = stats.min.*member + l * layerDist;

        // look at activePolygons and see which ones are no longer intersecting the plane:
        vector<PolygonBounds *> toRemove;
        for (PolygonBounds *poly : activePolygons)
            if (poly->max.*member < p)
                toRemove.push_back(poly);
        for (PolygonBounds *poly : toRemove)
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
            set<const Polygon *> unplacedPolygons;
            for (const auto &poly : activePolygons)
                unplacedPolygons.insert(poly->polygon);
            set<vector<LineSegment>> shapes = placePolygonsInLayer(unplacedPolygons, p, source, member, pointOrderingMethod);
            cout << "    " << shapes.size() << " separate shapes identified (cross section areas)." << endl;
            for (auto &list : shapes)
            {
                vector<int> cross_model_points;
                //print(cout, list, source);
                for (const auto &line : list)
                {
                    Point pPlane = projectLineOntoPlane(*line.first, *line.second, p, member);
                    assert(abs(pPlane.*member - p) < 0.000001);

                    if (args.output_cross_model)
                    {
                        cross_model_points.push_back((int)cross_model.points.size());
                        cross_model.verticeRead(pPlane);
                    }

                    double x = pPlane.*_2D_xaxis;
                    double y = pPlane.*_3D_yaxis;
                    //line.surfaceInfo;
                }

                if (args.output_cross_model)
                {
                    // TODO: break this bad boy up into many smaller polygons
                    cross_model.polygonRead(cross_model_points.data(), (int)cross_model_points.size());
                }
            }
        }
    }
}

template <typename TMember>
set<vector<LineSegment>> ModelGenerator::placePolygonsInLayer(set<const Polygon *> &unplacedPolygons, const double layerPosition, const ModelBuilder &source, TMember member, PointComparisonMethod pointOrderingMethod)
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

    set<vector<LineSegment>> shapes;
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
            std::cerr << e.what() << '\n';
            throw e;
        }
        shapes.insert(orderedPairs);
        debug_completed_surfaces.insert(debug_completed_surfaces.end(), placed.begin(), placed.end());
    }
    return shapes;
}

void ModelGenerator::generate(const ModelBuilder &source)
{
    if (modelHeight < 1.0)
        throw invalid_argument("Model height must be >= 1.0");

    ModelBuilder::Statistics stats = source.getStatistics();
    const double layerDist = stats.extent.height / modelHeight;

    vector<PolygonBounds> polyBounds;
    for (const auto &polygon : source.polygons)
    {
        polyBounds.push_back(PolygonBounds(&polygon));
    }

    Point *point_data = nullptr;
    if (args.output_cross_model)
    {
        cross_model.points.reserve(1000000);
        point_data = cross_model.points.data();
        cross_model.setCurrentColor({255, 100, 100});
    }
    slicePolygonsAlongAxis(
        polyBounds, layerDist, (int)(stats.extent.width / layerDist), source, stats,
        &Point::x,
        &Point::z,
        &Point::y,
        [](const PolygonBounds &a, const PolygonBounds &b) {
            return a.min.x < b.min.x;
        },
        [](const Point &a, const Point &b) {
            return a.x < b.x;
        });
    if (args.output_cross_model)
        cross_model.setCurrentColor({100, 255, 100});
    slicePolygonsAlongAxis(
        polyBounds, layerDist, (int)(stats.extent.height / layerDist), source, stats,
        &Point::y,
        &Point::x,
        &Point::z,
        [](const PolygonBounds &a, const PolygonBounds &b) {
            return a.min.y < b.min.y;
        },
        [](const Point &a, const Point &b) {
            return a.y < b.y;
        });
    if (args.output_cross_model)
        cross_model.setCurrentColor({100, 100, 255});
    slicePolygonsAlongAxis(
        polyBounds, layerDist, (int)(stats.extent.depth / layerDist), source, stats,
        &Point::z,
        &Point::x,
        &Point::y,
        [](const PolygonBounds &a, const PolygonBounds &b) {
            return a.min.z < b.min.z;
        },
        [](const Point &a, const Point &b) {
            return a.z < b.z;
        });
    if (args.output_cross_model)
    {
        if (point_data != cross_model.points.data())
            // TODO: reimplement ModelBuilder so that points can be re-allocated while polygons are being added.
            throw runtime_error("Data for vertices has been re-allocated.  Cannot write cross model.");
        ModelWriter_Wavefront writer(args.output_directory, "cross-model");
        writer.write(cross_model);
    }
}
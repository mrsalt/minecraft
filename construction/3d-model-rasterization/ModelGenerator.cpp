#include "ModelGenerator.h"
#include "model-writers/ModelWriter_Wavefront.h"
#include "Arguments.h"
#include "string_format.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>

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

PolygonBounds::PolygonBounds(const Polygon *polygon)
    : polygon(polygon)
{
    min = max = *polygon->vertices[0];
    for (size_t i = polygon->vertices.size() - 1; i > 0; i--)
    {
        Point &p = *polygon->vertices[i];
        if (p.x < min.x)
            min.x = p.x;
        if (p.x > max.x)
            max.x = p.x;
        if (p.y < min.y)
            min.y = p.y;
        if (p.y > max.y)
            max.y = p.y;
        if (p.z < min.z)
            min.z = p.z;
        if (p.z > max.z)
            max.z = p.z;
    }
}

string PolygonBounds::toString(const ModelBuilder &source) const
{
    return format("%s, bounds: %s - %s", polygon->toString(source).c_str(), min.toString().c_str(), max.toString().c_str());
}

void write_debug_model(
    const string &filename,
    const ModelBuilder &source,
    const set<const Polygon *> &unplacedPolygons,
    const Polygon *firstPiece,
    const vector<const Polygon *> placed,
    const Polygon *currentPiece,
    double p);

struct SpanningPair : pair<Point *, Point *>
{
    SpanningPair()
        : pair<Point *, Point *>{nullptr, nullptr}
    {
    }

    SpanningPair(Point *a, Point *b)
        : pair<Point *, Point *>{a, b}
    {
    }

    operator bool() const
    {
        return first != nullptr && second != nullptr;
    }

    static SpanningPair create(Point *a, Point *b, PointComparisonMethod lessThan)
    {
        if (lessThan(*a, *b))
            return SpanningPair(a, b);
        else
            return SpanningPair(b, a);
    }

    void print(ostream &out, const Point *first, bool printCoordinates = false) const
    {
        out << "#" << (this->first - first) + 1 << " - #" << (this->second - first) + 1;
        if (printCoordinates)
            out << " " << *this->first << ", " << *this->second;
    }
};

vector<SpanningPair> enumerateEdges(const Polygon *polygon, PointComparisonMethod aPrecedesB)
{
    vector<SpanningPair> edges;
    const auto &vertices = polygon->vertices;
    for (size_t i = 0; i < vertices.size() - 1; i++)
        edges.push_back(SpanningPair::create(vertices[i], vertices[i + 1], aPrecedesB));
    edges.push_back(SpanningPair::create(vertices[vertices.size() - 1], vertices[0], aPrecedesB));
    return edges;
}

void print(ostream &out, const vector<SpanningPair> &list, const ModelBuilder &source)
{
    out << "list @" << &list << endl;
    for (auto &sp : list)
    {
        out << "  ";
        sp.print(cout, source.points.data());
        out << endl;
    }
}

pair<bool, SpanningPair> areAdjacent(const Polygon &first, const Polygon &second, PointComparisonMethod aPrecedesB)
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
        return {true, SpanningPair::create(common_vertices[0], common_vertices[1], aPrecedesB)};
    }
    else
    {
        return {false, SpanningPair()};
    }
}

template <typename TMember>
void ModelGenerator::slicePolygonsAlongAxis(
    vector<PolygonBounds> &polyBounds,
    const double layerDist,
    const int layerCount,
    const ModelBuilder &source,
    const ModelBuilder::Statistics &stats,
    TMember member,
    PolygonComparisonMethod polyOrderingMethod,
    PointComparisonMethod pointOrderingMethod)
{
    std::sort(polyBounds.begin(), polyBounds.end(), polyOrderingMethod);

    set<PolygonBounds *> activePolygons;
    int nextPoly = 0;
    for (int l = 0; l <= layerCount; l++)
    {
        double p = stats.min.*member + l * layerDist;
        cout << "Finding intersecting polygons at p = " << p << endl;

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

        if (activePolygons.size())
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
            //    a SpanningPair, and add that SpanningPair to a vector representing the current
            //    set of adjacent polygons.  Remove that adjacent polygon from the set of
            //    unplaced polygons.  Consider the adjacent polygon as the 'current' polygon
            //    and go back to step 2.
            // 4. When no unplaced polygon can be matched, check and see if the current polygon
            //    borders the 'first' polygon.  Add those shared vertices (spanning pair) to
            //    the vector.  Add this vector to the set of shapes for this layer and go back
            //    to step 1.

            set<const Polygon *> unplacedPolygons;
            for (const auto & poly : activePolygons)
                unplacedPolygons.insert(poly->polygon);
            set<vector<SpanningPair>> shapes;
            vector<const Polygon *> debug_completed_surfaces;

            while (unplacedPolygons.size())
            {
                assert(unplacedPolygons.size() > 2); // it takes at least 3 unplaced polygons to make a shape.
                auto it = unplacedPolygons.begin();
                const Polygon *firstPiece = *it;
                unplacedPolygons.erase(it);
                vector<SpanningPair> orderedPairs;
                vector<const Polygon *> placed;
                placed.push_back(firstPiece);

                try
                {
                    const Polygon *currentPiece = firstPiece;
                    const Polygon *closestPiece = nullptr;
                    bool firstMatched = false;
                    while (!firstMatched)
                    {
                        //size_t polygonNumber = currentPiece->polygon - source.polygons.data();
                        //cout << format("Trying to match polygon %s", currentPiece->toString(source).c_str()) << endl;

                        SpanningPair common_edge;

                        for (it = unplacedPolygons.begin(); it != unplacedPolygons.end(); it++)
                        {
                            auto pair = areAdjacent(*currentPiece, **it, pointOrderingMethod);
                            if (pair.first)
                            {
                                closestPiece = *it;
                                // common edge spans the plane:
                                if (pair.second.first->*member <= p &&
                                    pair.second.second->*member > p)
                                {
                                    currentPiece = *it;
                                    placed.push_back(currentPiece);
                                    unplacedPolygons.erase(it);
                                    common_edge = pair.second;
                                    break;
                                }
                            }
                        }
                        if (!common_edge)
                        {
                            auto ret = areAdjacent(*currentPiece, *firstPiece, pointOrderingMethod);
                            if (!ret.first)
                            {
                                write_debug_model("debug", source, unplacedPolygons, firstPiece, placed, currentPiece, closestPiece, p);
                                throw model_error(format("Unable to find match for polygon %s", currentPiece->toString(source).c_str()));
                            }
                            assert(ret.first);
                            firstMatched = true;
                            common_edge = ret.second;
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

            cout << "For layer #" << l << ", " << shapes.size() << " separate shapes identified:" << endl;
            for (auto &list : shapes)
            {
                print(cout, list, source);
            }
        }
    }
}

double dist(const Point &a, const Point &b)
{
    double dx = (a.x - b.x);
    double dy = (a.y - b.y);
    double dz = (a.z - b.z);
    return sqrt(dx * dx + dy * dy + dz * dz);
}

Point center(const Polygon *p)
{
    Point a{0, 0, 0};
    for (auto &v : p->vertices)
        a += *v;
    a.x /= (double)p->vertices.size();
    a.y /= (double)p->vertices.size();
    a.z /= (double)p->vertices.size();
    return a;
}

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
    SolidColorSurface gray(100, 100, 100);  // completed sections
    SolidColorSurface blue(50, 50, 150);    // head of current section
    SolidColorSurface white(255, 255, 255); // current section, middle pieces
    SolidColorSurface green(50, 150, 50);   // current polygon
    SolidColorSurface red(150, 50, 50);     // unplaced polygons
    SolidColorSurface purple(128, 0, 128);  // closest piece

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

    slicePolygonsAlongAxis(
        polyBounds, layerDist, (int)(stats.extent.width / layerDist), source, stats,
        &Point::x,
        [](const PolygonBounds &a, const PolygonBounds &b) {
            return a.min.x < b.min.x;
        },
        [](const Point &a, const Point &b) {
            return a.x < b.x;
        });
}
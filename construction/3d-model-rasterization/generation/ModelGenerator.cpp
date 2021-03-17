#include "Arguments.h"
#include "debug.h"
#include "ModelGenerator.h"
#include "ModelWriter_Wavefront.h"
#include "PolygonSegmentation.h"
#include "PolygonUnion.h"
#include "string_format.h"
#include "cairo-util.h"

#include <assert.h>
#include <map>
#include <set>

using namespace std;

extern const Arguments &args;

size_t commonVertices(const Polygon &first, const Polygon &second, Point* common_vertices[], size_t common_vertice_size)
{
    size_t count_common = 0;
    for (auto p1 : first.vertices)
    {
        for (auto p2 : second.vertices)
        {
            if (p1 == p2)
            {
                assert(count_common < common_vertice_size);
                common_vertices[count_common++] = p1;
            }
        }
    }
    return count_common;
}

template <typename TMember>
pair<bool, LineSegment> areAdjacent(const Polygon& first, const Polygon& second, PointComparisonMethod aPrecedesB, const double layerPosition, TMember member)
{
    const int common_size = 5;
    Point* common_vertices[common_size];
    size_t count = commonVertices(first, second, common_vertices, common_size);
    if (count == 2)
    {
        return { true, LineSegment::create(common_vertices[0], common_vertices[1], aPrecedesB) };
    }
    else if (count < 2)
    {
        return { false, LineSegment() };
    }
    // This is a very rare case, but it does occur in some models.  In a properly constructed model, only
    // two vertices of any two adjacent polygons should be common.  This is not true for all models however.
    // If there are more than two common vertices, we need to find the two common vertices that form a line
    // segment spanning a plane.
    for (size_t i = 0; i < count; i++)
    {
        size_t second = count == i - 1 ? 0 : i + 1;
        LineSegment segment = LineSegment::create(common_vertices[i], common_vertices[second], aPrecedesB);
        if (segment.spansPlane(layerPosition, member))
        {
            return { true, segment };
        }
    }
    return { false, LineSegment() };
}

template <typename TMember>
Point projectLineOntoPlane(const Point &pMin, const Point &pMax, double planeValue, TMember member)
{
    //https://www.youtube.com/watch?v=qVvvy5hsQwk
    Point pVec = pMax - pMin;
    double t = (planeValue - pMin.*member) / pVec.*member;
    return pMin + pVec * t;
}

IntSize ModelGenerator::calculateCubeDimensions()
{
    return {(size_t)ceil(stats.extent.width / layerDist), (size_t)ceil(stats.extent.height / layerDist), (size_t)ceil(stats.extent.depth / layerDist)};
}

ModelGenerator::ModelGenerator(const ModelBuilder &source, double model_height)
    : source{source}, stats{source.getStatistics()}, layerDist{stats.extent.height / model_height}, cubeModel(calculateCubeDimensions())
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

static vector<LineSegment2D> getLineSegments(Point2D p1, Point2D p2, Point2D increment, size_t count)
{
    vector<LineSegment2D> segments;
    for (size_t n = 0; n < count; n++)
    {
        LineSegment2D slice{ p1, p2 };
        segments.push_back(slice);
        p1 += increment;
        p2 += increment;
    }
    return segments;
}

template <typename TMember, typename CMember>
void ModelGenerator::slicePolygonsAlongAxis(
    vector<PolygonBounds> &polyBounds,
    const int layerCount,
    TMember member,
    TMember _2D_xaxis,
    TMember _2D_yaxis,
    CMember cubeMember_xAxis,
    CMember cubeMember_yAxis,
    PolygonComparisonMethod polyOrderingMethod,
    PointComparisonMethod pointOrderingMethod,
    const Color &color)
{
    std::sort(polyBounds.begin(), polyBounds.end(), polyOrderingMethod);

    const char *axisTitle = (member == &Point::x ? "x" : member == &Point::y ? "y" : member == &Point::z ? "z" : "?");
    if (args.verbose_output)
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

        if (args.verbose_output)
            cout << "  at layer #" << l << " (position: " << p << ") there are " << activePolygons.size() << " polygons to place." << endl;
        if (activePolygons.size())
        {
            vector<vector<LineSegment>> shapes_by_lines = placePolygonsInLayer(activePolygons, p, member, pointOrderingMethod);
            if (args.verbose_output)
                cout << "    " << shapes_by_lines.size() << " separate shapes identified (cross section areas)." << endl;
            auto shapes_2D = intersectSurfacesWithPlane(shapes_by_lines, p, member, _2D_xaxis, _2D_yaxis);
            combinePolygons(shapes_2D);

            vector<LineSegment2D> slices1, slices2;
            size_t steps1, steps2;
            {
                size_t slices = (size_t)((stats.max.*_2D_yaxis - stats.min.*_2D_yaxis) / layerDist) + 1;
                steps1 = (size_t)((stats.max.*_2D_xaxis - stats.min.*_2D_xaxis) / layerDist);
                Point2D p1{ stats.min.*_2D_xaxis, stats.min.*_2D_yaxis };
                Point2D p2{ stats.max.*_2D_xaxis, stats.min.*_2D_yaxis };
                Point2D increment{ 0, layerDist };
                slices1 = getLineSegments(p1, p2, increment, slices);
            }
            {
                size_t slices = (size_t)((stats.max.*_2D_xaxis - stats.min.*_2D_xaxis) / layerDist) + 1;
                steps2 = (size_t)((stats.max.*_2D_yaxis - stats.min.*_2D_yaxis) / layerDist);
                Point2D p1{ stats.min.*_2D_xaxis, stats.min.*_2D_yaxis };
                Point2D p2{ stats.min.*_2D_xaxis, stats.max.*_2D_yaxis };
                Point2D increment{ layerDist, 0 };
                slices2 = getLineSegments(p1, p2, increment, slices);
            }

            SegmentDebug sd;
            SegmentDebug* segmentDebug = nullptr;
            if (args.debug_segments)
                segmentDebug = &sd;

            if (segmentDebug)
            {
                vector<const LineSegment2D*> segments;
                for (auto& segment : slices1)
                    segments.push_back(&segment);
                for (auto& segment : slices2)
                    segments.push_back(&segment);
                segments.push_back(nullptr);

                drawPolygonsToFileWithSegments(format("plane-%s-%03d-before-segmentation.svg", axisTitle, (int)l), shapes_2D, segments.data());
            }

            for (LineSegment2D & slice : slices1)
            {
                //drawPolygonsToFile(format("%s-slice-%03d.svg", axisTitle, (int)l), shapes_2D, &slice);
                segmentPolygons(slice, steps1, shapes_2D, member, _2D_xaxis, _2D_yaxis, cubeMember_xAxis, p, segmentDebug);
            }

            for (LineSegment2D& slice : slices2)
            {
                //drawPolygonsToFile(format("%s-slice-%03d.svg", axisTitle, (int)l), shapes_2D, &slice);
                segmentPolygons(slice, steps2, shapes_2D, member, _2D_xaxis, _2D_yaxis, cubeMember_yAxis, p, segmentDebug);
            }

            /*if (segmentDebug)
            {
                vector<const LineSegment2D*> segments;
                for (auto& segment : segmentDebug->segments)
                    segments.push_back(&segment);
                segments.push_back(nullptr);
                drawPolygonsToFileWithSegments(format("plane-%s-%03d-after-segmentation.svg", axisTitle, (int)l), shapes_2D, segments.data());
            }*/

            if (args.output_cross_model)
            {
                insert_cross_model_polygons(shapes_2D, member, _2D_xaxis, _2D_yaxis, p, color);
            }
        }
    }
}

template <typename TMember, typename CMember>
void ModelGenerator::segmentPolygons(const LineSegment2D &slice, size_t steps, vector<vector<LineSegment2D>> &polygons, TMember& member, TMember &_2D_xaxis, TMember &_2D_yaxis, CMember &cubeMember, double layerConstValue, SegmentDebug* segmentDebug)
{
    SliceData segmenter(slice, polygons);

    if (!segmenter.polygonsAreIntersectedBySlice())
        return; // nothing to do

    Point2D v = (slice.second - slice.first);
    v = (v / v) * layerDist;
    bool inside = false;
    size_t intersection = 0;
    const Point2D zero{0., 0.};
    Point2D p = slice.first;
    auto& intersections = segmenter.intersections();
    size_t countIntersections = intersections.size();
    bool isVerticalSlice = abs(slice.first.x - slice.second.x) < 0.000001;
    static SolidColorSurface segmentColor({ 120, 120, 230 }); // light blue
    static SolidColorSurface cubeSegmentColor({ 230, 230, 120 }); // light yellow

    if (segmentDebug)
    {
        const DividingSegment* prev = nullptr;
        for (const DividingSegment& segment : intersections)
        {
            if (prev && prev->is_even)
                segmentDebug->segments.push_back(LineSegment2D(prev->intersection_point, segment.intersection_point, &segmentColor));
            prev = &segment;
        }
    }
    DividingSegment nextSegment = intersections[intersection];
    Point2D nextPoint = nextSegment.intersection_point;
    for (size_t s = 0; s < steps && intersection < countIntersections; s++)
    {
        Point2D cubeEnd = p + v;
        bool insideAtCubeStart = inside;
        Point2D minSolidPoint = p;
        Point2D maxSolidPoint = p;
        while (true)
        {
            double distPastStartOfCube = isVerticalSlice ? nextPoint.y - p.y : nextPoint.x - p.x;
            if (distPastStartOfCube < 0.0)
                break;
            double distToEndOfCube = isVerticalSlice ? cubeEnd.y - nextPoint.y : cubeEnd.x - nextPoint.x;
            if (distToEndOfCube < 0.0)
                break;
            if (nextSegment.is_even != inside)
            {
                if (inside)
                    maxSolidPoint = nextPoint;
                else if (minSolidPoint == p)
                    minSolidPoint = nextPoint;
                inside = nextSegment.is_even;
            }
            intersection++;
            if (intersection == countIntersections)
            {
                assert(!inside);
                break;
            }
            nextSegment = intersections[intersection];
            nextPoint = nextSegment.intersection_point;
        }
        if (inside)
        {
            maxSolidPoint = cubeEnd;
        }
        if (minSolidPoint != maxSolidPoint)
        {
            if (segmentDebug)
            {
                segmentDebug->segments.push_back(LineSegment2D(minSolidPoint, maxSolidPoint, &cubeSegmentColor));
            }
            double minPoint = ((minSolidPoint - p) / v).scalarDistanceFromOrigin();
            double maxPoint = ((maxSolidPoint - p) / v).scalarDistanceFromOrigin();
            assert(minPoint >= 0.0);
            assert(maxPoint < 1.000001);

            Point point;
            point.*_2D_xaxis = p.x;
            point.*_2D_yaxis = p.y;
            point.*member = layerConstValue;

            CubeData &cube = cubeModel.get(IntSize((point - stats.min) / layerDist));
            (cube.*cubeMember).update(minPoint, maxPoint);
        }
        p = cubeEnd;
    }

    if (args.output_cross_model)
    {
        // segmenting polygons by this slice is the process of dividing the polygon into smaller
        // polygons wherever the 'slice' intersects with the polygon.  This is only needed
        // if we want to output a cross section model.  Otherwise, the data available after
        // constructing a 'SliceData' is enough to tell us which cube edges are inside a solid
        // portion of the model or not.
        polygons = segmenter.segmentPolygons();
    }
}

template <typename TMember>
void ModelGenerator::insert_cross_model_polygons(const vector<vector<LineSegment2D>> &shapes_2D, TMember &member, TMember &_2D_xaxis, TMember &_2D_yaxis, double &p, const Color &color)
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
                    auto pair = areAdjacent(*currentPiece, *adjacentPolygon, pointOrderingMethod, layerPosition, member);
                    if (pair.first)
                    {
                        //cout << format("  found adjacent polygon %s", adjacentPolygon->toString(source).c_str()) << endl;
                        closestPiece = adjacentPolygon;
                        // common edge spans the plane:
                        if (pair.second.spansPlane(layerPosition, member))
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
                    auto ret = areAdjacent(*currentPiece, *firstPiece, pointOrderingMethod, layerPosition, member);
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

//static Color getColorForFace(double sliceNumber)
//{
//    Color gray{ 20, 20, 20 };
//    return gray;
//}

static Color getColorForFace(double sliceNumber)
{
    Color gray{ 125, 125, 125 };
    static Color colors[] = {
        {0, 0, 0},       // black          (0)
        {150, 125, 125}, // pink           (5)
        {0, 0, 0},       // black          (10)
        {125, 150, 125}, // light green    (15)
        {0, 0, 0},       // black          (20)
        //{125, 125, 150}, // light blue     (25)
        {100, 100, 255}, // bright blue     (25)
        {0, 0, 0},       // black          (30)
        {150, 150, 125}, // yellow         (35)
        {0, 0, 0},       // black          (40)
        {150, 125, 150}, // purple         (45)
        {0, 0, 0},       // black          (50)
        {125, 50, 50},   // darker red     (55)
        {0, 0, 0},       // black          (60)
        {50, 125, 50},   // darker green   (65)
        {0, 0, 0},       // black          (70)
        {50, 50, 125},   // darker blue    (75)
        {0, 0, 0},       // black          (80)
        {125, 125, 50},  // darker yellow  (85)
        {0, 0, 0},       // black          (90)
        {125, 50, 125},  // darker purple  (95)
        {0, 0, 0},       // black          (100)
    };
    //bool fifthPlane = ((int)(sliceNumber + 0.1)) % 5 == 0;
    //int fifthColor = abs(((int)(sliceNumber + 0.1)) / 5);
    //return fifthPlane ? colors[fifthColor] : gray;
    Color c = { (uint8_t)50, (uint8_t)(50 + (sliceNumber * 4)), (uint8_t)50 };
    //Color c = { (uint8_t)(50 + (sliceNumber * 4)), (uint8_t)50, (uint8_t)(50 + (sliceNumber * 4)) }; // purple
    return c;
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
        &CubeData::z,
        &CubeData::y,
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
        &CubeData::x,
        &CubeData::z,
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
        &CubeData::x,
        &CubeData::y,
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

    ModelBuilder solid_block_model;
    map<Point, int> solid_point_map;
    vector<pair<vector<int>, Color>> solid_model_polygons;

    auto addPoly = [&solid_block_model, &solid_point_map, &solid_model_polygons](const vector<Point> & poly, double sliceNumber)
    {
        vector<int> plane_polygon;
        for (auto& point : poly)
        {
            auto it = solid_point_map.find(point);
            int index;
            if (it == solid_point_map.end())
            {
                index = (int)solid_block_model.points.size();
                solid_point_map.insert({ point, index });
                solid_block_model.verticeRead(point);
            }
            else
            {
                index = it->second;
            }
            plane_polygon.push_back(index);
        }

        solid_model_polygons.push_back({ plane_polygon, getColorForFace(sliceNumber) });
    };

    IntSize size = cubeModel.getSize();
    cubeModel.iterate([addPoly, &size, this](const IntSize &start, const IntSize &end, const IntSize &increment, const CubeData **firstCube) {
        auto cube = firstCube;
        assert(start.height != size.height);
        assert(start.depth != size.depth);
        for (IntSize p = start; p != end; p += increment)
        {
            if (*cube)
            {
                assert(p.width != size.width);
                double x = (double)p.width;
                double y = (double)p.height;
                double z = (double)p.depth;
                //if (x != size.width && z != size.depth)
                    addPoly({ {x, y, z}, {x + 1, y, z}, {x + 1, y, z + 1}, {x, y, z + 1} }, y); // xz plane (bottom)
                //if (x != size.width && y != size.height)
                    addPoly({{x, y, z}, {x + 1, y, z}, {x + 1, y + 1, z}, {x, y + 1, z}}, z); // xy plane (front)
                //if (z != size.depth && y != size.height)
                    addPoly({{x, y, z}, {x, y + 1, z}, {x, y + 1, z + 1}, {x, y, z + 1}}, x); // yz plane (left)
                if (!cubeModel.nextCubeAlongYAxis(cube)) // if next cube in Y direction is empty, add a face.
                    addPoly({ {x, y + 1, z}, {x + 1, y + 1, z}, {x + 1, y + 1, z + 1}, {x, y + 1, z + 1} }, y + 1.0); // xz plane + 1
                if (!cubeModel.nextCubeAlongZAxis(cube)) // if next cube in Z direction is empty, add a face.
                    addPoly({ {x, y, z + 1}, {x + 1, y, z + 1}, {x + 1, y + 1, z + 1}, {x, y + 1, z + 1} }, z + 1.0); // xy plane + 1
                if (!cubeModel.nextCubeAlongXAxis(cube)) // if next cube in X direction is empty, add a face.
                    addPoly({ {x + 1, y, z}, {x + 1, y + 1, z}, {x + 1, y + 1, z + 1}, {x + 1, y, z + 1} }, x + 1.0); // yz plane + 1
            }
            cube++;
        }
    });

    for (auto& poly : solid_model_polygons)
    {
        solid_block_model.setCurrentColor(poly.second);
        solid_block_model.polygonRead(poly.first.data(), (int)poly.first.size());
    }
    ModelWriter_Wavefront writer(args.output_directory, "solid-block-model");
    writer.write(solid_block_model);
}
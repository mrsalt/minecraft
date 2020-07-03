#include "cairo-util.h"
#include "Rectangle.h"
#include <cairo.h>
#include <cairo-svg.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

const double line_width = 1.0;
const double dot_radius = 0.75;
const double desiredSize = 600.0;
const double shrink = 0.0;

Rectangle getBounds(const vector<LineSegment2D> &polygon)
{
    Rectangle bounds = polygon.front().bounds;
    for (const auto &segment : polygon)
    {
        bounds += segment.bounds;
    }
    return bounds;
}

typedef const vector<LineSegment2D> *(*GetNextPolygonCallback)(void *ctx);
Rectangle getBounds(GetNextPolygonCallback callback, void *ctx)
{
    auto poly = callback(ctx);
    Rectangle bounds = getBounds(*poly);
    while (poly = callback(ctx))
    {
        bounds += getBounds(*poly);
    }
    return bounds;
}

void mungeColor(uint32_t &colorMunge, const Point2D &point)
{
    static hash<double> double_hash;
    colorMunge += (uint32_t)double_hash(point.x);
    colorMunge += (uint32_t)double_hash(point.y);
}

void set_color_based_on_polygon(cairo_t *cr, const vector<LineSegment2D> &polygon, uint32_t initial)
{
    uint32_t colorMunge = initial;
    for (auto &segment : polygon)
    {
        mungeColor(colorMunge, segment.first);
        mungeColor(colorMunge, segment.second);
    }
    srand(colorMunge);
    double r = (rand() % 255) / 255.0;
    double g = (rand() % 255) / 255.0;
    double b = (rand() % 255) / 255.0;
    cairo_set_source_rgba(cr, r, g, b, 0.5);
}

Point2D shrink_point(const Point2D &p, const Point2D &center, double half_width, double half_height, double max_shrink)
{
    return {p.x + (center.x - p.x) / half_width * max_shrink,
            p.y + (center.y - p.y) / half_height * max_shrink};
}

vector<LineSegment2D> shrink_polygon(const vector<LineSegment2D> &polygon, const double max_shrink)
{
    if (!max_shrink)
        return polygon;
    Rectangle bounds = getBounds(polygon);
    Point2D center = {(bounds.min.x + bounds.max.x) / 2., (bounds.min.y + bounds.max.y) / 2.};
    vector<LineSegment2D> ret;
    double half_width = (bounds.max.x - bounds.min.x) / 2.;
    double half_height = (bounds.max.y - bounds.min.y) / 2.;
    for (const auto &segment : polygon)
    {
        ret.push_back({shrink_point(segment.first, center, half_width, half_height, max_shrink),
                       shrink_point(segment.second, center, half_width, half_height, max_shrink),
                       segment.surface});
    }
    return ret;
}

void draw_dot(cairo_t *cr, double scale, const Point2D &p, Rectangle &rect, double padding)
{
    cairo_new_sub_path(cr);
    cairo_arc(cr, scale * (p.x - rect.min.x + padding), scale * (p.y - rect.min.y + padding), dot_radius, 0., 2 * M_PI);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);
}

void draw_polygon(const vector<LineSegment2D> &polygon, cairo_t *cr, double scale, Rectangle &rect, double padding)
{
    for (const auto &segment : polygon)
    {
        cairo_move_to(cr, scale * (segment.first.x - rect.min.x + padding), scale * (segment.first.y - rect.min.y + padding));
        cairo_line_to(cr, scale * (segment.second.x - rect.min.x + padding), scale * (segment.second.y - rect.min.y + padding));
    }
    cairo_stroke(cr);
    for (size_t i = 0; i < polygon.size(); i++)
    {
        const auto &segment = polygon[i];
        draw_dot(cr, scale, segment.first, rect, padding);
        if (!segment.second.isReallyCloseTo(polygon[(i == polygon.size() - 1) ? 0 : i + 1].first))
            draw_dot(cr, scale, segment.second, rect, padding);
    }
}

void drawPolygonsToFileHelper(string filename, GetNextPolygonCallback callback, void *ctx, const LineSegment2D **slices)
{
    cairo_surface_t *surface;
    cairo_t *cr;
    Rectangle rect = getBounds(callback, ctx);
    double xExtent = rect.max.x - rect.min.x;
    double yExtent = rect.max.y - rect.min.y;
    double padding;
    double scale;

    if (xExtent > yExtent)
    {
        padding = xExtent / 10.0;
        scale = desiredSize / (xExtent + padding * 2);
    }
    else
    {
        padding = yExtent / 10.0;
        scale = desiredSize / (yExtent + padding * 2);
    }

    surface = cairo_svg_surface_create(filename.c_str(),
                                       desiredSize,
                                       desiredSize);
    cairo_svg_surface_set_document_unit(surface, CAIRO_SVG_UNIT_PX);

    cr = cairo_create(surface);
    cairo_set_line_width(cr, line_width);

    uint32_t count = 0;
    while (auto poly = callback(ctx))
    {
        set_color_based_on_polygon(cr, *poly, count++);
        draw_polygon(shrink_polygon(*poly, shrink / scale), cr, scale, rect, padding);
    }

    if (slices)
    {
        const LineSegment2D* slice;
        while (slice = *slices++)
        {
            if (slice->surface)
            {
                Color c = slice->surface->getRGB();
                cairo_set_source_rgba(cr, c.R / 255.0, c.G / 255.0, c.B / 255.0, 0.5);
            }
            else
            {
                cairo_set_source_rgba(cr, 0, 0, 0, 0.5);
            }
            cairo_move_to(cr, scale * (slice->first.x - rect.min.x + padding), scale * (slice->first.y - rect.min.y + padding));
            cairo_line_to(cr, scale * (slice->second.x - rect.min.x + padding), scale * (slice->second.y - rect.min.y + padding));
            cairo_stroke(cr);

            draw_dot(cr, scale, slice->first, rect, padding);
            draw_dot(cr, scale, slice->second, rect, padding);
        }
    }

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

const vector<LineSegment2D> *GetNextPoly(void *ctx)
{
    auto &p = *reinterpret_cast<pair<int, const vector<vector<LineSegment2D>> *> *>(ctx);
    if (p.first == p.second->size())
    {
        p.first = 0;
        return nullptr;
    }
    return &p.second->at(p.first++);
}

void drawPolygonsToFileWithSegments(string filename, const vector<vector<LineSegment2D>> &polygons, const LineSegment2D **slices)
{
    pair<int, const vector<vector<LineSegment2D>> *> ctx{0, &polygons};
    drawPolygonsToFileHelper(filename, GetNextPoly, &ctx, slices);
}

void drawPolygonsToFile(std::string filename, const std::vector<std::vector<LineSegment2D>>& polygons, const LineSegment2D* slice)
{
    pair<int, const vector<vector<LineSegment2D>>*> ctx{ 0, &polygons };
    const LineSegment2D* slices[] = { slice, nullptr };
    drawPolygonsToFileHelper(filename, GetNextPoly, &ctx, slices);
}

const vector<LineSegment2D> *GetNextPolySingle(void *ctx)
{
    auto &p = *reinterpret_cast<pair<int, const vector<LineSegment2D> *> *>(ctx);
    if (p.first == 1)
    {
        p.first = 0;
        return nullptr;
    }
    p.first++;
    return p.second;
}

void drawPolygonToFile(string filename, const vector<LineSegment2D> &polygon, const LineSegment2D **slices)
{
    pair<int, const vector<LineSegment2D> *> ctx{0, &polygon};
    drawPolygonsToFileHelper(filename, GetNextPolySingle, &ctx, slices);
}
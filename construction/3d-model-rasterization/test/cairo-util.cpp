#include "cairo-util.h"
#include "Rectangle.h"
#include <cairo.h>
#include <cairo-svg.h>

using namespace std;

Rectangle getBounds(const vector<LineSegment2D> polygon)
{
    Rectangle bounds = polygon.front().bounds;
    for (const auto& segment : polygon)
    {
        bounds += segment.bounds;
    }
    return bounds;
}

Rectangle getBounds(const vector<vector<LineSegment2D>> &polygons)
{
    Rectangle bounds = getBounds(polygons.front());
    for (auto &polygon : polygons)
    {
        bounds += getBounds(polygon);
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

Point2D shrink_point(const Point2D& p, const Point2D& center, double half_width, double half_height, double max_shrink)
{
    return { p.x + (center.x - p.x) / half_width * max_shrink,
             p.y + (center.y - p.y) / half_height * max_shrink };
}

vector<LineSegment2D> shrink_polygon(const vector<LineSegment2D>& polygon, const double max_shrink)
{
    Rectangle bounds = getBounds(polygon);
    Point2D center = { (bounds.min.x + bounds.max.x) / 2., (bounds.min.y + bounds.max.y) / 2. };
    vector<LineSegment2D> ret;
    double half_width = (bounds.max.x - bounds.min.x) / 2.;
    double half_height = (bounds.max.y - bounds.min.y) / 2.;
    for (const auto& segment : polygon)
    {
        ret.push_back({
            shrink_point(segment.first, center, half_width, half_height, max_shrink),
            shrink_point(segment.second, center, half_width, half_height, max_shrink),
            segment.surface});
    }
    return ret;
}

void draw_polygon(const vector<LineSegment2D>& polygon, cairo_t* cr, double scale, Rectangle& rect, double padding)
{
    const auto& first = polygon.front().first;
    cairo_move_to(cr, scale * (first.x - rect.min.x + padding), scale * (first.y - rect.min.y + padding));
    for (const auto& segment : polygon)
    {
        cairo_line_to(cr, scale * (segment.second.x - rect.min.x + padding), scale * (segment.second.y - rect.min.y + padding));
    }
    cairo_stroke(cr);
}

void drawPolygonsToFile(string filename, const vector<vector<LineSegment2D>> &polygons)
{
    cairo_surface_t *surface;
    cairo_t *cr;
    Rectangle rect = getBounds(polygons);
    double xExtent = rect.max.x - rect.min.x;
    double yExtent = rect.max.y - rect.min.y;
    double padding;
    double scale;
    const double desiredSize = 600.0;
    const double shrink = 2.0;

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
    cairo_set_line_width(cr, 2);

    uint32_t count = 0;
    for (auto &polygon : polygons)
    {
        set_color_based_on_polygon(cr, polygon, count++);
        draw_polygon(shrink_polygon(polygon, shrink / scale), cr, scale, rect, padding);
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

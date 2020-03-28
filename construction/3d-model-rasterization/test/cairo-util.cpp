#include "cairo-util.h"
#include "Rectangle.h"
#include <cairo.h>
#include <cairo-svg.h>

using namespace std;

void addPoint(Point2D &min, Point2D &max, const Point2D &point)
{
    if (point.x < min.x)
        min.x = point.x;
    if (point.x > max.x)
        max.x = point.x;
    if (point.y < min.y)
        min.y = point.y;
    if (point.y > max.y)
        max.y = point.y;
}

Rectangle getBoundingRectangle(const vector<vector<LineSegment2D>> &polygons)
{
    Point2D min = polygons.front().front().first;
    Point2D max = min;
    for (auto &polygon : polygons)
    {
        for (auto &segment : polygon)
        {
            addPoint(min, max, segment.first);
            addPoint(min, max, segment.second);
        }
    }
    return Rectangle{min, max};
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

void drawPolygonsToFile(string filename, const vector<vector<LineSegment2D>> &polygons)
{
    cairo_surface_t *surface;
    cairo_t *cr;
    Rectangle rect = getBoundingRectangle(polygons);
    double xExtent = rect.max.x - rect.min.x;
    double yExtent = rect.max.y - rect.min.y;
    double padding;
    double scale;
    const double desiredSize = 600.0;
    if (xExtent > yExtent)
    {
        padding = xExtent / 10.0;
        scale = desiredSize / (xExtent + padding * 2);
    }
    else
    {
        padding = yExtent / 10.0;
        scale = desiredSize / (xExtent + padding * 2);
    }

    surface = cairo_svg_surface_create(filename.c_str(),
                                       desiredSize,
                                       desiredSize);
    cairo_svg_surface_set_document_unit(surface, CAIRO_SVG_UNIT_PX);

    cr = cairo_create(surface);
    cairo_set_line_width(cr, 2);

    uint32_t count = 0;
    double maxY = scale * (yExtent + padding * 2); // svg Y origin is backwards from what I'm used too...
    for (auto &polygon : polygons)
    {
        set_color_based_on_polygon(cr, polygon, count++);
        const auto &first = polygon.front().first;
        cairo_move_to(cr, scale * (first.x - rect.min.x + padding), maxY - scale * (first.y - rect.min.y + padding));
        for (const auto &segment : polygon)
        {
            cairo_line_to(cr, scale * (segment.second.x - rect.min.x + padding), maxY - scale * (segment.second.y - rect.min.y + padding));
        }
        cairo_stroke(cr);
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

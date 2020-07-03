#pragma once
#include <string>
#include <vector>
#include "LineSegment.h"

void drawPolygonsToFileWithSegments(std::string filename, const std::vector<std::vector<LineSegment2D>> &polygons, const LineSegment2D **slices = nullptr);
void drawPolygonsToFile(std::string filename, const std::vector<std::vector<LineSegment2D>>& polygons, const LineSegment2D* slices = nullptr);
void drawPolygonToFile(std::string filename, const std::vector<LineSegment2D> &polygon, const LineSegment2D **slices = nullptr);

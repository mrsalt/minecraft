#pragma once
#include <string>
#include <vector>
#include "LineSegment.h"

void drawPolygonsToFile(std::string filename, const std::vector<std::vector<LineSegment2D>> &polygons);

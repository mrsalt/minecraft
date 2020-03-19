#pragma once
#include "ModelBuilder.h"
#include "Polygon.h"
#include <set>
#include <string>
#include <vector>

void write_debug_model(
    const std::string &filename,
    const ModelBuilder &source,
    const std::set<const Polygon *> &unplacedPolygons,
    const Polygon *firstPiece,
    const std::vector<const Polygon *> placed,
    const Polygon *currentPiece,
    const Polygon *closestPiece,
    double p);

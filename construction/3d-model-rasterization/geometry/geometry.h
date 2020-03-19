#pragma once
#include "Point.h"
#include "Polygon.h"

double dist(const Point &a, const Point &b);
Point center(const Polygon *p);

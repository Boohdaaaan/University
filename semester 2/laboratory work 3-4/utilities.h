#ifndef UTILITIES_H
#define UTILITIES_H

#include "line.h"  // Залежність від Line
#include "point.h" // Залежність від Point
#include "quadrilateral.h"

namespace util {
    int orientation(geometry::Point p, geometry::Point q, geometry::Point r);
    bool doIntersect(geometry::Line l1, geometry::Line l2);
    bool doQuadrilateralsIntersect(geometry::Quadrilateral& q1, geometry::Quadrilateral& q2);  // Посилання
}

#endif
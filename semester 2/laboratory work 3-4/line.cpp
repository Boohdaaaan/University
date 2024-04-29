#include "line.h"
#include <stdexcept>

namespace geometry {

    // Constructor
    Line::Line(const Point& point1, const Point& point2) : p1(point1), p2(point2) {
        // Check if the points are distinct
        if (point1.getX() == point2.getX() && point1.getY() == point2.getY()) {
            throw std::invalid_argument("Points cannot be coincident");
        }
    }

    // Accessor Functions
    const Point& Line::getPoint1() const {
        return p1;
    }

    const Point& Line::getPoint2() const {
        return p2;
    }

} // namespace geometry
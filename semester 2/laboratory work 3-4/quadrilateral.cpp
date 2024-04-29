#include "quadrilateral.h"
#include <stdexcept>

namespace geometry {

    // Constructor
    Quadrilateral::Quadrilateral(const Point& point1, const Point& point2, const Point& point3, const Point& point4) :
            p1(point1), p2(point2), p3(point3), p4(point4) {
        // Check if the points form a degenerate quadrilateral
        if ((point1.getX() == point2.getX() && point1.getY() == point2.getY()) ||
            (point1.getX() == point3.getX() && point1.getY() == point3.getY()) ||
            (point1.getX() == point4.getX() && point1.getY() == point4.getY()) ||
            (point2.getX() == point3.getX() && point2.getY() == point3.getY()) ||
            (point2.getX() == point4.getX() && point2.getY() == point4.getY()) ||
            (point3.getX() == point4.getX() && point3.getY() == point4.getY())) {
            throw std::invalid_argument("The points cannot form a degenerate quadrilateral");
            }
    }

    // Accessor Functions
    const Point& Quadrilateral::getPoint1() const {
        return p1;
    }

    const Point& Quadrilateral::getPoint2() const {
        return p2;
    }

    const Point& Quadrilateral::getPoint3() const {
        return p3;
    }

    const Point& Quadrilateral::getPoint4() const {
        return p4;
    }

} // namespace geometry
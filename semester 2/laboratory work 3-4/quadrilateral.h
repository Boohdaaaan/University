#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "point.h"

namespace geometry {

    class Quadrilateral {
    private:
        Point p1, p2, p3, p4;

    public:
        Quadrilateral(const Point& point1, const Point& point2, const Point& point3, const Point& point4);

        const Point& getPoint1() const;
        const Point& getPoint2() const;
        const Point& getPoint3() const;
        const Point& getPoint4() const;
        int orientation;
        bool doIntersect;
        bool doQuadrilateralsIntersect;
    };

} // namespace geometry

#endif // QUADRILATERAL_H
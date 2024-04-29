#ifndef LINE_H
#define LINE_H

#include "point.h"

namespace geometry {

    class Line {
    private:
        Point p1, p2;

    public:
        Line(const Point& point1, const Point& point2);

        const Point& getPoint1() const;
        const Point& getPoint2() const;
    };

} // namespace geometry

#endif // LINE_H
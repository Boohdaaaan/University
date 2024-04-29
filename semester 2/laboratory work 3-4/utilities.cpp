#include "utilities.h"

namespace util {

    int orientation(geometry::Point p, geometry::Point q, geometry::Point r) {
        float val = (q.getY() - p.getY()) * (r.getX() - q.getX()) -
                    (q.getX() - p.getX()) * (r.getY() - q.getY());
        if (val == 0.0) return 0;  // collinear
        return (val > 0) ? 1 : 2;  // clock or counterclockwise
    }

    bool doIntersect(geometry::Line l1, geometry::Line l2) {
        int o1 = orientation(l1.getPoint1(), l1.getPoint2(), l2.getPoint1());
        int o2 = orientation(l1.getPoint1(), l1.getPoint2(), l2.getPoint2());
        int o3 = orientation(l2.getPoint1(), l2.getPoint2(), l1.getPoint1());
        int o4 = orientation(l2.getPoint1(), l2.getPoint2(), l1.getPoint2());

        return o1 != o2 && o3 != o4;
    }

    bool doQuadrilateralsIntersect(geometry::Quadrilateral& q1, geometry::Quadrilateral& q2) {
        geometry::Point points1[] = {q1.getPoint1(), q1.getPoint2(), q1.getPoint3(), q1.getPoint4()};
        geometry::Point points2[] = {q2.getPoint1(), q2.getPoint2(), q2.getPoint3(), q2.getPoint4()};

        for (int i = 0; i < 4; i++) {
            geometry::Line l1 = {points1[i], points1[(i+1) % 4]};
            for (int j = 0; j < 4; j++) {
                geometry::Line l2 = {points2[j], points2[(j+1) % 4]};
                if (doIntersect(l1, l2)) {
                    return true;
                }
            }
        }
        return false;
    }
}
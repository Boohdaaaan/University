#ifndef POINT_H
#define POINT_H

namespace geometry {

    class Point {
    private:
        double x, y;

    public:
        Point(double xCoord, double yCoord);

        double getX() const;
        double getY() const;
    };

} // namespace geometry

#endif // POINT_H

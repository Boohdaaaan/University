#include "point.h"
#include <stdexcept>

namespace geometry {

    // Constructor
    Point::Point(double xCoord, double yCoord) {
        // Check if the coordinates are non-negative
        if (xCoord < 0 || yCoord < 0) {
            throw std::invalid_argument("Coordinates must be positive numbers");
        }

        // Initialize the coordinates of the point
        x = xCoord;
        y = yCoord;
    }

    // Accessor Functions
    double Point::getX() const {
        return x;
    }

    double Point::getY() const {
        return y;
    }

} // namespace geometry
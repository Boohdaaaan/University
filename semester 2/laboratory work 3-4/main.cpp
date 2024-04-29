#include <iostream>
#include "point.h"
#include "quadrilateral.h"
#include "line.h"
#include "utilities.h"


using namespace std;
using namespace geometry;
using namespace util;




int main() {
    // Output author's name and task description
    cout << "Kuznetsov Bohdan K-15\n";
    cout << "Checking the intersection of two quadrilaterals\n";

    try {
        // Get coordinates of vertices of the first quadrilateral from the user
        cout << "Enter the coordinates of vertices of the first quadrilateral (x1, y1), (x2, y2), (x3, y3), (x4, y4): ";
        double x1, y1, x2, y2, x3, y3, x4, y4;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

        // Create the first quadrilateral
        Quadrilateral quad1(Point(x1, y1), Point(x2, y2), Point(x3, y3), Point(x4, y4));

        // Get coordinates of vertices of the second quadrilateral from the user
        cout << "Enter the coordinates of vertices of the second quadrilateral (x1, y1), (x2, y2), (x3, y3), (x4, y4): ";
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

        // Create the second quadrilateral
        Quadrilateral quad2(Point(x1, y1), Point(x2, y2), Point(x3, y3), Point(x4, y4));

        // Check if the quadrilaterals intersect
        if (doQuadrilateralsIntersect(quad1, quad2)) {
            cout << "The quadrilaterals intersect." << endl;
        } else {
            cout << "The quadrilaterals do not intersect." << endl;
        }
        cout << "Bye." << endl;
    } catch (const std::exception& e) {
        // Handle exceptions
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
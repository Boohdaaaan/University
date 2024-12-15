// variant 1
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <random>

class PointGenerator {
public:
    PointGenerator(const std::pair<double, double>& x0,
                   const std::vector<std::pair<double, double>>& base_points)
        : current_point(x0), base_points(base_points), dist(0, base_points.size() - 1)
    {
        rng.seed(std::random_device{}());
    }

    std::pair<double, double> operator()() {
        int k = dist(rng); // Рандомний індекс у проміжку [0, k-1]
        // порахувати y_t = f_{i_t}(y_{t-1}) = (b_k + y_{t-1}) / 2
        double new_x = (base_points[k].first + current_point.first) / 2.0;
        double new_y = (base_points[k].second + current_point.second) / 2.0;
        current_point = std::make_pair(new_x, new_y);
        return current_point;
    }

private:
    std::pair<double, double> current_point;
    std::vector<std::pair<double, double>> base_points;
    std::mt19937 rng;
    std::uniform_int_distribution<> dist;
};

int main() {
    setlocale(LC_ALL, "ukrainian");
    
    // Читаємо вхідні дані з "input.txt"
    std::ifstream infile("input.txt");
    if (!infile) {
        std::cerr << "Error opening input.txt" << std::endl;
        return 1;
    }

    int n;
    infile >> n;

    double x0, y0;
    infile >> x0 >> y0;
    std::pair<double, double> initial_point(x0, y0);

    std::vector<std::pair<double, double>> base_points;
    double x, y;
    while (infile >> x >> y) {
        base_points.emplace_back(x, y);
    }
    infile.close();

    if (base_points.empty()) {
        std::cerr << "Немає базових точок." << std::endl;
        return 1;
    }

    PointGenerator pg(initial_point, base_points);

    // Записуємо "output.txt"
    std::ofstream outfile("output.txt");
    if (!outfile) {
        std::cerr << "Error opening output.txt" << std::endl;
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        std::pair<double, double> point = pg();
        outfile << point.first << " " << point.second << "\n";
    }
    outfile.close();

    return 0;
}
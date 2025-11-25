#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// ============================================================================
// Математичні функції
// ============================================================================

double f(double x) {
    double cosh_2x = cosh(2.0 * x);
    return 8.0 / (cosh_2x * cosh_2x);
}

// ============================================================================
// Структури даних
// ============================================================================

struct SplineSegment {
    double a;
    double b;
    double c;
    double d;
    double x_left;
    double x_right;
};

class NaturalCubicSpline {
private:
    vector<double> nodes_x;
    vector<double> nodes_y;
    vector<SplineSegment> segments;
    
public:
    NaturalCubicSpline(const vector<double>& x_nodes, const vector<double>& y_nodes)
        : nodes_x(x_nodes), nodes_y(y_nodes) {
        buildSpline();
    }
    
    double evaluate(double x) const {
        int n = segments.size();
        for (int i = 0; i < n; i++) {
            if (x >= segments[i].x_left && x <= segments[i].x_right) {
                const SplineSegment& seg = segments[i];
                double dx = x - seg.x_left;
                return seg.a + seg.b * dx + seg.c * dx * dx + seg.d * dx * dx * dx;
            }
        }
        
        if (x < segments[0].x_left) {
            const SplineSegment& seg = segments[0];
            double dx = x - seg.x_left;
            return seg.a + seg.b * dx + seg.c * dx * dx + seg.d * dx * dx * dx;
        } else {
            const SplineSegment& seg = segments[n-1];
            double dx = x - seg.x_right;
            double b_end = seg.b + 2 * seg.c * (seg.x_right - seg.x_left) + 
                          3 * seg.d * (seg.x_right - seg.x_left) * (seg.x_right - seg.x_left);
            return seg.a + seg.b * (seg.x_right - seg.x_left) + 
                   seg.c * (seg.x_right - seg.x_left) * (seg.x_right - seg.x_left) +
                   seg.d * (seg.x_right - seg.x_left) * (seg.x_right - seg.x_left) * (seg.x_right - seg.x_left) +
                   b_end * dx;
        }
    }
    
    int getSegmentCount() const {
        return segments.size();
    }
    
    const SplineSegment& getSegment(int i) const {
        return segments[i];
    }
    
    const vector<double>& getNodesX() const {
        return nodes_x;
    }
    
    const vector<double>& getNodesY() const {
        return nodes_y;
    }
    
private:
    vector<double> solveTridiagonal(const vector<double>& a, 
                                    const vector<double>& b,
                                    const vector<double>& c,
                                    const vector<double>& d) const {
        int n = b.size();
        vector<double> x(n);
        vector<double> alpha(n);
        vector<double> beta(n);
        
        alpha[0] = -c[0] / b[0];
        beta[0] = d[0] / b[0];
        
        for (int i = 1; i < n; i++) {
            double denominator = b[i] + a[i] * alpha[i-1];
            alpha[i] = -c[i] / denominator;
            beta[i] = (d[i] - a[i] * beta[i-1]) / denominator;
        }
        
        x[n-1] = beta[n-1];
        for (int i = n-2; i >= 0; i--) {
            x[i] = alpha[i] * x[i+1] + beta[i];
        }
        
        return x;
    }
    
    void buildSpline() {
        int n = nodes_x.size();
        
        if (n < 2) {
            cerr << "Помилка: потрібно принаймні 2 вузли" << endl;
            return;
        }
        
        vector<double> h(n - 1);
        for (int i = 0; i < n - 1; i++) {
            h[i] = nodes_x[i+1] - nodes_x[i];
            if (h[i] <= 0) {
                cerr << "Помилка: вузли повинні бути в порядку зростання" << endl;
                return;
            }
        }
        
        if (n == 2) {
            SplineSegment seg;
            seg.a = nodes_y[0];
            seg.b = (nodes_y[1] - nodes_y[0]) / h[0];
            seg.c = 0.0;
            seg.d = 0.0;
            seg.x_left = nodes_x[0];
            seg.x_right = nodes_x[1];
            segments.push_back(seg);
            return;
        }
        
        int m = n - 2;
        
        vector<double> lower_diag(m);
        vector<double> main_diag(m);
        vector<double> upper_diag(m);
        vector<double> right_side(m);
        
        for (int i = 0; i < m; i++) {
            int idx = i + 1;
            
            lower_diag[i] = h[idx-1] / (h[idx-1] + h[idx]);
            main_diag[i] = 2.0;
            upper_diag[i] = h[idx] / (h[idx-1] + h[idx]);
            
            double term1 = (nodes_y[idx+1] - nodes_y[idx]) / h[idx];
            double term2 = (nodes_y[idx] - nodes_y[idx-1]) / h[idx-1];
            right_side[i] = 6.0 * (term1 - term2) / (h[idx-1] + h[idx]);
        }
        
        vector<double> M = solveTridiagonal(lower_diag, main_diag, upper_diag, right_side);
        
        vector<double> M_full(n);
        M_full[0] = 0.0;
        for (int i = 0; i < m; i++) {
            M_full[i+1] = M[i];
        }
        M_full[n-1] = 0.0;
        
        segments.clear();
        for (int i = 0; i < n - 1; i++) {
            SplineSegment seg;
            seg.x_left = nodes_x[i];
            seg.x_right = nodes_x[i+1];
            
            seg.a = nodes_y[i];
            seg.b = (nodes_y[i+1] - nodes_y[i]) / h[i] - h[i] * (2.0 * M_full[i] + M_full[i+1]) / 6.0;
            seg.c = M_full[i] / 2.0;
            seg.d = (M_full[i+1] - M_full[i]) / (6.0 * h[i]);
            
            segments.push_back(seg);
        }
    }
};

// ============================================================================
// Виведення результатів
// ============================================================================

void printNodesTable(const vector<double>& x_nodes, const vector<double>& y_nodes) {
    cout << "\n" << string(80, '=') << endl;
    cout << "ТАБЛИЦЯ ВУЗЛІВ ІНТЕРПОЛЯЦІЇ" << endl;
    cout << string(80, '=') << endl;
    cout << setw(8) << "№" << setw(20) << "x" << setw(25) << "f(x)" << endl;
    cout << string(80, '-') << endl;
    
    for (size_t i = 0; i < x_nodes.size(); i++) {
        cout << setw(8) << i+1 
             << setw(20) << fixed << setprecision(10) << x_nodes[i]
             << setw(25) << scientific << setprecision(10) << y_nodes[i] << endl;
    }
    cout << string(80, '-') << endl;
}

void printSplineCoefficients(const NaturalCubicSpline& spline) {
    cout << "\n" << string(80, '=') << endl;
    cout << "КОЕФІЦІЄНТИ СПЛАЙНУ" << endl;
    cout << string(80, '=') << endl;
    cout << setw(5) << "№" << setw(15) << "Інтервал" 
         << setw(15) << "a" << setw(15) << "b" << setw(15) << "c" << setw(15) << "d" << endl;
    cout << string(80, '-') << endl;
    
    for (int i = 0; i < spline.getSegmentCount(); i++) {
        const SplineSegment& seg = spline.getSegment(i);
        cout << setw(5) << i+1 
             << setw(7) << fixed << setprecision(3) << "[" << seg.x_left << ", " << seg.x_right << "]"
             << setw(15) << scientific << setprecision(8) << seg.a
             << setw(15) << seg.b
             << setw(15) << seg.c
             << setw(15) << seg.d << endl;
    }
    cout << string(80, '-') << endl;
}

void outputSVGGraph(const NaturalCubicSpline& spline, 
                    const string& filename = "graph.svg",
                    int num_points = 1000,
                    int width = 1200, int height = 800) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Помилка: не вдалося відкрити файл " << filename << endl;
        return;
    }
    
    const vector<double>& nodes_x = spline.getNodesX();
    const vector<double>& nodes_y = spline.getNodesY();
    double x_min = nodes_x[0];
    double x_max = nodes_x[nodes_x.size() - 1];
    
    // Знаходимо діапазон значень для масштабування
    double y_min = f(x_min);
    double y_max = f(x_min);
    for (int i = 0; i <= num_points; i++) {
        double x = x_min + (x_max - x_min) * i / num_points;
        double fx = f(x);
        double sx = spline.evaluate(x);
        if (fx < y_min) y_min = fx;
        if (fx > y_max) y_max = fx;
        if (sx < y_min) y_min = sx;
        if (sx > y_max) y_max = sx;
    }
    
    // Додаємо невеликий відступ
    double y_range = y_max - y_min;
    if (y_range < 1e-10) y_range = 1.0;
    double y_padding = y_range * 0.1;
    y_min -= y_padding;
    y_max += y_padding;
    y_range = y_max - y_min;
    
    double x_range = x_max - x_min;
    double padding = 80;
    double plot_width = width - 2 * padding;
    double plot_height = height - 2 * padding;
    
    // Функції перетворення координат
    auto x_to_svg = [&](double x) {
        return padding + (x - x_min) / x_range * plot_width;
    };
    auto y_to_svg = [&](double y) {
        return height - padding - (y - y_min) / y_range * plot_height;
    };
    
    // SVG заголовок
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<svg width=\"" << width << "\" height=\"" << height 
         << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    file << "<rect width=\"" << width << "\" height=\"" << height 
         << "\" fill=\"white\"/>\n";
    
    // Сітка
    file << "<g stroke=\"#e0e0e0\" stroke-width=\"1\">\n";
    int grid_lines_x = 10;
    int grid_lines_y = 10;
    for (int i = 0; i <= grid_lines_x; i++) {
        double x_val = x_min + (x_max - x_min) * i / grid_lines_x;
        double x_pos = x_to_svg(x_val);
        file << "<line x1=\"" << x_pos << "\" y1=\"" << padding 
             << "\" x2=\"" << x_pos << "\" y2=\"" << (height - padding) << "\"/>\n";
    }
    for (int i = 0; i <= grid_lines_y; i++) {
        double y_val = y_min + (y_max - y_min) * i / grid_lines_y;
        double y_pos = y_to_svg(y_val);
        file << "<line x1=\"" << padding << "\" y1=\"" << y_pos 
             << "\" x2=\"" << (width - padding) << "\" y2=\"" << y_pos << "\"/>\n";
    }
    file << "</g>\n";
    
    // Побудова графіка f(x)
    file << "<path d=\"M ";
    for (int i = 0; i <= num_points; i++) {
        double x = x_min + (x_max - x_min) * i / num_points;
        double fx = f(x);
        if (i == 0) {
            file << x_to_svg(x) << "," << y_to_svg(fx);
        } else {
            file << " L " << x_to_svg(x) << "," << y_to_svg(fx);
        }
    }
    file << "\" stroke=\"#0066cc\" stroke-width=\"3\" fill=\"none\" stroke-linecap=\"round\" stroke-linejoin=\"round\"/>\n";
    
    // Побудова графіка S(x)
    file << "<path d=\"M ";
    for (int i = 0; i <= num_points; i++) {
        double x = x_min + (x_max - x_min) * i / num_points;
        double sx = spline.evaluate(x);
        if (i == 0) {
            file << x_to_svg(x) << "," << y_to_svg(sx);
        } else {
            file << " L " << x_to_svg(x) << "," << y_to_svg(sx);
        }
    }
    file << "\" stroke=\"#cc0000\" stroke-width=\"3\" fill=\"none\" stroke-dasharray=\"8,4\" stroke-linecap=\"round\" stroke-linejoin=\"round\"/>\n";
    
    // Вузли інтерполяції
    for (size_t i = 0; i < nodes_x.size(); i++) {
        double x = nodes_x[i];
        double y = nodes_y[i];
        file << "<circle cx=\"" << x_to_svg(x) << "\" cy=\"" << y_to_svg(y) 
             << "\" r=\"4\" fill=\"#333333\" stroke=\"white\" stroke-width=\"1\"/>\n";
    }
    
    // Ось координат
    file << "<g stroke=\"#333333\" stroke-width=\"2\">\n";
    if (x_min <= 0 && x_max >= 0) {
        double zero_x = x_to_svg(0);
        file << "<line x1=\"" << zero_x << "\" y1=\"" << padding 
             << "\" x2=\"" << zero_x << "\" y2=\"" << (height - padding) << "\"/>\n";
    }
    if (y_min <= 0 && y_max >= 0) {
        double zero_y = y_to_svg(0);
        file << "<line x1=\"" << padding << "\" y1=\"" << zero_y 
             << "\" x2=\"" << (width - padding) << "\" y2=\"" << zero_y << "\"/>\n";
    }
    file << "</g>\n";
    
    // Підписи осей
    file << "<text x=\"" << (width / 2) << "\" y=\"" << (height - 20) 
         << "\" text-anchor=\"middle\" font-family=\"Arial, sans-serif\" font-size=\"16\" fill=\"#333333\">x</text>\n";
    file << "<text x=\"20\" y=\"" << (height / 2) 
         << "\" text-anchor=\"middle\" font-family=\"Arial, sans-serif\" font-size=\"16\" fill=\"#333333\" transform=\"rotate(-90, 20, " << (height / 2) << ")\">y</text>\n";
    
    // Заголовок
    file << "<text x=\"" << (width / 2) << "\" y=\"30\" text-anchor=\"middle\" "
         << "font-family=\"Arial, sans-serif\" font-size=\"20\" font-weight=\"bold\" fill=\"#333333\">"
         << "Природній кубічний сплайн</text>\n";
    
    // Легенда
    double legend_x = width - 200;
    double legend_y = padding + 30;
    file << "<rect x=\"" << (legend_x - 10) << "\" y=\"" << (legend_y - 25) 
         << "\" width=\"180\" height=\"60\" fill=\"white\" stroke=\"#333333\" stroke-width=\"1\" opacity=\"0.9\"/>\n";
    file << "<line x1=\"" << (legend_x) << "\" y1=\"" << legend_y 
         << "\" x2=\"" << (legend_x + 30) << "\" y2=\"" << legend_y 
         << "\" stroke=\"#0066cc\" stroke-width=\"3\"/>\n";
    file << "<text x=\"" << (legend_x + 40) << "\" y=\"" << (legend_y + 5) 
         << "\" font-family=\"Arial, sans-serif\" font-size=\"14\" fill=\"#333333\">f(x)</text>\n";
    file << "<line x1=\"" << (legend_x) << "\" y1=\"" << (legend_y + 20) 
         << "\" x2=\"" << (legend_x + 30) << "\" y2=\"" << (legend_y + 20) 
         << "\" stroke=\"#cc0000\" stroke-width=\"3\" stroke-dasharray=\"8,4\"/>\n";
    file << "<text x=\"" << (legend_x + 40) << "\" y=\"" << (legend_y + 25) 
         << "\" font-family=\"Arial, sans-serif\" font-size=\"14\" fill=\"#333333\">S(x)</text>\n";
    
    file << "</svg>\n";
    file.close();
    cout << "\nSVG графік збережено у файл: " << filename << endl;
}

// ============================================================================
// Main
// ============================================================================

int main() {
    cout << string(80, '=') << endl;
    cout << "Лабораторна робота 5: Побудова природнього кубічного сплайну" << endl;
    cout << "Функція: f(x) = 8 / (cosh²(2x))" << endl;
    cout << "Інтервал: [-4, 4]" << endl;
    cout << "Автор: Кузнецов Богдан, група ТК-31" << endl;
    cout << string(80, '=') << endl;
    
    // Введення кількості вузлів
    int n;
    cout << "\nВведіть кількість вузлів інтерполяції (n >= 2): ";
    cin >> n;
    
    if (n < 2) {
        cerr << "Помилка: кількість вузлів повинна бути принаймні 2" << endl;
        return 1;
    }
    
    // Створюємо рівномірний розподіл вузлів на інтервалі [-4, 4]
    vector<double> nodes_x(n);
    vector<double> nodes_y(n);
    
    double a = -4.0;
    double b = 4.0;
    double h = (b - a) / (n - 1);
    
    for (int i = 0; i < n; i++) {
        nodes_x[i] = a + i * h;
        nodes_y[i] = f(nodes_x[i]);
    }
    
    // Виводимо таблицю вузлів
    printNodesTable(nodes_x, nodes_y);
    
    cout << "\nПобудова природнього кубічного сплайну..." << endl;
    NaturalCubicSpline spline(nodes_x, nodes_y);
    
    printSplineCoefficients(spline);
    
    cout << "\n" << string(80, '=') << endl;
    cout << "АНАЛІЗ ПОХИБКИ ІНТЕРПОЛЯЦІЇ" << endl;
    cout << string(80, '=') << endl;
    
    int test_points = 100;
    double max_error = 0.0;
    double mean_error = 0.0;
    
    for (int i = 0; i <= test_points; i++) {
        double x = a + (b - a) * i / test_points;
        double fx = f(x);
        double sx = spline.evaluate(x);
        double error = abs(fx - sx);
        
        if (error > max_error) {
            max_error = error;
        }
        mean_error += error;
    }
    mean_error /= (test_points + 1);
    
    cout << "Максимальна похибка: " << scientific << setprecision(10) << max_error << endl;
    cout << "Середня похибка: " << scientific << setprecision(10) << mean_error << endl;
    
    outputSVGGraph(spline, "graph.svg");
    
    cout << "\n" << string(80, '=') << endl;
    cout << "РОБОТУ ЗАВЕРШЕНО" << endl;
    cout << string(80, '=') << endl;
    
    return 0;
}


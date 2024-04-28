#include <iostream>
#include <cmath>

// Task: The task is to create a program that approximates the computation of a function 𝑓(𝑥). f(x) defined analytically and by an infinite sum within a given interval (a, b). 

void about();
bool getArgs(double &x, double &epsilon);
double calcFunc(double x);
double calcSum(double x, double epsilon);
void printHeader();
void printResult(double x, double f_lib, double f_sum, double diff);

int main() {
    about();

    double x, epsilon;
    if (!getArgs(x, epsilon)) {
        std::cout << "Invalid input values. Program terminated." << std::endl;
        return 1;
    }

    double f_lib = calcFunc(x);
    double f_sum = calcSum(x, epsilon);
    double diff = std::abs(f_lib - f_sum);

    printHeader();
    printResult(x, f_lib, f_sum, diff);

    std::cout << "Program terminated." << std::endl;
    return 0;
}

void about() {
    std::cout << "Laboratory work 2. Approximate calculation of functions" << std::endl;
    std::cout << "K-15 Bohdan Kuznetsov" << std::endl;
}

bool getArgs(double &x, double &epsilon) {
    double a = -1.0;
    double b = 1.0;

    while (true) {
        std::cout << "Enter the value of the argument x in the interval (" << a << "; " << b << "): ";
        if (!(std::cin >> x) || x <= a || x >= b) {
            std::cout << "Invalid value for x. Please enter a valid value." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "Enter the value of epsilon (a positive number): ";
        if (!(std::cin >> epsilon) || epsilon <= 0) {
            std::cout << "Invalid value for epsilon. Please enter a valid value." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        break;
    }

    return true;
}

double calcFunc(double x) {
    return std::sqrt(1 - x);
}

double calcSum(double x, double epsilon) {
    double result = 1 - x / 2;
    double term = 1 - x / 2;

    for (int n = 2; std::abs(term) >= epsilon; ++n) {
        term *= ((2 * n - 3) / (2.0 * n)) * x;
        result += term;
    }

    return result;
}

void printHeader() {
    std::cout << "x\t\tf_lib(x)\t\tf_sum(x)\t\tdiff" << std::endl;
}

void printResult(double x, double f_lib, double f_sum, double diff) {
    std::cout << x << "\t\t" << f_lib << "\t\t" << f_sum << "\t\t" << diff << std::endl;
}

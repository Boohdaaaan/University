#include "matrix.h"

// Constructor that initializes the matrix by loading data from a file
Matrix::Matrix(const std::string& filePath) {
    loadFromFile(filePath);
}

// Load matrix data from the specified file
void Matrix::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        exit(1);
    }
    file >> size; // Read the size of the matrix
    data.resize(size, std::vector<int>(size)); // Resize the matrix
    // Read matrix values from the file
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file >> data[i][j];
        }
    }
    file.close();
}

// Print the matrix to the console
void Matrix::print() const {
    for (const auto& row : data) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

// Get the size of the matrix
int Matrix::getSize() const {
    return size;
}

// Get the value of a specific cell in the matrix
int Matrix::getCellValue(int row, int col) const {
    return data[row][col];
}

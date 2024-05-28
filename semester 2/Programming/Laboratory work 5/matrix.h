#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Matrix {
public:
    Matrix(const std::string& filePath);
    void print() const;
    int getSize() const;
    int getCellValue(int row, int col) const;

private:
    std::vector<std::vector<int>> data;
    int size;
    void loadFromFile(const std::string& filePath);
};

#endif

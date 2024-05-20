#include "walk.h"

// Traverse the lower sector of the matrix in a spiral pattern
void traverseLowerSectorSpiral(const Matrix& matrix, std::vector<int>& sequence) {
    int startRow = matrix.getSize() - 1;  // Start from the bottom row
    int startCol = matrix.getSize() - 1;  // Start from the bottom-right corner
    int endRow = matrix.getSize() / 2;    // End in the middle row
    int endCol = matrix.getSize() / 2;    // End in the middle column

    // Traverse until we reach the center of the matrix
    while (startRow >= endRow && startCol >= endCol) {
        // Move left along the bottom row
        for (int i = startCol; i >= endCol; --i) {
            sequence.push_back(matrix.getCellValue(startRow, i));
        }
        startRow--;  // Move one row up

        // Move up along the left column
        for (int i = startRow; i >= endRow; --i) {
            sequence.push_back(matrix.getCellValue(i, endCol));
        }
        endCol++;  // Move one column right

        // Check if there are more rows to process
        if (startRow >= endRow) {
            // Move right along the top row of the current section
            for (int i = endCol; i <= startCol; ++i) {
                sequence.push_back(matrix.getCellValue(endRow, i));
            }
            endRow++;  // Move one row down
        }

        // Check if there are more columns to process
        if (startCol >= endCol) {
            // Move down along the right column of the current section
            for (int i = endRow; i <= startRow; ++i) {
                sequence.push_back(matrix.getCellValue(i, startCol));
            }
            startCol--;  // Move one column left
        }
    }
}

// Traverse the right sector of the matrix in a snake pattern
void traverseRightSectorSnake(const Matrix& matrix, std::vector<int>& sequence) {
    // Loop through columns starting from the middle to the right edge
    for (int col = matrix.getSize() / 2; col < matrix.getSize(); ++col) {
        if (col % 2 == 0) {  // For even columns
            // Move top-down
            for (int row = 0; row < matrix.getSize(); ++row) {
                sequence.push_back(matrix.getCellValue(row, col));
            }
        } else {  // For odd columns
            // Move bottom-up
            for (int row = matrix.getSize() - 1; row >= 0; --row) {
                sequence.push_back(matrix.getCellValue(row, col));
            }
        }
    }
}

#include <iostream>
#include <fstream>
#include <vector>
#include "matrix.h"
#include "walk.h"

// Function to display laboratory information
void displayLaboratoryInfo() {
    std::cout << "Author: Bohdan Kuznetsov K-15\n";
    std::cout << "Laboratory 5: Matrix Traversal and Operation\n";
}

// Minimum operation between two sequences
std::vector<int> performMinimumOperation(const std::vector<int>& seq1, const std::vector<int>& seq2) {
    std::vector<int> result;
    // Iterate through both sequences and take the minimum of corresponding elements
    for (size_t i = 0; i < std::min(seq1.size(), seq2.size()); ++i) {
        result.push_back(std::min(seq1[i], seq2[i]));
    }
    return result;
}

// Write the resulting sequence to a file
void writeResultToFile(const std::vector<int>& result, const std::string& filePath) {
    std::ofstream file(filePath);
    // Check if the file is opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        exit(1); // Exit if the file could not be opened
    }
    // Write each element of the result sequence to the file
    for (const auto& elem : result) {
        file << elem << " ";
    }
    file.close(); // Close the file
}

int main(int argc, char* argv[]) {
    // Check if the correct number of command-line arguments are provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
        return 1;
    }

    // Output author's name and laboratory title
    displayLaboratoryInfo();

    // Get file paths from command-line arguments
    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];

    // Create a Matrix object and load data from the input file
    Matrix matrix(inputFilePath);

    // Vectors to store the sequences resulting from matrix traversal
    std::vector<int> sequence1, sequence2;
    // Traverse the lower sector of the matrix in a spiral pattern
    traverseLowerSectorSpiral(matrix, sequence1);
    // Traverse the right sector of the matrix in a snake pattern
    traverseRightSectorSnake(matrix, sequence2);

    // Perform the minimum operation on the two sequences
    std::vector<int> result = performMinimumOperation(sequence1, sequence2);

    // Write the result to the output file
    writeResultToFile(result, outputFilePath);

    std::cout << "Program execution completed.\n";

    return 0;
}

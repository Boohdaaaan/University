#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>


// Function to print Laboratory information
void displayLaboratoryInfo() {
    std::cout << "Author: Bohdan Kuznetsov K-15\n";
    std::cout << "Laboratory 6: Find shortest subsequences with all numbers from 1 to N appearing at least twice.\n";
}


// Function to check if the current sequence is valid
bool is_valid_sequence(const std::unordered_map<int, int>& count_map, int N) {
    if (count_map.size() != N) return false; // Check if the size of the count_map is equal to N
    for (const auto& pair : count_map) { // Check if each number appears at least twice
        if (pair.second < 2) return false;
    }
    return true;
}

// Function to find subsequences of the required criteria
void find_subsequences(const std::vector<int>& sequence, int N, std::vector<std::pair<int, int>>& results) {
    int n = sequence.size();
    std::unordered_map<int, int> count_map;
    int left = 0;
    int min_length = n + 1;

    // Sliding window approach to find the subsequences
    for (int right = 0; right < n; ++right) {
        if (sequence[right] >= 1 && sequence[right] <= N) {
            count_map[sequence[right]]++; // Increase the count of the current number
        }

        // Check if the current window is valid
        while (is_valid_sequence(count_map, N)) {
            int current_length = right - left + 1;
            if (current_length < min_length) { // Update the minimum length
                min_length = current_length;
                results.clear(); // Clear previous results
            }
            if (current_length == min_length) { // Add the current window to results
                results.push_back({left, current_length});
            }

            if (sequence[left] >= 1 && sequence[left] <= N) { // Move the left boundary of the window
                count_map[sequence[left]]--;
                if (count_map[sequence[left]] == 0) {
                    count_map.erase(sequence[left]);
                }
            }
            left++;
        }
    }
}

// Main function
int main(int argc, char* argv[]) {
    // Check if the correct number of arguments are passed
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    // Open the input file
    std::ifstream input_file(argv[1]);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open input file.\n";
        return 1;
    }

    // Open the output file
    std::ofstream output_file(argv[2]);
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not open output file.\n";
        return 1;
    }

    // Print laboratory information
    displayLaboratoryInfo();

    int N;
    input_file >> N;
    if (N < 2) { // Check if N is at least 2
        std::cerr << "Error: N must be at least 2.\n";
        return 1;
    }

    // Read the input sequence
    std::vector<int> sequence;
    int num;
    while (input_file >> num) {
        sequence.push_back(num);
    }

    // Find subsequences
    std::vector<std::pair<int, int>> results;
    find_subsequences(sequence, N, results);

    // Output the results
    output_file << sequence.size() << "\n";
    if (!results.empty()) {
        output_file << results[0].second << "\n";
        for (const auto& result : results) {
            output_file << result.first << " " << result.second << "\n";
        }
    } else {
        output_file << "No valid subsequences found.\n";
    }

    return 0;
}

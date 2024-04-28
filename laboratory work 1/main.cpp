#include <iostream>
#include <string>
#include <cmath>

// Task: Create a program that converts a given number in base 2 to a number in base 32.

void about();

std::string convertToBase32(int num) {
    const std::string digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string result;
    while (num > 0) {
        int remainder = num % 32;
        result.insert(result.begin(), digits[remainder]);
        num /= 32;
    }
    return result;
}

int main() {
    about();
    std::string input;
    std::cout << "Please, enter base-2 number:";
    std::cin >> input;
    std::cout << "Processing... \n";

    // Checking for errors in the entered number
    bool isValid = true;
    for (char digit : input) {
        if (!(digit >= '0' && digit <= '1')) {
            isValid = false;
            break;
        }
    }

    if (!isValid) {
        std::cout << "Invalid input. Please, enter base-2 number.";
        return 1;
    }

    // Converting the number to base-32 numbering system
    int decimalNum = 0;
    int power = 0;
    for (int i = input.size() - 1; i >= 0; --i) {
        if (input[i] == '1') {
            decimalNum += std::pow(2, power);
        }
        ++power;
    }

    std::string base32Num = convertToBase32(decimalNum);

    std::cout << "Your base-2 number " << input << " as base-32 number: \n" << base32Num << std::endl;

    return 0;
}

void about() {
    std::cout << "Laboratory work 1. Base conversion." << std::endl;
    std::cout << "K-15 Bohdan Kuznetsov" << std::endl;
}
// variant 13

#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <vector>
#include <random>
#include <fstream>
#include <chrono>
#include <thread>

class DualIntStructure {
private:
    // Окремі м'ютекси для кожного поля 
    mutable std::shared_mutex mutex0_; // 10% читання, 10% запису — стандартний shared_mutex
    mutable std::shared_mutex mutex1_; // 50% читання, 10% запису — shared_mutex для поля з частим читанням
    mutable std::mutex string_mutex_;  // 20% операцій зі строками
    
    int field0_;
    int field1_;

public:
    DualIntStructure() : field0_(0), field1_(0) {}

    int getField0() const {
        std::shared_lock lock(mutex0_);
        return field0_;
    }

    void setField0(int value) {
        std::unique_lock lock(mutex0_);
        field0_ = value;
    }

    int getField1() const {
        std::shared_lock lock(mutex1_);
        return field1_;
    }

    void setField1(int value) {
        std::unique_lock lock(mutex1_);
        field1_ = value;
    }

    operator std::string() const {
        std::lock_guard string_lock(string_mutex_);
        std::shared_lock lock0(mutex0_);
        std::shared_lock lock1(mutex1_);
        return "Field0: " + std::to_string(field0_) + ", Field1: " + std::to_string(field1_);
    }
};

// генерація тестових файлів із різними розподілами
void generateTestFile(const std::string& filename, 
                     double read0_freq, double write0_freq,
                     double read1_freq, double write1_freq,
                     double string_freq, int num_operations) {
    std::ofstream file(filename);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> value_dis(-1000, 1000);

    for (int i = 0; i < num_operations; ++i) {
        double rand = dis(gen);
        double sum = 0.0;

        if ((sum += read0_freq) > rand) {
            file << "read 0\n";
        } else if ((sum += write0_freq) > rand) {
            file << "write 0 " << value_dis(gen) << "\n";
        } else if ((sum += read1_freq) > rand) {
            file << "read 1\n";
        } else if ((sum += write1_freq) > rand) {
            file << "write 1 " << value_dis(gen) << "\n";
        } else {
            file << "string\n";
        }
    }
    file.close();
}

// обробка команд із файлу
void processFile(const std::string& filename, DualIntStructure& data) {
    std::ifstream file(filename);
    std::string line;
    std::string command;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> command;
        
        if (command == "read") {
            int field;
            iss >> field;
            if (field == 0) data.getField0();
            else data.getField1();
        }
        else if (command == "write") {
            int field, value;
            iss >> field >> value;
            if (field == 0) data.setField0(value);
            else data.setField1(value);
        }
        else if (command == "string") {
            std::string str = data;
        }
    }
}

void runTest(const std::vector<std::string>& filenames, int num_threads) {
    DualIntStructure data;
    std::vector<std::thread> threads;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(processFile, std::ref(filenames[i]), std::ref(data));
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Тест з " << num_threads << " потоками - " 
              << duration.count() << "ms\n";
}

int main() {
    setlocale(LC_ALL, "ukrainian")
    const int NUM_OPERATIONS = 100000;
    
    // Генерація тестових файлів із різними розподілами
    // Збалансований розподіл
    generateTestFile("balanced.txt", 0.2, 0.2, 0.2, 0.2, 0.2, NUM_OPERATIONS);
    
    // Розподіл відповідно до вимог
    generateTestFile("constrained.txt", 0.1, 0.1, 0.5, 0.1, 0.2, NUM_OPERATIONS);
    
    // Зміщений розподіл
    generateTestFile("skewed.txt", 0.05, 0.05, 0.7, 0.15, 0.05, NUM_OPERATIONS);
    
    // Тест із різною кількістю потоків
    std::vector<std::string> files = {"balanced.txt", "constrained.txt", "skewed.txt"};
    
    std::cout << "Запуск однопотокового тесту...\n";
    runTest({files[0]}, 1);
    
    std::cout << "\nЗапуск двопотокового тесту...\n";
    runTest({files[0], files[1]}, 2);
    
    std::cout << "\nЗапуск трипотокового тесту...\n";
    runTest({files[0], files[1], files[2]}, 3);
    
    return 0;
}
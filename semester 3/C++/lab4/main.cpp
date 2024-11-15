#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>

class CustomLock {
private:
    mutable bool flag;  
    mutable int waiting;
    
public:
    CustomLock() : flag(false), waiting(0) {}
    
    void lock() const {  
        waiting++;
        while (flag) {
            std::this_thread::yield();
        }
        flag = true;
        waiting--;
    }
    
    void unlock() const {  
        flag = false;
    }
};

class MultiThreadedStructure {
private:
    int field0;
    int field1;
    mutable CustomLock lock0;  
    mutable CustomLock lock1;
    mutable CustomLock stringLock;

public:
    MultiThreadedStructure() : field0(0), field1(0) {}

    int readField0() const {  
        lock0.lock();
        int value = field0;
        lock0.unlock();
        return value;
    }

    void writeField0(int value) {
        lock0.lock();
        field0 = value;
        lock0.unlock();
    }

    int readField1() const {  
        lock1.lock();
        int value = field1;
        lock1.unlock();
        return value;
    }

    void writeField1(int value) {
        lock1.lock();
        field1 = value;
        lock1.unlock();
    }

    friend std::ostream& operator<<(std::ostream& os, const MultiThreadedStructure& mts) {
        mts.stringLock.lock();
        mts.lock0.lock();
        mts.lock1.lock();
        
        os << "Field0: " << mts.field0 << ", Field1: " << mts.field1;
        
        mts.lock1.unlock();
        mts.lock0.unlock();
        mts.stringLock.unlock();
        
        return os;
    }
};

// Генерація тестових файлів
void generateTestFile(const std::string& filename, 
                     int numOperations,
                     double readProb0,
                     double writeProb0,
                     double readProb1,
                     double writeProb1) {
    std::ofstream file(filename);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (int i = 0; i < numOperations; ++i) {
        double prob = dis(gen);
        double cumProb = 0.0;
        
        if ((cumProb += readProb0) > prob) {
            file << "read 0\n";
        } else if ((cumProb += writeProb0) > prob) {
            file << "write 0 1\n";
        } else if ((cumProb += readProb1) > prob) {
            file << "read 1\n";
        } else if ((cumProb += writeProb1) > prob) {
            file << "write 1 1\n";
        } else {
            file << "string\n";
        }
    }
    file.close();
}

void processFile(const std::string& filename, MultiThreadedStructure& data, double& duration) {
    std::ifstream file(filename);
    std::vector<std::string> operations;
    std::string line;
    
    while (std::getline(file, line)) {
        operations.push_back(line);
    }
    file.close();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (const auto& op : operations) {
        std::istringstream iss(op);
        std::string operation;
        iss >> operation;
        
        if (operation == "read") {
            int fieldNum;
            iss >> fieldNum;
            if (fieldNum == 0) {
                data.readField0();
            } else {
                data.readField1();
            }
        } else if (operation == "write") {
            int fieldNum, value;
            iss >> fieldNum >> value;
            if (fieldNum == 0) {
                data.writeField0(value);
            } else {
                data.writeField1(value);
            }
        } else if (operation == "string") {
            std::stringstream ss;
            ss << data;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration<double, std::milli>(end - start).count();
}

void runTest(const std::vector<std::string>& filenames, int numThreads) {
    MultiThreadedStructure data;
    std::vector<std::thread> threads;
    std::vector<double> durations(numThreads);
    
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(processFile, std::ref(filenames[i]), std::ref(data), std::ref(durations[i]));
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    double totalDuration = 0;
    for (double duration : durations) {
        totalDuration += duration;
    }
    
    std::cout << "Test with " << numThreads << " thread(s) completed in " 
              << totalDuration << " ms" << std::endl;
}

int main() {
    const int numOperations = 100000;
    
    // Сценарій A: частоти відповідають умові
    generateTestFile("test_a_1.txt", numOperations, 0.25, 0.05, 0.05, 0.25);
    generateTestFile("test_a_2.txt", numOperations, 0.25, 0.05, 0.05, 0.25);
    generateTestFile("test_a_3.txt", numOperations, 0.25, 0.05, 0.05, 0.25);
    
    // Сценарій B: всі частоти рівні
    generateTestFile("test_b_1.txt", numOperations, 0.20, 0.20, 0.20, 0.20);
    generateTestFile("test_b_2.txt", numOperations, 0.20, 0.20, 0.20, 0.20);
    generateTestFile("test_b_3.txt", numOperations, 0.20, 0.20, 0.20, 0.20);
    
    // Сценарій C: частоти дуже сильно не відповідають умові і не є рівними
    generateTestFile("test_c_1.txt", numOperations, 0.50, 0.10, 0.10, 0.20);
    generateTestFile("test_c_2.txt", numOperations, 0.50, 0.10, 0.10, 0.20);
    generateTestFile("test_c_3.txt", numOperations, 0.50, 0.10, 0.10, 0.20);
    
    std::cout << "Testing Scenario A (Matching frequencies):\n";
    runTest({"test_a_1.txt"}, 1);
    runTest({"test_a_1.txt", "test_a_2.txt"}, 2);
    runTest({"test_a_1.txt", "test_a_2.txt", "test_a_3.txt"}, 3);
    
    std::cout << "\nTesting Scenario B (Equal frequencies):\n";
    runTest({"test_b_1.txt"}, 1);
    runTest({"test_b_1.txt", "test_b_2.txt"}, 2);
    runTest({"test_b_1.txt", "test_b_2.txt", "test_b_3.txt"}, 3);
    
    std::cout << "\nTesting Scenario C (Deviating frequencies):\n";
    runTest({"test_c_1.txt"}, 1);
    runTest({"test_c_1.txt", "test_c_2.txt"}, 2);
    runTest({"test_c_1.txt", "test_c_2.txt", "test_c_3.txt"}, 3);
    
    return 0;
}
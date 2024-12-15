// variant 9

#include <algorithm>
#include <chrono>
#include <execution>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <future>

// Utility function for timing measurements
template<typename Func>
double measure_time(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

// Custom parallel merge implementation
template<typename Iterator>
void custom_parallel_merge(Iterator first1, Iterator last1,
                         Iterator first2, Iterator last2,
                         Iterator result, size_t num_threads) {
    size_t total_size = (last1 - first1) + (last2 - first2);
    size_t chunk_size = total_size / num_threads;
    
    std::vector<std::future<void>> futures;
    std::vector<Iterator> splits1, splits2;
    
    // Calculate split points
    for (size_t i = 0; i < num_threads; ++i) {
        size_t target = i * chunk_size;
        auto split_point = std::lower_bound(first1, last1, *(first2 + target));
        splits1.push_back(split_point);
        splits2.push_back(first2 + target);
    }
    splits1.push_back(last1);
    splits2.push_back(last2);
    
    // Launch threads
    for (size_t i = 0; i < num_threads; ++i) {
        futures.push_back(std::async(std::launch::async,
            [=]() {
                std::merge(splits1[i], splits1[i + 1],
                          splits2[i], splits2[i + 1],
                          result + (splits1[i] - first1) + (splits2[i] - first2));
            }));
    }
    
    // Wait for all threads
    for (auto& future : futures) {
        future.wait();
    }
}

int main() {
    setlocale(LC_ALL, "ukrainian");
    const size_t data_size = 100000;
    const size_t max_threads = std::thread::hardware_concurrency();
    
    std::cout << "Hardware concurrency: " << max_threads << " потоків\n\n";
    
    // Generate test data
    std::vector<int> vec1(data_size), vec2(data_size), result(data_size * 2);
    std::iota(vec1.begin(), vec1.end(), 0);
    std::iota(vec2.begin(), vec2.end(), data_size);
    
    // 1. Стандартний merge
    std::cout << "1. Стандартна операція merge:\n";
    double seq_time = measure_time([&]() {
        std::merge(vec1.begin(), vec1.end(),
                  vec2.begin(), vec2.end(),
                  result.begin());
    });
    std::cout << "Time: " << seq_time << " ms\n\n";
    
    // 2. Паралельні політики
    std::cout << "2. Паралельні політики:\n";
    
    double par_time = measure_time([&]() {
        std::merge(std::execution::par,
                  vec1.begin(), vec1.end(),
                  vec2.begin(), vec2.end(),
                  result.begin());
    });
    std::cout << "std::execution::par час: " << par_time << " ms\n";
    
    double par_unseq_time = measure_time([&]() {
        std::merge(std::execution::par_unseq,
                  vec1.begin(), vec1.end(),
                  vec2.begin(), vec2.end(),
                  result.begin());
    });
    std::cout << "std::execution::par_unseq час: " << par_unseq_time << " ms\n\n";
    
    // 3. Власниий паралельний алгоритм з різними K
    std::cout << "3. Власниий паралельний алгоритм:\n";
    std::cout << std::setw(10) << "Потоків" << std::setw(15) << "Час (ms)\n";
    std::cout << std::string(25, '-') << '\n';
    
    double best_time = std::numeric_limits<double>::max();
    size_t best_k = 0;
    
    for (size_t k = 1; k <= max_threads * 2; ++k) {
        double custom_time = measure_time([&]() {
            custom_parallel_merge(vec1.begin(), vec1.end(),
                                vec2.begin(), vec2.end(),
                                result.begin(), k);
        });
        
        std::cout << std::setw(10) << k << std::setw(15) << custom_time << '\n';
        
        if (custom_time < best_time) {
            best_time = custom_time;
            best_k = k;
        }
    }
    
    std::cout << "\nКращий випадок:\n";
    std::cout << "K = " << best_k << " потоків (Hardware threads: " << max_threads << ")\n";
    std::cout << "Час: " << best_time << " ms\n";
    std::cout << "Відношення K/Hardware threads: " << static_cast<double>(best_k) / max_threads << '\n';
    
    return 0;
}
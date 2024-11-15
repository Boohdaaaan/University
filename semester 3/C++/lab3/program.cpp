#include <iostream>
#include <thread>
#include <barrier>
#include <syncstream>
#include <vector>
#include <semaphore>

using namespace std;

// Function to simulate computation
void f(char set, int action) {
    osyncstream(cout) << "From set " << set << " action " << action << " completed." << endl;
}

class ComputationGraph {
private:
    vector<thread> threads;
    binary_semaphore thread_semaphore{7};  // Limit to nt=7 threads
    
    // Barriers for synchronization between stages
    barrier a_sync{7};  // 7 actions
    barrier b_sync{8};  // 8 actions
    barrier c_sync{8};  // 8 actions
    barrier d_sync{9};  // 9 actions
    barrier e_sync{8};  // 8 actions
    barrier f_sync{6};  // 6 actions
    barrier g_sync{5};  // 5 actions
    barrier h_sync{4};  // 4 actions
    barrier i_sync{8};  // 8 actions
    barrier j_sync{8};  // 8 actions

    void execute_action(char set, int action, barrier& sync) {
        thread_semaphore.acquire();
        threads.emplace_back([&, set, action] {
            f(set, action);
            sync.arrive_and_wait();
            thread_semaphore.release();
        });
    }

    void execute_set(char set, int count, barrier& sync) {
        for(int i = 1; i <= count; ++i) {
            execute_action(set, i, sync);
        }
    }

public:
    void run() {
        osyncstream(cout) << "Computation started." << endl;

        // Stage 1: Set 'a' (no dependencies)
        execute_set('a', 7, a_sync);
        
        // Wait for 'a' to complete before starting its dependents
        for(auto& t : threads) {
            if(t.joinable()) t.join();
        }
        threads.clear();

        // Stage 2: Sets 'b', 'c', 'd' (depend on 'a')
        execute_set('b', 8, b_sync);
        execute_set('c', 8, c_sync);
        execute_set('d', 9, d_sync);
        
        // Wait for 'b', 'c', 'd' to complete
        for(auto& t : threads) {
            if(t.joinable()) t.join();
        }
        threads.clear();

        // Stage 3: Sets 'e', 'f', 'g' (depend on 'c', 'b', 'd' respectively)
        execute_set('e', 8, e_sync);
        execute_set('f', 6, f_sync);
        execute_set('g', 5, g_sync);
        
        // Wait for 'e', 'f', 'g' to complete
        for(auto& t : threads) {
            if(t.joinable()) t.join();
        }
        threads.clear();

        // Stage 4: Set 'h' (depends on 'g')
        execute_set('h', 4, h_sync);
        
        // Wait for 'h' to complete
        for(auto& t : threads) {
            if(t.joinable()) t.join();
        }
        threads.clear();

        // Stage 5: Set 'i' (depends on 'e' and 'f')
        execute_set('i', 8, i_sync);
        
        // Wait for 'i' to complete
        for(auto& t : threads) {
            if(t.joinable()) t.join();
        }
        threads.clear();

        // Stage 6: Set 'j' (depends on 'h' and 'i')
        execute_set('j', 8, j_sync);
        
        // Wait for all remaining threads
        for(auto& t : threads) {
            if(t.joinable()) t.join();
        }

        osyncstream(cout) << "Computation completed." << endl;
    }
};

int main() {
    ComputationGraph graph;
    graph.run();
    return 0;
}
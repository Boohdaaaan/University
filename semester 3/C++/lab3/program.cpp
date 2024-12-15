// variant 3

#include <iostream>
#include <thread>
#include <syncstream>
#include <latch>
#include <barrier>
#include <vector>
#include <chrono>

using namespace std;

// заглушка. і-а дія з набору
void f(char set, int i) {
    osyncstream(cout) << "З набору " << set << " виконано дію '" << i << endl;
    this_thread::sleep_for(chrono::milliseconds(100));
}

// Виконати повний набір дій
void execute_set(char set, int start, int count, latch& l) {
    for (int i = start; i < start + count; ++i) {
        f(set, i);
    }
    l.count_down();
}

int main() {
    setlocale(LC_ALL, "ukrainian");

    const int nt = 7; // кількість потоків

    // Latches для синхронізації
    latch a_done(1);     // 7
    latch b_done(1);     // 8
    latch c_done(1);     // 8
    latch d_done(1);     // 9
    latch e_done(1);     // 8
    latch f_done(1);     // 6
    latch g_done(1);     // 5
    latch h_done(1);     // 4
    latch i_done(1);     // 8
    latch j_done(1);     // 8 

    osyncstream(cout) << "Обчислення розпочато." << endl;

    vector<jthread> threads;

    // Виконуємо 'a'
    threads.emplace_back([&]() {
        execute_set('a', 1, 7, a_done);
    });

    // Чекаємо завершення 'a'
    a_done.wait();

    // Виконуємо паралельно 'b', 'c', 'd'
    threads.emplace_back([&]() {
        execute_set('b', 1, 8, b_done);
    });
    threads.emplace_back([&]() {
        execute_set('c', 1, 8, c_done);
    });
    threads.emplace_back([&]() {
        execute_set('d', 1, 9, d_done);
    });

    // Чекаємо завершення 'b', щоб почати 'e'
    b_done.wait();
    threads.emplace_back([&]() {
        execute_set('e', 1, 8, e_done);
    });

    // Чекаємо завершення 'c', щоб почати 'f', 'g'
    c_done.wait();
    threads.emplace_back([&]() {
        execute_set('f', 1, 6, f_done);
    });
    threads.emplace_back([&]() {
        execute_set('g', 1, 5, g_done);
    });

    // Чекаємо завершення 'd', щоб почати 'h'
    d_done.wait();
    threads.emplace_back([&]() {
        execute_set('h', 1, 4, h_done);
    });

    // Чекаємо завершення 'e', 'f', щоб почати 'i'
    e_done.wait();
    f_done.wait();
    threads.emplace_back([&]() {
        execute_set('i', 1, 8, i_done);
    });

    // Чекаємо завершення 'g', 'h', щоб почати 'j'
    g_done.wait();
    h_done.wait();
    threads.emplace_back([&]() {
        execute_set('j', 1, 8, j_done);
    });

    // Чекаємо завершення 'i', 'j'
    i_done.wait();
    j_done.wait();

    osyncstream(cout) << "Обчислення завершено." << endl;

    return 0;
}
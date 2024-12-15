// variant 14

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <syncstream>
using namespace std;
using namespace std::chrono;

void calculate(const string& name, int seconds) {
    this_thread::sleep_for(seconds * 1s);
    osyncstream(cout) << name << '\n';
}

void slow(const string& name) {
    calculate(name, 7);
}

void quick(const string& name) {
    calculate(name, 1);
}

void work() {
    setlocale(LC_ALL, "ukrainian");
    auto start = steady_clock::now();

    // Запуск A1 (slow) та A2 (fast) паралельно
    auto futA1 = async(launch::async, slow, "A1");
    auto futA2 = async(launch::async, quick, "A2");

    // Запуск C2 (slow) паралельно з вище наведеними
    auto futC2 = async(launch::async, slow, "C2");

    // Запуск C1 (fast) паралельно з вище наведеними
    auto futC1 = async(launch::async, quick, "C1");

    // Чекати завершення A1 та A2 перед B
    futA1.wait();
    futA2.wait();

    quick("B");  // B залежить від A1 та A2

    // Чекати завершення C1 та B (вже завершено) перед D1
    futC1.wait();
    quick("D1");  // D1 залежить від B та C1

    // Чекати завершення C2 перед D2
    futC2.wait();
    quick("D2");  // D2 залежить від C2

    auto end = steady_clock::now();
    auto duration = duration_cast<seconds>(end - start).count();

    osyncstream(cout) << "Загальний час: " << duration << " секунд\n";
    osyncstream(cout) << "Робота завершена!\n";
}

int main() {
    work();  // Виклик функції work
    return 0;
}
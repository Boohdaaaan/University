#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int x = 10;
    int y = 20;
    int z = 30;

    // без захоплень (не рахується)
    auto lambda1 = []() {
        return 42;
    };
    
    //з захопленням за значенням [=] (не рахується)
    auto lambda2 = [=]() {
        return x + y;
    };
    
    // з захопленням за посиланням [&] (рахується)
    auto lambda3 = [&]() {
        x = 100;
        return y + z;
    };
    
    // зі змішаними захопленнями, включаючи за посиланням (рахується)
    auto lambda4 = [x, &y]() {
        y = 200;
        return x;
    };
    
    // з явним захопленням за посиланням (рахується)
    auto lambda5 = [&x, &z]() {
        x = 300;
        z = 400;
        return x + z;
    };
    
    // тільки з захопленням за значенням (не рахується)
    auto lambda6 = [x, y]() {
        return x + y;
    };
    
    // з захопленням за посиланням у стандартній бібліотеці (рахується)
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::for_each(vec.begin(), vec.end(), [&](int& n) {
        n *= 2;
    });
    
    return 0;
}


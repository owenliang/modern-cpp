// g++ -std=c++14 -o lambda lambda.cpp
#include <iostream>
#include <string>
#include <functional>

int main(int argc, char** argv) {
    auto f1 = [](int a, int b) {
        return a + b;
    };
    std::cout << f1(1, 2) << std::endl;

    auto f2 = [&]() {
        return f1(1, 2);
    };
    std::cout << f2() << std::endl;

    std::function<int ()> f = f2;
    std::cout << f() << std::endl;
    return 0;
}
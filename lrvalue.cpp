// 左右值
// g++ -std=c++14 -o lrvalue lrvalue.cpp
#include <iostream>
#include <vector>

void test(const std::vector<int>& vec) {
    std::vector<int> copy = vec;
    std::cout << "const std::vector<int>&, " << "copy size:" << copy.size() << ", lvalue ref size:" << vec.size() << std::endl;
}

// 打开宏注释可以走到右值引用版本
#if 0
void test(std::vector<int>&& vec) {
    std::cout << "in fact, vec is lvalue now, " << &vec << std::endl;
    std::vector<int> steal = std::move(vec);
    std::cout << "std::vector<int>&&, " << "steal size:" << steal.size() << ", rvalue size:" << vec.size() << std::endl;
}
#endif

int main(int argc, char** argv) {
    std::vector<int> v{1,2,3};
    test(v);
    test(std::vector<int>{1,2,3});
    test(std::move(v)); // static_cast<std::vector<int>&&>
    return 0;
}

/*
 这段代码告诉我们：
    1，没法取地址的东西就是右值，函数可以通过右值引用或者const左值引用接受它们
    2，函数使用右值引用的时候，可以偷走右值里面的数据，避免了copy的代价
    3，在函数内偷右值里面的数据是有讲究的，因为在函数内右值引用本身是可以取地址的（因此是个左值），为了让其他vector可以偷走它必须std::move再把它变成一个右值哦，这样才能走到vector的move constructor哦~
    4，如果想强行偷走一个左值的东西，可以std::move让它转成右值，然后就可以走vector的move constructor了哦~
*/


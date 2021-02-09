// 可变参数模板
// g++ -std=c++17 -o variadic_tpl variadic_tpl.cpp
#include <iostream>

double sum() {
    return 0;
}

template<typename T, typename... Args>
double sum(T v, Args... args) {
    return v + sum(args...);
}

//////////////////////////////////////////////////

template <typename T, typename... Args>
double sum_super_cool(T v, Args... args) {
    if constexpr (sizeof...(args) == 0) {
        return v;
    }
    return v + sum(args...);
}

int main(int argc, char** argv) {
    double s = sum(1, 1.1, 2.2, 3, 4.5);
    std::cout << s << std::endl;

    double ss = sum_super_cool(1, 1.1, 2.2, 3, 4.5);
    std::cout << s << std::endl;
    return 0;
}

/*
 sum函数告诉我们：
    1，可变参数的...出现在3个地方，要注意~
    2，依靠重载来终结递归运算，这个和模板没啥关系，实际上这是一个运行时递归，编译器只是实例化了多个函数版本~
    3，模板函数编译时会生成多个不同Args的实例化版本代码~
    4，而且得注意实例化的sum(T v, Args... args)版本会递归调用到sum(T v)版本，所以sum(T v)得先于sum(T v, Args... args)定义，否则会提示找不到sum(T v)函数~

sum_super_cool函数告诉我们：
    1，利用if constexpr可以在模板实例化过程中根据args参数个数来修剪代码~
    2，修剪的效果就是如果args列表长度为0就会直接把代码编译为return static_cast<double>(v)，后面的return代码就被删掉了。
    3，如果args列表长度不为0，那么if constexpr内的代码就被删掉了。
*/
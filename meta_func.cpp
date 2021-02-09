// 模板元编程
// g++ -std=c++17 -o meta_func meta_func.cpp
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

// meta function 1：用于判断2个类型是否一样的struct
template<typename T1, typename T2>
struct _is_same {
    static const bool value = false;
};

template<typename T>
struct _is_same<T, T> {
    static const bool value = true;
};

////////////////////////////////////////////////////////

// meta function 2：用于断言，如果为真则可以取到::type，否则取不到
template<bool is_true, typename T>
struct _enable_if {
    using type = T;
};

template<typename T>
struct _enable_if<false, T> {
    // 不定义type, 那么尝试获取type的代码将被SFINAE（Substitution failure is not an error）
};

/////////////////////////////////////////////////////////

// distance版本1：基于if constexpr实现分支处理
template<typename Iter>
size_t _distance1(Iter s, Iter e) {
    // 迭代器的类型
    using category = typename std::iterator_traits<Iter>::iterator_category;

    // constexpr if在编译器执行, 完成对代码的裁剪
    if constexpr (_is_same<category, std::random_access_iterator_tag>::value) {
        std::cout << "_distance1 版本A" << std::endl;
        return e - s;
    } else {
        size_t d = 0;
        while (s != e) {
            ++s;
            ++d;
        }
        std::cout << "_distance1 版本B" << std::endl;
        return d;
    }
}

// distance版本2：模板参数+SFINAE
template<typename Iter, typename _enable_if<_is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value, int>::type = 0>
size_t _distance2(Iter s, Iter e) {
    std::cout << "_distance2 版本A" << std::endl;
    return e - s;
}

template<typename Iter, typename _enable_if<!_is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value, int>::type = 0>
size_t _distance2(Iter s, Iter e) {
    size_t d = 0;
    while (s != e) {
        ++s;
        ++d;
    }
    std::cout << "_distance2 版本B" << std::endl;
    return d;
}

// distance版本3：返回值+SFINAE
template<typename Iter>
typename _enable_if<_is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value, size_t>::type _distance3(Iter s, Iter e) {
    std::cout << "_distance3 版本A" << std::endl;
    return e - s;
}

template<typename Iter>
typename _enable_if<!_is_same<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>::value, size_t>::type _distance3(Iter s, Iter e) {
    size_t d = 0;
    while (s != e) {
        ++s;
        ++d;
    }
    std::cout << "_distance3 版本B" << std::endl;
    return d;
}

int main(int argc, char** argv) {
    std::vector<int> vec{1,2,3,4,5};
    size_t d1 = _distance1(vec.begin(), vec.begin() + 3);
    d1 = _distance2(vec.begin(), vec.begin() + 3);
    d1 = _distance3(vec.begin(), vec.begin() + 3);

    std::map<std::string, int> m{{"a", 10}, {"b", 11}, {"c", 12}, {"d", 13}};
    size_t d2 = _distance1(m.find("a"), m.find("d"));
    d2 = _distance2(m.find("a"), m.find("d"));
    d2 = _distance3(m.find("a"), m.find("d"));
    return 0;
}

/*
 这段代码告诉我们：
    1,模板元编程的基础是strcut的特化和属性来实现的
    2,调用模板函数时，编译器会在多个候选版本中尝试实例化并选择匹配度最高的
    3,在尝试实例化过程中，如果有语法编译失败情况（这里的_enable_if<false,T>::value可能就没有）则会跳过该函数版本继续匹配其他版本
    4,_distance2借助_enable_if+_is_same在模板参数中触发SFINAE，走了一个int=0的无效模板参数方式来埋入这个断言
    5,_distance3借助_enable_if+_is_same在函数返回值中触发SFINAE，也是一个常见方式
*/
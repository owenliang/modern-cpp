// g++ -std=c++14 -o auto auto.cpp
#include <iostream>
#include <map>
#include <string>

auto add(int a, int b) {
    return a + b;
}

auto find_num(const std::map<std::string, int>& datas, const std::string& target) {
    auto iter = datas.find(target);
    if (iter != datas.end()) {
        return iter->second;
    }
    return -1;
}

int main(int argc, char** argv) {
    auto s = add(1, 2);
    std::cout << s << std::endl;

    std::map<std::string, int> datas{{"xiaoming", 10}, {"xiaohong", 12}};
    auto age = find_num(datas, "xiaoming");
    std::cout << age << std::endl;
    return 0;
}
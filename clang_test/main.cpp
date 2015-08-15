#include <iostream>
#include <string>

void foo(int(*f)()) {
    std::cout << f() << std::endl;
}

void foo(std::string(*f)()) {
    std::string s = f();
    std::cout << s << std::endl;
}

int main() {
    auto bar = [] () -> std::string {
        return std::string("bla");
    };

    foo(bar);

    return 0;
}

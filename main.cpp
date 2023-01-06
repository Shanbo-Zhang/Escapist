#include <iostream>

int main() {
    std::cout << std::is_arithmetic<std::string>::value << std::endl;
    return 0;
}

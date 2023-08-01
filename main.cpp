#include <iostream>
#include <string>
#include <vector>
#include "escapist/string.h"
#include "escapist/list.h"

int main() {
    List<int> l(12, 10), l2(l);
    std::cout << l.SetAt(2,6).ConstAt(1);
    std::cout << l.SetAt(2,6).ConstAt(2);
    std::cout << l.SetAt(2,6).ConstAt(3);
    std::cout << l.SetAt(2,6).ConstAt(4);
}

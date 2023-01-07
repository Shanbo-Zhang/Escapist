#include <iostream>
#include "Common/ArrayList.h"

#define Output()\
for (SizeType index = 0; index < arr.GetSize(); ++index) {\
std::cout << arr.GetConstAt(index)<< ' ';                 \
}               \
std::cout << std::endl;

int main() {
    std::cout<<std::is_nothrow_copy_constructible<int>::value;
}

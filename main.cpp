#include <iostream>
#include "Common/ArrayList.h"

int main() {
    ArrayList<int> arr(100, 5);
    for (SizeType index = 0; index < arr.GetSize(); ++index) {
        std::cout << arr.GetConstAt(index)<<std::endl;
    }
}

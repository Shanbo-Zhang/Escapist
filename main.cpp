#include <iostream>
#include <string>
#include <vector>
#include "escapist/string.h"
#include "escapist/list.h"

int *A() {
    int *a = static_cast<int *>(::malloc(sizeof(int)));
    return (int *&&) a;
}

int main() {
    List<int> l(12,10),l2(l);
}

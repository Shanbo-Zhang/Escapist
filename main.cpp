#include <iostream>
#include "Escapist/Common/ArrayList.h"
#include "Escapist/Common/ByteArray.h"
#include "Escapist/Common/Tuple.h"

#define Output()\
for (SizeType index = 0; index < arr.GetSize(); ++index) {\
std::cout << arr.GetConstAt(index)<< ' ';                 \
}               \
std::cout << std::endl;

int main() {
    Tuple<ArrayList<int>, SizeType> tuple(ArrayList<int>(123, 5), 5);
    ArrayList<int> &arrayList = GetTupleValue<0>(tuple);
    arrayList.GetAt(2) = 456;
    std::cout<<arrayList.GetAt(2);
}

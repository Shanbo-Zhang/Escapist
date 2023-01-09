#include <iostream>
#include "Escapist/Common/ArrayList.h"
#include "Escapist/Common/ByteArray.h"
#include "Escapist/Common/String.h"
#include "Escapist/Common/Tuple.h"

int main() {
    ::srand(::time(nullptr));
    ByteArray pack;
    pack.WriteChar('E').WriteChar('S').WriteChar('P')
            .WriteShort(0x0825)
            .WriteShort(0x0825)
            .WriteShort(0x0825)
            .WriteInt(::rand())
            .WriteLongLong(::time(nullptr))
            .WriteLongLong(2441277578)
            .WriteInt(::rand())
            .WriteChar('S').WriteChar('B').WriteChar('Z');
    String str = pack.GetString();
    std::wcout << str.GetConstData() << std::endl;
    std::wcout << str.GetLength() << std::endl;
    std::wcout << str.GetCapacity() << std::endl;
    std::wcout << wcslen(str.GetConstData()) << std::endl;
    std::wcout << String (L"12312390890890899080890123").Reverse().GetConstData();
}

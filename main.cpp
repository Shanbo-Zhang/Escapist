#include <iostream>
#include "Escapist/Common/ArrayList.h"
#include "Escapist/Common/ByteArray.h"
#include "Escapist/Common/String.h"
#include "Escapist/Common/Tuple.h"
#include "Escapist/Common/Convert.h"
#include "Escapist/Common/Socket.h"

int main(){
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


    DatagramServer ds;
    char addr[4]={12,34,56,78};
    ds.Bind(addr, 8000);
}

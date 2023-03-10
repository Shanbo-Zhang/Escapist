#include <iostream>
#include <vector>
#include "Escapist/Common/ArrayList.h"
#include "Escapist/Common/ByteArray.h"
#include "Escapist/Common/String.h"
#include "Escapist/Common/Tuple.h"
#include "Escapist/Common/Convert.h"
#include "Escapist/Common/Socket.h"
#include "Escapist/Common/HashMap.h"
#include "Escapist/Common/Time.h"

#include "Escapist/GUI/Application.h"
#include "Escapist/GUI/Window.h"

int Main(int argc, Char **argv) {
    Application a(L"Example Application", argc, argv);
    Window w;
    w.Create(nullptr, String(L"12123123122"), Rect(200, 200, 950, 700), Flag<WindowStyle>(WindowStyle::Default));
    ::ShowWindow(w.GetHWND(),SW_SHOWNORMAL);
    return a.Execute();
}
/*
int main(int argc,char** argv){
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

    std::vector<std::vector<int>> vec{
            {1,2,3},
            {4,5,6},
            {7,8,9}
    };

    DatagramServer ds;
    char addr[4]={12,34,56,78};
    ds.Bind(addr, 8000);
}
*/
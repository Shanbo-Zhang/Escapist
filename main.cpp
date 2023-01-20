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
#include "Escapist/GUI/PushButton.h"
#include <string>

class ExampleWindow : public Window {
public:
    bool OnCreate() override {
        //::MessageBox(nullptr, L"Example Window", L"Example", MB_OK);
        return true;
    }

    bool OnDestroy() override {
        Application::Instance()->Quit(0);
        return true;
    }
};

class ExampleButton : public PushButton {
    bool OnClick() override {
        ::MessageBox(nullptr, L"Button is clicked", L"Notification", MB_OK);
        return false;
    }

    bool OnDoubleClick() override {
        ::MessageBox(nullptr, L"Button is double clicked", L"Notification", MB_OK);
        return false;
    }
};

int Main(int argc, Char **argv) {
    Application a(L"Example Application", argc, argv);
    ExampleWindow window;
    window.SetRect(
            Rect().SetLeft(200).SetTop(200)
                    .SetWidth(750).SetHeight(500));
    window.SetBackgroundColor(255, 255, 255);
    window.Create(TEXT("123123"), Flag<WindowStyle>(WindowStyle::DefaultStyle));

    ExampleButton btn;
    btn.SetRect(Rect().SetLeft(8).SetTop(8).SetWidth(200).SetHeight(30));
    btn.Create(
            L"Example Button",
            Flag<PushButton::TextAlignment>(PushButton::TextAlignment::HCenter)
                    .AddFlag(PushButton::TextAlignment::VCenter),
            &window
    );

    window.Show();
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
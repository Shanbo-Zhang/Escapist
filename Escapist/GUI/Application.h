//
// Created by Escap on 1/24/2023.
//

#ifndef ESCAPIST_APPLICATION_H
#define ESCAPIST_APPLICATION_H

#include "General.h"
#include "../Common/String.h"
#include "../Common/ArrayList.h"

class Application {
private:
    static Application *instance;
    static HICON hIconDefault;

    bool running_;
    bool closing_;

    const String name_;
    ArrayList<String> arguments_;

public:
    Application(const Char *const name, int argc, Char **argv)
            : name_(name),
              arguments_(argc, argc),
              running_(false),
              closing_(false) {
        assert(!instance);
        for (SizeType i = 0; i < argc; ++i) {
            arguments_.SetAt(i, String(argv[i]));
        }
        instance = this;
    }

    static Application &Instance() noexcept {
        return *instance;
    }

    static HICON GetDefaultIcon() noexcept {
        return hIconDefault;
    }

    const String &GetName() const noexcept {
        return name_;
    }

    const ArrayList<String> &GetArguments() const noexcept {
        return arguments_;
    }

    bool IsRunning() const noexcept {
        return running_;
    }

    bool IsClosing() const noexcept {
        return closing_;
    }

    int Execute() {
        assert(!running_);
        running_ = true;
        MSG msg;
        while (::GetMessage(&msg, nullptr, 0, 0)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        return (int) msg.wParam;
    }
};

#endif //ESCAPIST_APPLICATION_H

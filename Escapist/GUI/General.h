//
// Created by Escap on 1/22/2023.
//

#ifndef ESCAPIST_GUI_GENERAL_H
#define ESCAPIST_GUI_GENERAL_H

#include "../General.h"
#include "../Common/ArrayList.h"

template<typename ...Args>
class Delegate {
public:
    virtual ~Delegate() {}

    virtual void Execute(Args...args) = 0;
};

template<typename From, typename To, typename ...Args>
class MethodDelegate : public Delegate<Args...> {
private:
    From *from_;

    void (To::*method_)(Args...args);

public:
    MethodDelegate() noexcept
            : from_(nullptr), method_(nullptr) {}

    MethodDelegate(From *const from, void (To::*method)(Args...args)) noexcept
            : from_(from), method_(method) {}

    MethodDelegate(const MethodDelegate<From, To, Args...> &other) noexcept
            : from_(other.from_), method_(other.method_) {}

    void Execute(Args...args) {
        return (from_->*method_)(args...);
    }
};

template<typename ...Args>
class EventHandler {
private:
    ArrayList<Delegate<Args...> *> delegates_;

public:
    ~EventHandler() {
        for (SizeType i = 0; i < delegates_.GetSize(); ++i) {
            delete delegates_.GetAt(i);
        }
    }

    EventHandler &Add(Delegate<Args...> *const delegate) {
        delegates_.Append(delegate);
    }

    EventHandler &Remove(Delegate<Args...> *const delegate) {
        // TODO: ArrayList::Remove!
    }

    void Execute(Args...args) {
        for (SizeType i = 0; i < delegates_.GetSize(); ++i) {
            delegates_.GetAt(i)->Execute(args...);
        }
    }
};

template<typename From, typename To, typename ...Args>
MethodDelegate<From, To, Args...> *MakeDelegate(From *obj, void(To::*func)(Args...args)) {
    return new MethodDelegate<From, To, Args...>(obj, func);
}

enum class WindowState {
    Normal = 0x00000000,
    Hide = 0x00000001,
    Maximized = 0x00000002,
    Minimized = 0x00000004
};

enum class WindowStyle {
    Window = 0x00000001,
    Child = 0x00000002,
    Foreign = 0x00000004,

    Frameless = 0x00000100,
    ThinFrame = 0x00000200,
    ThickFrame = 0x00000400,

    SystemMenu = 0x00001000,
    MinimizeBox = 0x00002000,
    MaximizeBox = 0x00004000,
    HaveCaption = 0x00008000,

    TopMost = 0x00010000,

    Default = Window | ThickFrame | SystemMenu | MinimizeBox | MaximizeBox | HaveCaption
};

enum class EventCode {
    Create,
    Close,
    Destroy
};

#endif //ESCAPIST_GENERAL_H

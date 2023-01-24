//
// Created by Escap on 1/24/2023.
//

#ifndef ESCAPIST_WINDOW_H
#define ESCAPIST_WINDOW_H

#include "General.h"
#include "Application.h"
#include "../Common/String.h"
#include "../Common/ArrayList.h"
#include "../Common/HashMap.h"
#include "../Common/Flag.h"
#include "../Common/Point.h"
#include "../Common/Rect.h"

class Window {
private:
    static HashMap<HWND, Window *> *windowMap_;

    static LRESULT __stdcall WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        Window* window = nullptr;
        if (msg == WM_NCCREATE) {
            assert(window = (Window*)(((LPCREATESTRUCT)lParam)->lpCreateParams));
            if (!Window::windowMap_) {
                Window::windowMap_ = new HashMap<HWND, Window*>(
                        8,
                        [](const HWND& key) {
                            SizeType hash = (SizeType)key;
                            hash ^= (hash >> 20) ^ (hash >> 12);
                            return hash ^ (hash >> 7) ^ (hash >> 4);
                        });
                window->hWnd_ = hWnd;
                Window::windowMap_->Put(hWnd, window);
            }
        }
        else if (Window::windowMap_) {
            window = Window::windowMap_->GetValue(hWnd);
        }

        return ::DefWindowProc(hWnd,msg,wParam,lParam);
    }

protected:
    HWND hWnd_;
    Window *parent_;
    ArrayList<Window *> childs_;

    Rect rect_;
    Point point_;

    HICON hIcon_;
    HICON hIconSm_;

    Flag<WindowState> state_;
    Flag<WindowStyle> style_;

    DWORD style_sys_;
    DWORD extendedStyle_sys_;

    String class_;
    String text_;

    unsigned long foregroundColor_;
    unsigned long backgroundColor_;

    static DWORD StyleFromFlag(Flag<WindowStyle> style) {
        DWORD result = 0;
        if (style.HasFlag(WindowStyle::Child)) {
            result |= WS_CHILD;
        }
        if (style.HasFlag(WindowStyle::HaveCaption)) {
            result |= WS_CAPTION;
        }
        if (style.HasFlag(WindowStyle::ThickFrame)) {
            result |= (WS_THICKFRAME | WS_OVERLAPPED);
        } else if (style.HasFlag(WindowStyle::ThinFrame)) {
            result |= (WS_THICKFRAME);
        } else if (style.HasFlag(WindowStyle::Frameless)) {
            result |= (WS_POPUP);
        }
        if (style.HasFlag(WindowStyle::SystemMenu)) {
            result |= WS_SYSMENU;
        }
        if (style.HasFlag(WindowStyle::MaximizeBox)) {
            result |= WS_MAXIMIZEBOX;
        }
        if (style.HasFlag(WindowStyle::MinimizeBox)) {
            result |= WS_MINIMIZEBOX;
        }
        return result;
    }

    static const String RegisterWindowClass(Window *window) {
        assert(window->class_.IsEmptyOrNull());
        window->class_.Append(Application::Instance().GetName())
                .Append('-')
                .Append(window->text_);

        WNDCLASS wc;
        while (::GetClassInfo(
                ::GetModuleHandle(nullptr),
                window->class_.Left(256).GetConstData(),
                &wc)) {
            window->class_.Append('-')
                    .Append(window->text_);
            // TODO: or add UUID
        }

        wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wc.lpfnWndProc = Window::WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = ::GetModuleHandle(nullptr);
        wc.hIcon = window->hIcon_ ? Application::Instance().GetDefaultIcon() : nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = ::CreateSolidBrush(window->backgroundColor_);
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = window->class_.Left(256).GetConstData();
        assert(::RegisterClass(&wc));
        return window->class_;
    }

public:
    Window() = default;

    ~Window() noexcept {
        if (windowMap_
            && windowMap_->RemoveValue(hWnd_).GetCount()) {
            delete windowMap_;
        }
    }

    Window *const GetParent() noexcept {
        return parent_;
    }

    HWND GetHWND() const noexcept {
        return hWnd_;
    }

    void Create(
            Window *parent,
            const String &windowText_,
            const Rect &windowRect,
            const Flag<WindowStyle> &windowStyle) {
        style_ = windowStyle;
        style_sys_ = Window::StyleFromFlag(style_);
        extendedStyle_sys_ = WS_EX_LEFT;
        rect_ = windowRect;
        text_.Assign(windowText_);
        parent_ = parent;

        ::CreateWindowEx(
                extendedStyle_sys_,
                RegisterWindowClass(this).GetConstData(),
                text_.GetConstData(),
                style_sys_,
                rect_.GetLeft(),
                rect_.GetTop(),
                rect_.GetWidth(),
                rect_.GetHeight(),
                parent_ ? parent_->hWnd_ : nullptr,
                nullptr,
                ::GetModuleHandle(nullptr),
                this
        );
    }
};

HashMap<HWND, Window *> *Window::windowMap_ = nullptr;

#endif //ESCAPIST_WINDOW_H

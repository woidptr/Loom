#pragma once
#include <Windows.h>
#include "../Hook.hpp"

struct WindowProcHookTag {};

class WindowProcHook : public Hook<WindowProcHookTag, LRESULT(HWND, UINT, WPARAM, LPARAM)> {
public:
    WindowProcHook() : Hook($get_signature("MainWindow::_windowProc")) {
        this->hook();
    }
};
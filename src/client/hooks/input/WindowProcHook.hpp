#pragma once
#include <Windows.h>
#include "../Hook.hpp"

class WindowProcHook : public Hook<LRESULT(HWND, UINT, WPARAM, LPARAM)> {
public:
    WindowProcHook() : Hook($getSignature("MainWindow::_windowProc")) {
        this->hook();
    }
};
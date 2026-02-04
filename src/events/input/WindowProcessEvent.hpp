#pragma once
#include <Windows.h>
#include <events/EventHandler.hpp>

struct WindowProcessEvent : public BaseEventContext {
    HWND hWnd;
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
};
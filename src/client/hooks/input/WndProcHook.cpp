#include <Windows.h>
#include <windowsx.h>
#include "WndProcHook.hpp"
#include <core/Logger.hpp>

LRESULT __stdcall WndProcHook::callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		for (auto& kcb : keyboardCallbacks) {
			kcb((int)wParam);
		}

		break;
	case WM_KEYUP:
		break;
	case WM_MOUSEMOVE:
		for (auto& mcb : mouseCallbacks) {
			mcb(0, 0);
		}

		break;
	}

	return CallWindowProcW((WNDPROC)original, hWnd, uMsg, wParam, lParam);
}

void WndProcHook::registerKeyboardCallback(std::function<void(int)> fn) {
	keyboardCallbacks.push_back(fn);
}

void WndProcHook::registerMouseCallback(std::function<void(int, int)> fn) {
	mouseCallbacks.push_back(fn);
}

WndProcHook::WndProcHook() : Hook(Signatures::Level::tick) {
	this->hook();
}
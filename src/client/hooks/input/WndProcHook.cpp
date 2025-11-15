#include <Windows.h>
#include <windowsx.h>
#include "WndProcHook.h"
#include "../../../core/Logger.h"

LONG_PTR WndProcHook::original = NULL;

POINT freezePosClient = { 0, 0 };   // mouse freeze point in client coordinates
POINT freezePosScreen = { 0, 0 };   // same point converted to screen coords
bool ignoreWarp = false;

void onKeyPressed(int key) {
	if (key == VK_TAB) {
		// ShowCursor(FALSE);
		/*HWND wnd = FindWindow(NULL, "Minecraft");
		RECT rect;
		GetClientRect(wnd, &rect);

		POINT upperLeft = { rect.left, rect.top };
		POINT lowerRight = { rect.right, rect.bottom };

		ClientToScreen(wnd, &upperLeft);
		ClientToScreen(wnd, &lowerRight);

		RECT clipRect = { upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y };

		ClipCursor(&clipRect);*/
		ignoreWarp = true;
	}
	else
	{
		// ShowCursor(TRUE);
		// ClipCursor(NULL);

		ignoreWarp = false;
	}
}

LRESULT __stdcall WndProcHook::callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		for (auto& kcb : keyboardCallbacks) {
			kcb((int)wParam);
		}

		// onKeyPressed((int)wParam);
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

WndProcHook::WndProcHook() : Hook("WndProc", 1) {
	this->hook();
}
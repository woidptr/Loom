#pragma once
#include "../Hook.hpp"
#include <imgui.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class WndProcHook : public Hook<LRESULT __stdcall, HWND, UINT, WPARAM, LPARAM> {
private:
	static inline LONG_PTR original = NULL;
	static inline std::vector<std::function<bool(HWND, UINT, WPARAM, LPARAM)>> callbacks;
	static inline std::vector<std::function<void(int)>> keyboardCallbacks;
	static inline std::vector<std::function<void(int, int)>> mouseCallbacks;
public:
	WndProcHook();

	static LRESULT __stdcall callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void registerKeyboardCallback(std::function<void(int)> fn);
	void registerMouseCallback(std::function<void(int, int)> fn);
	void registerCallback(std::function<bool(HWND, UINT, WPARAM, LPARAM)> fn) {
		callbacks.push_back(fn);
	}

	virtual void hook() override {
		SetWindowLongPtr(FindWindow(NULL, "Minecraft"), GWLP_WNDPROC, (LONG_PTR)callback);

		Logger::info(std::format("Successfully initialized hook {}", this->name));
	}
};
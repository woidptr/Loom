#pragma once
#include "../Hook.hpp"

class WndProcHook : public Hook<LRESULT __stdcall, HWND, UINT, WPARAM, LPARAM> {
private:
	static inline LONG_PTR original = NULL;
	static inline std::vector<std::function<void(int)>> keyboardCallbacks;
	static inline std::vector<std::function<void(int, int)>> mouseCallbacks;
public:
	WndProcHook();

	static LRESULT __stdcall callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void registerCallback(std::function<LRESULT __stdcall(HWND, UINT, WPARAM, LPARAM)>) = delete;

	void registerKeyboardCallback(std::function<void(int)> fn);
	void registerMouseCallback(std::function<void(int, int)> fn);

	virtual void hook() override {
		SetWindowLongPtr(FindWindow(NULL, "Minecraft"), GWLP_WNDPROC, (LONG_PTR)callback);

		Logger::info(std::format("Successfully initialized hook {}", "WndProcHook"));
	}
};
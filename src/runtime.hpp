#pragma once
#include <Windows.h>
#include <kiero.hpp>
#include <core/Logger.hpp>
#include <core/FakeImports.hpp>
#include <hooks/HookManager.hpp>
#include <semaphore>
#include <client/Client.hpp>

struct RuntimeContext {
    HookManager* hookManager;
};

class Runtime {
private:
    static inline std::binary_semaphore ejectSignal{0};
    static inline HMODULE hModule = nullptr;

    static bool performStartupSequence();
public:
    static DWORD WINAPI init(LPVOID lpParam);
    static void eject();
    static void shutdown();
};
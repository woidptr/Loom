#pragma once
#include <Windows.h>
#include <kiero.hpp>
#include <core/Logger.hpp>
#include <core/FakeImports.hpp>
#include <hooks/HookManager.hpp>
#include <client/Client.hpp>

class Runtime {
public:
    static DWORD WINAPI init(LPVOID lpParam);
    static void shutdown();
};
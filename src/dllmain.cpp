#include <format>
#include <Windows.h>
#include "runtime.hpp"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, Runtime::init, (LPVOID)hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        Runtime::shutdown();
        break;
    }
    return TRUE;
}

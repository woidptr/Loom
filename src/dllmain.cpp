#include <Windows.h>
#include "client/Client.h"
#include "core/Logger.h"
#include <libhat/scanner.hpp>
#include <format>

void init(HMODULE hModule) {
    Logger::init();

    Logger::info("Test log");

    Client::construct();

    /*constexpr hat::fixed_signature pattern = hat::compile_signature<"E8 ? ? ? ? F3 41 0F 10 4C 24 ? F3 41 0F 10 04 24 F3 0F 11 45 ? F3 0F 11 75">();
    hat::scan_result result = hat::find_pattern(pattern, ".text");
    std::byte* address = result.get();*/

    // Logger::info(std::format("Tessellator::begin address {}", (uintptr_t)address));
}

void cleanup() {
    Client::destruct();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)init, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        cleanup();
        break;
    }
    return TRUE;
}

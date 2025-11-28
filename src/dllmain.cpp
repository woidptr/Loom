#include <Windows.h>
#include "client/Client.hpp"
#include "core/Logger.hpp"
#include <libhat/scanner.hpp>
#include <format>

void init(HMODULE hModule) {
    Logger::init();

    SignatureRegistry::registerSignatures();

    if (!SignatureRegistry::performHealthCheck()) {
        Logger::critical("Signature healthcheck failed, silently ejecting...");

        FreeLibraryAndExitThread(hModule, 0);
    }

    Client::construct();
}

void cleanup() {
    Client::destruct();
    Logger::shutdown();
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

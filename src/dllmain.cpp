#include <format>
#include <Windows.h>
#include <client/Client.hpp>
#include <core/Logger.hpp>
#include <core/Memory.hpp>
#include <libhat/scanner.hpp>
#include <core/FakeImports.hpp>

void init(HMODULE hModule) {
    Logger::init();

    SignatureRegistry::registerSignatures();

    if (!SignatureRegistry::performHealthCheck()) {
        $log_critical("Signature healthcheck failed, silently ejecting...");

        FreeLibraryAndExitThread(hModule, 0);
    }

    /*fi::FakeImportConfig::set_config({
        .mock_dll_name = "mcapi.dll",
        .resolve_address = Memory::ResolveAddress,
    });
    fi::load_all_imports();*/

    FakeImports::construct({
        .dll_name = "mcapi.dll",
        .resolve = Memory::ResolveAddress,
    });

    FakeImports::load_all_imports();

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

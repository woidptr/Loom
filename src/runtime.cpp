#include "runtime.hpp"
// #include <core/embedder.hpp>

DWORD WINAPI Runtime::init(LPVOID lpParam) {
    HMODULE hModule = static_cast<HMODULE>(lpParam);

    Logger::init();

    SignatureRegistry::registerSignatures();

    if (!SignatureRegistry::performHealthCheck()) {
        $log_critical("Signature healthcheck failed, silently ejecting...");

        return 0;
    }

    FakeImports::construct({
        .dll_name = "mcapi.dll",
        .resolve = Memory::ResolveAddress,
    });
    FakeImports::load_all_imports();

    kiero::init(kiero::RenderType::Auto);

    HookManager::construct();

    Client::construct();

    return 0;
}

void Runtime::shutdown() {
    Client::destruct();
    HookManager::destruct();
    kiero::shutdown();
    Logger::shutdown();
}
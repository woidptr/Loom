#include "runtime.hpp"

bool Runtime::performStartupSequence() {
    Logger::construct();
    SignatureRegistry::registerSignatures();

    if (!SignatureRegistry::performHealthCheck()) {
        $log_critical("Signature healthcheck failed, silently ejecting...");
        return false;
    }

    FakeImports::load_all_imports({
        .dll_name = MCAPI_DLL_NAME,
        .resolve = Memory::ResolveAddress,
    });

    kiero::init(kiero::RenderType::Auto);

    HookManager::construct();
    Client::construct();

    return true;
}

DWORD WINAPI Runtime::init(LPVOID lpParam) {
    hModule = static_cast<HMODULE>(lpParam);

    if (performStartupSequence()) {
        ejectSignal.acquire();
    }

    shutdown();

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

void Runtime::eject() {
    ejectSignal.release();
}

void Runtime::shutdown() {
    Client::destruct();
    HookManager::destruct();
    kiero::shutdown();
    Logger::destruct();
}
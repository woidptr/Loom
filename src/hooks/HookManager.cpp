#include "HookManager.hpp"
#include "impl/render/SetupAndRenderHook.hpp"

void HookManager::construct() {
    // hooks.emplace_back(new SetupAndRenderHook());
    renderHook = new SetupAndRenderHook();
    packetsHook = new IncomingPacketsHook();
}

void HookManager::destruct() {
    /*for (Hook* hook : hooks) {
        delete hook;
    }*/

    delete renderHook;
    delete packetsHook;
}
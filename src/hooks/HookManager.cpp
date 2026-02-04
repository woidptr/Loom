#include "HookManager.hpp"

void HookManager::construct() {
    // hooks.emplace_back(new SetupAndRenderHook());
    /*renderHook = new SetupAndRenderHook();
    packetsHook = new IncomingPacketsHook();
    windowProcHook = new WindowProcHook();*/

    InputHooks::init();
    RenderHooks::init();
    NetworkHooks::init();
}

void HookManager::destruct() {
    /*for (Hook* hook : hooks) {
        delete hook;
    }*/

    /*delete renderHook;
    delete packetsHook;
    delete windowProcHook;*/

    inlineHooks.clear();
    midHooks.clear();
}
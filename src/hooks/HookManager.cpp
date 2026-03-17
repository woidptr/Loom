#include "HookManager.hpp"

void HookManager::construct() {
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
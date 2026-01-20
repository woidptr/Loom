#pragma once
#include <vector>
// #include "Hook.hpp"

#include "impl/render/SetupAndRenderHook.hpp"
#include "impl/network/IncomingPacketsHook.hpp"
#include <safetyhook.hpp>

class HookManager {
private:
    // std::vector<Hook> hooks;
    static inline SetupAndRenderHook* renderHook = nullptr;
    static inline IncomingPacketsHook* packetsHook = nullptr;
    static inline std::vector<SafetyHookInline> inlineHooks;
    static inline std::vector<SafetyHookMid> midHooks;
public:
    static void construct();
    static void destruct();
};
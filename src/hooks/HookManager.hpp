#pragma once
#include <vector>
#include <safetyhook.hpp>
#include <sdk/GameContext.hpp>
#include <events/EventHandler.hpp>

#include <hooks/impl/RenderHooks.hpp>
#include <hooks/impl/InputHooks.hpp>
#include <hooks/impl/NetworkHooks.hpp>

class HookManager {
private:
    static inline std::vector<SafetyHookInline*> inlineHooks;
    static inline std::vector<SafetyHookMid*> midHooks;
public:
    static void construct();
    static void destruct();

    static void createInlineHook(SafetyHookInline& hook, void* target, void* destination) {
        hook = safetyhook::create_inline(target, destination);
        inlineHooks.emplace_back(&hook);
    }

    //void createMidHook(SafetyHookMid& hook, void* target) {
    //    // hook = safetyhook::create_mid(target, )
    //}
};
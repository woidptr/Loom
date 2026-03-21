#pragma once
#include <vector>
#include <safetyhook.hpp>
#include <sdk/GameContext.hpp>
#include <events/EventHandler.hpp>

#include <hooks/impl/RenderHooks.hpp>
#include <hooks/impl/InputHooks.hpp>
#include <hooks/impl/NetworkHooks.hpp>

template <typename T>
concept HookableFunction =
    std::is_member_function_pointer_v<T> ||
    (std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>);

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

    template <auto Fn>
        requires HookableFunction<decltype(Fn)>
    static void createInlineHook(SafetyHookInline& hook, void* destination) {
        using FnType = decltype(Fn);
        void* target_address = nullptr;

        if constexpr (std::is_member_function_pointer_v<FnType>) {
            union {
                FnType fn;
                void* addr;
            } extractor{};

            extractor.fn = Fn;
            target_address = extractor.addr;
        } else {
            target_address = std::bit_cast<void*>(Fn);
        }

        createInlineHook(hook, target_address, destination);
    }

    //void createMidHook(SafetyHookMid& hook, void* target) {
    //    // hook = safetyhook::create_mid(target, )
    //}
};
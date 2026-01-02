#pragma once
#include <string>
#include <safetyhook.hpp>
#include <core/Memory.hpp>

struct CallbackContext {
private:
    bool cancelFlag = false;
public:
    bool isCancelled() const { return cancelFlag; }
    void cancel() { cancelFlag = true; }
};

template <typename Tag, typename TFunc>
class Hook;

template <typename Tag, typename TReturn, typename...Args>
class Hook<Tag, TReturn(Args...)> {
private:
    using BeforeCallbackFunction = std::function<void(CallbackContext&, Args...)>;
    using AfterCallbackFunction = std::function<void(Args...)>;
    using ReturnCallbackFunction = std::function<std::optional<TReturn>(CallbackContext&, Args...)>;
private:
    uintptr_t address = 0;
private:
    static inline SafetyHookInline inlineHook;
    static inline std::vector<BeforeCallbackFunction> beforeCallbacks;
    static inline std::vector<AfterCallbackFunction> afterCallbacks;
    static inline ReturnCallbackFunction returnCallback = nullptr;
public:
    Hook(Signature* signature) : address(signature->getAddress()) {}
    Hook(uintptr_t address) : address(address) {}

    static TReturn callback(Args...args) {
        CallbackContext cbCtx;

        if constexpr (!std::is_void_v<TReturn>) {
            if (returnCallback) {
                std::optional<TReturn> value = returnCallback(cbCtx, args...);

                if (value.has_value()) {
                    return *value;
                }
            }
        }

        for (auto& cb : beforeCallbacks) {
            cb(cbCtx, args...);
        }

        if constexpr (!std::is_void_v<TReturn>) {
            if (cbCtx.isCancelled()) {
                return TReturn{};
            }

            TReturn value = inlineHook.call<TReturn>(args...);

            for (auto& cb : afterCallbacks) {
                cb(args...);
            }

            return value;
        } else {
            if (cbCtx.isCancelled()) {
                return;
            }

            inlineHook.call<TReturn>(args...);

            for (auto& cb : afterCallbacks) {
                cb(args...);
            }
        }
    }

    void registerCallbackBeforeOriginal(BeforeCallbackFunction&& fn) {
        beforeCallbacks.emplace_back(std::forward<BeforeCallbackFunction>(fn));
    }

    void registerCallbackAfterOriginal(AfterCallbackFunction&& fn) {
        afterCallbacks.emplace_back(std::forward<AfterCallbackFunction>(fn));
    }

    void registerReturnCallback(ReturnCallbackFunction&& fn)
        requires(!std::is_void_v<TReturn>)
    {
        returnCallback = std::forward<ReturnCallbackFunction>(fn);
    }

    void hook() {
        inlineHook = safetyhook::create_inline((void*)address, (void*)&callback);
    }
};

class MidHook {
private:
    using CallbackFunction = std::function<void(SafetyHookContext&)>;
private:
    uintptr_t address = 0;

    static inline SafetyHookMid midHook;
    static inline CallbackFunction callbackFunction = nullptr;
public:
    MidHook(Signature* signature) : address(signature->getAddress()) {}

    static void callback(SafetyHookContext& ctx) {
        if (callbackFunction) {
            callbackFunction(ctx);
        }
    }

    void registerCallback(CallbackFunction&& fn) {
        callbackFunction = std::forward<CallbackFunction>(fn);
    }

    void hook() {
        midHook = safetyhook::create_mid((void*)this->address, &MidHook::callback);
    }
};
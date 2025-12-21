#pragma once
#include <string>
#include <safetyhook.hpp>
#include <core/Signatures.hpp>

struct CallbackContext {
private:
    bool cancelFlag = false;
public:
    bool isCancelled() const {
        return cancelFlag;
    }

    void cancel() {
        cancelFlag = true;
    }
};

template <typename>
class Hook;

template <typename TReturn, typename...Args>
class Hook<TReturn(Args...)> {
private:
    using CallbackFunction = std::function<void(CallbackContext&, Args...)>;
    using ReturnCallbackFunction = std::function<std::optional<TReturn>(CallbackContext&, Args...)>;
private:
    uintptr_t address = 0;
private:
    static inline SafetyHookInline inlineHook;
    static inline std::vector<CallbackFunction> callbacks;
    static inline ReturnCallbackFunction returnCallback = nullptr;
public:
    Hook() {}
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

        for (auto& cb : callbacks) {
            cb(cbCtx, args...);
        }

        if (cbCtx.isCancelled()) {
            if constexpr (!std::is_void_v<TReturn>) {
                return TReturn{};
            } else {
                return;
            }
        }

        return inlineHook.call<TReturn>(args...);
    }

    void registerCallback(CallbackFunction&& fn) {
        callbacks.emplace_back(std::forward<CallbackFunction>(fn));
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
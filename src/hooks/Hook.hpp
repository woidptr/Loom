#pragma once
#include <cstdint>
#include <safetyhook.hpp>
#include <core/Memory.hpp>
#include <sdk/GameContext.hpp>
#include "EventHandler.hpp"

class Hook {
public:
    virtual ~Hook() = default;
    virtual void install() = 0;
};

class InlineHook : public Hook {
protected:
    SafetyHookInline hookObject;

    template <typename T, typename... Args>
    auto callOriginal(Args... args) {
        return hookObject.call<T>(args...);
    }
};

class MultiInlineHook : public Hook {
protected:
    std::unordered_map<int, safetyhook::InlineHook> hookMap;

    template <typename T, typename... Args>
    auto callOriginal(int id, Args... args) {
        if (hookMap.count(id)) return hookMap[id].call<T>(args...);
        return T{};
    }
};
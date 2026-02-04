#pragma once
#include <safetyhook.hpp>

template <typename T>
class InlineHook;

template <typename TReturn, typename... Args>
class InlineHook<TReturn(Args...)> {
private:
    SafetyHookInline hook;
public:
    SafetyHookInline& getHook() {
        return hook;
    }

    TReturn call(Args... args) {
        return hook.call<TReturn>(args...);
    }
};

template <typename TReturn, typename ClassType, typename... Args>
class InlineHookMemberImpl {
private:
    SafetyHookInline hook;
public:
    SafetyHookInline& getHook() {
        return hook;
    }

    TReturn call(ClassType self, Args... args) {
        return hook.call<TReturn, ClassType, Args...>(self, args...);
    }
};

#define $specialize_member_hook(cv_ref_quals, self_ptr_type) \
    template <typename TReturn, typename ClassType, typename... Args> \
    class InlineHook<TReturn(ClassType::*)(Args...) cv_ref_quals> \
        : public InlineHookMemberImpl<TReturn, self_ptr_type, Args...> { \
    };

$specialize_member_hook(, ClassType*)
$specialize_member_hook(const, const ClassType*)
$specialize_member_hook(volatile, volatile ClassType*)
$specialize_member_hook(const volatile, const volatile ClassType*)

#undef $specialize_member_hook
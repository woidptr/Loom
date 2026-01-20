#pragma once
#include "../../Hook.hpp"

struct SetupAndRenderEvent : public CancellableEventContext {
    ScreenView* screenView;
    MinecraftUIRenderContext* renderCtx;
};

class SetupAndRenderHook {
private:
    static inline SafetyHookInline inlineHook;
public:
    SetupAndRenderHook() {
        this->hook();
    }

    static void callback(ScreenView* screenView, MinecraftUIRenderContext* renderCtx);

    void hook() {
        inlineHook = safetyhook::create_inline((void*)$get_address("ScreenView::setupAndRender"), (void*)&callback);
    }
};

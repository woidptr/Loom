#include "SetupAndRenderHook.hpp"

void SetupAndRenderHook::callback(ScreenView *screenView, MinecraftUIRenderContext *renderCtx) {
    SetupAndRenderEvent event = SetupAndRenderEvent {
        .screenView = screenView,
        .renderCtx = renderCtx,
    };

    EventHandler::emit(event);

    if (event.isCancelled()) {
        return;
    }

    return inlineHook.call<void>(screenView, renderCtx);
}

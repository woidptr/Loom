#include "Zoom.hpp"
#include <client/hooks/HookRegistry.hpp>

Zoom::Zoom() : Module("Zoom") {
    Hooks::getFovHook->registerReturnCallback(
        [&](CallbackContext& cbCtx, LevelRendererPlayer* levelRendererPlayer, float a2, bool a3) -> std::optional<float> {
            return getFovCallback(levelRendererPlayer, a2, a3);
        }
    );
}

std::optional<float> Zoom::getFovCallback(LevelRendererPlayer* a1, float a2, bool a3) {
    if (!enabled) {
        return std::nullopt;
    }

    static bool once = false;

    if (!once) {
        /*GameContext::localPlayer = a1->localPlayer;

        void** vtable = *(void***)a1;
        uintptr_t staticVtable = reinterpret_cast<uintptr_t>(vtable) - reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

        $logDebug("LevelRendererPlayer vtable address: 0x{:X}", 0x140000000 + staticVtable);
        $logDebug("LevelRendererPlayer::callRenderNametags runtime address: 0x{:X}", (uintptr_t)vtable[11])*/

        once = true;
    }

    if (zoomedIn) {
        return zoomFov;
    } else {
        return std::nullopt;
    }
}

void Zoom::keyboardCallback(int key, bool held) {
    if (key == 'Z') {
        if (held) {
            zoomedIn = true;
        } else {
            zoomedIn = false;
        }
    }
}
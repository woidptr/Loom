#include "Keystrokes.hpp"

Keystrokes::Keystrokes() : Module("Keystrokes", "keystrokes") {
    listeners.reserve(1);

    $add_listener(HudElementRenderEvent, &Keystrokes::onHudElementRender);
}

void Keystrokes::onHudElementRender(HudElementRenderEvent* event) {
    if (LocalPlayer* lp = GameContext::clientInstance->getLocalPlayer()) {
        MoveInputComponent* mic = lp->mEntityContext.tryGetComponent<MoveInputComponent>();

        if (mic) {
            bool forward = mic->mFlagValues.test(static_cast<size_t>(MoveInputState::Flag::Up));
        }
    }
}
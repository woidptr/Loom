#include "ArmorHud.hpp"

ArmorHud::ArmorHud() : Module("Armor HUD") {
    listeners.reserve(1);

    $add_listener(HudElementRenderEvent, &ArmorHud::onHudElementRender);
}

void ArmorHud::onHudElementRender(HudElementRenderEvent* event) {
    /*if (LocalPlayer* lp = event.renderCtx->mClient->getLocalPlayer()) {
        ActorEquipmentComponent* equipment = lp->mEntityContext.tryGetComponent<ActorEquipmentComponent>();

        if (equipment) {

        }
    }*/
}
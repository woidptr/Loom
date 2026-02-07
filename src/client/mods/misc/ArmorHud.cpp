#include "ArmorHud.hpp"

ArmorHud::ArmorHud() : Module("Armor HUD") {
    listeners.reserve(1);

    listeners.emplace_back(EventHandler::subscribe<SetupAndRenderEvent, &ArmorHud::onRender>(*this));
}

void ArmorHud::onRender(SetupAndRenderEvent* event) {
    /*if (LocalPlayer* lp = event.renderCtx->mClient->getLocalPlayer()) {
        ActorEquipmentComponent* equipment = lp->mEntityContext.tryGetComponent<ActorEquipmentComponent>();

        if (equipment) {

        }
    }*/
}
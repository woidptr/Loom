#include "PotionHud.hpp"

PotionHud::PotionHud() : Module("Potion HUD", "potion_hud") {
    listeners.reserve(1);

    $add_listener(HudElementRenderEvent, &PotionHud::onHudElementRender);
}

void PotionHud::onHudElementRender(HudElementRenderEvent *event) {
    // if (LocalPlayer* lp = GameContext::clientInstance->getLocalPlayer()) {
    //     $log_debug("Got local player");
    //     MobEffectsComponent* mec = lp->mEntityContext.tryGetComponent<MobEffectsComponent>();
    //
    //     if (mec) {
    //         $log_debug("Got mob effects component");
    //         for (MobEffectInstance effect : mec->mMobEffects) {
    //             $log_debug("Effect duration: {}", effect.mDuration.mValue);
    //         }
    //     }
    // }
}
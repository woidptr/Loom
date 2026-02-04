#include "HudScreen.hpp"
#include <events/EventHandler.hpp>
#include <events/render/HudElementRenderEvent.hpp>

void HudScreen::render() {
    HudElementRenderEvent event{
        .isInEditMode = false,
    };

    EventHandler::emit(event);
}
#include "ScreenManager.hpp"
#include "screen/SettingsScreen.hpp"
#include "screen/HudScreen.hpp"

void ScreenManager::render() {
    std::string currentInGameScreen = GameContext::clientInstance->getTopScreenName();

    if (currentInGameScreen == "start_screen") {
        if (!currentScreen || currentScreen.get()->name != "SettingsScreen") {
            setScreen(std::make_unique<SettingsScreen>());
        }
    }
    else if (currentInGameScreen == "hud_screen") {
        if (!currentScreen || currentScreen.get()->name != "HudScreen") {
            setScreen(std::make_unique<HudScreen>());
        }
    }
    else {
        setScreen(nullptr);
    }

    if (currentScreen) {
        currentScreen->render();
    }
}
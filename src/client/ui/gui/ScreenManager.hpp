#pragma once
#include <memory>
#include "IScreen.hpp"
#include "screen/StartScreen.hpp"
#include <sdk/GameContext.hpp>

class ScreenManager {
private:
    static inline std::unique_ptr<IScreen> currentScreen;
public:
    static void setScreen(std::unique_ptr<IScreen> screen) {
        currentScreen = std::move(screen);
    }

    static IScreen* getCurrentScreen() {
        return currentScreen.get();
    }

    static void render() {
        std::string currentInGameScreen = GameContext::clientInstance->getTopScreenName();

        if (currentInGameScreen == "start_screen") {
            if (!currentScreen || currentScreen.get()->name != "StartScreen") {
                setScreen(std::make_unique<StartScreen>());
            }
        }
        else {
            setScreen(nullptr);
        }

        if (currentScreen) {
            currentScreen->render();
        }
    }
};
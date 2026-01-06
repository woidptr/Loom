#pragma once
#include <memory>
#include "IScreen.hpp"

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
        if (currentScreen) {
            currentScreen->render();
        }
    }
};
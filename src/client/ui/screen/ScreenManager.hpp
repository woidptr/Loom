#pragma once
#include <memory>
#include "Screen.hpp"

class ScreenManager {
private:
    static inline std::unique_ptr<Screen> currentScreen;
public:
    static void setScreen(std::unique_ptr<Screen> screen) {
        currentScreen = std::move(screen);
    }

    static Screen* getCurrentScreen() {
        return currentScreen.get();
    }

    static void render() {
        if (currentScreen) {
            currentScreen->render();
        }
    }
};
#pragma once
#include "IScreen.hpp"

class StartScreen : public IScreen {
public:
    StartScreen() : IScreen("start_screen") {}

    virtual void render() override;
};
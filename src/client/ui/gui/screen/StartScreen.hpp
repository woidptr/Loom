#pragma once
#include "../IScreen.hpp"

class StartScreen : public IScreen {
public:
    StartScreen() : IScreen("StartScreen") {}

    virtual void render() override;
};
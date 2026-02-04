#pragma once
#include <client/ui/gui/IScreen.hpp>

class HudScreen : public IScreen {
public:
    HudScreen() : IScreen("HudScreen") {}

    virtual void render() override;
};
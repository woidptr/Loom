#pragma once
#include "../IScreen.hpp"

class HudEditorScreen : public IScreen {
public:
    HudEditorScreen() : IScreen("CustomizationScreen") {}

    virtual void render() override;
};
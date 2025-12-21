#pragma once
#include "Screen.hpp"

class CustomizationScreen : public Screen {
public:
    CustomizationScreen() : Screen("CustomizationScreen") {}

    virtual void render() override;
};
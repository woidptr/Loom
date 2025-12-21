#pragma once
#include "../Hook.hpp"
#include <sdk/mc/client/render/screen/MinecraftUIRenderContext.hpp>
#include <core/Signatures.hpp>

class SetupAndRenderHook : public Hook<void(void*, MinecraftUIRenderContext*)> {
public:
    SetupAndRenderHook() : Hook($getSignature("ScreenView::setupAndRender")) {
        this->hook();
    }
};
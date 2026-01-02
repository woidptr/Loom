#pragma once
#include "../Hook.hpp"
#include <sdk/mc/client/gui/screens/ScreenView.hpp>
#include <sdk/mc/client/render/screen/MinecraftUIRenderContext.hpp>
#include <core/Signatures.hpp>

struct SetupAndRenderHookTag {};

class SetupAndRenderHook : public Hook<SetupAndRenderHookTag, void(ScreenView*, MinecraftUIRenderContext*)> {
public:
    SetupAndRenderHook() : Hook($get_signature("ScreenView::setupAndRender")) {
        this->hook();
    }
};
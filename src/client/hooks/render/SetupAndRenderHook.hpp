#pragma once
#include "../Hook.hpp"

#include <sdk/render/MinecraftUIRenderContext.hpp>

class SetupAndRenderHook : public Hook<void, void*, MinecraftUIRenderContext*> {
public:
	SetupAndRenderHook() : Hook(Signatures::ScreenView::setupAndRender) {
		this->hook();
	}
};
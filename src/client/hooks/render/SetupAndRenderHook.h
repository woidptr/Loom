#pragma once
#include "../Hook.h"

#include "../../../sdk/render/MinecraftUIRenderContext.h"

class SetupAndRenderHook : public Hook<void, void*, MinecraftUIRenderContext*> {
public:
	SetupAndRenderHook() : Hook(Signatures::ScreenView::setupAndRender.getName(), Signatures::ScreenView::setupAndRender.getAddress()) {
		this->hook();
	}
};
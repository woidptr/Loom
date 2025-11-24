#pragma once
#include "../Hook.hpp"
#include <sdk/render/MinecraftUIRenderContext.hpp>
#include <core/Signatures.hpp>

class SetupAndRenderHook : public Hook<void, void*, MinecraftUIRenderContext*> {
public:
	SetupAndRenderHook() : Hook(SignatureRegistry::getSignature("ScreenView::setupAndRender")) {
		this->hook();
	}
};
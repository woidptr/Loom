#pragma once
#include "ScreenContext.h"
#include <sdk/client/IClientInstance.h>
#include <sdk/math/RectangleArea.h>
#include <libhat/access.hpp>
#include <core/Logger.h>
#include <core/Signatures.h>

class UIScene {
public:
	void** vtable;
};

class MinecraftUIRenderContext {
public:
	IClientInstance* clientInstance;
	ScreenContext* screenContext;
public:
	void getUIScene() {
		Signature UIScene = Signature("MinecraftUIRenderContext::UIScene*", "48 89 AE ? ? ? ? C6 86 ? ? ? ? ? 48 8B C6", 1);

		Logger::info(std::format("UIScene offset: 0x{:X}", UIScene.getAddress()));
	}

	virtual ~MinecraftUIRenderContext();
	virtual void getLineLength();
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
};

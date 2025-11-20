#pragma once
#include "ScreenContext.h"
#include <sdk/client/IClientInstance.h>
#include <sdk/utility/RectangleArea.h>
#include <sdk/utility/mce.h>
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
	IClientInstance* getClientInstance() {
		return this->clientInstance;
	}

	void getUIScene() {
		Signature UIScene = Signature("MinecraftUIRenderContext::UIScene*", "48 89 AE ? ? ? ? C6 86 ? ? ? ? ? 48 8B C6", 1);

		Logger::info(std::format("UIScene offset: 0x{:X}", UIScene.getAddress()));
	}

	virtual ~MinecraftUIRenderContext();
	virtual void getLineLength();
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual void drawDebugText();
	virtual void drawText();
	virtual void flushText();
	virtual void drawImage();
	virtual void drawNineslice();
	virtual void flushImages();
	virtual void beginSharedMeshBatch();
	virtual void endSharedMeshBatch();
	virtual void drawRectangle(const RectangleArea& rect, const mce::Color& color, float alpha, int thickness);
	virtual void fillRectangle(const RectangleArea& rect, const mce::Color& color, float alpha);
	virtual void increaseStencilRef();
	virtual void decreaseStencilRef();
	virtual void resetStencilRef();
	virtual void fillRectangleStencil(const RectangleArea& rect);
	virtual void enableScissorTest(const RectangleArea& rect);
	virtual void disableScissorTest();
};

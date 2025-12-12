#pragma once
#include <sdk/mc/client/gui/screens/ScreenContext.hpp>
#include <sdk/mc/client/game/IClientInstance.hpp>
#include <sdk/mc/deps/input/RectangleArea.h>
#include <sdk/mc/deps/core/math/Color.h>
#include <libhat/access.hpp>
#include <core/Logger.hpp>
#include <core/Signatures.hpp>

class MinecraftUIRenderContext {
public:
	IClientInstance* clientInstance;
	ScreenContext* screenContext;
public:
	virtual ~MinecraftUIRenderContext();
	virtual float getLineLength();
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual void drawDebugText();
	virtual void drawText();
	virtual void flushText(float deltaTime);
	virtual void drawImage();
	virtual void drawNineslice();
	virtual void flushImages();
	virtual void beginSharedMeshBatch();
	virtual void endSharedMeshBatch();
	virtual void padding1();
	virtual void padding2();
	virtual void drawRectangle(const RectangleArea& rect, const mce::Color& color, float alpha, int thickness);
	virtual void fillRectangle(const RectangleArea& rect, const mce::Color& color, float alpha);
	virtual void increaseStencilRef();
	virtual void decreaseStencilRef();
	virtual void resetStencilRef();
	virtual void fillRectangleStencil(const RectangleArea& rect);
	virtual void enableScissorTest(const RectangleArea& rect);
	virtual void disableScissorTest();
};

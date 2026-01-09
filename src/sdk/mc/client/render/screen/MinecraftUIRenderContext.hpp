#pragma once
#include <sdk/mc/deps/input/RectangleArea.hpp>
#include <sdk/mc/deps/core/math/Color.hpp>
#include <sdk/mc/deps/core/math/Vec2.hpp>
#include <sdk/mc/deps/renderer/TexturePtr.hpp>
#include <libhat/access.hpp>
#include <core/Logger.hpp>
#include <core/Signatures.hpp>

class IClientInstance;
class ScreenContext;

class MinecraftUIRenderContext {
public:
	IClientInstance* mClient;
	ScreenContext* screenContext;
public:
	virtual ~MinecraftUIRenderContext() = 0;
	virtual float getLineLength() = 0;
	virtual float getTextAlpha() = 0;
	virtual void setTextAlpha(float alpha) = 0;
	virtual void drawDebugText() = 0;
	virtual void drawText() = 0;
	virtual void flushText(float deltaTime) = 0;
	virtual void drawImage(const mce::TexturePtr& texture, const Vec2& position, const Vec2& size, const Vec2& uv, const Vec2& uvSize, int rotation) = 0;
	virtual void drawNineslice() = 0;
	virtual void flushImages(const mce::Color& color, float alpha, const HashedString& materialNameHash) = 0;
	virtual void beginSharedMeshBatch() = 0;
	virtual void endSharedMeshBatch() = 0;
	virtual void reserveSharedMeshBatch(uint64_t vertexCount) = 0;
	virtual uint64_t getSharedMeshBatchVertexCount() = 0;
	virtual void drawRectangle(const RectangleArea& rect, const mce::Color& color, float alpha, int thickness) = 0;
	virtual void fillRectangle(const RectangleArea& rect, const mce::Color& color, float alpha) = 0;
	virtual void increaseStencilRef() = 0;
	virtual void decreaseStencilRef() = 0;
	virtual void resetStencilRef() = 0;
	virtual void fillRectangleStencil(const RectangleArea& rect) = 0;
	virtual void enableScissorTest(const RectangleArea& rect) = 0;
	virtual void disableScissorTest() = 0;
	virtual void setClippingRectangle(const RectangleArea& rect) = 0;
	virtual void setFullClippingRectangle() = 0;
	virtual void saveCurrentClippingRectangle() = 0;
	virtual void restoreSavedClippingRectangle() = 0;
	virtual RectangleArea getFullClippingRectangle() = 0;
	virtual bool updateCustom() = 0;
	virtual void renderCustom() = 0;
	virtual void cleanup() = 0;
	virtual void removePersistentMeshes() = 0;
	virtual mce::TexturePtr getTexture(const ResourceLocation& resourceLocation, bool forceReload) = 0;
};

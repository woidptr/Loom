#pragma once
#include "mc/client/render/screen/MinecraftUIRenderContext.hpp"
#include "mc/client/render/Tessellator.hpp"
#include "mc/client/game/IClientInstance.hpp"
#include "mc/client/actors/LocalPlayer.hpp"
#include <sdk/mc/deps/renderer/MaterialPtr.hpp>

class GameContext {
public:
	static inline MinecraftUIRenderContext* renderCtx = nullptr;
	static inline IClientInstance* clientInstance = nullptr;
	static inline Tessellator* tessellator = nullptr;
	static inline MaterialPtr* materialPtr = nullptr;

	static void registerRenderContext(MinecraftUIRenderContext* renderCtx);
};
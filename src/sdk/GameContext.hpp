#pragma once
#include "render/MinecraftUIRenderContext.hpp"
#include "render/Tessellator.hpp"
#include "client/IClientInstance.hpp"
#include "actors/LocalPlayer.hpp"

class GameContext {
public:
	static inline MinecraftUIRenderContext* renderCtx = nullptr;
	static inline IClientInstance* clientInstance = nullptr;
	static inline Tessellator* tessellator = nullptr;

	static void registerRenderContext(MinecraftUIRenderContext* renderCtx);
};
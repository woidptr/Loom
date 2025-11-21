#pragma once
#include "render/MinecraftUIRenderContext.hpp"
#include "client/IClientInstance.hpp"
#include "actors/LocalPlayer.hpp"

class GameContext {
public:
	static inline MinecraftUIRenderContext* renderCtx = nullptr;
	static inline IClientInstance* clientInstance = nullptr;

	static void registerRenderContext(MinecraftUIRenderContext* renderCtx);
};
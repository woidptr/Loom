#pragma once
#include "render/MinecraftUIRenderContext.h"
#include "client/IClientInstance.h"
#include "actors/LocalPlayer.h"

class GameContext {
public:
	static inline MinecraftUIRenderContext* renderCtx = nullptr;
	static inline IClientInstance* clientInstance = nullptr;

	static void registerRenderContext(MinecraftUIRenderContext* renderCtx);
};
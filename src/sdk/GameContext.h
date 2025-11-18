#pragma once
#include "render/MinecraftUIRenderContext.h"
#include "client/ClientInstance.h"
#include "actors/LocalPlayer.h"

class GameContext {
public:
	static inline MinecraftUIRenderContext* renderCtx = nullptr;
	static inline ClientInstance* clientInstance = nullptr;

	static void registerRenderContext(MinecraftUIRenderContext* renderCtx);
};
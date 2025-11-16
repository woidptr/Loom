#pragma once
#include "render/MinecraftUIRenderContext.h"
#include "client/ClientInstance.h"
#include "actors/LocalPlayer.h"

class SDK {
public:
	static inline MinecraftUIRenderContext* renderCtx = nullptr;
	static inline ClientInstance* clientInstance = nullptr;
};
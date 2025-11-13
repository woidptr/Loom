#pragma once
#include "ScreenContext.h"
#include "../client/ClientInstance.h"
#include <libhat/access.hpp>

class MinecraftUIRenderContext {
public:
	ClientInstance* clientInstance;
	ScreenContext* screenContext;
public:
	virtual ~MinecraftUIRenderContext();
};

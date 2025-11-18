#pragma once
#include "ScreenContext.h"
#include "../client/ClientInstance.h"
#include <libhat/access.hpp>

class MinecraftUIRenderContext {
public:
	ClientInstance* getClientInstance() {
		return hat::member_at<ClientInstance*>(this, 0x8);
	}

	ScreenContext* getScreenContext() {
		return hat::member_at<ScreenContext*>(this, 0x10);
	}

	virtual ~MinecraftUIRenderContext();
};

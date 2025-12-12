#include "GameContext.hpp"

void GameContext::registerRenderContext(MinecraftUIRenderContext* renderCtx) {
	GameContext::renderCtx = renderCtx;
	// GameContext::clientInstance = renderCtx->getClientInstance();
}
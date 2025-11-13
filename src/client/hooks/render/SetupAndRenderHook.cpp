#include "SetupAndRenderHook.h"
#include <libhat/scanner.hpp>
#include "../../../core/Logger.h"
#include <format>

#include "../../../sdk/render/MinecraftUIRenderContext.h"

typedef void(__thiscall* original)(void* a1, MinecraftUIRenderContext* renderCtx);

static original funcOriginal = nullptr;

void callback(void* a1, MinecraftUIRenderContext* renderCtx) {
	Logger::info("Testing hook");

	funcOriginal(a1, renderCtx);
}

SetupAndRenderHook::SetupAndRenderHook() : Hook("ScreenView::setupAndRender", Signatures::ScreenView::setupAndRender.getAddress()) {
	this->hook((void*)callback, (void**)&funcOriginal);
}
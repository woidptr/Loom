#include "SetupAndRenderHook.h"
#include <libhat/scanner.hpp>
#include "../../../core/Logger.h"
#include <format>

typedef void(__thiscall* original)(void* a1, void* a2);

static original funcOriginal = nullptr;

uintptr_t getAddress() {
	constexpr hat::fixed_signature pattern = hat::compile_signature<"48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA">();
	hat::scan_result result = hat::find_pattern(pattern, ".text");
	std::byte* address = result.get();

	Logger::info(std::format("setupAndRender address: {}", (uintptr_t)address));

	return (uintptr_t)address;
}

void callback(void* a1, void* a2) {
	Logger::info("Testing hook");

	funcOriginal(a1, a2);
}

SetupAndRenderHook::SetupAndRenderHook() : Hook("ScreenView::setupAndRender", getAddress()) {
	this->hook((void*)callback, (void**)&funcOriginal);
}
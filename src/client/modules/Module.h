#pragma once
#include <string>

class Module {
private:
	std::string name;
public:
	Module(std::string name){
		this->name = name;
		// this->setupAndRenderHook = setupAndRenderHook;

		// setupAndRenderHook->registerCallback(renderCallback);
	}

	// static void renderCallback(void* a1, MinecraftUIRenderContext* renderCtx) {}
};
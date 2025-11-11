#include "Hook.h"
#include <MinHook.h>
#include <format>
#include "../../core/Logger.h"

Hook::Hook(std::string name, uintptr_t address) {
	this->name = name;
	this->address = address;
}

void Hook::hook(void* callback, void** original) {
	MH_CreateHook((void*)this->address, callback, original);

	if (MH_EnableHook((void*)this->address) != MH_OK) {
		Logger::info(std::format("Successfully initialized hook {} on {}", this->name, this->address));
	}
}

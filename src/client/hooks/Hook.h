#pragma once
#include <string>

class Hook {
private:
	std::string name;
	uintptr_t address;
public:
	Hook(std::string name, uintptr_t address);

	virtual void hook(void* callback, void** original);
};
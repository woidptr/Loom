#pragma once
#include <string>

class Hook {
public:
	std::string name;
	uintptr_t address;
public:
	Hook(std::string name, uintptr_t address);
	virtual ~Hook();

	virtual void hook(void* callback, void** original);
};
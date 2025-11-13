#pragma once
#include <string>
#include "../../core/Signatures.h"

class Hook {
public:
	std::string name;
	uintptr_t address;
public:
	Hook(std::string name, uintptr_t address);
	virtual ~Hook();

	virtual void hook(void* callback, void** original);
};
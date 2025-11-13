#pragma once
#include <vector>

#include "hooks/Hook.h"

class Client {
private:
	static std::vector<Hook*> hooks;
public:
	static void construct();
	static void destruct();
};
#pragma once
#include "../Module.hpp"

#include <client/hooks/world/GetGammaHook.hpp>

class Fullbright : public Module {
public:
	Fullbright(GetGammaHook* getGammaHook);
};
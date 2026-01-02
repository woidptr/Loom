#pragma once
#include <sdk/mc/world/actor/player/Player.hpp>
#include <core/Memory.hpp>

class LocalPlayer : public Player {
public:
	void setSprinting(bool a1) {
		return Memory::CallVFunc<void, bool>(140, this, a1);
	}
};

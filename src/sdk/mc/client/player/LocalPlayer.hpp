#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/world/actor/player/Player.hpp>

class LocalPlayer : public Player {
public:
	void setSprinting(bool a1) {
		return Memory::CallVFunc<void, bool>(140, this, a1);
	}
};

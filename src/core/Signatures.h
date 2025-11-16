#pragma once
#include <libhat/scanner.hpp>
#include "Logger.h"

class Signature {
private:
	std::string name;
	uintptr_t address;

public:
	Signature(std::string name, std::string signature, int16_t offset);

	std::string getName();
	uintptr_t getAddress();
};

namespace Signatures {
	namespace Tessellator {
		inline Signature begin = Signature("Tessellator::vertex", "E8 ? ? ? ? F3 41 0F 10 4C 24 ? F3 41 0F 10 04 24 F3 0F 11 45 ? F3 0F 11 75", 1);
	}

	namespace ScreenView {
		inline Signature setupAndRender = Signature("ScreenView::setupAndRender", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA", 0);
	}

	namespace Level {
		inline Signature tick = Signature("Level::tick", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B F9 45 33 F6 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8D 0D", 0);
	}
}

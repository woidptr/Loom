#pragma once
#include "../Hook.hpp"
#include <core/Signatures.hpp>

class LevelTickHook : public Hook<void(void*)> {
public:
    LevelTickHook() : Hook($getSignature("Level::tick")) {
        this->hook();
    }
};
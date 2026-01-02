#pragma once
#include "../Hook.hpp"
#include <core/Signatures.hpp>

struct LevelTickHookTag {};

class LevelTickHook : public Hook<LevelTickHookTag, void(void*)> {
public:
    LevelTickHook() : Hook($get_signature("Level::tick")) {
        this->hook();
    }
};
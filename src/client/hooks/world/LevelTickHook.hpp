#pragma once
#include "../Hook.hpp"
#include <core/Signatures.hpp>

struct LevelTickHookTag {};

class LevelTickHook : public InlineHook<LevelTickHookTag, void(void*)> {
public:
    LevelTickHook() : InlineHook($get_signature("Level::tick")) {
        this->hook();
    }
};
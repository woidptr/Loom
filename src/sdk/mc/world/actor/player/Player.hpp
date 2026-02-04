#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/world/actor/Mob.hpp>
#include <sdk/mc/world/actor/player/SerializedSkin.hpp>

class Player : public Mob {
public:
    // $field(std::unique_ptr<SerializedSkin>, mSkin);
};
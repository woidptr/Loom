#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/ecs/Entity.hpp>

class MobEffectInstance;

struct MobEffectsComponent : IEntityComponent {
public:
    static constexpr hat::fixed_string type_name = "struct MobEffectsComponent";
public:
    std::vector<MobEffectInstance> mMobEffects;
};
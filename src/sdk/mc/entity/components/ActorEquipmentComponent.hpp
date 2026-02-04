#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/ecs/Entity.hpp>

class SimpleContainer;

struct ActorEquipmentComponent : public IEntityComponent {
public:
    static constexpr hat::fixed_string type_name = "struct ActorEquipmentComponent";
public:
    std::unique_ptr<SimpleContainer> mHand;
    std::unique_ptr<SimpleContainer> mArmor;
};
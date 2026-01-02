#pragma once
#include <memory>
#include "AbstractScene.hpp"
#include <core/Memory.hpp>

class IClientInstance;

class IMinecraftGame {};

class SceneFactory {
public:
    $padding(0x10);

    IClientInstance* mClient;
    IMinecraftGame* mMinecraft;
public:
    virtual ~SceneFactory() = 0;
public:
    $member_function(std::shared_ptr<AbstractScene>, createHudScreen);
    // $member_function(std::shared_ptr<AbstractScene>, createPauseScreen);

    std::shared_ptr<AbstractScene> createPauseScreen() {
        return Memory::CallMember<decltype(&SceneFactory::createPauseScreen)>($get_address("SceneFactory::createPauseScreen"), *this);
    }
};
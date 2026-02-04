#pragma once
#include <sdk/Predefine.hpp>
#include "AbstractScene.hpp"

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
    MCAPI std::shared_ptr<AbstractScene> createHudScreen();
    MCAPI std::shared_ptr<AbstractScene> createPauseScreen();
    MCAPI std::shared_ptr<AbstractScene> createDeathScreen();
};
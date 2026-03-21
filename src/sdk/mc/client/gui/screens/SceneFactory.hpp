#pragma once
#include <sdk/Predefine.hpp>
#include "AbstractScene.hpp"

class IClientInstance;
class IMinecraftGame;

class SceneFactory {
public:
    $padding(0x10);

    IClientInstance* mClient;
    IMinecraftGame* mMinecraft;
public:
    virtual ~SceneFactory() = 0;
public:
    /// @sig {48 89 5C 24 ? 55 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B F2 48 8B D9 48 89 55 ? 33 FF 0F 57 C0 0F 11 45 ? 0F 57 C9 0F 11 4D ? 66 0F 6F 05}
    MCAPI std::shared_ptr<AbstractScene> createHudScreen();

    /// @sig {E8 ? ? ? ? 45 33 C0 48 8B D0 48 8B CF 48 8B C3 FF 15 ? ? ? ? 90 4C 89 74 24}
    MCAPI std::shared_ptr<AbstractScene> createPauseScreen();

    /// @sig {48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B F2 48 8B D9 48 89 54 24 ? 33 ED 0F 57 C0 0F 11 44 24 ? 0F 57 C9 F3 0F 7F 4C 24 ? B9 ? ? ? ? E8 ? ? ? ? 48 8B C8 48 89 44 24 ? 66 0F 6F 05 ? ? ? ? F3 0F 7F 44 24 ? 0F 10 05 ? ? ? ? 0F 11 00 0F B7 05}
    MCAPI std::shared_ptr<AbstractScene> createDeathScreen();
};
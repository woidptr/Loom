#pragma once
#include <memory>
#include <core/Memory.hpp>
#include <sdk/mc/client/gui/screens/AbstractScene.hpp>

class ISceneStack {
public:
    void pushScreen(std::shared_ptr<AbstractScene> scene, bool flush);
};
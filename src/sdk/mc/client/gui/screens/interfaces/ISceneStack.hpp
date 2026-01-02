#pragma once
#include <memory>
#include <core/Memory.hpp>
#include <sdk/mc/client/gui/screens/AbstractScene.hpp>

class ISceneStack {
public:
    // $virtual_function_args(void, pushScreen, 20, (std::shared_ptr<AbstractScene> scene, bool flush), (scene, flush));
    void pushScreen(std::shared_ptr<AbstractScene> scene, bool flush);
};
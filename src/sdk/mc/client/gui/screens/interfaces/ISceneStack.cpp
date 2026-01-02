#include "ISceneStack.hpp"
#include <core/Memory.hpp>

void ISceneStack::pushScreen(std::shared_ptr<AbstractScene> scene, bool flush) {
    return Memory::CallVFunc<void, std::shared_ptr<AbstractScene>, bool>(20, this, scene, flush);
}
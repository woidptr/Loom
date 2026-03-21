#include "IClientInstance.hpp"
#include <core/Memory.hpp>

LocalPlayer* IClientInstance::getLocalPlayer() {
    return Memory::CallVirtual<decltype(&IClientInstance::getLocalPlayer)>(31, *this);
}

SceneFactory* IClientInstance::getSceneFactory() {
    return Memory::CallVirtual<decltype(&IClientInstance::getSceneFactory)>(AddressResolver::get("IClientInstance::getSceneFactory"), *this);
}

NotNullNonOwnerPtr<ISceneStack> IClientInstance::getClientSceneStack() {
    /*NotNullNonOwnerPtr<ISceneStack> result{};
    Memory::CallVFunc<NotNullNonOwnerPtr<ISceneStack>*, NotNullNonOwnerPtr<ISceneStack>*>(234, this, &result);
    return result.value;*/

    return Memory::CallVirtual<decltype(&IClientInstance::getClientSceneStack)>(AddressResolver::get("IClientInstance::getClientSceneStack"), *this);
}

std::string IClientInstance::getTopScreenName() {
    return Memory::CallVirtual<decltype(&IClientInstance::getTopScreenName)>(138, *this);
}

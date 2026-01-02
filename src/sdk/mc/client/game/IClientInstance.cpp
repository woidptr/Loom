#include "IClientInstance.hpp"
#include <core/Memory.hpp>

NotNullNonOwnerPtr<ISceneStack> IClientInstance::getClientSceneStack() {
    /*NotNullNonOwnerPtr<ISceneStack> result{};
    Memory::CallVFunc<NotNullNonOwnerPtr<ISceneStack>*, NotNullNonOwnerPtr<ISceneStack>*>(234, this, &result);
    return result.value;*/

    return Memory::CallVirtual<decltype(&IClientInstance::getClientSceneStack)>(234, *this);
}
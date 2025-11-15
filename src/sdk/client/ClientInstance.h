#pragma once
#include "../actors/LocalPlayer.h"

template <unsigned int IIdx, typename TRet, typename... TArgs>
static inline TRet CallVFunc(void* thisptr, TArgs... argList) {
	using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
	return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
}

class ClientInstance {
public:
	LocalPlayer* getLocalPlayer() {

	}
};

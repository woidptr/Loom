#pragma once
#include "../actors/LocalPlayer.hpp"

class IClientInstance {
public:
	template <unsigned int IIdx, typename TRet, typename... TArgs>
	static inline TRet CallVFunc(void* thisptr, TArgs... argList) {
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}

	LocalPlayer* getLocalPlayer() {
		return CallVFunc<31, LocalPlayer*>(this);
	}
};
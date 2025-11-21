#pragma once

class LocalPlayer {
public:
	template <unsigned int IIdx, typename TRet, typename... TArgs>
	static inline TRet CallVFunc(void* thisptr, TArgs... argList) {
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}

	void setSprinting(bool a1) {
		return CallVFunc<140, void, bool>(this, a1);
	}
};

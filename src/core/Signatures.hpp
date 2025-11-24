#pragma once
#include <MinHook.h>
#include <unordered_map>
#include <memory>
#include <libhat/scanner.hpp>
#include "Logger.hpp"

class Signature {
private:
	uintptr_t address;
public:
	Signature(std::string signature, int16_t offset);

	uintptr_t getAddress();

	template <typename TRet, typename...Args>
	TRet call(Args... args) {
		if (this->address == 0) {
			return;
		}

		using _func = TRet(*)(Args...);
		_func func = reinterpret_cast<_func>(this->address);
		return func(args...);
	}
};

class SignatureRegistry {
private:
	static inline std::unordered_map<std::string, std::unique_ptr<Signature>> signatures;
	static inline bool healthy = true;
public:
	static void registerSignatures();
	static bool performHealthCheck();
	static Signature* getSignature(std::string name);
};

//namespace MinecraftUIRenderContext {
//	inline Signature UIScene = Signature("MinecraftUIRenderContext::UIScene*", "48 89 AE ? ? ? ? C6 86 ? ? ? ? ? 48 8B C6", 1);
//}

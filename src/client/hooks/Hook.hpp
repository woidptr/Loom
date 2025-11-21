#pragma once
#include <MinHook.h>
#include <string>
#include <core/Signatures.hpp>

template <typename TReturn, typename...Args>
class Hook {
public:
	Signature signature;

	static inline TReturn(*original)(Args...);
	static inline std::vector<std::function<void(Args...)>> callbacks;
public:
	Hook(const Signature& signature) : signature(signature) { }

	virtual ~Hook() {
		MH_DisableHook((void*)this->signature.getAddress());
	}

	static void callback(Args...args) {
		for (auto& cb : callbacks) {
			cb(args...);
		}

		original(args...);
	}

	void registerCallback(std::function<TReturn(Args...)> fn) {
		callbacks.push_back(fn);
	}

	virtual void hook() {
		if (this->signature.getAddress() == 0) {
			return;
		}

		MH_CreateHook((void*)this->signature.getAddress(), (void*)&callback, (void**)&original);

		if (MH_EnableHook((void*)this->signature.getAddress()) == MH_OK) {
			Logger::info(std::format("Successfully initialized hook {} on 0x{:X}", this->signature.getName(), this->signature.getAddress()));
		}
	}
};
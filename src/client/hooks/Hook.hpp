#pragma once
#include <MinHook.h>
#include <string>
#include <core/Signatures.hpp>

template <typename TReturn, typename...Args>
class Hook {
public:
	std::string name;
	uintptr_t address = 0;

	static inline TReturn(*original)(Args...);
	static inline std::vector<std::function<void(Args...)>> callbacks;
public:
	Hook(Signature* signature) : address(signature->getAddress()) {}
	Hook(std::string name, uintptr_t address) : name(name), address(address) {}
	Hook(std::string name) : name(name) {}

	virtual ~Hook() {
		MH_DisableHook((void*)this->address);
	}

	static TReturn callback(Args...args) {
		for (auto& cb : callbacks) {
			cb(args...);
		}

		return original(args...);
	}

	void registerCallback(std::function<void(Args...)> fn) {
		callbacks.push_back(fn);
	}

	virtual void hook() {
		if (this->address == 0) {
			return;
		}

		MH_CreateHook((void*)this->address, (void*)&callback, (void**)&original);

		if (MH_EnableHook((void*)this->address) == MH_OK) {
			Logger::info(std::format("Successfully initialized hook on 0x{:X}", this->address));
		}
	}
};
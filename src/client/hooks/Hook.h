#pragma once
#include <MinHook.h>
#include <string>
#include "../../core/Signatures.h"

template <typename TReturn, typename...Args>
class Hook {
public:
	std::string name;
	uintptr_t address;

	static inline TReturn(*original)(Args...);
	static inline std::vector<std::function<void(Args...)>> callbacks;
public:
	Hook(std::string name, uintptr_t address) {
		this->name = name;
		this->address = address;
	}

	virtual ~Hook() {
		MH_DisableHook((void*)this->address);
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
		if (this->address == 0) {
			Logger::error(std::format("Failed to resolve address from signature {}", this->name));

			return;
		}

		MH_CreateHook((void*)this->address, (void*)&callback, (void**)&original);

		if (MH_EnableHook((void*)this->address) == MH_OK) {
			Logger::info(std::format("Successfully initialized hook {} on {}", this->name, this->address));
		}
	}
};
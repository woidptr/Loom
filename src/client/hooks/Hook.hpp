#pragma once
#include <MinHook.h>
#include <string>
#include <core/Signatures.hpp>

struct CallbackContext {
private:
	bool cancelFlag = false;
public:
	bool isCancelled() const {
		return cancelFlag;
	}

	void cancel() {
		cancelFlag = true;
	}
};

template <typename TReturn, typename...Args>
class Hook {
public:
	std::string name;
	uintptr_t address = 0;

	static inline TReturn(*original)(Args...);
	static inline std::vector<std::function<void(CallbackContext&, Args...)>> callbacks;
	static inline std::function<TReturn(CallbackContext&, Args&...)> returnCallback = nullptr;
public:
	Hook(Signature* signature) : address(signature->getAddress()) {}
	Hook(std::string name, uintptr_t address) : name(name), address(address) {}

	virtual ~Hook() {
		MH_DisableHook((void*)this->address);
	}

	static TReturn callback(Args...args) {
		CallbackContext cbCtx;

		if constexpr (!std::is_void_v<TReturn>) {
			if (returnCallback) {
				std::optional<TReturn> value = returnCallback(cbCtx, args...);

				if (value.has_value()) {
					return *value;
				}
			}
		}

		for (auto& cb : callbacks) {
			cb(cbCtx, args...);
		}

		if (cbCtx.isCancelled()) {
			return TReturn{};
		}

		return original(args...);
	}

	void registerCallback(std::function<void(CallbackContext&, Args...)> fn) {
		callbacks.push_back(fn);
	}

	void registerReturnCallback(std::function<TReturn(CallbackContext&, Args...)> fn)
		requires(!std::is_void_v<TReturn>)
	{
		returnCallback = fn;
	}

	virtual void hook() {
		MH_STATUS status;

		status = MH_CreateHook((void*)this->address, (void*)&callback, (void**)&original);

		if (status != MH_OK) {
			Logger::error(std::format("Failed to create hook at 0x{:X}: {}", this->address, MH_StatusToString(status)));

			return;
		}

		status = MH_EnableHook((void*)this->address);

		if (status != MH_OK) {
			Logger::error(std::format("Failed to enable hook at 0x{:X}: {}", this->address, MH_StatusToString(status)));

			return;
		}
	}
};
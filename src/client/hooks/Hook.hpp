#pragma once
#include <MinHook.h>
#include <string>
#include <core/Signatures.hpp>

struct CallbackContextBase {
protected:
	bool cancelFlag = false;
public:
	bool isCancelled() const {
		return cancelFlag;
	}

	void cancel() {
		cancelFlag = true;
	}
};

template <typename TReturn>
struct CallbackContext : public CallbackContextBase {
	std::optional<TReturn> overrideReturn;

	void setReturn(TReturn value) {
		overrideReturn = value;
	}
};

template<>
struct CallbackContext<void> : public CallbackContextBase {};

template <typename TReturn, typename...Args>
class Hook {
public:
	std::string name;
	uintptr_t address = 0;

	static inline TReturn(*original)(Args...);
	static inline std::vector<std::function<void(CallbackContext<TReturn>&, Args...)>> callbacks;
public:
	Hook(Signature* signature) : address(signature->getAddress()) {}
	Hook(std::string name, uintptr_t address) : name(name), address(address) {}

	virtual ~Hook() {
		MH_DisableHook((void*)this->address);
	}

	static TReturn callback(Args...args) {
		CallbackContext<TReturn> cbCtx;

		for (auto& cb : callbacks) {
			cb(cbCtx, args...);

			if constexpr (!std::is_void_v<TReturn>) {
				if (cbCtx.overrideReturn.has_value()) {
					return *cbCtx.overrideReturn;
				}
			}

			if (cbCtx.isCancelled()) {
				return TReturn{};
			}
		}

		return original(args...);
	}

	void registerCallback(std::function<void(CallbackContext<TReturn>&, Args...)> fn) {
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
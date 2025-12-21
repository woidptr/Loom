#pragma once
#include <cstddef>
#include <libhat/access.hpp>

#define $_concat_impl(a, b) a##b

#define $_concatinate(a, b) $_concat_impl(a, b)

#define $padding(size) \
private: \
	std::byte $_concatinate(padding_, __COUNTER__)[size]; \
public:

#define $field(type, name, offset) \
public: \
	__forceinline type& get_##name() { \
		return hat::member_at<type>(this, offset); \
	} \
	__forceinline void set_##name(type value) { \
		hat::member_at<type>(this, offset) = value; \
	} \
public: \
	__declspec(property(get = get_##name, put = set_##name)) type name;

namespace Memory {
	namespace _internal {
		template <typename T>
		concept IsValidSignature = std::is_function_v<T> || std::is_member_function_pointer_v<T>;

		template <typename TReturn, typename... Args>
		struct BaseExecutor {
			template <typename ClassType>
			static __forceinline TReturn Call(uintptr_t addr, ClassType* thisPtr, Args... args) {
				using _func = TReturn(*)(ClassType*, Args...);
				return reinterpret_cast<_func>(addr)(thisPtr, args...);
			}
		};

		template <typename FuncSig>
		struct FunctionCaller;

		template <typename TReturn, typename... Args>
		struct FunctionCaller<TReturn(Args...)> : BaseExecutor<TReturn, Args...> {};

		template <typename TReturn, typename ClassType, typename... Args>
		struct FunctionCaller<TReturn(ClassType::*)(Args...)> : BaseExecutor<TReturn, Args...> {};
	}

	template <typename FuncSig, typename ClassType, typename... Args>
	__forceinline auto Call(uintptr_t addr, ClassType* thisPtr, Args&&... args) {
		return _internal::FunctionCaller<FuncSig>::Call(addr, thisPtr, std::forward<Args>(args)...);
	}
}
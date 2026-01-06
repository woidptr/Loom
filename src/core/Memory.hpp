#pragma once
#include <bit>
#include <cstddef>
#include <type_traits>
#include <libhat/access.hpp>
#include <core/Signatures.hpp>
#include <meta.h>
#include <Zydis/Zydis.h>

#define $get_signature(signature) SignatureRegistry::getSignature(signature)

#define $get_address(signature) SignatureRegistry::getSignature(signature)->getAddress()
// #define $get_offset(signature) *(int32_t*)$get_address(signature)
#define $get_offset(signature) Memory::GetOffset($get_address(signature))
#define $get_index(signature) $get_offset(signature) / 8

#define $_concat_impl(a, b) a##b

#define $_concatinate(a, b) $_concat_impl(a, b)

#define $padding(size) \
private: \
	std::byte $_concatinate(padding_, __COUNTER__)[size]; \
public:

#define $build_access(type, name, offset) \
public: \
	__forceinline type& get_##name() { \
		return hat::member_at<type>(this, offset); \
	} \
	__forceinline void set_##name(type value) { \
		hat::member_at<type>(this, offset) = value; \
	} \
public: \
	__declspec(property(get = get_##name, put = set_##name)) type name;

#define $field(type, name) $build_access(type, name, $get_offset(std::string(kawa::meta::type_name<std::remove_reference_t<decltype(*this)>>()) + "->" + #name));

#define $_arg_expansion(...) __VA_ARGS__

#define $member_function(return_type, function_name) \
	return_type function_name() { \
		using CurrentClass = std::remove_reference_t<decltype(*this)>; \
		std::string sig = std::string(kawa::meta::type_name<CurrentClass>()) + "::" + #function_name; \
		return Memory::CallMember<decltype(&CurrentClass::function_name)>($get_address(sig), *this); \
	}

#define $member_function_args(return_type, function_name, args_decl, args_pass) \
	return_type function_name args_decl { \
		using CurrentClass = std::remove_reference_t<decltype(*this)>; \
		std::string sig = std::string(kawa::meta::type_name<CurrentClass>()) + "::" + #function_name; \
		return Memory::CallMember<decltype(&CurrentClass::function_name)>($get_address(sig), *this); \
	}

#define $virtual_function(return_type, function_name, index) \
	return_type function_name() { \
		using CurrentClass = std::remove_reference_t<decltype(*this)>; \
		return Memory::CallVirtual<decltype(&CurrentClass::function_name)>(index, *this); \
	}

#define $virtual_function_args(return_type, function_name, index, args_decl, args_pass) \
	return_type function_name args_decl { \
		using CurrentClass = std::remove_reference_t<decltype(*this)>; \
		return Memory::CallVirtual<decltype(&CurrentClass::function_name)>(index, *this, $_arg_expansion args_pass); \
	}

namespace Memory {
	namespace _internal {
		template<typename TReturn, typename... Args>
		struct BaseExecutor {
			template<typename ClassType>
			static TReturn CallMember(uintptr_t addr, ClassType& thisPtr, Args&&... args) {
				static_assert(sizeof(void(ClassType::*)(Args...)) == 8);

				using _func = TReturn(ClassType::*)(Args...);
				// _func func = nullptr;

				_func func = std::bit_cast<_func>(addr);

				// std::memcpy(&func, &addr, sizeof(void*));

				return (thisPtr.*func)(std::forward<Args>(args)...);
			}

			template<typename ClassType>
			static TReturn CallVirtual(size_t index, ClassType& thisPtr, Args&&... args) {
				uintptr_t* vtable = *reinterpret_cast<uintptr_t**>(&thisPtr);
				uintptr_t funcAddress = vtable[index];
				using _func = TReturn(ClassType::*)(Args...);
				_func func = std::bit_cast<_func>(funcAddress);
				return (thisPtr.*func)(std::forward<Args>(args)...);
			}
		};

		template <typename FuncSig>
		struct FunctionCaller;

		template <typename TReturn, typename... Args>
		struct FunctionCaller<TReturn(Args...)> : BaseExecutor<TReturn, Args...> {};

		template <typename TReturn, typename ClassType, typename... Args>
		struct FunctionCaller<TReturn(ClassType::*)(Args...)> : BaseExecutor<TReturn, Args...> {};
	}

	template<typename Func, typename Class, typename... Args>
	concept ValidMemberCall =
		std::is_member_function_pointer_v<Func> &&
		requires(Class & obj, Args&&... args) {
		(obj.*std::declval<Func>())(std::forward<Args>(args)...);
	};

	template<typename FuncSig, typename ClassType, typename... Args>
	requires ValidMemberCall<FuncSig, ClassType, Args...>
	decltype(auto) CallMember(uintptr_t addr, ClassType& thisPtr, Args&&... args) {
		return _internal::FunctionCaller<FuncSig>::CallMember(addr, thisPtr, std::forward<Args>(args)...);
	}

	template<typename FuncSig, typename ClassType, typename... Args>
	requires ValidMemberCall<FuncSig, ClassType, Args...>
	decltype(auto) CallVirtual(size_t index, ClassType& thisPtr, Args&&... args) {
		return _internal::FunctionCaller<FuncSig>::CallVirtual(index, thisPtr, std::forward<Args>(args)...);
	}

	template <typename TReturn, typename... Args>
	TReturn CallVFunc(size_t index, void* thisptr, Args... args) {
		// 1. Get the VTable pointer (first 8 bytes of the object)
		auto vtable = *reinterpret_cast<uintptr_t**>(thisptr);

		// 2. Get the function address at the specific index
		uintptr_t funcAddress = vtable[index];

		// 3. Cast the address to a function pointer
		// Note: 'this' is passed explicitly as the first argument
		using Fn = TReturn(*)(void*, Args...);

		// 4. Call it
		return reinterpret_cast<Fn>(funcAddress)(thisptr, args...);
	}

	inline int32_t GetOffset(uintptr_t addr) {
		ZydisDecoder decoder;
		ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

		ZydisDecodedInstruction instruction;
		ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];

		if (ZYAN_SUCCESS(ZydisDecoderDecodeFull((&decoder), (void*)addr, 15, &instruction, operands))) {
			for (int i = 0; i < instruction.operand_count; i++) {
				if (operands[i].type == ZYDIS_OPERAND_TYPE_MEMORY) {
					if (operands[i].mem.disp.has_displacement) {
						return (int32_t)operands[i].mem.disp.value;
					}
				}
			}
		}

		return 0;
	}

	inline uintptr_t ResolveInstructionTarget(uintptr_t addr) {
		ZydisDecoder decoder;
		ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

		ZydisDecodedInstruction instruction;
		ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];

		if (ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, (void*)addr, 15, &instruction, operands))) {
			if (instruction.attributes & ZYDIS_ATTRIB_IS_RELATIVE) {
				uintptr_t absoluteAddress = 0;
				ZydisCalcAbsoluteAddress(&instruction, operands, addr, &absoluteAddress);
				return absoluteAddress;
			}
		}

		return addr;
	}
}
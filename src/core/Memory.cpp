#include "Memory.hpp"

int32_t Memory::GetOffset(uintptr_t addr) {
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

uintptr_t Memory::ResolveInstructionTarget(uintptr_t addr) {
	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

	ZydisDecodedInstruction instruction;
	ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];

	if (ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, (void*)addr, 15, &instruction, operands))) {
		for (int i = 0; i < instruction.operand_count; i++) {
	        if (operands[i].type == ZYDIS_OPERAND_TYPE_MEMORY) {
				uintptr_t absoluteAddress = 0;
				if (ZYAN_SUCCESS(ZydisCalcAbsoluteAddress(&instruction, &operands[i], addr, &absoluteAddress))) {
					return absoluteAddress;
				}
			}
		}
		/*if (instruction.attributes & ZYDIS_ATTRIB_IS_RELATIVE) {
			uintptr_t absoluteAddress = 0;
			ZydisCalcAbsoluteAddress(&instruction, operands, addr, &absoluteAddress);
			return absoluteAddress;
		}*/
	}

	return addr;
}

uintptr_t Memory::ResolveAddress(const char* mangledName) {
	char buffer[1024];

	DWORD flags = UNDNAME_NAME_ONLY;

	UnDecorateSymbolName(mangledName, buffer, sizeof(buffer), flags);

	return $get_address(std::string(buffer));
}

namespace Memory {
	AddressResolver::AddressResolver() {
		ZydisDecoderInit(this->decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
	}

	uintptr_t AddressResolver::getAddress(const std::string &id) {
		auto item = addressMap.find(id);
		if (item != addressMap.end()) {
			uintptr_t address = item->second;

			return address;
		}

		return 0;
	}

	int32_t AddressResolver::getPointer(const std::string &id) {

	}
}
#include "Signatures.h"

Signature::Signature(std::string name, std::string signature, int16_t offset) : name(name) {
	using parsed_t = hat::result<hat::signature, hat::signature_error>;
	parsed_t pattern = hat::parse_signature(signature);

	hat::scan_result result = hat::find_pattern(pattern.value(), ".text");

	Logger::info("iusdhiusdhfiusdhfui");

	for (int i = 0; i <= 16; ++i) {
		try {
			uintptr_t r = (uintptr_t)result.rel(i);
			Logger::info(std::format("rel({}) -> 0x{:X}", i, r));
		}
		catch (const std::exception& e) {
			Logger::info(std::format("rel({}) -> exception: {}", i, e.what()));
		}
	}

	if (offset == 0) {
		this->address = (uintptr_t)result.get();
	}
	else {
		this->address = (uintptr_t)result.rel(offset);
	}
}

std::string Signature::getName() {
	return this->name;
}

uintptr_t Signature::getAddress() {
	return this->address;
}

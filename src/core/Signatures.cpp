#include "Signatures.hpp"

Signature::Signature(std::string name, std::string signature, int16_t offset) : name(name) {
	using parsed_t = hat::result<hat::signature, hat::signature_error>;
	parsed_t pattern = hat::parse_signature(signature);

	hat::scan_result result = hat::find_pattern(pattern.value(), ".text");

	if (!result.has_result()) {
		this->address = 0;

		Logger::error(std::format("Failed to resolve address from signature {}", this->name));

		return;
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

#include "Signatures.hpp"

Signature::Signature(std::string signature, int16_t offset) {
	using parsed_t = hat::result<hat::signature, hat::signature_error>;
	parsed_t pattern = hat::parse_signature(signature);

	hat::scan_result result = hat::find_pattern(pattern.value(), ".text");

	if (result.get() != 0) {
		if (offset == 0) {
			this->address = (uintptr_t)result.get();
		}
		else {
			this->address = (uintptr_t)result.rel(offset);
		}
	}
}

uintptr_t Signature::getAddress() {
	return this->address;
}

void SignatureRegistry::registerSignatures() {
	signatures.insert(std::make_pair("Tessellator::begin", std::make_unique<Signature>("E8 ? ? ? ? F3 41 0F 10 4C 24 ? F3 41 0F 10 04 24 F3 0F 11 45 ? F3 0F 11 75", 1)));
	signatures.insert(std::make_pair("ScreenView::setupAndRender", std::make_unique<Signature>("E8 ? ? ? ? 48 8B 4B ? 48 85 C9 74 ? 48 8B 01 48 8B D7 48 8B 40 ? FF 15 ? ? ? ? 90", 1))); // 48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA
	signatures.insert(std::make_pair("Level::tick", std::make_unique<Signature>("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B F9 45 33 F6 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8D 0D", 0)));
	signatures.insert(std::make_pair("Keyboard::feed", std::make_unique<Signature>("40 53 48 83 EC ? 80 79 ? ? 8B DA", 0)));
}

bool SignatureRegistry::performHealthCheck() {
	for (const auto& [name, signature] : signatures) {
		if (signature.get()->getAddress() == 0) {
			Logger::info(std::format("Failed to resolve signature {}", name));
			healthy = false;
		}
		else
		{
			Logger::info(std::format("Successfully resolved signature {} at 0x{:X}", name, signature.get()->getAddress()));
		}
	}

	return healthy;
}

Signature* SignatureRegistry::getSignature(std::string name) {
	auto item = signatures.find(name);
	if (item != signatures.end()) {
		Signature* signature = item->second.get();

		return signature;
	}

	return nullptr;
}

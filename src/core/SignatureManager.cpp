#include "SignatureManager.h"

SignatureManager::SignatureManager() {
	// this->signatures.push_back(new Signature("key::press", "2B 3D 6F"));
}

Signature* SignatureManager::getSignatureByName(std::string name) {
	for (Signature* signature : this->signatures) {
		if (signature->name == name) {
			return signature;
		}
	}

	return nullptr;
}

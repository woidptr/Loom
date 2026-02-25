#include "Signatures.hpp"
#include <fstream>
#include <core/Memory.hpp>

Signature::Signature(std::string signature) {
    using parsed_t = hat::result<hat::signature, hat::signature_error>;
    parsed_t pattern = hat::parse_signature(signature);

    std::optional<hat::process::module> minecraft = hat::process::get_module("Minecraft.Windows.exe");

    if (!minecraft.has_value()) {
        return;
    }

    hat::scan_result result = hat::find_pattern(pattern.value(), ".text");

    uintptr_t startAddress = (uintptr_t)minecraft.value().address();
    uintptr_t endAddress = (uintptr_t)minecraft.value().get_module_data().size();

    // $logInfo("Start address: 0x{:X}, result: 0x{:X}", startAddress, (uintptr_t)result.get());

    uintptr_t res = (uintptr_t)result.get();

    if (res != 0) {
        $log_debug("Address after scan: 0x{:X}", res);
        this->address = Memory::ResolveInstructionTarget(res);

        $log_debug("Address after zydis: 0x{:X}", this->address);
    }
}

uintptr_t Signature::getAddress() {
    return this->address;
}

void SignatureRegistry::registerSignatures() {
    /*
    * Signatures marked with `::` are sigs to the functions
    * Signatures marked with `$$` are sigs to the virtual indexes
    * Signatures marked with `->` are sigs to the field offsets
    */

    Resource sigs = Resource("signatures.json");

    json data = json::parse(sigs.str());

    // simdjson::dom::parser parser;

    // simdjson::dom::element data = parser.parse(sigs.str());

    for (const auto& [name, entry] : data.items()) {
        std::string pattern = entry.at("signature").get<std::string>();
        signatures.insert(std::make_pair(name, std::make_unique<Signature>(pattern)));
    }
}

bool SignatureRegistry::performHealthCheck() {
    for (const auto& [name, signature] : signatures) {
        if (signature.get()->getAddress() == 0) {
            $log_error("Failed to resolve signature {}", name);
            healthy = false;
        }
        else
        {
            $log_info("Successfully resolved signature {} at 0x{:X}", name, signature.get()->getAddress());
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

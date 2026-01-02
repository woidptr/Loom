#include "Signatures.hpp"

Signature::Signature(std::string signature, uint16_t offset = 0, uint16_t displacement = 0) {
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

    if ((uintptr_t)result.get() != 0) {
        if (offset == 0) {
            this->address = (uintptr_t)result.get();
        }
        else {
            this->address = (uintptr_t)result.rel(offset);
        }

        if (displacement > 0) {
            this->address += displacement;
        }
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

    signatures.insert(std::make_pair("Tessellator->color", std::make_unique<Signature>("89 86 ? ? ? ? 44 38 36 75 ? C6 86 ? ? ? ? ? 0F 57 DB 0F 57 D2", 0)));
    signatures.insert(std::make_pair("Tessellator::begin", std::make_unique<Signature>("40 57 48 83 EC ? 80 B9 ? ? ? ? ? 4C 8B D1", 0)));
    signatures.insert(std::make_pair("Tessellator::vertex", std::make_unique<Signature>("E8 ? ? ? ? F3 0F 10 4E ? 0F 2F F9", 1)));
    signatures.insert(std::make_pair("Tessellator::end", std::make_unique<Signature>("E8 ? ? ? ? 48 8D 55 ? 49 8B CC E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? F3 45 0F 11 65", 1)));

    signatures.insert(std::make_pair("MeshHelpers::renderMeshImmediately", std::make_unique<Signature>("48 89 5C 24 ? 55 56 57 41 56 41 57 48 81 EC ? ? ? ? 49 8B F1 4D 8B F0 48 8B FA", 0)));

    signatures.insert(std::make_pair("ScreenView::setupAndRender", std::make_unique<Signature>("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA", 0))); // E8 ? ? ? ? 48 8B 4B ? 48 85 C9 74 ? 48 8B 01 48 8B D7 48 8B 40 ? FF 15 ? ? ? ? 90

    signatures.insert(std::make_pair("ScreenContext->guiData", std::make_unique<Signature>("48 8B B8 ? ? ? ? 48 89 7C 24 ? 48 85 C9", 0)));
    signatures.insert(std::make_pair("ScreenContext->tessellator", std::make_unique<Signature>("48 8B 9A ? ? ? ? C6 44 24 ? ? 41 B9 04 00 00 00 41 B0 01 48 8B CB E8 ? ? ? ? 0F 57 DB", 0)));

    signatures.insert(std::make_pair("IClientInstance$$getSceneFactory", std::make_unique<Signature>("48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8D 54 24 ? 48 8B C8 E8 ? ? ? ? 45 33 C0 48 8B D0 48 8B CF 48 8B C3 FF 15 ? ? ? ? 90 4C 89 74 24", 0)));
    signatures.insert(std::make_pair("IClientInstance$$getClientSceneStack", std::make_unique<Signature>("48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 8B 78 ? 48 8B 07 48 8B 98 ? ? ? ? 48 8B 06 48 8B CE 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8D 54 24 ? 48 8B C8 E8 ? ? ? ? 45 33 C0 48 8B D0 48 8B CF 48 8B C3 FF 15 ? ? ? ? 90 4C 89 74 24", 0)));

    signatures.insert(std::make_pair("SceneFactory::createHudScreen", std::make_unique<Signature>("48 89 5C 24 ? 55 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B F2 48 8B D9 48 89 55 ? 33 FF 0F 57 C0 0F 11 45 ? 0F 57 C9 0F 11 4D ? 66 0F 6F 05", 0)));
    signatures.insert(std::make_pair("SceneFactory::createPauseScreen", std::make_unique<Signature>("E8 ? ? ? ? 45 33 C0 48 8B D0 48 8B CF 48 8B C3 FF 15 ? ? ? ? 90 4C 89 74 24", 1)));

    // signatures.insert(std::make_pair("Level::tick", std::make_unique<Signature>("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B F9 45 33 F6 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8D 0D", 0)));

    signatures.insert(std::make_pair("MainWindow::_windowProc", std::make_unique<Signature>("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B F1 49 8B F8 8B F2", 0)));

    signatures.insert(std::make_pair("Dimension::getTimeOfDay", std::make_unique<Signature>("44 8B C2 B8 F1 19 76 05 F7 EA", 0)));

    signatures.insert(std::make_pair("LevelRendererPlayer::getFov", std::make_unique<Signature>("4C 8B DC 53 48 83 EC ? 0F 29 74 24", 0)));

    signatures.insert(std::make_pair("Options::getGamma", std::make_unique<Signature>("48 83 EC 28 48 8B 01 48 8D 54 24 ?? 41 B8 36 00 00 00 48 8B 40 08 FF 15 ?? ?? ?? ??", 0)));
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

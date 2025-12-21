#pragma once
#include <unordered_map>
#include <memory>
#include <libhat/scanner.hpp>
#include "Logger.hpp"

#define $getSignature(signature) SignatureRegistry::getSignature(signature)

#define $getAddress(signature) SignatureRegistry::getSignature(signature)->getAddress()
#define $getOffset(signature) *(int32_t*)$getAddress(signature)

class Signature {
private:
    uintptr_t address = 0;
public:
    Signature(std::string signature, uint16_t offset, uint16_t displacement);

    uintptr_t getAddress();
};

template <typename TSig>
struct SignatureV2 {
    uintptr_t addr = 0;

    SignatureV2(TSig& sig, uint16_t offset = 0, uint16_t displacement = 0) {
        hat::scan_result result = hat::find_pattern(sig, ".text");

        if ((uintptr_t)result.get() != 0) {
            if (offset == 0) {
                this->addr = (uintptr_t)result.get();
            }
            else {
                this->addr = (uintptr_t)result.rel(offset);
            }

            if (displacement > 0) {
                this->addr += displacement;
            }
        }
    }
};

namespace Signatures {
    namespace ScreenView {
        constexpr hat::fixed_signature setupAndRenderSig = hat::compile_signature<"48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA">();
        const inline SignatureV2 setupAndRender = SignatureV2(setupAndRenderSig);
    }
}

class SignatureRegistry {
private:
    static inline std::unordered_map<std::string, std::unique_ptr<Signature>> signatures;
    static inline bool healthy = true;
public:
    static void registerSignatures();
    static bool performHealthCheck();
    static Signature* getSignature(std::string name);
};

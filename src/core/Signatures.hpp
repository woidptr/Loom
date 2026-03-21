#pragma once
#include <optional>
#include <cstdint>
#include <libhat/scanner.hpp>
#include <core/utils/hash.hpp>

namespace sigs {
    enum class AddressType : uint8_t {
        Direct = 0,
        Field = 1,
        VtableIndex = 2,
        VtablePtr = 3,
    };

    struct EmbeddedSignature {
        const char* name;
        uint64_t hash;
        hat::signature_view view;
        AddressType type;
    };

    std::span<const EmbeddedSignature> get_all() noexcept;
}
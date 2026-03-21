#pragma once
#include <cstdint>
#include <sdk/mc/deps/core/string/HashedString.hpp>

namespace hash {
    struct hashed_string {
        uint64_t value;

        constexpr hashed_string(const char* path)
            : value(HashedString::computeHash(std::string_view{path})) {}

        constexpr hashed_string(std::string_view path)
            : value(HashedString::computeHash(path)) {}

        constexpr bool operator==(const uint64_t other) const noexcept {
            return value == other;
        }

        friend constexpr bool operator==(const uint64_t lhs, const hashed_string& rhs) noexcept {
            return lhs == rhs.value;
        }
    };
}
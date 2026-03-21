#pragma once
#include <cstdint>
#include <cstddef>
#include <string_view>
#include <libhat/fixed_string.hpp>
#include <core/utils/hash.hpp>

class Resource {
private:
    const uint8_t* m_data = nullptr;
    size_t m_size = 0;
public:
    Resource(hash::hashed_string hashed_path);

    const uint8_t* data() const;
    size_t size() const;
    std::string_view str() const;
};
#pragma once
#include <cstdint>
#include <cstddef>
#include <string_view>

class Resource {
private:
    const uint8_t* data = nullptr;
    size_t size = 0;
public:
    Resource(std::string_view path);
};
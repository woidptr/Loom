#pragma once
#include <cstddef>
#include <string>
#include <span>

#define $load_asset(asset) extern "C" char _binary_assets_##asset##_start, _binary_assets_##asset##_end;
#define $get_asset(asset) Asset{&_binary_assets_##asset##_start, &_binary_assets_##asset##_end};

struct Asset {
private:
    const char* _begin;
    const char* _end;
public:
    Asset(const char* begin, const char* end) : _begin(begin), _end(end) {}

    const char* data() const { return _begin; }
    const char* begin() const { return _begin; }
    const char* end() const { return _end; }
    size_t size() const { return size_t(_end - _begin); }
    std::string_view str() const { return std::string_view{ this->data(), this->size() }; }
};
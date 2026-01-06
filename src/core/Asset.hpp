#pragma once
#include <cstddef>
#include <string>
#include <span>

#define $load_asset(asset) extern "C" char _binary_assets_##asset##_start, _binary_assets_##asset##_end;
#define $get_asset(asset) Asset{&_binary_assets_##asset##_start, &_binary_assets_##asset##_end};

#define $load_signatures() extern "C" char _binary_signatures_json_start, _binary_signatures_json_end;
#define $get_signatures() Asset{&_binary_signatures_json_start, &_binary_signatures_json_end};

struct Asset {
private:
    const char* _begin;
    const char* _end;
public:
    Asset(const char* begin, const char* end);

    const char* data() const;
    const char* begin() const;
    const char* end() const;
    size_t size() const;
    std::string_view str() const;
};
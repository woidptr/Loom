#include "Asset.hpp"

Asset::Asset(const char* begin, const char* end) : _begin(begin), _end(end) {}

const char* Asset::data() const {
    return _begin;
}

const char* Asset::begin() const {
    return _begin;
}

const char* Asset::end() const {
    return _end;
}

size_t Asset::size() const {
    return size_t(_end - _begin);
}

std::string_view Asset::str() const {
    return std::string_view{ this->data(), this->size() };
}
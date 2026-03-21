#pragma once
#include <unordered_map>
#include <memory>
#include <libhat/scanner.hpp>
#include <json.hpp>
#include <core/Resource.hpp>
#include <core/Logger.hpp>
#include <core/Signatures.hpp>

struct Address {
    uintptr_t addr;
    sigs::AddressType type;
};

class AddressResolver {
private:
    static inline std::unordered_map<uint64_t, Address> addresses;
public:
    static bool construct();
    static uintptr_t get(hash::hashed_string name_hash);
};

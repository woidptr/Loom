#include "AddressResolver.hpp"
#include <fstream>
#include <core/Memory.hpp>
#include <core/Signatures.hpp>

bool AddressResolver::construct() {
    bool is_healthy = true;

    for (const auto& signature : sigs::get_all()) {
        hat::scan_result result = hat::find_pattern(signature.view, ".text");
        if (!result.has_result()) {
            is_healthy = false;
            $log_error("Failed to resolve signature {}", signature.hash);
            continue;
        }

        const uintptr_t res = reinterpret_cast<uintptr_t>(result.get());
        const uintptr_t address = Memory::ResolveInstructionTarget(res);
        Address addr = Address{
            .addr = address,
            .type = signature.type,
        };

        addresses[signature.hash] = addr;

        $log_info("Successfully resolved signature {} at 0x{:X}", signature.name, address);
    }

    return is_healthy;
}

uintptr_t AddressResolver::get(hash::hashed_string name_hash) {
    auto iterator = addresses.find(name_hash.value);
    if (iterator != addresses.end()) {
        Address addr = iterator->second;

        switch (addr.type) {
            case sigs::AddressType::Direct:
                return addr.addr;
            case sigs::AddressType::Field:
                return Memory::GetOffset(addr.addr);
            case sigs::AddressType::VtableIndex:
                return Memory::GetOffset(addr.addr) / 8;
            default:
                break;
        }
    }

    return 0;
}

#pragma once
#include <cstdint>
#include <cstddef>
#include <bit>
#include <functional>
#include <mutex>
#include <string>
#include <wtypes.h>
#include <windows.h>
#include <core/Logger.hpp>
#include <libhat/memory_protector.hpp>
#include <libhat/process.hpp>

struct ImportResolver {
private:
    using ResolveAddressPtrFn = uintptr_t(*)(const char* mangledName);
public:
    std::string dll_name{};
    ResolveAddressPtrFn resolve{ nullptr };
};

class FakeImports {
public:
    static inline ImportResolver resolver;
public:
    static void load_all_imports(const ImportResolver& config);
};
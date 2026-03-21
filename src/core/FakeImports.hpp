#pragma once
#include <string>
#include <core/Logger.hpp>

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
#pragma once
#include <unordered_map>
#include <memory>
#include <libhat/scanner.hpp>
#include <nlohmann/json.hpp>
#include <simdjson.h>
#include <Resource.hpp>
#include <core/Logger.hpp>

using json = nlohmann::json;

class Signature {
private:
    uintptr_t address = 0;
public:
    Signature(std::string signature);

    uintptr_t getAddress();
};

class SignatureRegistry {
private:
    static inline std::unordered_map<std::string, std::unique_ptr<Signature>> signatures;
    static inline bool healthy = true;
public:
    static void registerSignatures();
    static bool performHealthCheck();
    static Signature* getSignature(std::string name);
};

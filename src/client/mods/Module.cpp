#include "Module.hpp"

#include "imgui.h"

Module::Module(std::string name, std::string id)
    : name(std::move(name)), id(std::move(id)) {}

Module::~Module() {
    listeners.clear();
}

std::string Module::getName() const {
    return name;
}

std::string Module::getId() const {
    return id;
}

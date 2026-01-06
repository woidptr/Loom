#pragma once
#include <string>

class IScreen {
public:
    std::string name;
public:
    IScreen(std::string name) : name(std::move(name)) {}

    // virtual ~IScreen() = 0;
    virtual void render() = 0;
};
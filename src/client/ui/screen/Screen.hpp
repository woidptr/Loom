#pragma once
#include <string>

class Screen {
private:
    std::string name;
public:
    Screen(std::string name) : name(std::move(name)) {}

    virtual void render() = 0;
};
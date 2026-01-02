#pragma once
#include <core/Memory.hpp>

class AbstractScene {
public:
    $padding(0x4);
public:
    virtual ~AbstractScene() = 0;
};
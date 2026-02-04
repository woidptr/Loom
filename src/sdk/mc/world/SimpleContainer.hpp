#pragma once
#include <sdk/Predefine.hpp>

class ItemStack;

class SimpleContainer {
public:
    int mSize;
    std::vector<ItemStack> mItems;
};
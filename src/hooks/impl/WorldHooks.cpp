#include "WorldHooks.hpp"
#include <hooks/HookManager.hpp>
#include <hooks/InlineHook.hpp>

namespace WorldHooks {
    InlineHook<float(void*, int32_t, float)> _Dimension_getTimeOfDay_hook;
    float _Dimension_getTimeOfDay_detour(void* dimension, int32_t time, float alpha) {

    }

    void init() {

    }
}
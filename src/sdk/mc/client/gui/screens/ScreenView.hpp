#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/client/gui/controls/VisualTree.hpp>
#include <sdk/mc/client/render/screen/MinecraftUIRenderContext.hpp>

class ScreenView {
public:
    // $build_access(VisualTree*, mVisualTree, 0x48);
public:
    // member functions

    /// @sig {48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA}
    /// @type direct
    MCAPI void setupAndRender(MinecraftUIRenderContext* renderCtx);
};
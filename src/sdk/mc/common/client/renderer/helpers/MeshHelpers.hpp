#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/client/gui/screens/ScreenContext.hpp>

class Tessellator;
namespace mce { class MaterialPtr; }

namespace MeshHelpers {
    /// @sig {48 89 5C 24 ? 55 56 57 41 56 41 57 48 81 EC ? ? ? ? 49 8B F1 4D 8B F0 48 8B FA}
    MCAPI void renderMeshImmediately(ScreenContext& screenContext, Tessellator& tessellator, mce::MaterialPtr const& material);
}
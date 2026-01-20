#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/client/gui/screens/ScreenContext.hpp>

class Tessellator;
namespace mce { class MaterialPtr; }

namespace MeshHelpers {
    MCAPI void renderMeshImmediately(ScreenContext& screenContext, Tessellator& tessellator, mce::MaterialPtr const& material);
}
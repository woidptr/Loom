#pragma once
#include "../Hook.hpp"
#include <sdk/mc/client/gui/screens/ScreenContext.hpp>
#include <sdk/mc/client/render/Tessellator.hpp>
#include <sdk/mc/deps/renderer/MaterialPtr.hpp>
#include <core/Signatures.hpp>

struct RenderMeshHookTag {};

class RenderMeshHook : public InlineHook<RenderMeshHookTag, void(ScreenContext*, Tessellator*, MaterialPtr*)> {
public:
    RenderMeshHook() : InlineHook($get_signature("MeshHelpers::renderMeshImmediately")) {
        this->hook();
    }
};
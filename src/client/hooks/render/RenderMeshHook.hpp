#pragma once
#include "../Hook.hpp"
#include <sdk/mc/client/gui/screens/ScreenContext.hpp>
#include <sdk/mc/client/render/Tessellator.hpp>
#include <sdk/mc/deps/renderer/MaterialPtr.hpp>
#include <core/Signatures.hpp>

class RenderMeshHook : public Hook<void(ScreenContext*, Tessellator*, MaterialPtr*)> {
public:
    RenderMeshHook() : Hook($getSignature("MeshHelpers::renderMeshImmediately")) {
        this->hook();
    }
};
#pragma once
#include <sdk/Predefine.hpp>

namespace mce {
    class MeshContext;
    class MaterialPtr;
    class TexturePtr;
    class ClientTexture {};
    class ServerTexture {};

    class Mesh {
    public:
        /// @sig {48 8B C4 53 56 57 48 81 EC ? ? ? ? 48 8B D9}
        MCAPI void renderMesh(
            const mce::MeshContext& meshContext,
            const mce::MaterialPtr& materialPtr,
            const std::variant<std::monostate, mce::TexturePtr, mce::ClientTexture, mce::ServerTexture>& texture
        ) const;
    };
}
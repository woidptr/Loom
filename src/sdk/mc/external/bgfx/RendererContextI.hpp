#pragma once
#include <sdk/Predefine.hpp>

namespace bgfx {
    struct RendererType {
        enum Enum : int32_t {
            Noop = 0x0000,
            Direct3D9 = 0x0001,
            Direct3D11 = 0x0002,
            Direct3D12 = 0x0003,
            Direct3D12RTX = 0x0004,
            Gnm = 0x0005,
            Agc = 0x0006,
            Metal = 0x0007,
            OpenGLES = 0x0008,
            OpenGL = 0x0009,
            Vulkan = 0x000A,
            Nvn = 0x000B,
            Count = 0x000C,
        };
    };

    class RendererContextI {
    public:
        virtual ~RendererContextI() = 0;
        virtual RendererType::Enum getRendererType() const = 0;
    };
}

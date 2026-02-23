#include "Resource.hpp"
#include <array>

namespace {

constexpr uint8_t asset_0[] = {
    #embed "C:/Users/firel/source/repos/Loom/assets/fonts/Arimo_Medium.embed.ttf"
};
constexpr uint8_t asset_1[] = {
    #embed "C:/Users/firel/source/repos/Loom/assets/fonts/Montserrat_Medium.embed.ttf"
};
constexpr uint8_t asset_2[] = {
    #embed "C:/Users/firel/source/repos/Loom/assets/icons/close.embed.png"
};
constexpr uint8_t asset_3[] = {
    #embed "C:/Users/firel/source/repos/Loom/assets/test.embed.png"
};
constexpr uint8_t asset_4[] = {
    #embed "C:/Users/firel/source/repos/Loom/assets/test2.embed.png"
};
constexpr uint8_t asset_5[] = {
    #embed "C:/Users/firel/source/repos/Loom/signatures.embed.json"
};

struct EmbeddedResource {
    std::string_view path;
    const uint8_t* data;
    size_t size;
};

constexpr std::array<EmbeddedResource, 6> resources = {{
    {"assets/fonts/Arimo_Medium.ttf", asset_0, sizeof(asset_0)},
    {"assets/fonts/Montserrat_Medium.ttf", asset_1, sizeof(asset_1)},
    {"assets/icons/close.png", asset_2, sizeof(asset_2)},
    {"assets/test.png", asset_3, sizeof(asset_3)},
    {"assets/test2.png", asset_4, sizeof(asset_4)},
    {"signatures.json", asset_5, sizeof(asset_5)},
}};
}

Resource::Resource(std::string_view path) {
    for (const auto& entry : resources) {
        if (entry.path == path) {
            data = entry.data;
            size = entry.size;
            return;
        }
    }
}
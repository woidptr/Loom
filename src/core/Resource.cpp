#include "Resource.hpp"
#include <array>

namespace {

constexpr unsigned char asset_0[] = {
    #embed "C:/Users/firel/source/repos/Loom/assets/fonts/Arimo_Medium.embed.ttf"
};
constexpr unsigned char asset_1[] = {
    #embed "C:/Users/firel/source/repos/Loom/assets/fonts/Montserrat_Medium.embed.ttf"
};
constexpr unsigned char asset_2[] = {
    #embed "C:/Users/firel/source/repos/Loom/assets/icons/close.embed.png"
};
constexpr unsigned char asset_3[] = {
    #embed "C:/Users/firel/source/repos/Loom/signatures.embed.json"
};

struct EmbeddedResource {
    std::string_view path;
    const uint8_t* data;
    size_t size;
};

constexpr std::array<EmbeddedResource, 4> resources = {{
    {"assets/fonts/Arimo_Medium.ttf", asset_0, sizeof(asset_0)},
    {"assets/fonts/Montserrat_Medium.ttf", asset_1, sizeof(asset_1)},
    {"assets/icons/close.png", asset_2, sizeof(asset_2)},
    {"signatures.json", asset_3, sizeof(asset_3)},
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
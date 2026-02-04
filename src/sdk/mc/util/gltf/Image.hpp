#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/util/gltf/ObjectID.hpp>

namespace gltf {
    struct Image {
        enum class ImageMimeType : int {
            Png = 0,
        };

        std::string uri;
        gltf::ObjectID bufferView;
        gltf::Image::ImageMimeType mimeType;
        std::string name;
        int id;
    };
}
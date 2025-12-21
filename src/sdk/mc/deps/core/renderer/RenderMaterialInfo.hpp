#pragma once
#include <memory>
#include <sdk/mc/deps/core/string/HashedString.hpp>

namespace mce {
    class RenderMaterialInfo : public std::enable_shared_from_this<RenderMaterialInfo> {
        HashedString mHahsedString;

        RenderMaterialInfo(const HashedString& name);
    };
}
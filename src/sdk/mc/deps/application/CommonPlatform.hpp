#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/core/utility/NonOwnerPointer.hpp>
#include <sdk/mc/client/game/IMinecraftGame.hpp>

namespace Bedrock {
    class CommonPlatform {
    public:
        $padding(32);

        Bedrock::NonOwnerPointer<IMinecraftGame> mMinecraftGame;
        std::atomic<bool> mbQueueRenderParameterSignal;
        std::atomic<bool> mHasInitialized;
    };
}
#pragma once
#include <sdk/Predefine.hpp>
#include "EffectDuration.hpp"

class MobEffectInstance {
public:
    uint32_t mId;
    EffectDuration mDuration;
    std::optional<EffectDuration> mDurationEasy;
    std::optional<EffectDuration> mDurationNormal;
    std::optional<EffectDuration> mDurationHard;
    int mAplifier;
    bool mDisplayOnScreenTextureAnimation;
    bool mIsCounterPausedThisTick;
    bool mAmbient;
    bool mEffectVisible;
};

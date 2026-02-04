#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/world/actor/player/TrustedSkinFlag.hpp>
#include <sdk/mc/deps/core/image/Image.hpp>
#include <sdk/mc/deps/core/math/Color.hpp>

class SerializedSkin {
public:
    std::string mId;
    std::string mPlayFabId;
    std::string mFullId;
    std::string mResourcePath;
    std::string mDefaultGeometryName;
    mce::Image mSkinImage;
    mce::Image mCapeImage;

    $padding(64);

    std::string mAnimationData;
    std::string mCapeId;

    $padding(24);

    unsigned char mArmSizeType;

    $padding(64);

    mce::Color mSkinColor;
    TrustedSkinFlag mIsTrustedSkin;
    bool mIsPremium;
    bool mIsPersona;
    bool mIsPersonaCapeOnClassicSkin;
    bool mIsPrimaryUser;
    bool mOverridesPlayerAppearance;
};
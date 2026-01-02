#pragma once
#include <array>
#include <chrono>
#include <sdk/mc/deps/core/math/Vec2.hpp>
#include <sdk/mc/deps/core/math/Vec3.hpp>
#include <sdk/mc/platform/brstd/bitset.hpp>

struct MoveInputState {
    enum class Flag : int {
        SneakDown = 0,
        SneakToggleDown = 1,
        WantDownSlow = 2,
        WantUpSlow = 3,
        BlockSelectDown = 4,
        AscendBlock = 5,
        DescendBlock = 6,
        JumpDown = 7,
        SprintDown = 8,
        UpLeft = 9,
        UpRight = 10,
        DownLeft = 11,
        DownRight = 12,
        Up = 13,
        Down = 14,
        Left = 15,
        Right = 16,
        Ascend = 17,
        Descend = 18,
        ChangeHeight = 19,
        LookCenter = 20,
        SneakInputCurrentlyDown = 21,
        SneakInputWasReleased = 22,
        SneakInputWasPressed = 23,
        JumpInputWasReleased = 24,
        JumpInputWasPressed = 25,
        JumpInputCurrentlyDown = 26,
        Count = 27,
    };

    brstd::bitset<27, unsigned int> mFlagValues;
    Vec2 mAnalogMoveVector;
    unsigned char mLookSlightDirField;
    unsigned char mLookNormalDirField;
    unsigned char mLookSmoothDirField;
};

struct MoveInputComponent : IEntityComponent {
public:
    enum class Flag : int {
        Sneaking = 0,
        Sprinting = 1,
        WantUp = 2,
        WantDown = 3,
        Jumping = 4,
        AutoJumpingInWater = 5,
        MoveInputStateLocked = 6,
        PersistSneak = 7,
        AutoJumpEnabled = 8,
        IsCameraRelativeMovementEnabled = 9,
        IsRotControlledByMoveDirection = 10,
        Count = 11,
    };

public:
    MoveInputState mInputState;
    MoveInputState mRawInputState;
    unsigned char mHoldAutoJumpInWaterTicks;
    Vec2 mMove;
    Vec2 mLookDelta;
    Vec2 mInteractDir;
    Vec3 mDisplacement;
    Vec3 mDisplacementDelta;
    Vec3 mCameraOrientation;
    brstd::bitset<11, unsigned short> mFlagValues;
    std::array<bool, 2> mIsPaddling;
};
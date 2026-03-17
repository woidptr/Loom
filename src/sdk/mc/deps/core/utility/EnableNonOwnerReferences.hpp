#pragma once
#include <sdk/Predefine.hpp>

namespace Bedrock {
    class EnableNonOwnerReferences {
    public:
        struct ControlBlock;

        struct ControlBlock {
        public:
            bool mIsValid;
        };
    public:
        std::shared_ptr<::Bedrock::EnableNonOwnerReferences::ControlBlock> mControlBlock;
    public:
        EnableNonOwnerReferences& operator=(EnableNonOwnerReferences const&) = default;
    public:
        virtual ~EnableNonOwnerReferences();
    };
}
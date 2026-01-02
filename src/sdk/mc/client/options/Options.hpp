#pragma once
#include <sdk/mc/options/option_types/Option.hpp>
#include <sdk/mc/options/option_types/OptionID.hpp>

class Options {
public:
    virtual ~Options() = 0;
    virtual void load() = 0;
    virtual void tickSaveTimer() = 0;
    virtual void notifySaveAllowed() = 0;
    virtual void saveIfNeeded() = 0;
    virtual void requestSaveDeferral() = 0;
    virtual void request() = 0;
    virtual bool getPrimaryUserStatus() const = 0;
    virtual void setClientInstanceCoordinator() = 0;
    virtual void getFileStorageLocation() = 0;
    virtual void setFileStorageLocation() = 0;
    virtual void onScreenSizeChanged(int width, int height) = 0;
    virtual bool isValidOption() const = 0;
    virtual Option* get(OptionID optionId) = 0;
    virtual Option* get(OptionID optionId) const = 0;
};
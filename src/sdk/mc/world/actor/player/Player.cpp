#include "Player.hpp"

std::string Player::getXuid() const {
    return std::string("");
    // return Memory::CallVFunc<decltype(&Player::getXuid)>(232, *this);
}

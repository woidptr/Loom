#pragma once
#include <string>
#include <format>
#include <spdlog/spdlog.h>
#include "FileManager.hpp"

#define $log_debug(msg, ...)    spdlog::debug(msg, ##__VA_ARGS__);
#define $log_info(msg, ...)     spdlog::info(msg, ##__VA_ARGS__);
#define $log_warning(msg, ...)  spdlog::warn(msg, ##__VA_ARGS__);
#define $log_error(msg, ...)    spdlog::error(msg, ##__VA_ARGS__);
#define $log_critical(msg, ...) spdlog::critical(msg, ##__VA_ARGS__);

class Logger {
public:
    static void construct();
    static void destruct();
};

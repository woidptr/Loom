#pragma once
#include <string>
#include <format>
#include <spdlog/spdlog.h>
#include "FileManager.hpp"

#define $log_debug(...)    SPDLOG_DEBUG(__VA_ARGS__)
#define $log_info(...)     SPDLOG_INFO(__VA_ARGS__)
#define $log_warning(...)  SPDLOG_WARN(__VA_ARGS__)
#define $log_error(...)    SPDLOG_ERROR(__VA_ARGS__)
#define $log_critical(...) SPDLOG_CRITICAL(__VA_ARGS__)

class Logger {
public:
    static void construct();
    static void destruct();
};

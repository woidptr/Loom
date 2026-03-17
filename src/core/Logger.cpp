#include "Logger.hpp"
#include <Windows.h>
#include <print>
#include <iostream>
#include <filesystem>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <fcntl.h>
#include <io.h>

void Logger::construct() {
    std::vector<spdlog::sink_ptr> sinks;

#if defined(DEBUG_MODE)
    AllocConsole();
    SetConsoleTitleA("Loom Console");

    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    // freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    // freopen_s((FILE**)stdin, "CONIN$", "r", stdin);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sinks.emplace_back(console_sink);
#endif

    fs::path logFilePath = FileManager::getLogsFolder() / "latest.log";
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    sinks.emplace_back(file_sink);

    spdlog::init_thread_pool(8192, 1);

    auto async_logger = std::make_shared<spdlog::async_logger>(
        "LoomLogger",
        sinks.begin(), sinks.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block
    );

    async_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [Loom] [%l]: %v");
    spdlog::set_default_logger(async_logger);

    spdlog::flush_on(spdlog::level::debug);

#if defined(DEBUG_MODE)
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif
}

void Logger::destruct() {
    spdlog::shutdown();

#if defined(DEBUG_MODE)
    if (stdout) fclose(stdout);
    if (stderr) fclose(stderr);
    if (stdin) fclose(stdin);

    FreeConsole();
#endif
}
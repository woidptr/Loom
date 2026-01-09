#pragma once
#include <string>
#include <format>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>
#include <chrono>
#include "FileManager.hpp"

#define $log_debug(msg, ...)    Logger::getInstance()->debug(msg, ##__VA_ARGS__);
#define $log_info(msg, ...)     Logger::getInstance()->info(msg, ##__VA_ARGS__);
#define $log_warning(msg, ...)  Logger::getInstance()->warning(msg, ##__VA_ARGS__);
#define $log_error(msg, ...)    Logger::getInstance()->error(msg, ##__VA_ARGS__);
#define $log_critical(msg, ...) Logger::getInstance()->critical(msg, ##__VA_ARGS__);

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical,
};

class Logger {
private:
    static inline Logger* instance = nullptr;

    std::atomic<bool> running{ false };
    std::mutex queueMutex;
    std::condition_variable cv;
    std::queue<std::string> logQueue;
    std::thread worker;

    FILE* logFile = nullptr;
private:
    void writeToFile(std::string msg);
    void processLogs();

    template <typename... Args>
    void log(LogLevel level, std::format_string<Args...> msg, Args&&... args) {
        std::lock_guard<std::mutex> lock(queueMutex);

        std::string logLevel;

        switch (level) {
        case LogLevel::Debug:
            logLevel = "DEBUG";
            break;
        case LogLevel::Info:
            logLevel = "INFO";
            break;
        case LogLevel::Warning:
            logLevel = "WARNING";
            break;
        case LogLevel::Error:
            logLevel = "ERROR";
            break;
        case LogLevel::Critical:
            logLevel = "CRITICAL";
            break;
        }

        std::string message = std::format(
            "[{}] [Loom] [{}]: {}\n",
            std::format(
                "{:%Y-%m-%d %H:%M:%S}",
                std::chrono::zoned_time{
                    std::chrono::current_zone(),
                    std::chrono::system_clock::now()
                }
            ),
            logLevel,
            std::format(msg, std::forward<Args>(args)...)
        );

        logQueue.push(std::move(message));

        cv.notify_one();
    }
public:
    Logger();
    ~Logger();

    static void init();
    static void shutdown();
    static Logger* getInstance();

    template <typename...Args>
    void debug(std::format_string<Args...> fmt, Args&&... args) {
        this->log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }

    template <typename...Args>
    void info(std::format_string<Args...> fmt, Args&&... args) {
        this->log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template <typename...Args>
    void warning(std::format_string<Args...> fmt, Args&&... args) {
        this->log(LogLevel::Warning, fmt, std::forward<Args>(args)...);
    }

    template <typename...Args>
    void error(std::format_string<Args...> fmt, Args&&... args) {
        this->log(LogLevel::Error, fmt, std::forward<Args>(args)...);
    }

    template <typename...Args>
    void critical(std::format_string<Args...> fmt, Args&&... args) {
        this->log(LogLevel::Critical, fmt, std::forward<Args>(args)...);
    }
};

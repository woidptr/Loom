#include "Logger.hpp"
#include <Windows.h>
#include <print>
#include <iostream>
#include <filesystem>
#include <fmt/printf.h>
#include <fmt/color.h>

Logger::Logger() {
#ifndef NDEBUG
    AllocConsole();
    SetConsoleTitleA("Loom");
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

    fs::path logFilePath = FileManager::getLogsFolder() / "latest.log";

    fopen_s(&logFile, logFilePath.string().c_str(), "w");

    running = true;
    worker = std::thread(&Logger::processLogs, this);
}

Logger::~Logger() {
#ifndef NDEBUG
    if (stdout) fclose(stdout);
    if (stderr) fclose(stderr);
    if (stdin)  fclose(stdin);

    FreeConsole();
#endif

    if (worker.joinable()) {
        worker.join();
    }

    if (logFile) {
        fclose(logFile);
    }

    running = false;
}

void Logger::init() {
    if (instance == nullptr) {
        instance = new Logger();
    }
}

Logger* Logger::getInstance() {
    return instance;
}

void Logger::shutdown() {
    if (instance) {
        delete instance;
    }
}

void Logger::processLogs() {
    while (running || !logQueue.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [&] {
            return !logQueue.empty() || !running;
            });

        while (!logQueue.empty()) {
            std::string msg = std::move(logQueue.front());

            logQueue.pop();
            lock.unlock();

#ifndef NDEBUG
            fmt::print("{}", msg);
#endif

            writeToFile(msg);

            lock.lock();
        }
    }
}

void Logger::writeToFile(std::string msg) {
    fwrite(msg.c_str(), 1, msg.size(), logFile);
}
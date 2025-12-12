#include "Logger.hpp"
#include <Windows.h>
#include <print>
#include <iostream>
#include <filesystem>
#include <fmt/printf.h>
#include <fmt/color.h>

void Logger::init() {
#ifndef NDEBUG
	AllocConsole();
	SetConsoleTitleA("Loom");
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

	fs::path logFilePath = FileManager::getLogsFolder() / "latest.log";

	fopen_s(&logFile, logFilePath.string().c_str(), "w");

	running = true;
	worker = std::thread(processLogs);
}

void Logger::shutdown() {
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

void Logger::processLogs() {
	while (running || !logQueue.empty()) {
		std::unique_lock<std::mutex> lock(queueMutex);
		cv.wait(lock, [] {
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

void Logger::log(std::string logType, std::string msg) {
	std::lock_guard<std::mutex> lock(queueMutex);

	logQueue.push(std::format("[Loom] [{}]: {}\n", logType, msg));

	cv.notify_one();
}

void Logger::writeToFile(std::string msg) {
	fwrite(msg.c_str(), 1, msg.size(), logFile);
}

void Logger::debug(std::string msg) {
	log("DEBUG", msg);
}

void Logger::info(std::string msg) {
	log("INFO", msg);
}

void Logger::warning(std::string msg) {
	log("WARNING", msg);
}

void Logger::error(std::string msg) {
	log("ERROR", msg);
}

void Logger::critical(std::string msg) {
	log("CRITICAL", msg);
}

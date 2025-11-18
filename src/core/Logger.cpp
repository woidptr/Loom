#include "Logger.h"
#include <Windows.h>
#include <print>
#include <iostream>
#include <filesystem>
#include <fmt/printf.h>
#include <fmt/color.h>

std::atomic<bool> Logger::running{ false };
std::mutex Logger::queueMutex;
std::condition_variable Logger::cv;
std::queue<std::string> Logger::logQueue;
std::thread Logger::worker;

void Logger::init() {
#ifndef NDEBUG
	AllocConsole();
	SetConsoleTitleA("Loom");
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

	running = true;
	worker = std::thread(processLogs);
}

void Logger::shutdown() {
#ifndef NDEBUG
	FreeConsole();
#endif

	if (worker.joinable()) {
		worker.join();
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
			fmt::print("{}\n", msg);
#endif

			lock.lock();
		}
	}
}

void Logger::log(std::string logType, std::string msg) {
	std::lock_guard<std::mutex> lock(queueMutex);

	logQueue.push(std::format("[Loom] [{}]: {}", logType, msg));

	cv.notify_one();
}

void Logger::writeToFile(std::string msg) {

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

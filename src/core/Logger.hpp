#pragma once
#include <string>
#include <format>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>
#include "FileManager.hpp"

class Logger {
private:
	static inline std::atomic<bool> running{ false };
	static inline std::mutex queueMutex;
	static inline std::condition_variable cv;
	static inline std::queue<std::string> logQueue;
	static inline std::thread worker;

	static inline FILE* logFile = nullptr;

	static void log(std::string logType, std::string msg);
	static void writeToFile(std::string msg);
	static void processLogs();
public:
	static void init();
	static void shutdown();

	static void debug(std::string msg);
	static void info(std::string msg);
	static void warning(std::string msg);
	static void error(std::string msg);
	static void critical(std::string msg);
};

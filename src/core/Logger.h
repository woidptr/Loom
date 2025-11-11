#pragma once
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>

class Logger {
private:
	static std::atomic<bool> running;
	static std::mutex queueMutex;
	static std::condition_variable cv;
	static std::queue<std::string> logQueue;
	static std::thread worker;

	static void log(std::string logType, std::string msg);
	static void writeToFile(std::string msg);
	static void processLogs();
public:
	static void init();

	static void debug(std::string msg);
	static void info(std::string msg);
	static void warning(std::string msg);
	static void error(std::string msg);
	static void critical(std::string msg);
};

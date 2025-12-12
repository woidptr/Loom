#pragma once
#include <Windows.h>
#include <shlobj.h>
#include <filesystem>

namespace fs = std::filesystem;

class FileManager {
public:
	static fs::path getRootFolder();
	static fs::path getLogsFolder();
	static fs::path getSettingsFolder();
};
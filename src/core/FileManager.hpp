#pragma once
#include <Windows.h>
#include <shlobj.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class FileManager {
private:
    static void ensureExists(fs::path path);
public:
    static fs::path getRootFolder();
    static fs::path getLogsFolder();
    static fs::path getSettingsFolder();
    static fs::path getMainSettingsFile();
    static fs::path getProfileSettingFile(const std::string& profile);
};
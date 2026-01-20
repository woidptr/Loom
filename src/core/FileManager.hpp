#pragma once
#include <Windows.h>
#include <shlobj.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class FileManager {
public:
    static fs::path getRootFolder();
    static fs::path getLogsFolder();
    static fs::path getSettingsFolder();
    static fs::path getMainSettingsFile();
    static fs::path getProfileFolder(std::string profileName);
};
#include "FileManager.hpp"
#include "Logger.hpp"

fs::path FileManager::getRootFolder() {
    PWSTR path = nullptr;

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);

    if (SUCCEEDED(hr)) {
        fs::path rootPath = fs::path(path) / ".loom";

        if (!fs::exists(rootPath)) {
            fs::create_directories(rootPath);
        }

        return rootPath;
    }

    CoTaskMemFree(path);
}

fs::path FileManager::getLogsFolder() {
    fs::path logsPath = getRootFolder() / "logs";

    if (!fs::exists(logsPath)) {
        fs::create_directories(logsPath);
    }

    return logsPath;
}

fs::path FileManager::getSettingsFolder() {
    fs::path settingsPath = getRootFolder() / "settings";

    if (!fs::exists(settingsPath)) {
        fs::create_directories(settingsPath);
    }

    return settingsPath;
}

fs::path FileManager::getMainSettingsFile() {
    fs::path mainSettingsFilePath = getSettingsFolder() / "settings.json";

    if (!fs::exists(mainSettingsFilePath)) {
        std::ofstream file(mainSettingsFilePath);
        file.close();
    }
}

fs::path FileManager::getProfileFolder(std::string profileName) {
    fs::path profilePath = getSettingsFolder() / profileName;

    if (!fs::exists(profilePath)) {
        fs::create_directories(profilePath);
    }

    return profilePath;
}
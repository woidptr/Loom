#include "FileManager.hpp"
#include "Logger.hpp"

void FileManager::ensureExists(fs::path path) {
    if (!fs::exists(path)) {
        fs::create_directories(path);
    }
}

fs::path FileManager::getRootFolder() {
    PWSTR path = nullptr;

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);

    if (SUCCEEDED(hr)) {
        fs::path rootPath = fs::path(path) / ".loom";
        ensureExists(rootPath);

        return rootPath;
    }

    CoTaskMemFree(path);
}

fs::path FileManager::getLogsFolder() {
    fs::path logsPath = getRootFolder() / "logs";
    ensureExists(logsPath);

    return logsPath;
}

fs::path FileManager::getSettingsFolder() {
    fs::path settingsPath = getRootFolder() / "settings";
    ensureExists(settingsPath);

    return settingsPath;
}

fs::path FileManager::getMainSettingsFile() {
    fs::path mainSettingsFilePath = getSettingsFolder() / "settings.json";

    if (!fs::exists(mainSettingsFilePath)) {
        std::ofstream file(mainSettingsFilePath);
        file.close();
    }

    return mainSettingsFilePath;
}

fs::path FileManager::getProfilesFile() {
    fs::path profilesFilePath = getSettingsFolder() / "profiles.json";
    return profilesFilePath;
}

fs::path FileManager::getProfileSettingFile(const std::string& profile) {
    fs::path profilesPath = getSettingsFolder() / "profiles";
    ensureExists(profilesPath);

    fs::path settingProfilePath = profilesPath / profile;
}

fs::path FileSystem::getAppDataPath() {
    fs::path rootPath;

    const char* appData = getenv("APPDATA");
    rootPath = fs::path(appData);

    return rootPath / ".myraclient";
}
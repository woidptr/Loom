#include "FileManager.h"

void FileManager::getRootFolder() {
	PWSTR path = nullptr;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);

	if (SUCCEEDED(hr)) {

	}

	CoTaskMemFree(path);
}
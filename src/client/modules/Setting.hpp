#pragma once
#include <string>

class Setting {
private:
	std::string name;
public:
	Setting(std::string name) : name(name) {}
	virtual ~Setting() = default;
};

class BoolSetting : public Setting {
private:
	bool value;
public:
	BoolSetting(std::string name, bool value) : Setting(name), value(value) {}
};
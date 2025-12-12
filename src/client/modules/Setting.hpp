#pragma once
#include <string>

class Setting {
public:
	std::string name;

	Setting(std::string name) : name(name) {}

	virtual ~Setting() = default;
};

class BoolSetting : public Setting {
public:
	bool value;

	BoolSetting(std::string name, bool value) : Setting(name), value(value) {}
};

class FloatSetting : public Setting {
public:
	float value;

	float minValue;
	float maxValue;

	FloatSetting(std::string name, float value, float minValue, float maxValue)
		: Setting(name), value(value), minValue(minValue), maxValue(maxValue) {}
};

class StringSetting : public Setting {
public:
	std::string value;

	StringSetting(std::string name, std::string value) : Setting(name), value(value) {}
};
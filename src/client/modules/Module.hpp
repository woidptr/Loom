#pragma once
#include <string>
#include "Setting.hpp"

class Module {
private:
	std::string name;
	bool enabled = false;

	std::vector<Setting> settings;
public:
	Module(std::string name) {
		this->name = name;
	}
};
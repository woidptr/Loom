#pragma once
#include <string>

class Module {
private:
	std::string name;
public:
	Module(std::string name) {
		this->name = name;
	}
};
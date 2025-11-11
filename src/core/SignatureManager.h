#pragma once
#include <string>
#include <vector>
#include <libhat/scanner.hpp>

struct Signature {
public:
	std::string name;
	std::string pattern;
	uintptr_t address;

	Signature(std::string name, std::string pattern) {
		this->name = name;
		this->pattern = pattern;
	}
};

class SignatureManager {
private:
	std::vector<Signature*> signatures;
public:
	SignatureManager();

	Signature* getSignatureByName(std::string name);
	void addSignature(const char pattern) {}
};

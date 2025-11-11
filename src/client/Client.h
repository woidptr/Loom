#pragma once
#include <vector>

#include "../core/SignatureManager.h"
#include "hooks/Hook.h"

class Client {
private:
	SignatureManager* signatureManager;
	std::vector<Hook*> hooks;
public:
	Client();

	void initHooks();
};
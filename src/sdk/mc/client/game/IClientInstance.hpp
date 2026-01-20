#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/client/player/LocalPlayer.hpp>
#include <sdk/mc/client/gui/GuiData.hpp>
#include <sdk/mc/client/gui/screens/SceneFactory.hpp>
#include <sdk/mc/client/gui/screens/interfaces/ISceneStack.hpp>

template<typename T>
class NotNullNonOwnerPtr {
public:
	void* ptr1;
	void* ptr2;
	T* value;
};

class IClientInstance {
public:
	virtual ~IClientInstance() = 0;

	LocalPlayer* getLocalPlayer();
	SceneFactory* getSceneFactory();
	NotNullNonOwnerPtr<ISceneStack> getClientSceneStack();
	std::string getTopScreenName();
};
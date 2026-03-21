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

	/// @sig {48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8D 54 24 ? 48 8B C8 E8 ? ? ? ? 45 33 C0 48 8B D0 48 8B CF 48 8B C3 FF 15 ? ? ? ? 90 4C 89 74 24}
	/// @type vtable_index
	SceneFactory* getSceneFactory();

	/// @sig {48 8B 80 ? ? ? ? FF 15 ? ? ? ? 90 48 8B 78 ? 48 8B 07 48 8B 98 ? ? ? ? 48 8B 06 48 8B CE 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 48 8D 54 24 ? 48 8B C8 E8 ? ? ? ? 45 33 C0 48 8B D0 48 8B CF 48 8B C3 FF 15 ? ? ? ? 90 4C 89 74 24}
	/// @type vtable_index
	NotNullNonOwnerPtr<ISceneStack> getClientSceneStack();

	std::string getTopScreenName();
};
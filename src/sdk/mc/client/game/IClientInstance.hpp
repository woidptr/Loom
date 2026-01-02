#pragma once
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

	$virtual_function(LocalPlayer*, getLocalPlayer, 31);
	$virtual_function(SceneFactory*, getSceneFactory, $get_index("IClientInstance$$getSceneFactory"));
	// $virtual_function(NotNullNonOwnerPtr<ISceneStack>, getClientSceneStack, $get_index("IClientInstance$$getSceneFactory"));
	NotNullNonOwnerPtr<ISceneStack> getClientSceneStack();
};
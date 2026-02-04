#include "GameContext.hpp"

void GameContext::registerClientInstance(IClientInstance* clientInstance) {
	std::call_once(clientInstanceInitializer, [&](){
		GameContext::clientInstance = clientInstance;
		GameContext::sceneFactory = clientInstance->getSceneFactory();
		GameContext::sceneStack = clientInstance->getClientSceneStack().value;
	});
}
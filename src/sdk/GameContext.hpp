#pragma once
#include <sdk/mc/deps/core/utility/BinaryStream.hpp>
#include <sdk/mc/client/gui/screens/SceneFactory.hpp>
#include <sdk/mc/client/gui/screens/ScreenContext.hpp>
#include <sdk/mc/client/render/screen/MinecraftUIRenderContext.hpp>
#include <sdk/mc/client/render/Tessellator.hpp>
#include <sdk/mc/client/game/IClientInstance.hpp>
#include <sdk/mc/client/player/LocalPlayer.hpp>
#include <sdk/mc/deps/renderer/MaterialPtr.hpp>
#include <sdk/mc/deps/renderer/Mesh.hpp>
#include <sdk/mc/deps/renderer/MeshContext.hpp>
#include <sdk/mc/common/client/renderer/helpers/MeshHelpers.hpp>
#include <sdk/mc/deps/renderer/TexturePtr.hpp>
#include <sdk/mc/network/Packet.h>
#include <sdk/mc/network/MinecraftPackets.hpp>
#include <sdk/mc/network/IPacketHandlerDispatcher.hpp>
#include <sdk/mc/world/level/dimension/Dimension.hpp>
#include <sdk/mc/client/gui/screens/ScreenView.hpp>

// containers
#include <sdk/mc/world/Container.hpp>
#include <sdk/mc/world/SimpleContainer.hpp>

// components
#include <sdk/mc/entity/components/ActorEquipmentComponent.hpp>
#include <sdk/mc/entity/components/MoveInputComponent.hpp>

class GameContext {
private:
	static inline std::once_flag clientInstanceInitializer;
public:
	static inline IClientInstance* clientInstance = nullptr;
	static inline SceneFactory* sceneFactory = nullptr;
	static inline ISceneStack* sceneStack = nullptr;

	static void registerClientInstance(IClientInstance* clientInstance);
};
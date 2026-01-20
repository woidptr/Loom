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

class GameContext {
public:
	static inline MinecraftUIRenderContext* renderCtx = nullptr;
	static inline IClientInstance* clientInstance = nullptr;
	static inline Tessellator* tessellator = nullptr;
	static inline mce::MaterialPtr* materialPtr = nullptr;
	static inline LocalPlayer* localPlayer = nullptr;

	static void registerRenderContext(MinecraftUIRenderContext* renderCtx);
};
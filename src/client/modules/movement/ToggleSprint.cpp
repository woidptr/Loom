#include "ToggleSprint.hpp"
#include <core/Logger.hpp>
#include <sdk/GameContext.hpp>
#include <sdk/mc/deps/renderer/TexturePtr.hpp>
#include <core/Signatures.hpp>
#include <client/ui/impl/ImGuiImplMC.hpp>
#include <sdk/mc/entity/components/MoveInputComponent.hpp>
#include <hooks/impl/network/IncomingPacketsHook.hpp>

#include <hooks/impl/render/SetupAndRenderHook.hpp>

ToggleSprint::ToggleSprint() : Module("Toggle Sprint") {
    listeners.reserve(1);

    listeners.emplace_back(EventHandler::subscribe<SetupAndRenderEvent, &ToggleSprint::onRender>(*this));
}

void ToggleSprint::onRender(SetupAndRenderEvent& event) {
    void** vtable = *(void***)event.renderCtx;
    uintptr_t staticVtable = reinterpret_cast<uintptr_t>(vtable) - reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
    // Logger::info(std::format("MinecraftUIRenderContext vtable address: 0x{:X}", staticVtable));
    // Logger::info(std::format("Text alpha: {}", renderCtx->getTextAlpha()));
    static bool initTest = false;
    if (!initTest) {
        GameContext::clientInstance = event.renderCtx->mClient;
        uintptr_t* vtable = *(uintptr_t**)event.renderCtx->mClient;
        uintptr_t funcRuntimeAddr = vtable[$get_index("IClientInstance$$getSceneFactory")];
        uintptr_t moduleBase = (uintptr_t)GetModuleHandle(nullptr);
        uintptr_t funcAddr = funcRuntimeAddr - moduleBase;
        uintptr_t standardImageBase = 0x140000000;
        uintptr_t idaAddr = standardImageBase + funcAddr;

        // $log_debug("IDA address for IClientInstance::getLocalPlayer: {}");
        $log_debug("IDA address for IClientInstance::getSceneFactory: 0x{:X}", idaAddr);
        // uintptr_t ci_staticVtable = reinterpret_cast<uintptr_t>(ci_vtable) - reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

        // $log_debug("ClientInstance vtable address: 0x{:X}", ci_staticVtable);
        // $log_debug("ClientInstance::getLocalPlayer dynamic address: {:P}", ci_vtable[31]);
        // $logDebug("ClientInstance::getLocalPlayer static address: {:P}", ((void*)ci_staticVtable)[31]);

        // $log_debug("GuiData runtime address: 0x{:X}", (uintptr_t)renderCtx->screenContext->guiData);

        // $logDebug("LocalPlayer runtime address: 0x{:X}", (uintptr_t)renderCtx->clientInstance->getLocalPlayer());
        // $logDebug("ScreenView->tessellator offset: 0x{:X}", *(int32_t*)$getSignatureAddr("ScreenContext->tessellator"));

        // ImGui::CreateContext();
        // ImGui_ImplMC_Init(renderCtx);

        initTest = true;
    }

    // $logDebug("Screen name: {}", screenView->mVisualTree->name);

    /*ImGui_ImplMC_NewFrame(renderCtx);
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplMC_RenderDrawData(ImGui::GetDrawData(), renderCtx);*/

    // $logDebug("Gui scale: {}", renderCtx->screenContext->guiData->mScreenSizeData.totalScreenSize.x);

    // ResourceLocation resLoc("textures/items/potato_baked");

    // mce::TexturePtr texture = renderCtx->getTexture(resLoc, true);
    // ResourceLocation resLoc("C:\\Users\\firel\\source\\repos\\Loom\\assets\\test.png");
    // $logDebug("String size: {}", resLoc.mFullHash);
    // mce::TexturePtr texturePtr = mce::TexturePtr(resLoc);
    // renderCtx->drawImage(texture, Vec2{ 50, 50 }, Vec2{ 200, 200 }, Vec2{ 0, 0 }, Vec2{ 100, 100 }, 0);
    // renderCtx->flushImages(mce::Color(1.f, 1.f, 1.f, 0.f), 1.f, HashedString("test_material"));

    renderCallback(event.renderCtx);
}

void ToggleSprint::renderCallback(MinecraftUIRenderContext* renderCtx) {
    // if (!enabled) {
    //     return;
    // }

    if (LocalPlayer* lp = renderCtx->mClient->getLocalPlayer()) {
        MoveInputComponent* mic = lp->mEntityContext.tryGetComponent<MoveInputComponent>();

        if (mic) {
            mic->mInputState.mFlagValues.set(static_cast<size_t>(MoveInputState::Flag::SprintDown), true);
            mic->mRawInputState.mFlagValues.set(static_cast<size_t>(MoveInputState::Flag::SprintDown), true);
            mic->mFlagValues.set(static_cast<size_t>(MoveInputComponent::Flag::Sprinting), true);
        }
    }
}
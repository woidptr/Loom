#include "ToggleSprint.hpp"
#include <core/Logger.hpp>
#include <sdk/GameContext.hpp>
#include <sdk/mc/deps/renderer/TexturePtr.hpp>
#include <core/Signatures.hpp>
#include <client/ui/impl/ImGuiImplMC.hpp>
#include <sdk/mc/entity/components/MoveInputComponent.hpp>
#include <events/render/SetupAndRenderEvent.hpp>

ToggleSprint::ToggleSprint() : Module("Toggle Sprint") {
    listeners.reserve(2);

    $add_listener(SetupAndRenderEvent, &ToggleSprint::onRender);
    $add_listener(KeyboardEvent, &ToggleSprint::onKey);
    $add_listener(HudElementRenderEvent, &ToggleSprint::onHudElementRender);
}

void ToggleSprint::onRender(SetupAndRenderEvent* event) {
    void** vtable = *(void***)event->renderCtx;
    uintptr_t staticVtable = reinterpret_cast<uintptr_t>(vtable) - reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
    // Logger::info(std::format("MinecraftUIRenderContext vtable address: 0x{:X}", staticVtable));
    // Logger::info(std::format("Text alpha: {}", renderCtx->getTextAlpha()));
    static bool initTest = false;
    if (!initTest) {
        uintptr_t* vtable = *(uintptr_t**)event->renderCtx->mClient;
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

    if (LocalPlayer* lp = event->renderCtx->mClient->getLocalPlayer()) {
        MoveInputComponent* mic = lp->mEntityContext.tryGetComponent<MoveInputComponent>();

        if (mic) {
            mic->mInputState.mFlagValues.set(static_cast<size_t>(MoveInputState::Flag::SprintDown), true);
            mic->mRawInputState.mFlagValues.set(static_cast<size_t>(MoveInputState::Flag::SprintDown), true);
            mic->mFlagValues.set(static_cast<size_t>(MoveInputComponent::Flag::Sprinting), true);
        }
    }
}

void ToggleSprint::onKey(KeyboardEvent* event) {
    // std::shared_ptr<AbstractScene> scene = GameContext::sceneFactory->createPauseScreen();
    // $log_debug("Scene id: {}", scene.get()->mSceneId);
    GameContext::sceneStack->pushScreen(GameContext::sceneFactory->createPauseScreen(), false);

    // $log_debug("Current screen name: {}", GameContext::clientInstance->getTopScreenName());
}

void ToggleSprint::onHudElementRender(HudElementRenderEvent* event) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 p_min = ImVec2(10.f, 10.f);
    ImVec2 p_max = ImVec2(100.f, 40.f);

    // int fps = static_cast<int>(std::round(ImGui::GetIO().Framerate));

    std::string fpsText = std::string("[Sprinting (Toggled)]");

    drawList->AddRectFilled(p_min, p_max, IM_COL32(0, 0, 0, 200), 8.f);
    drawList->AddRect(p_min, p_max, IM_COL32(0, 0, 0, 255), 8.f, ImDrawFlags_None, 2.f);

    ImVec2 boxSize = ImVec2(p_max.x - p_min.x, p_max.y - p_min.y);
    ImVec2 originalTextSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, 0.0f, fpsText.c_str());
    float scaleX = (boxSize.x * 0.8f) / originalTextSize.x;
    float scaleY = (boxSize.y * 0.8f) / originalTextSize.y;
    float finalScale = std::min(scaleX, scaleY);
    ImVec2 centeredPos = ImVec2(
        p_min.x + (boxSize.x - (originalTextSize.x * finalScale)) * 0.5f,
        p_min.y + (boxSize.y - (originalTextSize.y * finalScale)) * 0.5f
    );

    drawList->AddText(ImGui::GetFont(), ImGui::GetFontSize() * finalScale, p_min, IM_COL32(255, 255, 255, 255), fpsText.c_str());

    if (event->isInEditMode) {
        drawList->AddRectFilled(p_min, p_max, ImU32(9999), 8.f);
    }
}
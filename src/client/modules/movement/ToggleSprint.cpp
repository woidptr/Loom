#include "ToggleSprint.hpp"
#include <core/Logger.hpp>
#include <sdk/GameContext.hpp>
#include <sdk/mc/deps/renderer/TexturePtr.hpp>
#include <core/Signatures.hpp>
#include <client/ui/impl/ImGuiImplMC.hpp>

ToggleSprint::ToggleSprint(SetupAndRenderHook* setupAndRenderHook) : Module("Toggle Sprint") {
    setupAndRenderHook->registerCallback(
        [&](CallbackContext& cbCtx, void* screenView, MinecraftUIRenderContext* renderCtx) {
            void** vtable = *(void***)renderCtx;
            uintptr_t staticVtable = reinterpret_cast<uintptr_t>(vtable) - reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
            // Logger::info(std::format("MinecraftUIRenderContext vtable address: 0x{:X}", staticVtable));
            // Logger::info(std::format("Text alpha: {}", renderCtx->getTextAlpha()));
            static bool initTest = false;
            if (!initTest) {
                void** ci_vtable = *(void***)renderCtx->clientInstance;
                uintptr_t ci_staticVtable = reinterpret_cast<uintptr_t>(ci_vtable) - reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

                $logDebug("ClientInstance vtable address: 0x{:X}", ci_staticVtable);
                $logDebug("ClientInstance::getLocalPlayer dynamic address: {:P}", ci_vtable[31]);
                // $logDebug("ClientInstance::getLocalPlayer static address: {:P}", ((void*)ci_staticVtable)[31]);

                $logDebug("GuiData runtime address: 0x{:X}", (uintptr_t)renderCtx->screenContext->guiData);
                // $logDebug("ScreenView->tessellator offset: 0x{:X}", *(int32_t*)$getSignatureAddr("ScreenContext->tessellator"));

                // ImGui::CreateContext();
                // ImGui_ImplMC_Init(renderCtx);

                initTest = true;
            }

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
        }
    );
}

void ToggleSprint::tickCallback(void* a1) {

}
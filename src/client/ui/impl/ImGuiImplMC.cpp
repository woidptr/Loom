#include "ImGuiImplMC.hpp"
#include <sdk/mc/client/render/screen/MinecraftUIRenderContext.hpp>
#include <sdk/mc/client/render/Tessellator.hpp>
#include <sdk/mc/common/client/renderer/helpers/MeshHelpers.hpp>
#include <sdk/GameContext.hpp>

static Tessellator* tessellator;

IMGUI_IMPL_API bool ImGui_ImplMC_Init(MinecraftUIRenderContext* renderCtx) {
    tessellator = renderCtx->screenContext->tessellator;

    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedLines = false;
    style.AntiAliasedFill = false;

    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;

    unsigned char* pixels;
    int width, height, bytesPerPixel;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytesPerPixel);

    mce::TexturePtr texturePtr = renderCtx->getTexture(ResourceLocation("textures/items/diamond_sword"), true);
    io.Fonts->TexID = (void*)&texturePtr;

    return true;
}

IMGUI_IMPL_API void ImGui_ImplMC_Shutdown() {
    tessellator = nullptr;
}

IMGUI_IMPL_API void ImGui_ImplMC_NewFrame(MinecraftUIRenderContext* renderCtx) {
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize.x = renderCtx->screenContext->guiData->mScreenSizeData.totalScreenSize.x;
    io.DisplaySize.y = renderCtx->screenContext->guiData->mScreenSizeData.totalScreenSize.y + 2;
}

IMGUI_IMPL_API void ImGui_ImplMC_RenderDrawData(ImDrawData* drawData, MinecraftUIRenderContext* renderCtx) {
    float scale = renderCtx->screenContext->guiData->mGuiScale;

    for (int n = 0; n < drawData->CmdListsCount; n++) {
        const ImDrawList* cmdList = drawData->CmdLists[n];

        for (int cmdIndex = 0; cmdIndex < cmdList->CmdBuffer.size(); cmdIndex++) {
            tessellator->begin(mce::PrimitiveMode::TriangleList, 0, false);

            const ImDrawCmd& cmd = cmdList->CmdBuffer[cmdIndex];
            const ImDrawVert* vertices = cmdList->VtxBuffer.Data + cmd.VtxOffset;
            const ImDrawIdx* indices = cmdList->IdxBuffer.Data + cmd.IdxOffset;

            for (uint32_t i = 0; i < cmd.ElemCount; i += 3) {
                const ImDrawVert& v0 = vertices[indices[i + 2]];
                const ImDrawVert& v1 = vertices[indices[i + 1]];
                const ImDrawVert& v2 = vertices[indices[i + 0]];

                tessellator->color(v0.col);
                tessellator->vertexUV(Vec3(v0.pos.x / scale, v0.pos.y / scale, 0.0f), Vec2(v0.uv.x, v0.uv.y));

                tessellator->color(v1.col);
                tessellator->vertexUV(Vec3(v1.pos.x / scale, v1.pos.y / scale, 0.0f), Vec2(v1.uv.x, v1.uv.y));

                tessellator->color(v2.col);
                tessellator->vertexUV(Vec3(v2.pos.x / scale, v2.pos.y / scale, 0.0f), Vec2(v2.uv.x, v2.uv.y));
            }

            // renderCtx->saveCurrentClippingRectangle();
            // RectangleArea rect{ cmd.ClipRect.x / scale, cmd.ClipRect.z / scale, cmd.ClipRect.y / scale, cmd.ClipRect.w / scale };
            // renderCtx->setClippingRectangle(rect);

            MeshHelpers::renderMeshImmediately(*renderCtx->screenContext, *renderCtx->screenContext->tessellator, *GameContext::materialPtr);

            // renderCtx->restoreSavedClippingRectangle();
        }
    }
}
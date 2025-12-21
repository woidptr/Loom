#pragma once
#include <imgui.h>

class MinecraftUIRenderContext;

IMGUI_IMPL_API bool ImGui_ImplMC_Init(MinecraftUIRenderContext* renderCtx);
IMGUI_IMPL_API void ImGui_ImplMC_Shutdown();
IMGUI_IMPL_API void ImGui_ImplMC_NewFrame(MinecraftUIRenderContext* renderCtx);
IMGUI_IMPL_API void ImGui_ImplMC_RenderDrawData(ImDrawData* drawData, MinecraftUIRenderContext* renderCtx);
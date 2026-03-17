#include "Typography.hpp"

namespace UI::Elements::Typography {
    void DrawGradientText(ImDrawList *draw_list, ImVec2 pos, const char *text, ImU32 col_left, ImU32 col_right) {
        int vtx_start = draw_list->VtxBuffer.Size;
        draw_list->AddText(pos, IM_COL32_WHITE, text);
        int vtx_end = draw_list->VtxBuffer.Size;

        ImVec2 text_size = ImGui::CalcTextSize(text);
        float min_x = pos.x;
        float max_x = pos.x + text_size.x;

        ImVec4 c_left = ImGui::ColorConvertU32ToFloat4(col_left);
        ImVec4 c_right = ImGui::ColorConvertU32ToFloat4(col_right);

        for (int i = vtx_start; i < vtx_end; i++) {
            ImDrawVert& vert = draw_list->VtxBuffer.Data[i];

            float t = (vert.pos.x - min_x) / (max_x - min_x);

            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            float r = c_left.x + (c_right.x - c_left.x) * t;
            float g = c_left.y + (c_right.y - c_left.y) * t;
            float b = c_left.z + (c_right.z - c_left.z) * t;
            float a = c_left.w + (c_right.w - c_left.w) * t;

            vert.col = ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a));
        }
    }
}
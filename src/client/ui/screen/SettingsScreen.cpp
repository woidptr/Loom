#include "SettingsScreen.hpp"

void SettingsScreen::render() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 10.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 12.0f;

    style.WindowPadding = ImVec2(20, 20);
    style.FramePadding = ImVec2(10, 6);

    // ImVec4 backgroundColor = ImVec4(0.10f, 0.10f, 0.11f, 0.95f);
    // ImGui::PushStyleColor(backgroundColor);

    // Colors
    ImVec4 colWinBg = ImVec4(0.08f, 0.08f, 0.09f, 1.0f);
    ImVec4 colActive = ImVec4(0.85f, 0.22f, 0.22f, 1.0f); // Red
    ImVec4 colInactive = ImVec4(0.15f, 0.15f, 0.17f, 1.0f); // Gray
    ImVec4 colSearchBg = ImVec4(0.12f, 0.12f, 0.14f, 1.0f); // Darker Gray

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowSize(900, 500);

    const float baseW = 900.0f;
    const float baseH = 500.0f;

    float scale = std::min(viewport->Size.x / baseW, viewport->Size.y / baseH);
    scale = std::min(scale, 1.0f);

    ImVec2 scaledSize(baseW * scale, baseH * scale);

    ImVec2 size(baseW * scale, baseH * scale);

    ImVec2 centerPos = ImVec2(
        viewport->Pos.x + (viewport->Size.x - windowSize.x) * 0.5f,
        viewport->Pos.y + (viewport->Size.y - windowSize.y) * 0.5f
    );

    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;

    if (ImGui::Begin("Loom", nullptr, windowFlags)) {
        // ==========================================
        //             NAVIGATION BAR
        // ==========================================

        float headerHeight = 60.0f * scale; // Fixed height header
        float sidePadding = 20.0f;

        // -- Draw Background for Header (Optional, if you want a distinct top bar color) --
        // ImGui::GetWindowDrawList()->AddRectFilled(
        //    ImGui::GetCursorScreenPos(), 
        //    ImVec2(ImGui::GetCursorScreenPos().x + scaledSize.x, ImGui::GetCursorScreenPos().y + headerHeight), 
        //    ImColor(20, 20, 25, 255));

        // -- Move cursor to start of content (Manual Padding) --
        ImGui::SetCursorPos(ImVec2(sidePadding, (headerHeight - 35.0f) / 2.0f)); // Vertically center items

        static int currentTab = 0;

        // -- 1. LEFT SIDE: TABS --
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 8)); // Wider buttons

        ImGui::PushStyleColor(ImGuiCol_Button, (currentTab == 0) ? colActive : colInactive);
        if (ImGui::Button("Mods")) currentTab = 0;
        ImGui::PopStyleColor();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, (currentTab == 1) ? colActive : colInactive);
        if (ImGui::Button("Configs")) currentTab = 1;
        ImGui::PopStyleColor();

        ImGui::PopStyleVar(); // Pop FramePadding

        // -- 2. RIGHT SIDE: SEARCH & EXIT --
        static char search[128] = "";
        float searchW = 250.0f * scale;
        float exitBtnW = 35.0f;
        float spacing = 10.0f;

        // Align to right: WindowWidth - (Padding + ItemsWidth)
        float startX = scaledSize.x - sidePadding - (searchW + spacing + exitBtnW);

        // Only move if we aren't overlapping the left buttons
        if (startX > ImGui::GetCursorPosX()) ImGui::SetCursorPosX(startX);

        // Search Bar
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colSearchBg);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8)); // Match height
        ImGui::SetNextItemWidth(searchW);
        ImGui::InputTextWithHint("##search", "Search...", search, sizeof(search));
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        ImGui::SameLine();

        // Exit Button (Square)
        ImGui::PushStyleColor(ImGuiCol_Button, colInactive);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        if (ImGui::Button("X", ImVec2(exitBtnW, 35.0f))) { // Explicit height matching
            // Exit Logic
        }
        ImGui::PopStyleColor(2);

        // -- SEPARATOR --
        // Move cursor below the header height to start content
        ImGui::SetCursorPos(ImVec2(sidePadding, headerHeight));

        ImGui::BeginChild("Content", ImVec2(0, 0), false);

        if (currentTab == 0) {
            if (ImGui::BeginTable("Modules", 4)) {
                for (Module* mod : Client::getModules()) {
                    ImGui::TableNextColumn();
                    DrawModuleItem(mod);
                }
                ImGui::EndTable();
            }
        }
        else {
            ImGui::Text("Config files go here...");
        }

        ImGui::EndChild();
    }

    ImGui::End();
}
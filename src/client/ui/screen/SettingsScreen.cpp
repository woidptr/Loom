#include "SettingsScreen.hpp"

void SettingsScreen::render() {
    // Styling the window to match the reference dark theme
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.08f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 center = viewport->GetCenter();

    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(center, ImGuiCond_None, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;

    ImGui::Begin("MINECRAFT MODS WINDOW", nullptr, windowFlags);

    ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "MINECRAFT MODS");
    float closeBtnWidth = 30.f;
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - closeBtnWidth);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    if (ImGui::Button("X", ImVec2(closeBtnWidth, 0))) {
        ScreenManager::setScreen(nullptr);
    }
    ImGui::PopStyleColor();

    ImGui::Spacing();

    // --- Top Bar (Tabs & Search) ---
    static const char* categories[] = { "ALL", "NEW", "HUD", "WORLD", "VISUAL" };
    static int currentTab = 0;

    for (int i = 0; i < 5; i++) {
        if (i > 0) ImGui::SameLine();

        // Use a different color for the active tab (Blueish)
        if (i == currentTab) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.75f, 1.0f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        }

        if (ImGui::Button(categories[i], ImVec2(70, 30))) {
            currentTab = i;
        }
        ImGui::PopStyleColor();
    }

    // Search Bar (Right Aligned)
    static char searchBuf[128] = "";
    float searchWidth = 200.0f;
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - searchWidth);
    ImGui::PushItemWidth(searchWidth);
    ImGui::InputTextWithHint("##Search", "Search...", searchBuf, IM_ARRAYSIZE(searchBuf));
    ImGui::PopItemWidth();

    ImGui::Separator();
    ImGui::Spacing();

    // --- Grid Area ---
    float cardWidth = 160.0f;
    float cardHeight = 180.0f;
    float padding = 15.0f;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / (cardWidth + padding));
    if (columnCount < 1) columnCount = 1;

    ImGui::BeginChild("GridArea", ImVec2(0, 0), false);

    int shownCount = 0;
    for (Module* mod : $get_modules()) {
        // 1. Filter by Tab
        // if (currentTab != 0 && mod.category != categories[currentTab]) continue;

        // 2. Filter by Search
        if (searchBuf[0] != 0 && mod->getName().find(searchBuf) == std::string::npos) continue;

        // 3. Grid Logic
        if (shownCount % columnCount != 0) ImGui::SameLine(0.0f, padding);

        // 4. Draw the Card
        DrawModuleItem(mod);
        // DrawModCard(mod, cardWidth, cardHeight);

        shownCount++;
    }

    ImGui::EndChild();

    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}
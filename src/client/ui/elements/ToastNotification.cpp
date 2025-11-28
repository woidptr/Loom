#include "ToastNotification.hpp"

void ToastManager::addToast(std::string msg, float duration = 3.0f) {
    toasts.push_back(Toast{ msg, duration, duration });
}

void ToastManager::renderToasts() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screen = io.DisplaySize;

    const float padding = 20.0f;
    const float spacing = 10.0f;
    const float slideDist = 250.0f;

    float y = screen.y - padding;   // Bottom of screen

    for (int i = 0; i < toasts.size(); )
    {
        Toast& t = toasts[i];
        t.remaining -= io.DeltaTime;

        if (t.remaining <= 0.0f) {
            toasts.erase(toasts.begin() + i);
            continue;
        }

        float age = t.lifetime - t.remaining;

        // Slide-in first 0.3 sec
        float slideIn = std::min(age / 0.30f, 1.0f);

        // Slide-out last 0.5 sec
        float slideOut = 0.0f;
        if (t.remaining < 0.50f)
            slideOut = 1.0f - (t.remaining / 0.50f);

        float slide = (1.0f - slideIn) * slideDist + slideOut * slideDist;

        float alpha = (slideOut > 0) ? (1.0f - slideOut) : 1.0f;

        ImGui::SetNextWindowBgAlpha(alpha);

        // FIXED X POSITION — using anchor (1,0) not (1,1)
        ImGui::SetNextWindowPos(
            ImVec2(screen.x - padding + slide, y),
            ImGuiCond_Always,
            ImVec2(1.0f, 0.0f)               // anchor = right-top
        );

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(40, 20));

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.11f, 0.95f));

        ImGui::Begin(("toast_" + std::to_string(i)).c_str(),
            nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoMove);

        ImGui::SetWindowFontScale(2.0f);

        ImGui::TextUnformatted(t.text.c_str());
        ImGui::End();

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);

        y -= ImGui::GetItemRectSize().y + spacing;
        i++;
    }
}
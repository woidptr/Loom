#include "ToastNotification.hpp"

void ToastManager::addToast(std::string msg, float duration = 3.0f) {
    toasts.push_back(Toast{ msg, duration, duration });
}

void ToastManager::renderToasts() {
    ImGuiIO& io = ImGui::GetIO();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 screen = ImVec2(viewport->Size.x, viewport->Size.y);

    const float padding_x = 5.f;
    const float padding_y = 10.f;
    const float slideDist = 250.0f;

    float y = screen.y - padding_y;

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

        float alpha = (slideOut > 0) ? (0.75f - slideOut) : 0.75f;

        ImGui::SetNextWindowBgAlpha(alpha);

        ImGui::SetNextWindowPos(
            ImVec2(screen.x - padding_x + slide, y),
            ImGuiCond_Always,
            ImVec2(1.0f, 1.0f)
        );

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(40, 20));

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.11f, 0.70f));

        ImGui::Begin(("toast_" + std::to_string(i)).c_str(),
            nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoMove);

        ImGui::SetWindowFontScale(2.0f);

        ImGui::TextUnformatted(t.text.c_str());

        y -= ImGui::GetWindowSize().y + 2;

        ImGui::End();

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);

        i++;
    }
}
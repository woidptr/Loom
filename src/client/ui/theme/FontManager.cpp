#include "FontManager.hpp"
#include <Resource.hpp>

void FontManager::construct() {
    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->AddFontDefault();

    ImFontConfig fontConfig;
    fontConfig.FontDataOwnedByAtlas = false;

    Resource res = Resource("assets/fonts/Montserrat-ExtraBold.ttf");
    montserrat_extra_bold = io.Fonts->AddFontFromMemoryTTF(
        (void*)res.data(),
        res.size(),
        256.f,
        &fontConfig
    );

    io.Fonts->Build();
}

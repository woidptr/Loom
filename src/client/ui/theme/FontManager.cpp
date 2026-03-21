#include "FontManager.hpp"
#include <core/Resource.hpp>

void FontManager::construct() {
    const ImGuiIO& io = ImGui::GetIO();

    io.Fonts->AddFontDefault();

    ImFontConfig fontConfig;
    fontConfig.FontDataOwnedByAtlas = false;

    const Resource res_logo_font("assets/fonts/Montserrat-ExtraBold.ttf");
    montserrat_extra_bold = io.Fonts->AddFontFromMemoryTTF(
        const_cast<uint8_t*>(res_logo_font.data()),
        static_cast<int>(res_logo_font.size()),
        256.f,
        &fontConfig
    );

    io.Fonts->Build();
}

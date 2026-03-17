#pragma once
#include <imgui.h>

struct FontManager {
    static inline ImFont* montserrat_extra_bold = nullptr;

    static void construct();
};
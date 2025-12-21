#pragma once
#include <string>
#include <vector>
#include <imgui.h>

struct Toast {
    std::string text;
    float lifetime;
    float remaining;
};

class ToastManager {
public:
    static inline std::vector<Toast> toasts;
public:
    static void addToast(std::string msg, float duration);
    static void renderToasts();
};

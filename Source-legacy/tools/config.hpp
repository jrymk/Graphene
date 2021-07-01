#pragma once

#include "../ui/imgui_include.hpp"

namespace config {
    bool LaunchWindowMaximized = false;
    std::pair<int, int> WindowSize = {1280, 720};
    int AntiAliasingSamples = 4;
    bool VerticalSync = false;
    bool EnableDocking = true;
    bool EnableViewports = false;

}
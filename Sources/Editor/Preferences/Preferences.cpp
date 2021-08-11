#include "Preferences.h"
#include <iostream>
#include <fstream>
#include <Editor/Theme/Theme.h>
#include <Tracy.hpp>

namespace gfn {
    ZoneScoped

    void Preferences::serialize(nlohmann::json& j) {
        bindings.serialize(j["Key Binds"]);

        nlohmann::json themeOptions;
        themeOptions["Use dark theme"] = themes_use_dark_theme;
        j["Theme Options"] = themeOptions;

        nlohmann::json gfxOptions;
        gfxOptions["Default font"] = graphics_default_font;
        gfxOptions["Font size"] = graphics_font_size;
        gfxOptions["Launch with window maximized"] = graphics_launch_maximized;
        gfxOptions["Launch with window width"] = graphics_launch_window_width;
        gfxOptions["Launch with window height"] = graphics_launch_window_height;
        gfxOptions["Enable vertical sync"] = graphics_vertical_sync;
        gfxOptions["Anti-aliasing samples"] = graphics_antialiasing_samples;
        gfxOptions["Enable multiple viewports"] = graphics_multiple_viewports;
        j["Graphics Options"] = gfxOptions;

        nlohmann::json gvOptions;
        gvOptions["Zoom speed"] = graphview_zoom_speed;
        gvOptions["Selection tolerance"] = graphview_selection_tolerance;
        gvOptions["Selection highlight glow size"] = glow_size;
        j["Graph View Options"] = gvOptions;
    }

    void Preferences::deserialize(nlohmann::json& j) {
        ZoneScoped
        bindings.deserialize(j["Key Binds"]);

        nlohmann::json themeOptions = j["Theme Options"];
        if (!themeOptions.is_null())
            themes_use_dark_theme = themeOptions["Use dark theme"];

        nlohmann::json gfxOptions = j["Graphics Options"];
        if (!gfxOptions.is_null()) {
            graphics_default_font = gfxOptions["Default font"];
            graphics_font_size = gfxOptions["Font size"];
            graphics_launch_maximized = gfxOptions["Launch with window maximized"];
            graphics_launch_window_width = gfxOptions["Launch with window width"];
            graphics_launch_window_height = gfxOptions["Launch with window height"];
            graphics_vertical_sync = gfxOptions["Enable vertical sync"];
            graphics_antialiasing_samples = gfxOptions["Anti-aliasing samples"];
            graphics_multiple_viewports = gfxOptions["Enable multiple viewports"];
        }

        nlohmann::json gvOptions = j["Graph View Options"];
        if (!gvOptions.is_null()) {
            graphview_zoom_speed = gvOptions["Zoom speed"];
            graphview_selection_tolerance = gvOptions["Selection tolerance"];
            glow_size = gvOptions["Selection highlight glow size"];
        }

        //setTheme(this);
    }

    void Preferences::saveToFile() {
        ZoneScoped
        std::ofstream prefFile("preferences.json", std::ios::out);
        if (prefFile) {
            nlohmann::json j;
            serialize(j);

            auto jDump = j.dump(4, ' ', true);
            prefFile << jDump;
            prefFile.close();
            std::cout << "Saved preferences.json (Output size: " << jDump.size() << " bytes)\n";
        }
    }

    void Preferences::loadFromFile() {
        ZoneScoped
        std::ifstream prefFile("preferences.json", std::ios::in);
        if (prefFile) {
            nlohmann::json j = nlohmann::json::parse(prefFile);
            //std::cout << j.dump(4, ' ', true) << "\n";
            deserialize(j);
        }
    }

    void Preferences::checkSave() {
        ZoneScoped
        if (bindings.wantSaveBindings) {
            saveToFile();
            bindings.wantSaveBindings = false;
        }
    }

    void Preferences::preferencesPanel() {
        ImGui::SetNextWindowSizeConstraints(ImVec2(300.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(std::max(800.0f, 100.0f), std::max(400.0f, 100.0f)), ImGuiCond_Appearing);

        if (ImGui::BeginPopupModal("\ue8b8 Preferences")) {

            ImGui::BeginChild("PrefList##PrefsPanel",
                              ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 28.0f), true,
                              0);

            if (ImGui::BeginTabBar("##PrefListTabBar")) {
                if (ImGui::BeginTabItem("Graph View")) {
                    ImGui::DragFloat("Zoom speed", &graphview_zoom_speed, 0.1f, -10.0f, 10.0f, "%f");
                    ImGui::DragFloat("Selection tolerance", &graphview_selection_tolerance, 1.0f, 0.0f, 50.0f, "%fpx");
                    ImGui::DragFloat("Selection highlight glow size", &glow_size, 1.0f, 0.0f, 50.0f, "%fpx");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Themes")) {
                    gfn::text("*application restart required", HUE_RED);
                    ImGui::Checkbox("Use dark theme", &themes_use_dark_theme);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Graphics")) {
                    gfn::text("*application restart required", HUE_RED);
                    ImGui::InputText("Default font", graphics_default_font.data(), 1024);
                    ImGui::DragFloat("Font size", &graphics_font_size, 1.0f, 4.0f, 128.0f, "%fpt");
                    ImGui::Checkbox("Launch with window maximized", &graphics_launch_maximized);
                    ImGui::DragInt("Launch with window width", &graphics_launch_window_width, 1, 100, INT_MAX, "%dpx");
                    ImGui::DragInt("Launch with window height", &graphics_launch_window_height, 1, 100, INT_MAX, "%dpx");
                    ImGui::Checkbox("Enable vertical sync", &graphics_vertical_sync);
                    ImGui::DragInt("Anti-aliasing samples", &graphics_antialiasing_samples, 1, 0, 32, "%dx");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }

            ImGui::EndChild();

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 155.0f);
            if (gfn::button("\ue5cd Cancel##PREFS", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, false)) {
                showPrefsWindow = false;
                loadFromFile();
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine(ImGui::GetWindowWidth() - 80.0f);
            if (gfn::button("\ue161 Save##PREFS", HUE_CONTRAST, HUE_BLUE_CONTRAST, false, 0, 0, false)) {
                showPrefsWindow = false;
                saveToFile();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
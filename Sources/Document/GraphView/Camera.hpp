#pragma once

#include <imgui.h>
#include <Objects/Vec2f.hpp>
#include <Preferences/Preferences.hpp>

namespace gfn::editor::graphview {
    class Camera {
        gfn::Vec2f targetCenterCoord;
        double targetZoom;

    public:
        gfn::Vec2f centerCoord;
        double zoom;

        Camera() {
            centerCoord = gfn::Vec2f(0.0, 0.0);
            zoom = 1.0;
            //targetCenterCoord = gfn::Vec2f(0.0, 0.0);
            //targetZoom = 1.0;

        }

        bool _enableCameraPan = true;
        bool _enableCameraZoom = true;

        static ImVec2 canvasCoord() {
            return {ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2.0f,
                    ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2.0f};
        }

        ImVec2 map(gfn::Vec2f c) {
            return {float(canvasCoord().x - (centerCoord.x - c.x) * zoom),
                    float(canvasCoord().y + (centerCoord.y - c.y) * zoom)};
        }

        float map(double c) {
            return float(c * zoom);
        }

        /*ImVec2 mapNoSmooth(gfn::Vec2f c) {
            return {float(canvasCoord().x - (targetCenterCoord.x - c.x) * targetZoom),
                    float(canvasCoord().y + (targetCenterCoord.y - c.y) * targetZoom)};
        }

        float mapNoSmooth(double c) {
            return float(c * targetZoom);
        }*/

        void update(gfn::preferences::Preferences* preferences) {
            auto io = ImGui::GetIO();
            if (ImGui::IsItemHovered()) {
                zoom *= pow(preferences->graphview_zoom_speed, io.MouseWheel);

                if (ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
                    centerCoord.x += -io.MouseDelta.x / zoom;
                    centerCoord.y -= -io.MouseDelta.y / zoom;
                }
                centerCoord.x += -(io.MousePos.x - canvasCoord().x + (canvasCoord().x - io.MousePos.x) *
                                                                     (pow(preferences->graphview_zoom_speed,
                                                                          io.MouseWheel))) / zoom;
                centerCoord.y += (io.MousePos.y - canvasCoord().y + (canvasCoord().y - io.MousePos.y) *
                                                                    (pow(preferences->graphview_zoom_speed,
                                                                         io.MouseWheel))) / zoom;
            }
            // smooth pan and zoom
            //zoom *= pow(targetZoom / zoom, preferences->graphview_smooth_zoom_speed);
            //zoom += (targetZoom - zoom* preferences->graphview_smooth_zoom_speed);
            //targetZoom = std::max(targetZoom, DBL_MIN);
            //centerCoord.x += (centerCoord.x - centerCoord.x) * preferences->graphview_smooth_pan_speed;
            //centerCoord.y += (centerCoord.y - centerCoord.y) * preferences->graphview_smooth_pan_speed;
        }

    };

}
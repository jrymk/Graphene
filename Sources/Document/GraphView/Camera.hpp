#pragma once

#include <imgui.h>
#include <Objects/Vec2.h>
#include <Preferences/Preferences.hpp>
#include <Editor/HotKey.hpp>

namespace gfn::graphview {
    class Camera {
        gfn::Vec2 targetCenterCoord;
        double targetZoom;

    public:
        gfn::Vec2 centerCoord;
        double zoom;

        Camera() {
            centerCoord = gfn::Vec2(0.0, 0.0);
            zoom = 10.0;
            //targetCenterCoord = gfn::Vec2f(0.0, 0.0);
            //targetZoom = 1.0;

        }

        bool _enableCameraPan = true;
        bool _enableCameraZoom = true;

        static ImVec2 canvasCoord() {
            return {ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2.0f,
                    ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2.0f};
        }

        ImVec2 map(gfn::Vec2 c) {
            return {float(canvasCoord().x - (centerCoord.x - c.x) * zoom),
                    float(canvasCoord().y + (centerCoord.y - c.y) * zoom)};
        }

        float map(double c) {
            return float(c * zoom);
        }

        gfn::Vec2 rMap(ImVec2 c) {
            return {float(centerCoord.x + (c.x - canvasCoord().x) / zoom),
                    float(centerCoord.y - (c.y - canvasCoord().y) / zoom)};
        }

        double rMap(float c) {
            return c / zoom;
        }

        /*ImVec2 mapNoSmooth(gfn::Vec2f c) {
            return {float(canvasCoord().x - (targetCenterCoord.x - c.x) * targetZoom),
                    float(canvasCoord().y + (targetCenterCoord.y - c.y) * targetZoom)};
        }

        float mapNoSmooth(double c) {
            return float(c * targetZoom);
        }*/

        void update(gfn::preferences::Preferences *preferences, int mouseState) {
            MTR_SCOPE("graphview", "update camera");
            auto io = ImGui::GetIO();
            if (ImGui::IsItemHovered()) {
                float zoomVelocity = 0;

                // determine mouse wheel velocity based on the cursor state from selection (one frame later)
                if (mouseState & 0b1) {
                    if (gfn::editor::hkDown(gfn::Actions::ZOOM_IN_E)) {
                        if (gfn::editor::hkHasVelocity(gfn::Actions::ZOOM_IN_E))
                            zoomVelocity = gfn::editor::hkVelocity(gfn::Actions::ZOOM_IN_E);
                        else if (gfn::editor::hkPress(gfn::Actions::ZOOM_IN_E))
                            zoomVelocity = 1;
                    }
                    if (gfn::editor::hkDown(gfn::Actions::ZOOM_OUT_E)) {
                        if (gfn::editor::hkHasVelocity(gfn::Actions::ZOOM_OUT_E))
                            zoomVelocity = -gfn::editor::hkVelocity(gfn::Actions::ZOOM_OUT_E);
                        else if (gfn::editor::hkPress(gfn::Actions::ZOOM_OUT_E))
                            zoomVelocity = -1;
                    }
                } else if (mouseState & 0b10) {
                    if (gfn::editor::hkDown(gfn::Actions::ZOOM_IN_U)) {
                        if (gfn::editor::hkHasVelocity(gfn::Actions::ZOOM_IN_U))
                            zoomVelocity = gfn::editor::hkVelocity(gfn::Actions::ZOOM_IN_U);
                        else if (gfn::editor::hkPress(gfn::Actions::ZOOM_IN_U))
                            zoomVelocity = 1;
                    }
                    if (gfn::editor::hkDown(gfn::Actions::ZOOM_OUT_U)) {
                        if (gfn::editor::hkHasVelocity(gfn::Actions::ZOOM_OUT_U))
                            zoomVelocity = -gfn::editor::hkVelocity(gfn::Actions::ZOOM_OUT_U);
                        else if (gfn::editor::hkPress(gfn::Actions::ZOOM_OUT_U))
                            zoomVelocity = -1;
                    }
                } else if (mouseState & 0b100) {
                    if (gfn::editor::hkDown(gfn::Actions::ZOOM_IN_S)) {
                        if (gfn::editor::hkHasVelocity(gfn::Actions::ZOOM_IN_S))
                            zoomVelocity = gfn::editor::hkVelocity(gfn::Actions::ZOOM_IN_S);
                        else if (gfn::editor::hkPress(gfn::Actions::ZOOM_IN_S))
                            zoomVelocity = 1;
                    }
                    if (gfn::editor::hkDown(gfn::Actions::ZOOM_OUT_S)) {
                        if (gfn::editor::hkHasVelocity(gfn::Actions::ZOOM_OUT_S))
                            zoomVelocity = -gfn::editor::hkVelocity(gfn::Actions::ZOOM_OUT_S);
                        else if (gfn::editor::hkPress(gfn::Actions::ZOOM_OUT_S))
                            zoomVelocity = -1;
                    }
                }

                zoom *= pow(preferences->graphview_zoom_speed, zoomVelocity);

                if (((mouseState & 0b1) && gfn::editor::hkDown(gfn::Actions::CAMERA_PAN_E))
                    || ((mouseState & 0b10) && gfn::editor::hkDown(gfn::Actions::CAMERA_PAN_U))
                    || ((mouseState & 0b100) && gfn::editor::hkDown(gfn::Actions::CAMERA_PAN_S))) {
                    centerCoord.x += -io.MouseDelta.x / zoom;
                    centerCoord.y -= -io.MouseDelta.y / zoom;
                }

                centerCoord.x += -(io.MousePos.x - canvasCoord().x +
                                   (canvasCoord().x - io.MousePos.x) *
                                   (pow(preferences->graphview_zoom_speed, zoomVelocity))) / zoom;
                centerCoord.y += (io.MousePos.y - canvasCoord().y +
                                  (canvasCoord().y - io.MousePos.y) *
                                  (pow(preferences->graphview_zoom_speed, zoomVelocity))) / zoom;
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
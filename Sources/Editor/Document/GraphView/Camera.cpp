#include "Camera.h"
#include <minitrace.h>

namespace gfn {
    Camera::Camera(gfn::HKHandler* hk, gfn::Preferences* prefs) :
            hk(hk),
            prefs(prefs) {
        centerCoord = gfn::Vec2(0.0, 0.0);
        zoom = 10.0;
    }

    ImVec2 Camera::canvasCoord() {
        return {ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2.0f,
                ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2.0f};
    }

    ImVec2 Camera::map(gfn::Vec2 c) {
        return {float(canvasCoord().x - (centerCoord.x - c.x) * zoom),
                float(canvasCoord().y + (centerCoord.y - c.y) * zoom)};
    }

    float Camera::map(double c) {
        return float(c * zoom);
    }

    gfn::Vec2 Camera::rMap(ImVec2 c) {
        return {float(centerCoord.x + (c.x - canvasCoord().x) / zoom),
                float(centerCoord.y - (c.y - canvasCoord().y) / zoom)};
    }

    double Camera::rMap(float c) {
        return c / zoom;
    }

    void Camera::update(int mouseState) {
        MTR_SCOPE("graphview", "update camera");

        if (ImGui::IsItemHovered()
            && (((mouseState & 0b1) && hk->press(gfn::Actions::CAMERA_PAN_E))
                || ((mouseState & 0b10) && hk->press(gfn::Actions::CAMERA_PAN_U))
                || ((mouseState & 0b100) && hk->press(gfn::Actions::CAMERA_PAN_S)))) {
            // pan will only be enabled if camera pan binding is active AND window(invis button) is hovered
            // after that, if the pan hot key is satisfied, keep the latch on so panning will be kept on even
            // if the cursor is out of the graphview window
            std::cerr << "on pan\n";
            panLatch = true;
            onPanMouseState = mouseState;
        }

        /// ZOOM
        if (ImGui::IsItemHovered()) {
            float zoomVelocity = 0;
            // determine mouse wheel velocity based on the cursor state from selection (one frame later)
            if (mouseState & 0b1) {
                if (hk->down(gfn::Actions::ZOOM_IN_E)) {
                    if (hk->hasVelocity(gfn::Actions::ZOOM_IN_E))
                        zoomVelocity = hk->velocity(gfn::Actions::ZOOM_IN_E);
                    else if (hk->press(gfn::Actions::ZOOM_IN_E))
                        zoomVelocity = 1;
                }
                if (hk->down(gfn::Actions::ZOOM_OUT_E)) {
                    if (hk->hasVelocity(gfn::Actions::ZOOM_OUT_E))
                        zoomVelocity = -hk->velocity(gfn::Actions::ZOOM_OUT_E);
                    else if (hk->press(gfn::Actions::ZOOM_OUT_E))
                        zoomVelocity = -1;
                }
            } else if (mouseState & 0b10) {
                if (hk->down(gfn::Actions::ZOOM_IN_U)) {
                    if (hk->hasVelocity(gfn::Actions::ZOOM_IN_U))
                        zoomVelocity = hk->velocity(gfn::Actions::ZOOM_IN_U);
                    else if (hk->press(gfn::Actions::ZOOM_IN_U))
                        zoomVelocity = 1;
                }
                if (hk->down(gfn::Actions::ZOOM_OUT_U)) {
                    if (hk->hasVelocity(gfn::Actions::ZOOM_OUT_U))
                        zoomVelocity = -hk->velocity(gfn::Actions::ZOOM_OUT_U);
                    else if (hk->press(gfn::Actions::ZOOM_OUT_U))
                        zoomVelocity = -1;
                }
            } else if (mouseState & 0b100) {
                if (hk->down(gfn::Actions::ZOOM_IN_S)) {
                    if (hk->hasVelocity(gfn::Actions::ZOOM_IN_S))
                        zoomVelocity = hk->velocity(gfn::Actions::ZOOM_IN_S);
                    else if (hk->press(gfn::Actions::ZOOM_IN_S))
                        zoomVelocity = 1;
                }
                if (hk->down(gfn::Actions::ZOOM_OUT_S)) {
                    if (hk->hasVelocity(gfn::Actions::ZOOM_OUT_S))
                        zoomVelocity = -hk->velocity(gfn::Actions::ZOOM_OUT_S);
                    else if (hk->press(gfn::Actions::ZOOM_OUT_S))
                        zoomVelocity = -1;
                }
            }
            zoom *= pow(prefs->graphview_zoom_speed, zoomVelocity);

            centerCoord.x += -(ImGui::GetIO().MousePos.x - canvasCoord().x +
                    (canvasCoord().x - ImGui::GetIO().MousePos.x) *
                    (pow(prefs->graphview_zoom_speed, zoomVelocity))) / zoom;
            centerCoord.y += (ImGui::GetIO().MousePos.y - canvasCoord().y +
                    (canvasCoord().y - ImGui::GetIO().MousePos.y) *
                    (pow(prefs->graphview_zoom_speed, zoomVelocity))) / zoom;
        }

        if (panLatch) {
            /// PAN
            //panLatch = false;
            if (((onPanMouseState & 0b1) && hk->down(gfn::Actions::CAMERA_PAN_E))
                || ((onPanMouseState & 0b10) && hk->down(gfn::Actions::CAMERA_PAN_U))
                || ((onPanMouseState & 0b100) && hk->down(gfn::Actions::CAMERA_PAN_S))) {
                centerCoord.x += -ImGui::GetIO().MouseDelta.x / zoom;
                centerCoord.y -= -ImGui::GetIO().MouseDelta.y / zoom;
                panLatch = true;
            }
        }
    }
}
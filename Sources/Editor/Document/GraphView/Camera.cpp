#include "Camera.h"

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

    void Camera::update() {
        /// ZOOM
        if (_canZoomIn) {
            // determine mouse wheel velocity based on the cursor state from selection (one frame later)
            // (handled by selection)
            zoom *= pow(prefs->graphview_zoom_speed, (_zoomInVelocity < 0 ? 1 : _zoomInVelocity));

            centerCoord.x += -(ImGui::GetIO().MousePos.x - canvasCoord().x +
                               (canvasCoord().x - ImGui::GetIO().MousePos.x) *
                               (pow(prefs->graphview_zoom_speed, (_zoomInVelocity < 0 ? 1 : _zoomInVelocity)))) / zoom;
            centerCoord.y += (ImGui::GetIO().MousePos.y - canvasCoord().y +
                              (canvasCoord().y - ImGui::GetIO().MousePos.y) *
                              (pow(prefs->graphview_zoom_speed, (_zoomInVelocity < 0 ? 1 : _zoomInVelocity)))) / zoom;
        }

        if (_canZoomOut) {
            // determine mouse wheel velocity based on the cursor state from selection (one frame later)
            // (handled by selection)
            zoom /= pow(prefs->graphview_zoom_speed, (_zoomOutVelocity < 0 ? 1 : _zoomOutVelocity));

            centerCoord.x += -(ImGui::GetIO().MousePos.x - canvasCoord().x +
                    (canvasCoord().x - ImGui::GetIO().MousePos.x) *
                    (pow(prefs->graphview_zoom_speed, -(_zoomOutVelocity < 0 ? 1 : _zoomOutVelocity)))) / zoom;
            centerCoord.y += (ImGui::GetIO().MousePos.y - canvasCoord().y +
                    (canvasCoord().y - ImGui::GetIO().MousePos.y) *
                    (pow(prefs->graphview_zoom_speed, -(_zoomOutVelocity < 0 ? 1 : _zoomOutVelocity)))) / zoom;
        }

        if (_canPan) {
            // pan will only be enabled if camera pan binding is active AND window(invis button) is hovered
            // after that, if the pan hot key is satisfied, keep the latch on so panning will be kept on even
            // if the cursor is out of the graphview window
            // (handled by selection)
            centerCoord.x += -ImGui::GetIO().MouseDelta.x / zoom;
            centerCoord.y -= -ImGui::GetIO().MouseDelta.y / zoom;
        }
    }
}
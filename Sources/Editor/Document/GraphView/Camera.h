#ifndef CAMERA_H
#define CAMERA_H

#include <Core/Objects/Vec2.h>
#include <Editor/HotKeyHandler.h>
#include <Editor/Preferences/Preferences.h>
#include <imgui.h>

namespace gfn {
    class Camera {
        gfn::HKHandler* hk;
        gfn::Preferences* prefs;

    public:
        int hoverState = 0; // updated by selection
        bool _canPan = false;
        bool _canZoomIn = false;
        bool _canZoomOut = false;
        float _zoomInVelocity = 0.0f;
        float _zoomOutVelocity = 0.0f;

        gfn::Vec2 centerCoord;
        double zoom;

        Camera(gfn::HKHandler* hk, gfn::Preferences* prefs);

        ImVec2 canvasCoord();

        ImVec2 map(gfn::Vec2 c);

        float map(double c);

        gfn::Vec2 rMap(ImVec2 c);

        double rMap(float c);

        void update();
    };
}

#endif
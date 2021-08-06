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
        bool panLatch = false;
        bool onPanMouseState;

    public:
        gfn::Vec2 centerCoord;
        double zoom;

        Camera(gfn::HKHandler* hk, gfn::Preferences* prefs);

        ImVec2 canvasCoord();

        ImVec2 map(gfn::Vec2 c);

        float map(double c);

        gfn::Vec2 rMap(ImVec2 c);

        double rMap(float c);

        void update(int mouseState);
    };
}

#endif
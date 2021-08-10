#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <queue>
#include <Editor/Graphics/Graphics.h>
#include "Camera.h"
#include "Selection.h"
#include "Renderer.hpp"

namespace gfn {
///@brief renders and handles interaction to a specified interface
    class GraphView {
    public:
        gfn::Uuid docId;
        gfn::Interface* itf;
        gfn::HKHandler* hk;
        gfn::Preferences* prefs;
        gfn::Graphics* gfx;
        std::queue<std::string> commands;

        Camera camera;
        Selection selection;
        Renderer renderer;

        GraphView(gfn::Uuid docId, gfn::Interface* itf, gfn::HKHandler* hk, gfn::Preferences* prefs, gfn::Graphics* gfx);

        void execute(const std::string& cmd);

        void update();
    };
}

#endif
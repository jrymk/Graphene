#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "Camera.h"
#include "Selection.h"
#include "Renderer.hpp"

namespace gfn {
///@brief renders and handles interaction to a specified interface
    class GraphView {
    private:
        bool onVertexPreviewState = 0;
        bool vertexPreviewEnabled = false;

    public:
        gfn::Uuid docId;
        gfn::Interface* itf;
        gfn::HKHandler* hk;
        gfn::Preferences* prefs;

        Camera camera;
        Selection selection;
        Renderer renderer;

        GraphView(gfn::Uuid docId, gfn::Interface* itf, gfn::HKHandler* hk, gfn::Preferences* prefs);

        void update();
    };
}

#endif
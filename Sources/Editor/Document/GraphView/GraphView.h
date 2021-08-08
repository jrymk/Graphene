#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <queue>
#include "Camera.h"
#include "Selection.h"
#include "Renderer.hpp"

namespace gfn {
///@brief renders and handles interaction to a specified interface
    class GraphView {
    private:
        bool addingEdge = false;
        int onAddEdgeState = 0;
        gfn::Uuid addEdgeVertex;

    public:
        gfn::Uuid docId;
        gfn::Interface* itf;
        gfn::HKHandler* hk;
        gfn::Preferences* prefs;
        std::queue<std::string> commands;

        Camera camera;
        Selection selection;
        Renderer renderer;

        GraphView(gfn::Uuid docId, gfn::Interface* itf, gfn::HKHandler* hk, gfn::Preferences* prefs);

        void execute(const std::string& cmd);

        void update();
    };
}

#endif
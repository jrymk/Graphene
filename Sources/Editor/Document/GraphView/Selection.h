#ifndef SELECTION_H
#define SELECTION_H

#include "Camera.h"
#include <Core/Interface/Interface.h>
#include <Editor/HotKeyHandler.h>
#include <earcut.hpp>

namespace gfn {
    class Selection {
    private:
        gfn::Interface* itf;
        Camera* camera;
        gfn::HKHandler* hk;

        int _lassoPressState = 0;
        bool _lassoAddVertexMode = false;
        bool _lassoSubtractVertexMode = false;
        bool _lassoInvVertexMode = false;
        bool _lassoAddEdgeMode = false;
        bool _lassoSubtractEdgeMode = false;
        bool _lassoInvEdgeMode = false;
        gfn::Vec2 _prevMouseCoord;
        int _onPanState = 0;

        std::vector<std::vector<std::pair<double, double>>> _lassoVertices;
        std::vector<int> _lassoIndices;
        // to undo the selections every frame to properly perform the logic operation
        std::unordered_set<gfn::Uuid> _vertexSelectionOnLasso;
        std::unordered_set<gfn::Uuid> _edgeSelectionOnLasso;



        void updateClearSelection();

        void updateLassoSelection();

        void updateClickSelection();

        void updateSelectAll();

    public:
        gfn::Preferences* prefs;
        float distance(ImVec2 a, ImVec2 b);
        float distanceToALine(ImVec2 p, ImVec2 a, ImVec2 b);
        Selection(gfn::Interface* itf, Camera* camera, gfn::HKHandler* hk, gfn::Preferences* prefs);

        // these can be both active, usually we prioritize hoveredVertex, if it is null, take edge
        gfn::Vec2 mouseCoord;
        gfn::Vec2 mouseDelta;

        gfn::Uuid hoveredVertex;
        gfn::Uuid hoveredEdge;

        std::unordered_set<gfn::Uuid> vertexSelection;
        std::unordered_set<gfn::Uuid> edgeSelection;
        bool onChangeSelection = false;

        bool lassoSelecting = false;

        bool moveStarted = false;
        bool moving = false;
        bool moveEnded = false;

        bool press(int actionId);

        bool down(int actionId);

        bool release(int actionId);

        bool doubleClick(int actionId);

        bool hasVelocity(int actionId);

        float velocity(int actionId);

        void updateHoverState();

        void updateSelection();
    };
}

#endif
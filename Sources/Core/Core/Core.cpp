#include "Core.h"
#include <Tracy.hpp>

namespace gfn {
    Core::Core() :
            itf(),
            structure(&itf),
            placement() {
    }

    gfn::Interface* Core::getInterface() {
        return &itf;
    }

    void Core::update() {
        FrameMarkNamed("Core update")
        ZoneScoped
        parseCommands();

        if (itf.graph.getWrite().pendingUpdate) {
            // update component list with usergraph and rebuild block cut tree
            structure.componentify();
            itf.graph.getWrite().pendingUpdate = false;
            /// TODO: partial update
        }

        if (!itf.graph.getWrite().cfg.energySavingMode)
            placement.update(&itf, &structure);

        if (itf.graph.wantWrite()) {
            itf.graph.writeDone();
        }
    }
};

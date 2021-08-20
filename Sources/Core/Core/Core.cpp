#include <Core/Placement/PosInitializer.h>
#include "Core.h"

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
        parseCommands();

        if (itf.graph.getWrite().pendingUpdate) {
            // update component list with usergraph and rebuild block cut tree
            structure.componentify();
            itf.graph.getWrite().pendingUpdate = false;
            //itf.graph.getWrite().cfg.doGraphUpdate.value = true;
            itf.graph.getWrite().cfg.energySavingMode = false;
            /// TODO: partial update
        }

        if (!itf.graph.getWrite().cfg.energySavingMode)
            placement.update(&itf, &structure);

        if (itf.graph.wantWrite()) {
            itf.graph.writeDone();
        }
    }
};

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
        MTR_META_PROCESS_NAME("graphene-core");
        MTR_META_THREAD_NAME("core thread");
        MTR_SCOPE("core", "core cycle");

        parseAll();

        if (itf.graph.getWrite().pendingUpdate) {
            // update component list with usergraph and rebuild block cut tree
            structure.componentify();
            PosInitializer(&structure).init();
            itf.graph.getWrite().pendingUpdate = false;
            /// TODO: partial update
        }

        placement.update(&itf, &structure);

        if (itf.graph.wantWrite()) {
            itf.graph.writeDone();
        }
    }
};

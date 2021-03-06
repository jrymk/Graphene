#include "Placement.h"

namespace gfn {
    Placement::Placement() :
            pool(std::thread::hardware_concurrency() - 1) {

    }

    void Placement::update(Interface* itf, gfn::Structure* structure) {
        itf->graph.getWrite().cfg.energySavingMode = true;
        for (auto& c : structure->components)
            updateComponentMultiThreaded(itf, c);
    }
}
#include "Placement.h"

namespace gfn {
    void Placement::update(Interface* itf, gfn::Structure* structure) {
        for (auto& c : structure->components)
            updateComponentMultiThreaded(itf, c);
    }
}
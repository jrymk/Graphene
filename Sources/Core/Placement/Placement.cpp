#include "Placement.h"
#include <Tracy.hpp>

namespace gfn {
    Placement::Placement() :
            pool(std::thread::hardware_concurrency()) {

    }

    void Placement::update(Interface* itf, gfn::Structure* structure) {
        ZoneScoped
        for (auto& c : structure->components)
            updateComponentMultiThreaded(itf, c);
    }
}
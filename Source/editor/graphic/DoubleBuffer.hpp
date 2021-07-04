#pragma once

#include "Graphic.hpp"

namespace graphene::editor::graphic {
    Graphic* frontBuffer;
    Graphic* backBuffer;
    std::atomic<bool> swapReady = false;

    void init() {
        frontBuffer = new Graphic();
        backBuffer = new Graphic();
    }

    void swap() {
        std::swap(frontBuffer, backBuffer);
    }
    
}

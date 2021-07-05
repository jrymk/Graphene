#pragma once

#include <atomic>
#include "Graphic.hpp"

namespace graphene::editor::graphic {
Graphic* frontBuffer;
Graphic* backBuffer;
std::atomic<bool> swapReady;

void init() {
	frontBuffer = new Graphic();
	backBuffer = new Graphic();
	swapReady = true;
}

void swap() { std::swap(frontBuffer, backBuffer); }

} // namespace graphene::editor::graphic

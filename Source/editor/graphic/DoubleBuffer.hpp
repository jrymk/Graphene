#pragma once

#include <mutex>
#include <atomic>
#include "Graphic.hpp"

namespace graphene::editor::graphic {
std::mutex mutex;
Graphic* frontBuffer;
Graphic* backBuffer;
std::atomic<bool> swapReady;
std::atomic<bool> backBufferUpdated;

void init() {
	frontBuffer = new Graphic();
	backBuffer = new Graphic();
	swapReady = true;
	backBufferUpdated = false;
}

void swap() {
	mutex.lock();
	std::swap(frontBuffer, backBuffer);
	graphene::editor::graphic::backBufferUpdated = false;
	mutex.unlock();
}

} // namespace graphene::editor::graphic

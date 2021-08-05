#pragma once

#include <random>
#include <chrono>
#include <string>

namespace gfn::random {
std::mt19937& getEngine() {
	thread_local auto duration = std::chrono::steady_clock::now().time_since_epoch();
	thread_local auto count =
		duration.count() + std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

	thread_local static std::mt19937 engine(count);
	return engine;
}
} // namespace gfn::system::random
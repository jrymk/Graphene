#pragma once

#include <string>
#include <queue>
#include <mutex>

namespace gfn::core::input {
class blockingQueue {
	std::queue<std::string> queue;
	std::mutex mutex;

	void push(std::string command) {
		const std::lock_guard<std::mutex> lock(mutex);
		queue.push(command);
	}

	bool tryFront(std::string& output) {
		const std::lock_guard<std::mutex> lock(mutex);
		if (!queue.empty()) {
			output = queue.front();
			return true;
		} else
			return false;
	}

	bool tryPop() {
		const std::lock_guard<std::mutex> lock(mutex);
		if (!queue.empty()) {
			queue.pop();
			return true;
		} else
			return false;
	}
};
} // namespace gfn::core::input
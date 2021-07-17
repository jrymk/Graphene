#pragma once

#include <string>
#include <queue>
#include <mutex>

namespace gfn::core {
typedef std::string command; // I just don't like how std::string looks
};

namespace gfn::core::input {
class InputStream {
	std::queue<gfn::core::command> queue;
	std::mutex mutex;

  public:
	void push(gfn::core::command command) {
		const std::lock_guard<std::mutex> lock(mutex);
		queue.push(command);
	}

	bool tryFront(gfn::core::command& output) {
		const std::lock_guard<std::mutex> lock(mutex);
		if (!queue.empty()) {
			output = queue.front();
			return true;
		} else
			return false;
	}

	bool pop() {
		const std::lock_guard<std::mutex> lock(mutex);
		if (!queue.empty()) {
			queue.pop();
			return true;
		} else
			return false;
	}
};
} // namespace gfn::core::input
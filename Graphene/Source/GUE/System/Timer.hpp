#pragma once
#include <chrono>

namespace gue {
	class Timer {
	public:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;

		Timer() {
			//start = std::chrono::high_resolution_clock::now();
		}

		void restart() {
			start = std::chrono::high_resolution_clock::now();
		}

		double getSeconds() {
			std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
			return diff.count();
		}

		long long getMilliseconds() {
			std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
			return diff.count();
		}

		long long getMicroseconds() {
			std::chrono::microseconds diff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
			return diff.count();
		}

		long long getNanoseconds() {
			std::chrono::nanoseconds diff = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
			return diff.count();
		}
	};
}
#pragma once
#include <chrono>

namespace Utils {
	class Timer {
	public:
		std::chrono::time_point<std::chrono::steady_clock> start;

		Timer() {
			start = std::chrono::steady_clock::now();
		}

		void restart() {
			start = std::chrono::steady_clock::now();
		}

		double getSeconds() {
            std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start;
			return diff.count();
		}

		long long getMilliseconds() {
			std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
			return diff.count();
		}

		long long getMicroseconds() {
			std::chrono::microseconds diff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
			return diff.count();
		}

		long long getNanoseconds() {
			std::chrono::nanoseconds diff = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start);
			return diff.count();
		}
	};

	class FramerateCounter {
		Timer m_timer;
		std::queue<long long> rollingData;

	public:
		FramerateCounter() = default;

		void countFrame() {
			if (m_timer.getMicroseconds() >= 100000000) m_timer.restart();
			rollingData.push(m_timer.getMicroseconds());

			while (!rollingData.empty()) {
				long long frontData = rollingData.front();
				if (frontData >= 99000000) frontData -= 100000000;
				if (!(m_timer.getMicroseconds() - 1000000 <= frontData && m_timer.getMicroseconds() >= frontData))
					rollingData.pop();
				else { break; }
			}

		}

		void updateWithoutCount() {
			if (m_timer.getMicroseconds() >= 100000000) m_timer.restart();

			while (!rollingData.empty()) {
				long long frontData = rollingData.front();
				if (frontData >= 99000000) frontData -= 100000000;
				if (!(m_timer.getMicroseconds() - 1000000 <= frontData && m_timer.getMicroseconds() >= frontData))
					rollingData.pop();
				else { break; }
			}

		}


		int getFramerate() {
			return rollingData.size();
		}

	};
}
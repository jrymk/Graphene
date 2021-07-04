#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <deque>
#include "../../Dependencies/imgui/imgui.h"

namespace graphene::tools::profiler {
class Timer {
  public:
	std::chrono::time_point<std::chrono::steady_clock> start;

	Timer() { start = std::chrono::steady_clock::now(); }

	void restart() { start = std::chrono::steady_clock::now(); }

	double getSeconds() {
		std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start;
		return diff.count();
	}

	long long getMilliseconds() {
		std::chrono::milliseconds diff =
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
		return diff.count();
	}

	long long getMicroseconds() {
		std::chrono::microseconds diff =
			std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
		return diff.count();
	}

	long long getNanoseconds() {
		std::chrono::nanoseconds diff =
			std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start);
		return diff.count();
	}
};

class CycleMonitor {
  public:
	struct Process {
		std::string processName;
		long long timePointNanoS;
		ImU32 color;

		Process(const std::string& name, long long timePoint, ImU32 col)
			: processName(name), timePointNanoS(timePoint), color(col) {}
	};

  private:
	Timer globalTimer;
	Timer cycleTimer;
	std::deque<std::pair<long long, std::vector<Process>>> rollingData;

  public:
	CycleMonitor() = default;

	void newTimePoint(const std::string& processName, ImU32 col) {
		rollingData.back().second.push_back(Process(processName, cycleTimer.getMicroseconds(), col));
	}

	void resetCycle() {
		if (globalTimer.getMicroseconds() >= 100000000)
			globalTimer.restart();
		rollingData.emplace_back(globalTimer.getMicroseconds(), std::vector<Process>());

		while (!rollingData.empty()) {
			long long frontData = rollingData.front().first;
			if (frontData >= 99000000)
				frontData -= 100000000;
			if (!(globalTimer.getMicroseconds() - 1000000 <= frontData && globalTimer.getMicroseconds() >= frontData))
				rollingData.pop_front();
			else {
				break;
			}
		}
	}

	void updateWithoutCount() {
		if (globalTimer.getMicroseconds() >= 100000000)
			globalTimer.restart();

		while (!rollingData.empty()) {
			long long frontData = rollingData.front().first;
			if (frontData >= 99000000)
				frontData -= 100000000;
			if (!(globalTimer.getMicroseconds() - 1000000 <= frontData && globalTimer.getMicroseconds() >= frontData))
				rollingData.pop_front();
			else {
				break;
			}
		}
	}

	int getFramerate() { return rollingData.size(); }
};

} // namespace graphene::tools
#pragma once
#include <queue>
#include "../System/Timer.hpp"

namespace gue {
	class FramerateCounter {
		Timer m_timer;
		std::queue<long long> rollingData;

	public:
		FramerateCounter() = default;

		void frameCount() {
			if (m_timer.getMicroseconds() >= 100000000) m_timer.restart();
			
			rollingData.emplace(m_timer.getMicroseconds());

			while (true) {
				long long frontData = rollingData.front();
				if (frontData >= 99000000) frontData -= 100000000;
				if (m_timer.getMicroseconds() - frontData >= 1000000) {
					rollingData.pop();
				}
				else { break; }
			}
			
		}

		int getFramerate() {
			return rollingData.size();
		}
		
	};

	
}
#pragma once

#include "thread_pool/thread_pool.hpp"
#include <thread>
#include <iostream>
#include <cmath>
#include <queue>
#include <Structure/Structure.hpp>
#include <Properties/Properties.hpp>
#include <Configs/Configs.hpp>
#include <Core/Drawlgo/WithinComponent.hpp>
#include <System/Timer/Timer.hpp>

// updater is such a bad name, I need a new one
namespace gfn::core::drawlgo {
class Drawlgo {
	bool doMultithreading = false;
	int performanceCheckCounter = 0;

  public:
	void update(gfn::configs::Configs* configs, gfn::structure::Structure* structure,
				gfn::properties::Properties* properties) {
		if (++performanceCheckCounter == 10000) {
			performanceCheckCounter = 0;

			gfn::timer::Timer mTimer;
			thread_pool::ThreadPool thread_pool{};
			std::vector<std::future<void>> futures;
			for (auto& c : structure->componentList.getComponents()) {
				futures.emplace_back(thread_pool.Submit(updateComponent, configs, c, properties));
			}
			for (const auto& it : futures) {
				it.wait();
			}
			auto mTime = mTimer.getMicroseconds();

			gfn::timer::Timer sTimer;
			for (auto& c : structure->componentList.getComponents())
				updateComponent(configs, c, properties);
			auto sTime = sTimer.getMicroseconds();

			if (mTime < sTime)
				doMultithreading = true;
			else
				doMultithreading = false;

			std::cerr << "Performance analysis: \n    Single threaded: " << sTime << "us\n    Multi threaded: " << mTime
					  << "us\n        Multi threading: " << (doMultithreading ? "On\n" : "Off\n");
		} else {
			if (doMultithreading) {
				thread_pool::ThreadPool thread_pool{};
				std::vector<std::future<void>> futures;
				for (auto& c : structure->componentList.getComponents()) {
					futures.emplace_back(thread_pool.Submit(updateComponent, configs, c, properties));
				}
				for (const auto& it : futures) {
					it.wait();
				}
			} else {
				for (auto& c : structure->componentList.getComponents())
					updateComponent(configs, c, properties);
			}
		}
	}
};
} // namespace gfn::core::drawlgo
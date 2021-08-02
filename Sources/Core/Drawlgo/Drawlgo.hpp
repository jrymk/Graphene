#pragma once

#include <thread_pool.hpp>
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
        // optimize performance by measuring four multithreading modes every 30 seconds
        int multiThreadingMode = 2;
        gfn::timer::Timer performanceCheckTimer;

    public:
        void update(gfn::configs::Configs* configs, gfn::structure::Structure* structure,
                    gfn::props::Properties* properties) {
            if (performanceCheckTimer.getSeconds() >= 60.0) {
                // MULTITHREADING MODE 0
                gfn::timer::Timer cMvM;
                {
                    thread_pool::ThreadPool thread_pool{};
                    std::vector<std::future<unsigned long long>> futures;
                    for (auto& c : structure->componentList.getComponents())
                        futures.emplace_back(thread_pool.Submit(updateComponentMultiThreaded, configs, c));
                    for (auto& it : futures)
                        it.get();
                }
                auto cMvMtime = std::make_pair(cMvM.getMicroseconds(), 0);
                // MULTITHREADING MODE 1
                gfn::timer::Timer cMvS;
                {
                    thread_pool::ThreadPool thread_pool{};
                    std::vector<std::future<unsigned long long>> futures;
                    for (auto& c : structure->componentList.getComponents())
                        futures.emplace_back(thread_pool.Submit(updateComponentSingleThreaded, configs, c));
                    for (auto& it : futures)
                        it.get();
                }
                auto cMvStime = std::make_pair(cMvS.getMicroseconds(), 1);
                // MULTITHREADING MODE 2
                gfn::timer::Timer cSvM;
                {
                    for (auto& c : structure->componentList.getComponents())
                        updateComponentMultiThreaded(configs, c);
                }
                auto cSvMtime = std::make_pair(cSvM.getMicroseconds(), 2);
                // MULTITHREADING MODE 3
                gfn::timer::Timer cSvS;
                {
                    for (auto& c : structure->componentList.getComponents())
                        updateComponentSingleThreaded(configs, c);
                }
                auto cSvStime = std::make_pair(cSvS.getMicroseconds(), 3);

                multiThreadingMode = std::min(std::min(cMvMtime, cMvStime), std::min(cSvMtime, cSvStime)).second;
                std::cerr << "  CMVM[0]: " <<
                          cMvMtime.first << "  CMVS[1]: " << cMvStime.first << "  CSVM[2]: " << cSvMtime.first
                          << "  CSVS[3]: " << cSvStime.first << "\n";
                std::cerr << "MULTITHREADING MODE: " << multiThreadingMode << "\n";

                performanceCheckTimer.restart();
            } else {
                // MULTITHREADING MODE 0
                if (multiThreadingMode == 0) {
                    thread_pool::ThreadPool thread_pool{};
                    std::vector<std::future<unsigned long long>> futures;
                    for (auto& c : structure->componentList.getComponents())
                        futures.emplace_back(thread_pool.Submit(updateComponentMultiThreaded, configs, c));
                    for (auto& it : futures)
                        it.get();
                }
                // MULTITHREADING MODE 1
                if (multiThreadingMode == 1) {
                    thread_pool::ThreadPool thread_pool{};
                    std::vector<std::future<unsigned long long>> futures;
                    for (auto& c : structure->componentList.getComponents())
                        futures.emplace_back(thread_pool.Submit(updateComponentSingleThreaded, configs, c));
                    for (auto& it : futures)
                        it.get();
                }
                // MULTITHREADING MODE 2
                if (multiThreadingMode == 2) {
                    for (auto& c : structure->componentList.getComponents())
                        updateComponentMultiThreaded(configs, c);
                }
                // MULTITHREADING MODE 3
                if (multiThreadingMode == 3) {
                    for (auto& c : structure->componentList.getComponents())
                        updateComponentSingleThreaded(configs, c);
                }
            }
        }
    };
} // namespace gfn::core::drawlgo
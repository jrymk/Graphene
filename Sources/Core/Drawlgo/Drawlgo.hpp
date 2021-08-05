#pragma once

#include <thread_pool.hpp>
#include <thread>
#include <cmath>
#include <queue>
#include <Interface/Interface.hpp>
#include <Core/Drawlgo/WithinComponent.hpp>
#include <System/Timer.hpp>
#include <minitrace.h>

// updater is such a bad name, I need a new one
namespace gfn::core::drawlgo {
    class Drawlgo {
        // optimize performance by measuring four multithreading modes every 30 seconds
        int multiThreadingMode = 2;
        gfn::Timer performanceCheckTimer;

    public:
        void update(Interface* itf, gfn::Structure* structure) {
            MTR_SCOPE("core", "drawlgo update");

            if (performanceCheckTimer.getSeconds() >= 2.0) {
                MTR_SCOPE("core", "drawlgo performance check");
                // MULTITHREADING MODE 0
                gfn::Timer cMvM;
                {
                    MTR_SCOPE("core", "CMVM");
                    thread_pool::ThreadPool thread_pool{};
                    std::vector<std::future<unsigned long long>> futures;
                    for (auto& c : structure->getComponents())
                        futures.emplace_back(thread_pool.Submit(updateComponentMultiThreaded, itf, c));
                    for (auto& it : futures)
                        it.get();
                }
                auto cMvMtime = std::make_pair(cMvM.getMicroseconds(), 0);
                // MULTITHREADING MODE 1
                gfn::Timer cMvS;
                {
                    MTR_SCOPE("core", "CMVS");
                    thread_pool::ThreadPool thread_pool{};
                    std::vector<std::future<unsigned long long>> futures;
                    for (auto& c : structure->getComponents())
                        futures.emplace_back(thread_pool.Submit(updateComponentSingleThreaded, itf, c));
                    for (auto& it : futures)
                        it.get();
                }
                auto cMvStime = std::make_pair(cMvS.getMicroseconds(), 1);
                // MULTITHREADING MODE 2
                gfn::Timer cSvM;
                {
                    MTR_SCOPE("core", "CSVM");
                    for (auto& c : structure->getComponents())
                        updateComponentMultiThreaded(itf, c);
                }
                auto cSvMtime = std::make_pair(cSvM.getMicroseconds(), 2);
                // MULTITHREADING MODE 3
                gfn::Timer cSvS;
                {
                    MTR_SCOPE("core", "CSVS");
                    for (auto& c : structure->getComponents())
                        updateComponentSingleThreaded(itf, c);
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
                    MTR_SCOPE("core", "CMVM");
                    thread_pool::ThreadPool thread_pool{};
                    std::vector<std::future<unsigned long long>> futures;
                    for (auto& c : structure->getComponents())
                        futures.emplace_back(thread_pool.Submit(updateComponentMultiThreaded, itf, c));
                    for (auto& it : futures)
                        it.get();
                }
                // MULTITHREADING MODE 1
                if (multiThreadingMode == 1) {
                    MTR_SCOPE("core", "CMVS");
                    thread_pool::ThreadPool thread_pool{};
                    std::vector<std::future<unsigned long long>> futures;
                    for (auto& c : structure->getComponents())
                        futures.emplace_back(thread_pool.Submit(updateComponentSingleThreaded, itf, c));
                    for (auto& it : futures)
                        it.get();
                }
                // MULTITHREADING MODE 2
                if (multiThreadingMode == 2) {
                    MTR_SCOPE("core", "CSVM");

                    for (auto& c : structure->getComponents())
                        updateComponentMultiThreaded(itf, c);
                }
                // MULTITHREADING MODE 3
                if (multiThreadingMode == 3) {
                    MTR_SCOPE("core", "CSVS");
                    for (auto& c : structure->getComponents())
                        updateComponentSingleThreaded(itf, c);
                }
            }
        }
    };
} // namespace gfn::core::drawlgo
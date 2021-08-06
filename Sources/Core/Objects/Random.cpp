#include "Random.h"
#include <chrono>

namespace gfn {
    std::mt19937& getRng() {
        thread_local auto duration = std::chrono::steady_clock::now().time_since_epoch();
        thread_local auto count =
                duration.count() + std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

        thread_local static std::mt19937 engine(count);
        return engine;
    }
}
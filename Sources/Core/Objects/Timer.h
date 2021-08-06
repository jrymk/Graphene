#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

namespace gfn {
    class Timer {
    public:
        std::chrono::time_point<std::chrono::steady_clock> start;

        Timer();

        void restart();

        double getSeconds();

        unsigned long long getMilliseconds();

        unsigned long long getMicroseconds();

        unsigned long long getNanoseconds();
    };

    class TimePoint {
    public:
        std::chrono::time_point<std::chrono::steady_clock> tp;

        TimePoint();

        void restart();

        static double getSeconds(const TimePoint& baseline);

        static unsigned long long getMilliseconds(const TimePoint& baseline);

        static unsigned long long getMicroseconds(const TimePoint& baseline);

        static unsigned long long getNanoseconds(const TimePoint& baseline);
    };

    int getNowMillis();

    int getNowMicros();

    tm getNowUtc();

    tm getNowLocal();

    std::string getNowStamp();

    unsigned long long getNowUnix();
}

#endif
#include "Timer.h"

#include <sstream>
#include <iomanip>

namespace gfn {
    Timer::Timer() { start = std::chrono::steady_clock::now(); }

    void Timer::restart() { start = std::chrono::steady_clock::now(); }

    double Timer::getSeconds() {
        std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start;
        return diff.count();
    }

    unsigned long long Timer::getMilliseconds() {
        std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start);
        return diff.count();
    }

    unsigned long long Timer::getMicroseconds() {
        std::chrono::microseconds diff = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - start);
        return diff.count();
    }

    unsigned long long Timer::getNanoseconds() {
        std::chrono::nanoseconds diff = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - start);
        return diff.count();
    }


    TimePoint::TimePoint() { tp = std::chrono::steady_clock::now(); }

    void TimePoint::restart() { tp = std::chrono::steady_clock::now(); }

    double TimePoint::getSeconds(const TimePoint& baseline) {
        std::chrono::duration<double> diff = std::chrono::steady_clock::now() - baseline.tp;
        return diff.count();
    }

    unsigned long long TimePoint::getMilliseconds(const TimePoint& baseline) {
        std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - baseline.tp);
        return diff.count();
    }

    unsigned long long TimePoint::getMicroseconds(const TimePoint& baseline) {
        std::chrono::microseconds diff = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - baseline.tp);
        return diff.count();
    }

    unsigned long long TimePoint::getNanoseconds(const TimePoint& baseline) {
        std::chrono::nanoseconds diff = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - baseline.tp);
        return diff.count();
    }


    int getNowMillis() {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
        return ms;
    }

    int getNowMicros() {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        uint64_t us = (uint64_t) std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() % 1000;
        return us;
    }

    tm getNowUtc() {
        time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        tm utc_tm = *gmtime(&tt);
        return utc_tm;
    }

    tm getNowLocal() {
        time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        tm local_tm = *localtime(&tt);
        return local_tm;
    }

    std::string getNowStamp() {
        time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        tm local_tm = *localtime(&tt);
        std::stringstream ss;
        ss << std::setw(4) << std::setfill('0') << local_tm.tm_year + 1900 << std::setw(2) << local_tm.tm_mon
        << std::setw(2) << local_tm.tm_mday << "-" << std::setw(2) << local_tm.tm_hour << std::setw(2)
        << local_tm.tm_min << std::setw(2) << local_tm.tm_sec << ":" << std::setw(3) << getNowMillis() << ":"
        << std::setw(3) << getNowMicros();
        return ss.str();
    }

    unsigned long long getNowUnix() { return std::chrono::system_clock::now().time_since_epoch().count(); }
}
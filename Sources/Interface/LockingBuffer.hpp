#pragma once

#include <mutex>
#include <Core/Properties/Properties.hpp>
#include <System/Logging/Logging.hpp>

#undef interface

namespace gfn::interface {
template <class T>
class LockingBuffer {
	std::mutex mutex;
	T buffer;

  public:
	///@brief copies the buffer to a destination object for safe use
	///@param lock when enabled, the function will give up if the resource is in use, and the destination object is not
	/// changed
	///@returns if the copy was successful
	bool get(T& destination, bool lock = true) {
		logVerbose("Interface: Buffer get event");
		if (lock)
			mutex.lock();
		else {
			if (mutex.try_lock() >= 0) {
				// lock failed
				return false;
			}
		}
		destination = buffer;
		mutex.unlock();
		return true;
	}

	///@brief copies the source object to the buffer
	///@param lock when enabled, the function will give up if the resource is in use, and the buffer is not
	/// changed
	///@returns if the copy was successful
	bool put(T& source, bool lock = true) {
		logVerbose("Interface: Buffer put event");
		if (lock)
			mutex.lock();
		else {
			if (mutex.try_lock() >= 0) {
				// lock failed
				return false;
			}
		}
		buffer = source;
		mutex.unlock();
		return true;
	}
};
} // namespace gfn::interface
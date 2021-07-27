#pragma once

#include <mutex>
#include <atomic>
#include <utility>
#include <Logging/Logging.hpp>

#undef interface

namespace gfn::interface {
    template<class T>
    class TripleBuffer {
    private:
        T buffers[3] = {};

        T* readBuffer;
        T* availableBuffer;
        T* writeBuffer;

        std::atomic<bool> readBufferRead = false;
        std::atomic<bool> availableBufferUpToDate = false;
        std::mutex availableBufferMutex;

        std::atomic<int> writeOutpace = 0;
        std::atomic<int> readOutpace = 0;

    public:
        TripleBuffer() {
            readBuffer = &buffers[0];
            availableBuffer = &buffers[1];
            writeBuffer = &buffers[2];
        }

        // reader interface
        T* getRead() {
            // reader thread have complete ownership of this buffer
            return readBuffer;
        }

        // check if the read buffer is already read at least once, for content like command queue or log messages that only
        // needed to be pushed once
        bool isReadBufferRead() {
            readOutpace++;
            return readBufferRead;
        }

        void readDone() {
            // swaps read and available buffers IF available buffer is up to date, meaning the writer thread have updated
            // since last read
            const std::lock_guard<std::mutex> lock(availableBufferMutex);
            if (availableBufferUpToDate) {
                std::swap(readBuffer, availableBuffer);
                //readBuffer = availableBuffer.exchange(readBuffer);
                availableBufferUpToDate = false;
                readBufferRead = false;
            } else {
                readBufferRead = true;
                // available buffer is not up to date, we will no swap and let the reader read the same buffer again
            }
        }

        // writer interface
        T* getWrite() {
            // writer thread have complete ownership of this buffer
            return writeBuffer;
        }

        // start the write process if the read thread has used up the buffer, meaning available buffer is no longer up to
        // date
        bool wantWrite() {
            writeOutpace++;
            return !availableBufferUpToDate;
        }

        void writeDone() {
// set the available buffer to write buffer and mark available buffer up to date
            const std::lock_guard<std::mutex> lock(availableBufferMutex);
            std::swap(writeBuffer, availableBuffer);
            //availableBuffer.exchange(writeBuffer);
            availableBufferUpToDate = true;
        }

        std::pair<int, int> getOutpaceStatistics() {
            std::pair<int, int> p = {readOutpace, writeOutpace};
            readOutpace = 0;
            writeOutpace = 0;
            return p;
        }
    };
} // namespace gfn::interface
#ifndef TRIPLEBUFFER_H
#define TRIPLEBUFFER_H

#include <atomic>
#include <mutex>

#undef interface

namespace gfn {
    enum TripleBufferMode {
        TRIPLEBUFFER_COPY,
        TRIPLEBUFFER_SWAP,
    };

    // only accepts classes with default constructor
    // else use getBuffers() to initialize the contents
    template<class T>
    class TripleBuffer {
    private:
        T buffers[3] = {};

        T* readBuffer;
        T* availableBuffer;
        T* writeBuffer;

        // COPY: write buffer content is copied to available buffer,
        //       meaning the writer can use the write buffer as a persistent storage
        // SWAP: write buffer is swapped with available buffer,
        //       although saves copy time, but the writer can only store one time data like command buffer
        int mode;

        std::atomic<bool> readBufferRead = false;
        std::atomic<bool> availableBufferUpToDate = false;
        std::mutex availableBufferMutex;

        std::atomic<int> writeOutpace = 0;
        std::atomic<int> readOutpace = 0;

    public:
        TripleBuffer(TripleBufferMode mode);

        // reader interface
        T* getRead();

        // check if the read buffer is already read at least once, for content like command queue or log messages that only
        // needed to be pushed once
        bool isReadBufferRead();

        void readDone();

        // writer interface
        T& getWrite();

        void forceSwap();

        // start the write process if the read thread has used up the buffer, meaning available buffer is no longer up to
        // date
        bool wantWrite();

        void writeDone();

        std::pair<int, int> getOutpaceStatistics();

        /// DANGEROUS!! Only for interface constructor
        T** getBuffers();
    };
}

#include "TripleBuffer.tpp"

#endif
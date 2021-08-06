#include "TripleBuffer.h"

namespace gfn {
    template<class T>
    TripleBuffer<T>::TripleBuffer(TripleBufferMode mode) :
            mode(mode) {
        readBuffer = &buffers[0];
        availableBuffer = &buffers[1];
        writeBuffer = &buffers[2];
    }

    template<class T>
    T* TripleBuffer<T>::getRead() {
        // reader thread have complete ownership of this buffer
        return readBuffer;
    }

    template<class T>
    bool TripleBuffer<T>::isReadBufferRead() {
        readOutpace++;
        return readBufferRead;
    }

    template<class T>
    void TripleBuffer<T>::readDone() {
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

    template<class T>
    T& TripleBuffer<T>::getWrite() {
        // writer thread have complete ownership of this buffer
        return *writeBuffer;
    }

    template<class T>
    void TripleBuffer<T>::forceSwap() {
        std::swap(readBuffer, availableBuffer);
    }

    template<class T>
    bool TripleBuffer<T>::wantWrite() {
        writeOutpace++;
        return !availableBufferUpToDate;
    }

    template<class T>
    void TripleBuffer<T>::writeDone() {
        // set the available buffer to write buffer and mark available buffer up to date
        const std::lock_guard<std::mutex> lock(availableBufferMutex);
        if (mode == TRIPLEBUFFER_SWAP)
            std::swap(writeBuffer, availableBuffer);
        else if (mode == TRIPLEBUFFER_COPY)
            *availableBuffer = *writeBuffer;
        //availableBuffer.exchange(writeBuffer);
        availableBufferUpToDate = true;
    }

    template<class T>
    std::pair<int, int> TripleBuffer<T>::getOutpaceStatistics() {
        std::pair<int, int> p = {readOutpace, writeOutpace};
        readOutpace = 0;
        writeOutpace = 0;
        return p;
    }

    /// DANGEROUS!! Only for interface constructor
    template<class T>
    T** TripleBuffer<T>::getBuffers() {
        return &buffers;
    }
}